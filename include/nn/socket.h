#include <netinet/in.h>
#include <netdb.h>
#include <nn/types.h>
#include <poll.h>
#include <sys/select.h>
#include <sys/socket.h>

int nnsocketRecv(int socket, void* out, ulong outLen, int flags);
int nnsocketRecvFrom(int, void*, ulong, int, struct sockaddr*, u32*);
int nnsocketSend(int socket, const void* data, ulong dataLen, int flags);
int nnsocketSendTo(int, const void*, ulong, int, const struct sockaddr*, u32);
int nnsocketAccept(int, struct sockaddr*, u32*);
int nnsocketBind(int, const struct sockaddr*, u32);
u32 nnsocketConnect();  // returns nn::Result
int nnsocketGetPeerName(int, struct sockaddr*, u32*);
int nnsocketGetSockName(int, struct sockaddr*, u32*);
int nnsocketGetSockOpt(int, int, int, void*, u32*);
int nnsocketListen(int, int);
int nnsocketSetSockOpt(int socket, int socketLevel, int option, const void*, u32 len);
int nnsocketSockAtMark(int);
int nnsocketShutdown(int, int);
int nnsocketSocket(bool);
int nnsocketWrite(int domain, int type, int protocol);
int nnsocketRead(int, int, int);
u32 nnsocketClose();  // returns nn::Result
void nnsocketSelect(int, fd_set*, fd_set*, fd_set*, struct timeval*);
void nnsocketPoll(struct pollfd*, ulong, int);
void nnsocketFcntl(int, int, ...);
void nnsocketInetPton(int, const char*, void*);
const char* nnsocketInetNtop(int af, const void* src, char* dst, u32 size);
s32 nnsocketInetAton(const char* addressStr, struct in_addr* addressOut);
char* nnsocketInetNtoa(struct in_addr);
u16 nnsocketInetHtons(u16 val);
u32 nnsocketInetHtonl(u32);
u16 nnsocketInetNtohs(u16);
u32 nnsocketInetNtohl(u32);
s32 nnsocketGetLastErrno();
void nnsocketSetLastErrno(int);
s32 nnsocketRecvMsg(int, struct msghdr*, int);
s32 nnsocketSendMsg(int, const struct msghdr*, int);
s32 nnsocketIoctl(int, u32, void*, ulong);
s32 nnsocketOpen(const char*, int);
u32 nnsocketInitialize(void* pool, ulong poolSize, ulong allocPoolSize, int concurLimit);
s32 nnsocketFinalize();
s32 nnsocketGetAddrinfo(const char*, const char*, const struct addrinfo*, struct addrinfo**);
s32 nnsocketGetAdrinfoCancel(const char*, const char*, const struct addrinfo*, struct addrinfo**, int);
s32 nnsocketGetAddrinfoWithoutNsdResolve(const char*, const char*, const struct addrinfo*, struct addrinfo**);
s32 nnsocketGetAddrinfoWithoutNsdResolveCancel(const char*, const char*, const struct addrinfo*,
                                               struct addrinfo**, int);
s32 nnsocketFreeAddrinfo(struct addrinfo*);
s32 nnsocketGetNameInfo(const struct sockaddr*, u32, char*, u32, char*, u32, int);
s32 nnsocketGetNameInfoCancel(const struct sockaddr*, u32, char*, u32, char*, u32, int, int);
struct hostent* nnsocketGetHostByName(const char* name);
struct hostent* nnsocketGetHostByNameCancel(const char*, int);
struct hostent* nnsocketGetHostByNameWithoutNsdResolve(const char*);
struct hostent* nnsocketGetHostByNameWithoutNsdResolveCancel(const char*, int);
struct hostent* nnsocketGetHostByAddr(const void*, u32, int);
struct hostent* nnsocketGetHostByAddrCancel(const void*, u32, int, int);
s32 nnsocketRequestCancelHandle();
s32 nnsocketCancel(int);
s32 nnsocketGetHErrno();
s32 nnsocketHStrError(int);
s32 nnsocketGAIStrError(int);
s32 nnsocketSysctl(int*, ulong, void*, ulong*, void*, ulong);
s32 nnsocketDuplicateSocket(int, ulong);

