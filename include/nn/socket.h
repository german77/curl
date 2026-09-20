#pragma once

#include <netdb.h>
#include <netinet/in.h>
#include <nn/types.h>
#include <poll.h>
#include <sys/select.h>
#include <sys/socket.h>

enum Errno
{
    ESuccess = 0,
    EPerm = 1,
    ENoEnt = 2,
    ESrch = 3,
    EIntr = 4,
    EIo = 5,
    ENxIo = 6,
    E2Big = 7,
    ENoExec = 8,
    EBadf = 9,
    EChild = 10,
    EAgain = 11,
    EWouldBlock = EAgain,
    ENoMem = 12,
    EAcces = 13,
    EFault = 14,
    ENotBlk = 15,
    EBusy = 16,
    EExist = 17,
    EXDev = 18,
    ENoDev = 19,
    ENotDir = 20,
    EIsDir = 21,
    EInval = 22,
    ENFile = 23,
    EMFile = 24,
    ENotTy = 25,
    ETxtBsy = 26,
    EFBig = 27,
    ENoSpc = 28,
    ESPipe = 29,
    ERofs = 30,
    EMLink = 31,
    EPipe = 32,
    EDom = 33,
    ERange = 34,
    EDeadLk = 35,
    EDeadLock = EDeadLk,
    ENameTooLong = 36,
    ENoLck = 37,
    ENoSys = 38,
    ENotEmpty = 39,
    ELoop = 40,
    ENoMsg = 42,
    EIdrm = 43,
    EChrng = 44,
    EL2NSync = 45,
    EL3Hlt = 46,
    EL3Rst = 47,
    ELnrng = 48,
    EUnatch = 49,
    ENoCsi = 50,
    EL2Hlt = 51,
    EBade = 52,
    EBadr = 53,
    EXFull = 54,
    ENoAno = 55,
    EBadRqc = 56,
    EBadSsl = 57,
    EBFont = 59,
    ENoStr = 60,
    ENoData = 61,
    ETime = 62,
    ENoSr = 63,
    ENoNet = 64,
    ENoPkg = 65,
    ERemote = 66,
    ENoLink = 67,
    EAdv = 68,
    ESrmnt = 69,
    EComm = 70,
    EProto = 71,
    EMultiHop = 72,
    EDotDot = 73,
    EBadMsg = 74,
    EOverflow = 75,
    ENotUnuq = 76,
    EBadFd = 77,
    ERemChg = 78,
    ELibAcc = 79,
    ELibBad = 80,
    ELibScn = 81,
    ELibMax = 82,
    ELibExec = 83,
    EIlSeq = 84,
    ERestart = 85,
    EStrPipe = 86,
    EUsers = 87,
    ENotSock = 88,
    EDestAddrReq = 89,
    EMsgSize = 90,
    EPrototype = 91,
    ENoProtoOpt = 92,
    EProtoNoSupport = 93,
    ESocktNoSupport = 94,
    EOpNotSupp = 95,
    ENotSup = EOpNotSupp,
    EPfNoSupport = 96,
    EAfNoSupport = 97,
    EAddrInUse = 98,
    EAddrNotAvail = 99,
    ENetDown = 100,
    ENetUnreach = 101,
    ENetReset = 102,
    EConnAborted = 103,
    EConnReset = 104,
    ENoBufs = 105,
    EIsConn = 106,
    ENotConn = 107,
    EShutDown = 108,
    ETooManyRefs = 109,
    ETimedOut = 110,
    EConnRefused = 111,
    EHostDown = 112,
    EHostUnreach = 113,
    EAlready = 114,
    EInProgress = 115,
    EStale = 116,
    EUClean = 117,
    ENotNam = 118,
    ENAvail = 119,
    EIsNam = 120,
    ERemoteIo = 121,
    EDQuot = 122,
    ENoMedium = 123,
    EMediumType = 124,
    ECanceled = 125,
    ENoKey = 126,
    EKeyExpired = 127,
    EKeyRevoked = 128,
    EKeyRejected = 129,
    EOwnerDead = 130,
    ENotRecoverable = 131,
    ERfKill = 132,
    EHwPoison = 133,
    /* ... */
    EProcLim = 156,
};

