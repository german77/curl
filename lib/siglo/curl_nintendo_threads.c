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

    nnosLockMutex(&g_garbageCollectorThread.thread);
    int count = Curl_llist_count(g_garbageCollectorThread.activeList) +
                Curl_llist_count(g_garbageCollectorThread.inactiveList);
    nnosLockMutex(&g_garbageCollectorThread.thread);
    return count;
}

ThreadType* Curl_SigloThreadContextConstructor(long stack_size, void (*somethingA)(size_t),
                                               long somethingB)
{
    ThreadType* thread = Curl_SigloMalloc(sizeof(ThreadType));
    long result = -1;

    if (thread)
    {
        memset(thread, 0, sizeof(ThreadType));
        void* stack = Curl_SigloAllocAligned(0x1000, stack_size);
        thread->stack = stack;
        if (stack)
        {
            int priority = nnosGetThreadPriority(nnosGetCurrentThread());
            result = nnosCreateThread(thread, Curl_SigloThreadEntryThunk, thread, stack, stack_size,
                                      priority);

            if (!nnResultIsFailure(result))
            {
                nnosSetThreadNamePointer(thread, "LibcurlResolver");
                thread->somethingA = somethingA;
                thread->somethingB = somethingB;
            }
        }
        else
        {
            Curl_SigloFree(thread);
            thread = NULL;
            result = -1;
        }
    }

    if (nnResultIsFailure(result))
    {
        Curl_SigloThreadContextDestructor(thread);
        thread = NULL;
    }

    return thread;
}

void Curl_SigloThreadEntryThunk(ThreadType* thread)
{
    thread->somethingA(thread->somethingB);
    if (thread->somethingA != Curl_SigloThreadGCRunLoop)
        Curl_SigloThreadGCSetInactive(thread);
}

void Curl_SigloThreadContextDestructor(ThreadType* thread)
{
    if (thread)
    {
        nnosDestroyThread(thread);
        if (thread->stack)
        {
            Curl_SigloFreeAligned(thread->stack);
            thread->stack = NULL;
        }
        thread->somethingA = 0;
        thread->somethingB = 0;
        Curl_SigloFree(thread);
        thread = NULL;
    }
}

void Curl_SigloThreadGCZero()
{
    GCThread* thread = &g_garbageCollectorThread;

    thread->status = 0;
    memset(thread->event, 0, 0x58);
}
bool Curl_SigloThreadGCInitialize();
void Curl_SigloThreadGCRunLoop();

int Curl_SigloThreadGCSetActive(void* p)
{
#line 363
    NN_ASSERT(g_garbageCollectorThread.status,
              "Adding a thread structure to the garbage collector when it is uninitialized")

    nnosLockMutex(&g_garbageCollectorThread.thread);
    bool value = Curl_llist_insert_next(g_garbageCollectorThread.activeList,
                                        g_garbageCollectorThread.activeList->tail, p);
    nnosUnlockMutex(&g_garbageCollectorThread.thread);

    if (value)
        return 0;

    return -1;
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
int Curl_addThreadContextToList(struct curl_llist* list, void* e)
{
    bool value = Curl_llist_insert_next(list, list->tail, e);

    if (value)
        return 0;

    return -1;
}

int Curl_SigloThreadGCSetInactive(ThreadType* thread)
{
#line 411
    NN_ASSERT(g_garbageCollectorThread.status,
              "Adding a thread structure to the garbage collector when it is uninitialized")

    nnosLockMutex(&g_garbageCollectorThread.thread);

#line 428
    NN_ASSERT(Curl_removeThreadContextFromList(g_garbageCollectorThread.activeList, thread) == 0,
              "attempting to add a thread context (%p) to the inactive list that was never placed "
              "in the active list")

#line 435
    NN_ASSERT(Curl_addThreadContextToList(g_garbageCollectorThread.inactiveList, thread) == 0,
              "attempting to add a thread context (%p) to the inactive list that was never placed "
              "in the active list")

    nnosSignalEvent(&g_garbageCollectorThread.event);
    nnosUnlockMutex(&g_garbageCollectorThread.thread);
    return 0;
}
void Curl_SigloThreadGCFinalize();
