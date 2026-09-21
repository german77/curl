include(CheckCSourceCompiles)
# The begin of the sources (macros and includes)
set(_source_epilogue "#undef inline")

macro(add_header_include check header)
  if(${check})
    set(_source_epilogue "${_source_epilogue}\n#include <${header}>")
  endif(${check})
endmacro(add_header_include)

set(signature_call_conv)
if(HAVE_WINDOWS_H)
  add_header_include(HAVE_WINDOWS_H "windows.h")
  add_header_include(HAVE_WINSOCK2_H "winsock2.h")
  add_header_include(HAVE_WINSOCK_H "winsock.h")
  set(_source_epilogue
      "${_source_epilogue}\n#ifndef WIN32_LEAN_AND_MEAN\n#define WIN32_LEAN_AND_MEAN\n#endif")
  set(signature_call_conv "PASCAL")
  if(HAVE_LIBWS2_32)
    set(CMAKE_REQUIRED_LIBRARIES ws2_32)
  endif()
else(HAVE_WINDOWS_H)
  add_header_include(HAVE_SYS_TYPES_H "sys/types.h")
  add_header_include(HAVE_SYS_SOCKET_H "sys/socket.h")
endif(HAVE_WINDOWS_H)

check_c_source_compiles("${_source_epilogue}
  int main(void) {
    int flag = MSG_NOSIGNAL;
    (void)flag;
    return 0;
  }" HAVE_MSG_NOSIGNAL)

if(NOT HAVE_WINDOWS_H)
  add_header_include(HAVE_SYS_TIME_H "sys/time.h")
  add_header_include(TIME_WITH_SYS_TIME "time.h")
  add_header_include(HAVE_TIME_H "time.h")
endif()
check_c_source_compiles("${_source_epilogue}
int main(void) {
  struct timeval ts;
  ts.tv_sec  = 0;
  ts.tv_usec = 0;
  (void)ts;
  return 0;
}" HAVE_STRUCT_TIMEVAL)


include(CheckCSourceRuns)
set(CMAKE_REQUIRED_FLAGS)
if(HAVE_SYS_POLL_H)
  set(CMAKE_REQUIRED_FLAGS "-DHAVE_SYS_POLL_H")
endif(HAVE_SYS_POLL_H)
check_c_source_runs("
  #ifdef HAVE_SYS_POLL_H
  #  include <sys/poll.h>
  #endif
  int main(void) {
    return poll((void *)0, 0, 10 /*ms*/);
  }" HAVE_POLL_FINE)

set(HAVE_SIG_ATOMIC_T 1)
set(CMAKE_REQUIRED_FLAGS)
if(HAVE_SIGNAL_H)
  set(CMAKE_REQUIRED_FLAGS "-DHAVE_SIGNAL_H")
  set(CMAKE_EXTRA_INCLUDE_FILES "signal.h")
endif(HAVE_SIGNAL_H)
check_type_size("sig_atomic_t" SIZEOF_SIG_ATOMIC_T)
if(HAVE_SIZEOF_SIG_ATOMIC_T)
  check_c_source_compiles("
    #ifdef HAVE_SIGNAL_H
    #  include <signal.h>
    #endif
    int main(void) {
      static volatile sig_atomic_t dummy = 0;
      (void)dummy;
      return 0;
    }" HAVE_SIG_ATOMIC_T_NOT_VOLATILE)
  if(NOT HAVE_SIG_ATOMIC_T_NOT_VOLATILE)
    set(HAVE_SIG_ATOMIC_T_VOLATILE 1)
  endif(NOT HAVE_SIG_ATOMIC_T_NOT_VOLATILE)
endif(HAVE_SIZEOF_SIG_ATOMIC_T)

if(HAVE_WINDOWS_H)
  set(CMAKE_EXTRA_INCLUDE_FILES winsock2.h)
else()
  set(CMAKE_EXTRA_INCLUDE_FILES)
  if(HAVE_SYS_SOCKET_H)
    set(CMAKE_EXTRA_INCLUDE_FILES sys/socket.h)
  endif(HAVE_SYS_SOCKET_H)
endif()

check_type_size("struct sockaddr_storage" SIZEOF_STRUCT_SOCKADDR_STORAGE)
if(HAVE_SIZEOF_STRUCT_SOCKADDR_STORAGE)
  set(HAVE_STRUCT_SOCKADDR_STORAGE 1)
endif(HAVE_SIZEOF_STRUCT_SOCKADDR_STORAGE)

