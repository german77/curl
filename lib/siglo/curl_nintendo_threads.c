#include "../lib/curl_setup.h"

#include "curl_nintendo_threads.h"

#include "curl_nintendo_allocator.h"
#include "llist.h"
#include "nn/diag.h"
#include "nn/result.h"
#include "nn/thread.h"

GCThread g_garbageCollectorThread;

int Curl_GetTotalThreadCount()
{
    if (!g_garbageCollectorThread.status)
        return -1;

    nnosLockMutex(&g_garbageCollectorThread.mutex);
    int count = Curl_llist_count(g_garbageCollectorThread.activeThreads) +
                Curl_llist_count(g_garbageCollectorThread.inactiveThreads);
    nnosLockMutex(&g_garbageCollectorThread.mutex);
    return count;
}

SigloThread* Curl_SigloThreadContextConstructor(long stack_size, int (*thread_loop)(void*),
                                                void* somethingB)
{
    SigloThread* thread = Curl_SigloMalloc(sizeof(SigloThread));
    int result = -1;

    if (thread)
    {
        memset(thread, 0, sizeof(SigloThread));
        void* stack = Curl_SigloAllocAligned(0x1000, stack_size);
        thread->stack = stack;
        if (stack)
        {
            int priority = nnosGetThreadPriority(nnosGetCurrentThread());
            result = nnosCreateThread(&thread->nnThread, Curl_SigloThreadEntryThunk, thread, stack,
                                      stack_size, priority);

            if (!nnResultIsFailure(result & 0xffffffff))
            {
                nnosSetThreadNamePointer(&thread->nnThread, "LibcurlResolver");
                thread->threadLoop = thread_loop;
                thread->somethingB = somethingB;
            }
        }
        else
        {
            Curl_SigloFree(thread);
            thread = NULL;
        }
    }

    if (nnResultIsFailure(result & 0xffffffff))
    {
        Curl_SigloThreadContextDestructor(thread);
        return NULL;
    }

    return thread;
}

void Curl_SigloThreadEntryThunk(struct ThreadType* nn_thread)
{
    SigloThread* thread = (SigloThread*)nn_thread;
    thread->threadLoop(thread->somethingB);
    if (thread->threadLoop != Curl_SigloThreadGCRunLoop)
        Curl_SigloThreadGCSetInactive(thread);
}

static void Curl_SigloListDtor(void* user, void* p) {}

static void Curl_SigloDestroyThread(void* user, void* p)

{
    struct SigloThread* thread = p;
    if (thread == NULL)
        return;

    nnosDestroyThread(&thread->nnThread);
    if (thread->stack)
    {
        Curl_SigloFreeAligned(thread->stack);
        thread->stack = NULL;
    }
    thread->threadLoop = NULL;
    thread->somethingB = 0;
    Curl_SigloFree(thread);
}

void Curl_SigloThreadContextDestructor(SigloThread* thread)
{
    if (thread)
    {
        Curl_SigloDestroyThread(NULL, thread);
        thread = NULL;
    }
}

void Curl_SigloThreadGCZero()
{
    g_garbageCollectorThread.status = GCThreadStatus_Uninitialized;
    memset(&g_garbageCollectorThread.statusUpdateEvent, 0, 0x58);
    // Note: Stack is saved and restored but this doesn't use the stack
    __asm__("");
}

