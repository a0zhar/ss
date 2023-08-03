#include "../include/invoker.h"

NativeArg_s nativeArg;
uint64_t args[30];

void setVectors() {
	while (nativeArg.vectorCount) {
		nativeArg.vectorCount--;

		Vector3* argVector = nativeArg.argVectors[nativeArg.vectorCount];
		Vector4 tempVector = nativeArg.tempVectors[nativeArg.vectorCount];
		argVector->x = tempVector.x;
		argVector->y = tempVector.y;
		argVector->z = tempVector.z;
	}
}

void resetArgs() {
	nativeArg.argCount = 0;
	nativeArg.vectorCount = 0;
	nativeArg.argValues = nativeArg.returnValue = args;
}