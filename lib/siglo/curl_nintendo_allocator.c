#include "curl_setup.h"

#include "curl_nintendo_allocator.h"

#include "nn/diag.h"

Allocator gAllocator;

int Curl_SigloAllocatorInitializeDefaults()
{
    AllocatorVTable* vptr = &gAllocator.vptr;

    vptr->malloc = Curl_SigloMalloc;
    vptr->free = Curl_SigloFree;
    vptr->realloc = Curl_SigloRealloc;
    vptr->strdup = Curl_SigloStrdup;
    vptr->calloc = Curl_SigloCalloc;

    return 1;
}

int Curl_SigloAllocatorInitialize(void* (*malloc)(size_t), void (*free)(void*),
                                  void* (*realloc)(void*, size_t), char* (*strdup)(const char*),
                                  void* (*calloc)(size_t, size_t))
{
    if (!malloc || !free || !realloc || !strdup || !calloc)
        return 0;

    AllocatorVTable* vptr = &gAllocator.vptr;

    vptr->malloc = malloc;
    vptr->free = free;
    vptr->realloc = realloc;
    vptr->strdup = strdup;
    vptr->calloc = calloc;

    return 1;
}

int Curl_SigloCleanupAllocator()
{
    return 0;
}

void* Curl_SigloMalloc(size_t size)
{
    return gAllocator.vptr.malloc(size);
}

void Curl_SigloFree(void* ptr)
{
    gAllocator.vptr.free(ptr);
}

void* Curl_SigloRealloc(void* ptr, size_t size)
{
    return gAllocator.vptr.realloc(ptr, size);
}

char* Curl_SigloStrdup(const char* ptr)
{
    return gAllocator.vptr.strdup(ptr);
}

void* Curl_SigloCalloc(size_t nmemb, size_t size)
{
    return gAllocator.vptr.calloc(nmemb, size);
}

void** Curl_SigloAllocAligned(size_t alignment, size_t size)
{
    if (!gAllocator.vptr.malloc)
        return NULL;

    size_t offset = alignment + 7;
    void* ptr = gAllocator.vptr.malloc(size + offset);
    if (!ptr)
        return NULL;

    void** aligned = (void**)(((uintptr_t)ptr + offset) & -alignment);

    aligned[-1] = ptr;

    return *aligned;
}

void Curl_SigloFreeAligned(void** ptr)
{
#line 134
    NN_ASSERT(gAllocator.vptr.free != NULL, "aligned free called before initialized");

    gAllocator.vptr.free(ptr[-1]);
}