extern int nnsocketRecv(int socket, void* out, ulong outLen, int flags);
extern int nnsocketRecvFrom(int, void*, ulong, int, struct sockaddr*, u32*);
extern int nnsocketSend(int socket, const void* data, ulong dataLen, int flags);
extern int nnsocketSendTo(int, const void*, ulong, int, const struct sockaddr*, u32);
extern int nnsocketAccept(int, struct sockaddr*, u32*);
extern int nnsocketBind(int, const struct sockaddr*, u32);
extern u32 nnsocketConnect();  // returns nn::Result
extern int nnsocketGetPeerName(int, struct sockaddr*, u32*);
extern int nnsocketGetSockName(int, struct sockaddr*, u32*);
extern int nnsocketGetSockOpt(int, int, int, void*, u32*);
extern int nnsocketListen(int, int);
extern int nnsocketSetSockOpt(int socket, int socketLevel, int option, const void*, u32 len);
extern int nnsocketSockAtMark(int);
extern int nnsocketShutdown(int, int);
extern int nnsocketSocket();
extern int nnsocketWrite(int domain, int type, int protocol);
extern int nnsocketRead(int, int, int);
extern u32 nnsocketClose();  // returns nn::Result
extern void nnsocketSelect(int, fd_set*, fd_set*, fd_set*, struct timeval*);
extern void nnsocketPoll(struct pollfd*, ulong, int);
extern int nnsocketFcntl(int, int, ...);
extern void nnsocketInetPton(int, const char*, void*);
extern const char* nnsocketInetNtop(int af, const void* src, char* dst, u32 size);
extern s32 nnsocketInetAton(const char* addressStr, struct in_addr* addressOut);
extern char* nnsocketInetNtoa(struct in_addr);
extern u16 nnsocketInetHtons(u16 val);
extern u32 nnsocketInetHtonl(u32);
extern u16 nnsocketInetNtohs(u16);
extern u32 nnsocketInetNtohl(u32);
extern int nnsocketGetLastErrno();
extern void nnsocketSetLastErrno(int);
extern s32 nnsocketRecvMsg(int, struct msghdr*, int);
extern s64 nnsocketSendMsg(int, const struct msghdr*, int);
extern s32 nnsocketIoctl(int, u32, void*, ulong);
extern s32 nnsocketOpen(const char*, int);
extern u32 nnsocketInitialize(void* pool, ulong poolSize, ulong allocPoolSize, int concurLimit);
extern s32 nnsocketFinalize();
extern s32 nnsocketGetAddrinfo(const char*, const char*, const struct addrinfo*, struct addrinfo**);
extern s32 nnsocketGetAdrinfoCancel(const char*, const char*, const struct addrinfo*,
                                    struct addrinfo**, int);
extern s32 nnsocketGetAddrinfoWithoutNsdResolve(const char*, const char*, const struct addrinfo*,
                                                struct addrinfo**);
extern s32 nnsocketGetAddrinfoWithoutNsdResolveCancel(const char*, const char*,
                                                      const struct addrinfo*, struct addrinfo**,
                                                      int);
extern s32 nnsocketFreeAddrinfo(struct addrinfo*);
extern s32 nnsocketGetNameInfo(const struct sockaddr*, u32, char*, u32, char*, u32, int);
extern s32 nnsocketGetNameInfoCancel(const struct sockaddr*, u32, char*, u32, char*, u32, int, int);
extern struct hostent* nnsocketGetHostByName(const char* name);
extern struct hostent* nnsocketGetHostByNameCancel(const char*, int);
extern struct hostent* nnsocketGetHostByNameWithoutNsdResolve(const char*);
extern struct hostent* nnsocketGetHostByNameWithoutNsdResolveCancel(const char*, int);
extern struct hostent* nnsocketGetHostByAddr(const void*, u32, int);
extern struct hostent* nnsocketGetHostByAddrCancel(const void*, u32, int, int);
extern s32 nnsocketRequestCancelHandle();
extern s32 nnsocketCancel(int);
extern s32 nnsocketGetHErrno();
extern s32 nnsocketHStrError(int);
extern s32 nnsocketGAIStrError(int);
extern s32 nnsocketSysctl(int*, ulong, void*, ulong*, void*, ulong);
extern s32 nnsocketDuplicateSocket(int, ulong);
