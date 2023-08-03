#include "../include/lib.h"

//uint64_t (*syscall)(int num, ...) = (void*)0x537B964;
//uint64_t (*syscall)(int num, ...) = (uint64_t (*)(int, ...))(void*)0x6270664;
uint64_t (*syscall)(int num, ...) = (void*)0x6270664;

// 1.29 = 0x6270664
#define getFuncAddressByName(module, name) syscall(591, module, #name, &name);

// Define function pointers here (without initialization)
void *(*malloc)(size_t size);
void (*free)(void *ptr);
void *(*calloc)(size_t num, size_t size);
void *(*realloc)(void* ptr, size_t size);
void *(*memset)(void *destination, int value, size_t num);
void *(*memcpy)(void *destination, const void *source, size_t num);
int (*memcmp)(const void *s1, const void *s2, size_t n);
char *(*strcpy)(char *destination, const char *source);
char *(*strncpy)(char *destination, const char *source, size_t num);
char *(*strcat)(char *dest, const char *src);
char *(*strncat)(char *dest, const char *src, size_t n);
size_t (*strlen)(const char *s);
int (*strcmp)(const char *s1, const char *s2);
int (*strncmp)(const char *s1, const char *s2, size_t n);
int (*sprintf)(char *str, const char *format, ...);
int (*snprintf)(char *str, size_t size, const char *format, ...);
int (*sscanf)(const char *str, const char *format, ...);
char *(*strchr)(const char *s, int c);
char *(*strrchr)(const char *s, int c);
char *(*strstr)(char *str1, char *str2);
int (*atoi)(const char *str);
double (*atof)(const char *str);

void initLibs() {
    int libcHandle;
    syscall(594, "libSceLibcInternal.sprx", 0, &libcHandle, 0);

    getFuncAddressByName(libcHandle, malloc);
    getFuncAddressByName(libcHandle, free);
    getFuncAddressByName(libcHandle, calloc);
    getFuncAddressByName(libcHandle, realloc);
    getFuncAddressByName(libcHandle, memset);
    getFuncAddressByName(libcHandle, memcpy);
    getFuncAddressByName(libcHandle, memcmp);
    getFuncAddressByName(libcHandle, strcpy);
    getFuncAddressByName(libcHandle, strncpy);
    getFuncAddressByName(libcHandle, strcat);
    getFuncAddressByName(libcHandle, strncat);
    getFuncAddressByName(libcHandle, strlen);
    getFuncAddressByName(libcHandle, strcmp);
    getFuncAddressByName(libcHandle, strncmp);
    getFuncAddressByName(libcHandle, sprintf);
    getFuncAddressByName(libcHandle, snprintf);
    getFuncAddressByName(libcHandle, sscanf);
    getFuncAddressByName(libcHandle, strchr);
    getFuncAddressByName(libcHandle, strrchr);
    getFuncAddressByName(libcHandle, strstr);
    getFuncAddressByName(libcHandle, atoi);
    getFuncAddressByName(libcHandle, atof);
}
