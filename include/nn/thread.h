#pragma once

extern struct ThreadType* nnosGetCurrentThread();
extern long nnosCreateThread(struct ThreadType* thread, void (*)(struct ThreadType*), void* arg,
                            void* srcStack, long stackSize, int priority);
extern void nnosDestroyThread(struct ThreadType* thread);
extern void nnosStartThread(struct ThreadType* thread);
extern int nnosGetThreadPriority(struct ThreadType* thread);
extern void nnosSetThreadNamePointer(struct ThreadType* thread, const char* name);

extern void nnosInitializeEvent(struct ThreadType* thread);
extern void nnosFinalizeEvent(struct ThreadType* thread);
extern void nnosWaitEvent(struct ThreadType* thread);

extern void nnosInitializeMutex(struct ThreadType* thread);
extern void nnosFinalizeMutex(struct ThreadType* thread);
extern void nnosLockMutex(struct ThreadType* thread);
extern void nnosUnlockMutex(struct ThreadType* thread);
