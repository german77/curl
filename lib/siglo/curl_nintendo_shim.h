#ifndef HEADER_CURL_NINTENDO_SHIM
#define HEADER_CURL_NINTENDO_SHIM

#include <stdio.h>
#include <sys/stat.h>

time_t nnCurlShim_time(time_t* timer);
struct timeval nnCurlShim_get_timeval(struct timeval* timer);
struct hostent* nnCurlShim_gethostbyname(const char* name, int value);

size_t nnCurlShim_fwrite(const void* buffer, size_t size, size_t count, FILE* stream);
int nnCurlShim_fputs(const char* str, FILE* stream);
int nnCurlShim_fflush(FILE* stream);
int nnCurlShim_fputc(int ch, FILE* stream);

int nnCurlShim_fprintf(FILE* stream, char* format, ...);
int nnCurlShim_vfprintf(FILE* stream, const char* format, va_list vlist);
int nnCurlShim_printf(const char* format, ...);
int nnCurlShim_vprintf(const char* format, va_list vlist);

int nnCurlShim_fclose(FILE* stream);
FILE* nnCurlShim_fopen(const char* filename, const char* mode);
size_t nnCurlShim_fread(void* ptr, size_t size, size_t count, FILE* stream);
int nnCurlShim_fseek(FILE* stream, long offset, int origin);
char* nnCurlShim_fgets(char* str, int count, FILE* stream);

int nnCurlShim_fileno(FILE* stream);
int nnCurlShim_fstat(int fd, struct stat* statbuf);
int nnCurlShim_stat(const char* restrict path, struct stat* restrict statbuf);

#endif /* HEADER_CURL_NINTENDO_SHIM */
