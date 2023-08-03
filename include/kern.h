#pragma once
#ifndef _KERN_H_
#define _KERN_H_

#include "type.h" 
#include "ps4.h"

#define KERN_PTRACE_CHECK_505 0x30D9AA
#define KERN_PTRACE_CHECK_672 0x10F879
#define KERN_PTRACE_CHECK_755 0x361CF5
#define KERN_PTRACE_CHECK_900 0x41F4E5

#define KERN_DISABLE_ASLR_672 0x3CECE1

#define X86_CR0_WP (1 << 16)

extern uint64_t _KernelBase;
extern uint16_t _FW;

struct kernel_payload_args {
    void* syscall_handler;
    uint64_t user_arg;
};

extern void patch_ptrace(void);
#endif