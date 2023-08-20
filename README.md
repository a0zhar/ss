# Serotonin
A custom fork of ethylamine's PS4-RDR-2-Menu-Base Repo, which is a Mod Menu GUI base for the PS4 version of Red Dead Redemption 2. The purpose of this repository is not clear to me at the moment, and I doubt anyone will find it anytime soon. 

I will attempt to make the menu more efficient, less prone to bugs, and address other issues. Additionally, I will add documentation throughout the source code. I also plan to create a changelog as well as a markdown file containing any issues, bugs, etc., that I encounter in the source code.


## Documentation
The _main() function first check ps4 firmware version to make sure it's either 5.05, 6.72, 7.02, or 9.00. If the PS4 firmware isn't 6.72 it will run the function `disable_aslr()` to disable the Address Space Layout Randomization. Then it will call the Syscall execv using a patch_ptrace as argument, this patch_ptrace function will then if firmware is 6.72, do following:
  - Patch dynlib_dlsym
  - Disable ptrace checking
  - Disable ASLR (Address Space Layout Randomization)
Then It will begin to look for the **eboot.bin** executable of Running RDR2 Process, once its process is found it will check if the RDR2 Game Patch is supported. If it is it will then procede to run `setupEnvironment()` function, then enter a loop until setup is done. When loop is done, it will tell Ps4 Kernel to sleep 5 seconds, before running `startExecution()` function. Then its done.
**Purpose of the SetupEnvironment Function**: Following these steps, the program proceeds to write the struct variable "gtaVars" and its members—such as "allocationNeeded," "executableSpace," and "dataSpace"—to the game memory. The intention is to also write the "nativeHook" function into the game memory, commencing from the specified "PayloadAddress" offset. However, there's uncertainty regarding the success of this operation. Subsequently, a sequence of bytes, presumably representing assembly code found within a comment block under the "SyscallBytes" macro or its contents, is written to the game memory, beginning at the "SyscallAddress" offset. This process is repeated for another sequence of bytes representing assembly code under the "HookBytes" macro, starting from the "HookAddress" offset.

**Purpose of StartExecution Function**: After the "setupEnvironment" function has been executed, the "startExecution" function is called. This function aims to attach to the RDR2 game process, read a specified number of bytes (size of a "void*") from the game memory starting at the offset of "&gtaVars->executableSpace," and store this read data in a locally defined "executableSpace" variable. Following this, it writes the number of bytes equivalent to the value of "gtaPayloadSize" from the contents of the "gtaPayload" array (likely containing the raw data of "GTAPayload.bin") to the game memory, starting from the "executableSpace" offset. After completing this task, the function detaches from the game process. This procedure seemingly aims to write the "GTAPayload.bin" file, which contains the menu section encompassing the GUI, native functions, etc., into the game memory. This enables users to access and interact with the in-game menu GUI. It's important to note that the source code for the menu GUI is located in a separate folder and is compiled into a distinct ".bin" file compared to the primary "main" binary file responsible for game preparation.


---
Special thanks to <a href="https://github.com/ethylamine/PS4-RDR-2-Menu-Base">@Ethylamine</a> for making this Base :)
