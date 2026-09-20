#ifndef HEADER_CURL_NINTENDO_THREADS
#define HEADER_CURL_NINTENDO_THREADS

#include <stdint.h>

struct ThreadType  // nn::os::ThreadType
{
    char filler[0x1c0];
};

typedef struct SigloThread
{
    struct ThreadType nnThread;
    int (*threadLoop)(void*);
    void* somethingB;
    void* stack;
} SigloThread;

typedef struct MutexType
{
    void* filler[0x4];
} MutexType;

typedef struct EventType
{
    void* filler[0x5];
} EventType;

enum GCThreadStatus
{
    GCThreadStatus_Uninitialized,
    GCThreadStatus_Initialized,
    GCThreadStatus_Running,
    GCThreadStatus_Busy,
    GCThreadStatus_Stopped,
};

typedef struct GCThread
{
    enum GCThreadStatus status;
    int references;
    SigloThread* currentThread;
    struct EventType statusUpdateEvent;
    MutexType mutex;
    struct curl_llist* activeThreads;
    struct curl_llist* inactiveThreads;
    struct EventType startEvent;
} GCThread;

int Curl_GetTotalThreadCount();
SigloThread* Curl_SigloThreadContextConstructor(long stack_size, int (*somethingA)(void*),
                                                void* somethingB);
void Curl_SigloThreadEntryThunk(struct ThreadType*);
void Curl_SigloThreadContextDestructor(SigloThread* thread);
void Curl_SigloThreadGCZero();
GCThread* Curl_SigloThreadGCInitialize();
int Curl_SigloThreadGCRunLoop(void* somethingB);
int Curl_SigloThreadGCSetActive();
int Curl_SigloThreadGCSetInactive(SigloThread* thread);
void Curl_SigloThreadGCFinalize();
void Curl_SigloMiddlewareInfo();

#endif /* HEADER_CURL_NINTENDO_THREADS */