GCThread* Curl_SigloThreadGCInitialize()
{
    if (g_garbageCollectorThread.status == GCThreadStatus_Uninitialized)
    {
        g_garbageCollectorThread.status = GCThreadStatus_Initialized;
        g_garbageCollectorThread.references++;
        g_garbageCollectorThread.currentThread =
            Curl_SigloThreadContextConstructor(0x4000, Curl_SigloThreadGCRunLoop, 0);
        if (g_garbageCollectorThread.currentThread)
        {
            g_garbageCollectorThread.activeThreads = Curl_llist_alloc(Curl_SigloListDtor);
            if (g_garbageCollectorThread.activeThreads)
            {
                g_garbageCollectorThread.inactiveThreads =
                    Curl_llist_alloc(Curl_SigloDestroyThread);
                if (g_garbageCollectorThread.inactiveThreads)
                {
                    nnosInitializeMutex(&g_garbageCollectorThread.mutex, 1, 0);
                    nnosInitializeEvent(&g_garbageCollectorThread.statusUpdateEvent, 0,
                                        EventClearMode_AutoClear);
                    nnosInitializeEvent(&g_garbageCollectorThread.startEvent, 0,
                                        EventClearMode_AutoClear);
                    nnosStartThread(&g_garbageCollectorThread.currentThread->nnThread);
                    nnosWaitEvent(&g_garbageCollectorThread.startEvent);
                    nnosFinalizeEvent(&g_garbageCollectorThread.startEvent);
                    return &g_garbageCollectorThread;
                }
            }
        }
    }

    if (g_garbageCollectorThread.inactiveThreads)
    {
        Curl_llist_destroy(g_garbageCollectorThread.inactiveThreads, 0);
    }

    if (g_garbageCollectorThread.activeThreads)
    {
        Curl_llist_destroy(g_garbageCollectorThread.activeThreads, 0);
    }

    Curl_SigloDestroyThread(NULL, g_garbageCollectorThread.currentThread);
    Curl_SigloThreadGCZero();
    return 0;
}

int Curl_SigloThreadGCRunLoop(void* somethingB)
{
    g_garbageCollectorThread.status = GCThreadStatus_Running;
    nnosWaitEvent(&g_garbageCollectorThread.startEvent);

    do
    {
        nnosWaitEvent(&g_garbageCollectorThread.statusUpdateEvent);
        nnosLockMutex(&g_garbageCollectorThread.mutex);
        g_garbageCollectorThread.status = GCThreadStatus_Busy;

        struct curl_llist_element* it = g_garbageCollectorThread.inactiveThreads->head;
        while (it != NULL)
        {
            struct curl_llist_element* next = it->next;
            SigloThread* thread = (SigloThread*)it->ptr;

            nnosUnlockMutex(&g_garbageCollectorThread.mutex);
            nnosWaitThread(&thread->nnThread);
            nnosLockMutex(&g_garbageCollectorThread.mutex);

            Curl_llist_remove(g_garbageCollectorThread.inactiveThreads, it, NULL);
            it = next;
        }

        if (g_garbageCollectorThread.references == 0)
        {
            size_t count = Curl_llist_count(g_garbageCollectorThread.activeThreads) +
                           Curl_llist_count(g_garbageCollectorThread.inactiveThreads);
            if (count == 0)
            {
                g_garbageCollectorThread.status = GCThreadStatus_Stopped;
                nnosUnlockMutex(&g_garbageCollectorThread.mutex);
                return 0;
            }
        }

        nnosUnlockMutex(&g_garbageCollectorThread.mutex);
    } while (true);
}

int Curl_SigloThreadGCSetActive(void* p)
{
#line 363
    NN_ASSERT(g_garbageCollectorThread.status != GCThreadStatus_Uninitialized,
              "Adding a thread structure to the garbage collector when it is uninitialized")

    nnosLockMutex(&g_garbageCollectorThread.mutex);
    int ret = Curl_llist_insert_next(g_garbageCollectorThread.activeThreads,
                                     g_garbageCollectorThread.activeThreads->tail, p) ? 0 : -1;
    nnosUnlockMutex(&g_garbageCollectorThread.mutex);

    return ret;
}

