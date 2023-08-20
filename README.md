# Serotonin

Serotonin is a customized fork of Ethylamine's PS4-RDR-2-Menu-Base repository. This repository offers a Mod Menu GUI base designed for the PS4 version of Red Dead Redemption 2. The precise purpose of this repository is currently unclear, and it's unlikely that anyone will decipher it in the near future.

My goal is to enhance the menu's efficiency, minimize its susceptibility to bugs, and address various other issues. Moreover, I plan to incorporate thorough documentation throughout the source code. I also intend to create a changelog as well as a markdown file containing any encountered issues, bugs, and so forth within the source code.

## Documentation

The `_main()` function initially checks the PS4 firmware version to ensure it's either 5.05, 6.72, 7.02, or 9.00. If the PS4 firmware isn't 6.72, the function `disable_aslr()` is executed to deactivate Address Space Layout Randomization. Subsequently, the Syscall `execv` is invoked using `patch_ptrace` as an argument. This `patch_ptrace` function executes the following actions if the firmware version is 6.72:

- Patches `dynlib_dlsym`
- Disables ptrace checking
- Deactivates ASLR (Address Space Layout Randomization)

The program then starts searching for the **eboot.bin** executable of the active RDR2 process. If the RDR2 Game Patch is supported, it proceeds to execute the `setupEnvironment()` function and enters a loop until the setup is completed. After the loop concludes, the PS4 Kernel is instructed to sleep for 5 seconds before executing the `startExecution()` function. Once this is done, the process is complete.

**Purpose of the SetupEnvironment Function**: Following these steps, the program proceeds to write the struct variable "gtaVars" and its members—such as "allocationNeeded," "executableSpace," and "dataSpace"—to the game memory. The intention is to also write the "nativeHook" function into the game memory, commencing from the specified "PayloadAddress" offset. However, there's uncertainty regarding the success of this operation. Subsequently, a sequence of bytes, presumably representing assembly code found within a comment block under the "SyscallBytes" macro or its contents, is written to the game memory, beginning at the "SyscallAddress" offset. This process is repeated for another sequence of bytes representing assembly code under the "HookBytes" macro, starting from the "HookAddress" offset.

**Purpose of StartExecution Function**: After the "setupEnvironment" function has been executed, the "startExecution" function is called. This function aims to attach to the RDR2 game process, read a specified number of bytes (size of a "void*") from the game memory starting at the offset of "&gtaVars->executableSpace," and store this read data in a locally defined "executableSpace" variable. Following this, it writes the number of bytes equivalent to the value of "gtaPayloadSize" from the contents of the "gtaPayload" array (likely containing the raw data of "GTAPayload.bin") to the game memory, starting from the "executableSpace" offset. After completing this task, the function detaches from the game process. This procedure seemingly aims to write the "GTAPayload.bin" file, which contains the menu section encompassing the GUI, native functions, etc., into the game memory. This enables users to access and interact with the in-game menu GUI. It's important to note that the source code for the menu GUI is located in a separate folder and is compiled into a distinct ".bin" file compared to the primary "main" binary file responsible for game preparation.

---

Special thanks to [Ethylamine](https://github.com/ethylamine/PS4-RDR-2-Menu-Base) for creating this base :)
