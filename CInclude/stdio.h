#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>
#include <_size_t.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define EOF (-1)

#ifdef __GNUC__
#define restrict __restrict__
#endif

// dummy
typedef struct FileDescriptor
{
} FILE;

typedef long fpos_t;

int remove(const char* filename);
int rename(const char* from, const char* to);
FILE* tmpfile(void);
char* tmpnam(char* s);
int fclose(FILE* stream);
int fflush(FILE* stream);
FILE* fopen(const char* restrict filename, const char* restrict mode);
FILE* freopen(const char* restrict filename, const char * restrict mode,
              FILE * restrict stream);
void setbuf(FILE* restrict stream, char* restrict buf);
int setvbuf(FILE* restrict stream, char* restrict buf, int mode, size_t size);
int fprintf(FILE* restrict stream, const char* restrict format, ...);
int fscanf(FILE* restrict stream, const char * restrict format, ...);
int printf(const char* restrict format, ...)  __attribute__((__format__ (__printf__, 1, 2)));
int scanf(const char* restrict format, ...);
int snprintf(char* restrict s, size_t n, const char* restrict format, ...);    // C99
int sprintf(char* restrict s, const char* restrict format, ...);
int sscanf(const char* restrict s, const char* restrict format, ...);
int vfprintf(FILE* restrict stream, const char* restrict format, va_list arg);
int vfscanf(FILE* restrict stream, const char* restrict format, va_list arg);  // C99
int vprintf(const char* restrict format, va_list arg);
int vscanf(const char* restrict format, va_list arg);                          // C99
int vsnprintf(char* restrict s, size_t n, const char* restrict format,         // C99
              va_list arg);
int vsprintf(char* restrict s, const char* restrict format, va_list arg);
int vsscanf(const char* restrict s, const char* restrict format, va_list arg); // C99
int fgetc(FILE* stream);
char* fgets(char* restrict s, int n, FILE* restrict stream);
int fputc(int c, FILE* stream);
int fputs(const char* restrict s, FILE* restrict stream);
int getc(FILE* stream);
int getchar(void);
char* gets(char* s);  // removed in C++14
int putc(int c, FILE* stream);
int putchar(int c);
int puts(const char* s);
int ungetc(int c, FILE* stream);
size_t fread(void* restrict ptr, size_t size, size_t nmemb,
             FILE* restrict stream);
size_t fwrite(const void* restrict ptr, size_t size, size_t nmemb,
              FILE* restrict stream);
int fgetpos(FILE* restrict stream, fpos_t* restrict pos);
int fseek(FILE* stream, long offset, int whence);
int fsetpos(FILE*stream, const fpos_t* pos);
long ftell(FILE* stream);
void rewind(FILE* stream);
void clearerr(FILE* stream);
int feof(FILE* stream);
int ferror(FILE* stream);
void perror(const char* s);

#ifdef __cplusplus
}
#endif

#endif
