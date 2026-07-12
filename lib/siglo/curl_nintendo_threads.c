#include "../lib/curl_setup.h"

#include "curl_nintendo_threads.h"

#include "curl_nintendo_allocator.h"
#include "nn/diag.h"
#include "nn/result.h"
#include "nn/thread.h"

GCThread g_garbageCollectorThread;

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

void Curl_SigloThreadGCZero(){
    GCThread* thread=&g_garbageCollectorThread;

    thread->status=0;
    memset(thread->event, 0, 0x58);
}
void Curl_SigloThreadGCInitialize();
void Curl_SigloThreadGCRunLoop();
void Curl_SigloThreadGCSetActive();
void Curl_SigloThreadGCSetInactive(ThreadType* thread);
void Curl_SigloThreadGCFinalize();