int Curl_removeThreadContextFromList(struct curl_llist* list, void* e)
{
    struct curl_llist_element* it = list->head;

    while (it != NULL)
    {
        if (it->ptr == e)
        {
            Curl_llist_remove(list, it, 0);
            return 0;
        }
        it = it->next;
    }

    return -1;
}
static int Curl_addThreadContextToList(struct curl_llist* list, void* e)
{
    bool value = Curl_llist_insert_next(list, list->tail, e);

    if (value)
        return 0;

    return -1;
}

int Curl_SigloThreadGCSetInactive(SigloThread* thread)
{
#line 411
    NN_ASSERT(g_garbageCollectorThread.status != GCThreadStatus_Uninitialized,
              "Adding a thread structure to the garbage collector when it is uninitialized")

    nnosLockMutex(&g_garbageCollectorThread.mutex);

#line 427
    NN_ASSERT(Curl_removeThreadContextFromList(g_garbageCollectorThread.activeThreads, thread) == 0,
              "attempting to add a thread context (%p) to the inactive list that was never placed "
              "in the active list",
              thread)

#line 436
    NN_ASSERT(Curl_addThreadContextToList(g_garbageCollectorThread.inactiveThreads, thread) == 0,
              "unable to add an inactive thread context to the inactive thread list")

    nnosSignalEvent(&g_garbageCollectorThread.statusUpdateEvent);
    nnosUnlockMutex(&g_garbageCollectorThread.mutex);
    return 0;
}

void Curl_SigloThreadGCFinalize()
{
#line 457
    NN_ASSERT(g_garbageCollectorThread.status != GCThreadStatus_Uninitialized,
              "Adding a thread structure to the garbage collector when it is uninitialized")

    nnosLockMutex(&g_garbageCollectorThread.mutex);
    g_garbageCollectorThread.references--;
    nnosUnlockMutex(&g_garbageCollectorThread.mutex);

    nnosSignalEvent(&g_garbageCollectorThread.statusUpdateEvent);
    nnosWaitThread(&g_garbageCollectorThread.currentThread->nnThread);

    if (g_garbageCollectorThread.inactiveThreads)
    {
        Curl_llist_destroy(g_garbageCollectorThread.inactiveThreads, 0);
        g_garbageCollectorThread.inactiveThreads = NULL;
    }

    if (g_garbageCollectorThread.activeThreads)
    {
        Curl_llist_destroy(g_garbageCollectorThread.activeThreads, 0);
        g_garbageCollectorThread.activeThreads = NULL;
    }

    nnosFinalizeMutex(&g_garbageCollectorThread.mutex);
    nnosFinalizeEvent(&g_garbageCollectorThread.statusUpdateEvent);

    struct SigloThread* thread = g_garbageCollectorThread.currentThread;
    if (g_garbageCollectorThread.currentThread)
    {
        Curl_SigloDestroyThread(NULL, g_garbageCollectorThread.currentThread);
        g_garbageCollectorThread.currentThread = NULL;
    }

    g_garbageCollectorThread.status = GCThreadStatus_Uninitialized;
    memset(&g_garbageCollectorThread.statusUpdateEvent, 0, 0x58);
}

struct SigloThread* Curl_thread_create(int (*threadLoop)(void*), void* param_2)
{
    if (g_garbageCollectorThread.status == GCThreadStatus_Uninitialized)
        return NULL;

    nnosLockMutex(&g_garbageCollectorThread.mutex);
    int count = Curl_llist_count(g_garbageCollectorThread.activeThreads) +
                Curl_llist_count(g_garbageCollectorThread.inactiveThreads);
    nnosUnlockMutex(&g_garbageCollectorThread.mutex);

    if (count == -1)
        return NULL;

    if (count >= 0x32)
    {
        nnsocketSetLastErrno(EAgain);
        return NULL;
    }

    struct SigloThread* thread = Curl_SigloThreadContextConstructor(0x10000, threadLoop, param_2);
    if (thread == NULL)
    {
        return NULL;
    }

    Curl_SigloThreadGCSetActive(thread);
    nnosStartThread(&thread->nnThread);
    return thread;
}
