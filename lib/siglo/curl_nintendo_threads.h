#ifndef HEADER_CURL_NINTENDO_THREADS
#define HEADER_CURL_NINTENDO_THREADS

#include <stdint.h>

typedef struct ThreadType
{
    char filler[0x1c0];
    void (*somethingA)(size_t);
    long somethingB;
    void* stack;
} ThreadType;

typedef struct MutexType
{
    void* filler[0x4];
} MutexType;

typedef struct GCThread
{
    int status;
    int references;
    void* filler3;
    int event;
    void* filler[4];
    MutexType mutex;
    struct curl_llist* activeList;
    struct curl_llist* inactiveList;
} GCThread;

int Curl_GetTotalThreadCount();
ThreadType* Curl_SigloThreadContextConstructor(long stack_size, void (*somethingA)(size_t),
                                               long somethingB);
void Curl_SigloThreadEntryThunk(ThreadType*);
void Curl_SigloThreadContextDestructor(ThreadType* thread);
void Curl_SigloThreadGCZero();
long Curl_SigloThreadGCInitialize();
void Curl_SigloThreadGCRunLoop();
int Curl_SigloThreadGCSetActive();
int Curl_SigloThreadGCSetInactive(ThreadType* thread);
void Curl_SigloThreadGCFinalize();
void Curl_SigloMiddlewareInfo();

#endif /* HEADER_CURL_NINTENDO_THREADS */
