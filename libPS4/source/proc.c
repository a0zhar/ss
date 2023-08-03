#include "../include/kernel.h"
#include "../include/libc.h"
#include "../include/syscall.h"
#include "../include/proc.h"

#define NO_PROCESS 0

int findProcess(char *procName) {
	// Define the Management Information Base (MIB)
	// for sysctl to get process information.
	int mib[] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL };
	// Stores The Process-ID (PID)
	int ProcPID = NO_PROCESS;
	void *dump = NULL;
	size_t len = 0;

	// Loop until the process is found
	while (ProcPID == NO_PROCESS) {
		// Get the required buffer size for process information
		if (sysctl(mib, 3, NULL, &len, NULL, 0) != -1) {
			if (len > 0) {
				// Allocate memory to store the process information
				if ((dump = malloc(len)) == NULL) {
					// In case of failure, we return NO_PROCESS
					return NO_PROCESS;
				}

				// Get the process information
				if (sysctl(mib, 3, dump, &len, NULL, 0) != -1) {
					int structSize = *(int *)dump;
					for (size_t i = 0; i < (len / structSize); i++) {
						struct kinfo_proc *procInfo = (struct kinfo_proc *)((char *)dump + (i * structSize));
						// Compare the process name with the provided name
						if (!strcmp(procInfo->name, procName)) {
							// If found, set the process PID and exit the loop
							ProcPID = procInfo->pid;
							break;
						}
					}
				}
				// Free memory allocated for storage of Process information
				free(dump);
			}
		}
		// Sleep 1 Second
		sceKernelSleep(1);
	}
	// Return the Process ID of found Process
	return ProcPID;
}

void closeProcess(char *procname) {
	int pid = findProcess(procname);
	syscall(37, pid, SIGTERM);
}

void killProcess(char *procname) {
	int pid = findProcess(procname);
	syscall(37, pid, SIGKILL);
}

int ptrace(int req, int pid, void *addr, int data);
SYSCALL(ptrace, 26);

void PTRACE(int req, int pid, void *addr, int data) {
	while (ptrace(req, pid, addr, data)) {
		;
	}
}

void procAttach(int pid) {
	PTRACE(PT_ATTACH, pid, NULL, NULL);
}

void procDetach(int pid) {
	PTRACE(PT_DETACH, pid, NULL, NULL);
}

void procReadBytes(int pid, void *offset, void *buffer, size_t len) {
	struct ptrace_io_desc pt_desc;
	pt_desc.piod_op = PIOD_READ_D;
	pt_desc.piod_addr = buffer;
	pt_desc.piod_offs = offset;
	pt_desc.piod_len = len;
	PTRACE(PT_IO, pid, &pt_desc, NULL);
}

void procWriteBytes(int pid, void *offset, void *buffer, size_t len) {
	struct ptrace_io_desc pt_desc;
	pt_desc.piod_op = PIOD_WRITE_D;
	pt_desc.piod_addr = buffer;
	pt_desc.piod_offs = offset;
	pt_desc.piod_len = len;
	PTRACE(PT_IO, pid, &pt_desc, NULL);
}