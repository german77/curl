#ifndef HEADER_CURL_NINTENDO_ALLOCATOR
#define HEADER_CURL_NINTENDO_ALLOCATOR

#include <stdint.h>

typedef struct
{
    void* (*malloc)(size_t);
    void (*free)(void*);
    void* (*realloc)(void*, size_t);
    char* (*strdup)(const char*);
    void* (*calloc)(size_t, size_t);
} AllocatorVTable;

typedef struct Allocator
{
    AllocatorVTable vptr;
} Allocator;

int Curl_SigloAllocatorInitializeDefaults();
int Curl_SigloAllocatorInitialize(void* (*malloc)(size_t), void (*free)(void*),
                                  void* (*realloc)(void*, size_t), char* (*strdup)(const char*),
                                  void* (*calloc)(size_t, size_t));
int Curl_SigloCleanupAllocator();

void* Curl_SigloMalloc(size_t size);
void Curl_SigloFree(void* ptr);
void* Curl_SigloRealloc(void* ptr, size_t size);
char* Curl_SigloStrdup(const char*);
void* Curl_SigloCalloc(size_t nmemb, size_t size);

void** Curl_SigloAllocAligned(size_t alignment, size_t size);
void Curl_SigloFreeAligned(void** ptr);

#endif /* HEADER_CURL_NINTENDO_ALLOCATOR */
