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

typedef struct GCThread
{
    int status;
    int references;
    struct ThreadType thread;
    void* event;
} GCThread;

ThreadType* Curl_SigloThreadContextConstructor(long stack_size, void (*somethingA)(size_t), long somethingB);
void Curl_SigloThreadEntryThunk(ThreadType*);
void Curl_SigloThreadContextDestructor(ThreadType* thread);
void Curl_SigloThreadGCZero();
void Curl_SigloThreadGCInitialize();
void Curl_SigloThreadGCRunLoop();
void Curl_SigloThreadGCSetActive();
void Curl_SigloThreadGCSetInactive(ThreadType* thread);
void Curl_SigloThreadGCFinalize();

#endif /* HEADER_CURL_NINTENDO_THREADS */
