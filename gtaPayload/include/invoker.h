#pragma once
#ifndef _INVOKER_H_
#define _INVOKER_H_

#include "types.h"
#include <utility>

struct NativeArg_s {
	uint64_t* returnValue;
	uint32_t argCount;
	uint8_t padding1[4];
	uint64_t* argValues;
	uint32_t vectorCount;
	uint8_t padding2[4];
	Vector3* argVectors[4];
	Vector4 tempVectors[4];
};

extern NativeArg_s nativeArg;

extern void resetArgs();
extern void setVectors();

template<typename T>
inline void pushArg(T value) {
	*(T*)&nativeArg.argValues[nativeArg.argCount] = value;
	nativeArg.argCount++;
}

template<typename R>
inline R getReturn() {
	return *(R*)&nativeArg.returnValue[0];
}

template<typename N, typename... A>
N invoke(uint64_t nativeAddress, A &&... args) {
	resetArgs();
	int dummy[] = { 0, ((void)pushArg(std::forward<A>(args)), 0) ... };
	((void(*)(NativeArg_s*))(void*)nativeAddress)(&nativeArg);
	setVectors();
	return getReturn<N>();
}
#endif