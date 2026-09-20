#pragma once

enum EventClearMode
{
    EventClearMode_ManualClear = 0,
    EventClearMode_AutoClear = 1,
};

extern struct ThreadType* nnosGetCurrentThread();
extern long nnosCreateThread(struct ThreadType* thread, void (*)(struct ThreadType*), void* arg,
                             void* srcStack, long stackSize, int priority);
extern void nnosDestroyThread(struct ThreadType* thread);
extern void nnosStartThread(struct ThreadType* thread);
extern int nnosGetThreadPriority(struct ThreadType* thread);
extern void nnosSetThreadNamePointer(struct ThreadType* thread, const char* name);
extern void nnosWaitThread(struct ThreadType* thread);

extern void nnosInitializeEvent(struct EventType* event, bool, enum EventClearMode);
extern void nnosFinalizeEvent(struct EventType* event);
extern void nnosWaitEvent(struct EventType* event);
extern void nnosSignalEvent(struct EventType* event);

extern void nnosInitializeMutex(struct MutexType* mutex, bool, int);
extern void nnosFinalizeMutex(struct MutexType* mutex);
extern void nnosLockMutex(struct MutexType* mutex);
extern void nnosUnlockMutex(struct MutexType* mutex);
