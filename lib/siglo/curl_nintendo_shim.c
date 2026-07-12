#include "curl_setup.h"

#include "curl_nintendo_shim.h"

#include "curl_nintendo_allocator.h"
#include "nn/diag.h"
#include "nn/socket.h"
#include "nn/time.h"

time_t nnCurlShim_time(time_t* timer)
{
    time_t time = nnosGetSystemTick() / nnosGetSystemTickFrequency();
    if (timer)
        *timer = time;

    return time;
}

struct timeval nnCurlShim_get_timeval(struct timeval* timer)
{
    Tick tick = nnosGetSystemTick();
    struct timeval tv = {.tv_sec = tick / nnosGetSystemTickFrequency(),
                         .tv_usec = (nnosConvertTickToNanoSeconds(tick) / 1000) % 1000000};

    if (timer)
        *timer = tv;

    return tv;
}

struct hostent* nnCurlShim_gethostbyname(const char* name, int value)
{
    return nnsocketGetHostByNameCancel(name, value);
}

size_t nnCurlShim_fwrite(const void* buffer, size_t size, size_t count, FILE* stream)
{
    if (stream != stdout && stream != stderr)
    {
#line 88
        NN_ABORT_UNSUPPORTED();
    }

    size_t alloc_size = count * size;
    if (alloc_size == 0)
        return 0;

    void* ptr = Curl_SigloMalloc(alloc_size + 1);
#line 103
    NN_ASSERT(ptr != NULL, "unsupported");

    size_t clear_size = alloc_size != (size_t)-1 ? 1 : 0;
    memset(ptr + alloc_size, 0, clear_size);

    memcpy(ptr, buffer, alloc_size);
    Curl_SigloFree(ptr);

    return alloc_size;
}

int nnCurlShim_fputs(const char* str, FILE* stream)
{
    if (stream != stdout && stream != stderr)
    {
#line 129
        NN_ABORT_UNSUPPORTED();
    }

    if (!str)
        return -1;

    return strlen(str);
}

int nnCurlShim_fflush(FILE* stream)
{
    if (stream != stdout && stream != stderr)
    {
#line 160
        NN_ABORT_UNSUPPORTED();
    }

    return 0;
}

int nnCurlShim_fputc(int ch, FILE* stream)
{
    if (stream != stdout && stream != stderr)
    {
#line 177
        NN_ABORT_UNSUPPORTED();
    }

    return ch;
}

int nnCurlShim_fprintf(FILE* stream, char* format, ...)
{
    if (stream != stdout && stream != stderr)
    {
#line 205
        NN_ABORT_UNSUPPORTED();
    }

    return -1;
}

int nnCurlShim_vfprintf(FILE* stream, const char* format, va_list vlist)
{
    if (stream != stdout && stream != stderr)
    {
#line 230
        NN_ABORT_UNSUPPORTED();
    }

    return -1;
}

int nnCurlShim_printf(const char* format, ...)
{
    return -1;
}

int nnCurlShim_vprintf(const char* format, va_list vlist)
{
    return -1;
}

int nnCurlShim_fclose(FILE* stream)
{
#line 287
    NN_ABORT_UNSUPPORTED();
}

FILE* nnCurlShim_fopen(const char* filename, const char* mode)
{
#line 302
    NN_ABORT_UNSUPPORTED();
}

size_t nnCurlShim_fread(void* ptr, size_t size, size_t count, FILE* stream)
{
#line 317
    NN_ABORT_UNSUPPORTED();
}

int nnCurlShim_fseek(FILE* stream, long offset, int origin)
{
#line 332
    NN_ABORT_UNSUPPORTED();
}

char* nnCurlShim_fgets(char* str, int count, FILE* stream)
{
#line 347
    NN_ABORT_UNSUPPORTED();
}

int nnCurlShim_fileno(FILE* stream)
{
#line 362
    NN_ABORT_UNSUPPORTED();
}

int nnCurlShim_fstat(int fd, struct stat* statbuf)
{
#line 377
    NN_ABORT_UNSUPPORTED();
}

int nnCurlShim_stat(const char* restrict path, struct stat* restrict statbuf)
{
#line 392
    NN_ABORT_UNSUPPORTED();
}
