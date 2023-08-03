#include "../include/gta.h"
#include "ps4.h"

static uint64_t(*_syscall)(int num, ...) = (void*)SyscallAddress;

/* Function to perform the mmap system call
@param addr Starting address (if NULL, the system will choose it)
@param len Number of bytes to be mapped
@param prot Protections (Region Accessibility)
@param flags Specifies the type of the mapped object
@param fd If a value is provided (the mapping will occur from the object described by fd)
@param offset If fd value is provided, this will be used as the starting offset
@return Upon success, a pointer to the mapped region is returned. However, if an error occurred,
        the value of MAP_FAILED will be returned instead. */
void* ps4_mmap(void* addr, size_t len, int prot, int flags, int fd, off_t offset) {
    return (void*)_syscall(477, addr, len, prot, flags, fd, offset);
}

int nativeHook() {
    if (gtaVars->allocationNeeded) {
        if (!gtaVars->executableSpace) {
            // Allocate memory for executable space using mmap
            gtaVars->executableSpace = ps4_mmap(
                0,
                gtaVars->allocationSize,
                PROT_READ | PROT_WRITE | PROT_EXEC,
                MAP_PRIVATE | MAP_ANONYMOUS,
                -1, 0
            );
        } else if (!gtaVars->dataSpace) {
            // Allocate memory for data space using mmap
            gtaVars->dataSpace = ps4_mmap(
                0,
                gtaVars->allocationSize,
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS,
                -1, 0
            );
        } else {
            // Set the first byte of executableSpace to 0xC3 (retn)
            *(char*)gtaVars->executableSpace = 0xC3;
            gtaVars->allocationNeeded = FALSE;
        }
    } else {
        // Call the function located at executableSpace
        ((void(*)())gtaVars->executableSpace)();
    }

    return TRUE;
}