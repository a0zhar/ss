
#include "ps4.h"
#include "../include/kern.h"
#include "../include/proc.h"
#include "../include/gta.h"
#include "../include/config.h"

extern char gtaPayload[];
extern int gtaPayloadSize;

uint64_t _KernelBase;
uint16_t _FW;

int gamePID;
int GameVersion;

#define SendNotification(message) sceSysUtilSendSystemNotificationWithText(222, message)

// Function to check if the game version is supported
int isGamePatchSupported() {
	int isSupported = 0; // 0 = No, 1 = Yes
	procAttach(gamePID);

	uint8_t testBytes;
	procReadBytes(gamePID, GameCheckAddress, (void*)&testBytes, sizeof(testBytes));

	if (testBytes == 0x11) {
		SendNotification("Detected RDR2! Unsupported Version (1.00)");
		GameVersion = 0x64;
	} else if (testBytes == 0x00) {
		SendNotification("Detected RDR2! Supported Version (1.29)");
		GameVersion = 0x81;
		isSupported = 1;
	} else {
		SendNotification("Couldn't Detect RDR2, Try again.");
	}

	procDetach(gamePID);
	return isSupported;
}
int isSetupFinished() {
	procAttach(gamePID);
	int allocationNeeded;
	procReadBytes(gamePID, &gtaVars->allocationNeeded, &allocationNeeded, sizeof(allocationNeeded));
	procDetach(gamePID);
	return !allocationNeeded;
}

// Function to set up the environment for the mod
void setupEnvironment() {
	procAttach(gamePID);

	int allocationNeeded = TRUE;
	//int executableSize = (int)ceil((double)gtaPayloadSize / 0x4000) * 0x4000;
	int executableSize = (gtaPayloadSize + 0x4000 - 1) & ~(0x4000 - 1);
	void* buffer = NULL;

	procWriteBytes(gamePID, &gtaVars->allocationNeeded, &allocationNeeded, sizeof(allocationNeeded));
	procWriteBytes(gamePID, &gtaVars->executableSpace, &buffer, sizeof(buffer));
	procWriteBytes(gamePID, &gtaVars->dataSpace, &buffer, sizeof(buffer));
	procWriteBytes(gamePID, &gtaVars->allocationSize, &executableSize, sizeof(executableSize));
	procWriteBytes(gamePID, PayloadAddress, nativeHook, 0x3000);

	// ASM System Call Bytes
	uint8_t syscallASM[] = {
		0x48, 0x89, 0xF8, 0x48, 0x89, 0xF7, 0x48, 0x89, 0xD6,
		0x48, 0x89, 0xCA, 0x4D, 0x89, 0xC2, 0x4D, 0x89, 0xC8,
		0x4C, 0x8B, 0x4C, 0x24, 0x08, 0x0F, 0x05, 0xC3
	};

	procWriteBytes(gamePID, SyscallAddress, syscallASM, sizeof(syscallASM));

	// If the Game version is 1.29 (0x81 = 129)
	// we write the hook assembly code
	if (GameVersion == 0x81) {
		SendNotification("Writing Hook");
		uint8_t hookASM[] = { 0x48, 0x8D, 0x15, 0x48, 0x7C, 0x43, 0x05 };
		procWriteBytes(gamePID, HookAddress, hookASM, sizeof(hookASM));
	} else {
		SendNotification("Unsupported game version detected. Use 1.29 patch only");
	}

	procDetach(gamePID);
}
void startExecution() {
	procAttach(gamePID);
	void* executableSpace;
	procReadBytes(gamePID, &gtaVars->executableSpace, &executableSpace, sizeof(executableSpace));
	procWriteBytes(gamePID, executableSpace, gtaPayload, gtaPayloadSize);
	procDetach(gamePID);
}

int _main(void) {
	initKernel();
	initLibc();
	jailbreak();
	initSysUtil();

	_KernelBase = get_kernel_base();
	_FW = get_firmware();

	// If the PS4 Firmware isn't either (5.05, 6.72, 7.55, or 9.00)
	// we exit the main function early
	if (_FW != 505 && _FW != 672 &&
		_FW != 755 && _FW != 900)
		return 0;

	if (_FW != 672) disable_aslr();

    // System Call execv (11), 
	syscall(11, patch_ptrace);

    // Let the user know, that they should launch the game
	SendNotification(
		"Serotonin "SEROTONIN_VERSION" Loaded!\n"
		"Please launch RDR2..."
	);

	// Try and find the eboot.bin process of RDR2
	gamePID = findProcess("eboot.bin");

	sceKernelSleep(3); // sleep 3 seconds 

	if (!isGamePatchSupported()) 
		return 0;
	
	SendNotification("Setting Up Environment");
	setupEnvironment();

	// Enter a loop where it will continue until setup is finished
	do{} while (!isSetupFinished());
	sceKernelSleep(5);
	startExecution();

	SendNotification(
		"Serotonin Menu Loaded!\n"
		"To open the menu press (R1 + R3), enjoy!"
	);

	return 0;
}
