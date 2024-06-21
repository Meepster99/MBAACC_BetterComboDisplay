
#include <windows.h>
#include <cstring> 
#include <cstdio>
#include <psapi.h>
#include <cstdint>
#include <vector> 
#include <cmath>
#include <time.h>
#include <stdlib.h>

static_assert(sizeof(int*) == 4, "COMPILE AS 32BIT");


typedef unsigned char u8;

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(value, min_val, max_val) MAX(MIN((value), (max_val)), (min_val))

#define ATTACKDISPLAYADDR		0x00478bc0
#define ATTACKDISPLAYTOGGLE		0x005595b8

#define INPUTDISPLAYADDR		0x00479460
#define INPUTDISPLAYTOGGLE		0x005585f8

#define PLAYER1STATEADDR		0x00555140
#define PLAYER2STATEADDR		0x00555C3C

// thanks to fang for address help. 0x00400000 is the offset between cheat engine and exe addrs

#define P1CHARNUM				0x0074D8F8
#define P1CHARMOON				0x0074D900

// not as confident in these.
#define P2CHARNUM				0x0074D91C
#define P2CHARMOON				0x0074D924

float randFloat(float Min, float Max) {
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

void doPatch(unsigned addr, const u8* code, unsigned length) {
	
	LPVOID tempAddr = reinterpret_cast<LPVOID>(addr); 
	
	DWORD oldProtect;
	VirtualProtect(tempAddr, length, PAGE_EXECUTE_READWRITE, &oldProtect);
	
	memcpy(tempAddr, code, length);
	
	VirtualProtect(tempAddr, length, oldProtect, NULL);
}

void patchFunction(DWORD patchAddr, DWORD newAddr) {
	
	u8 jumpCode[] = {0xE9, 0x00, 0x00, 0x00, 0x00};
	
	DWORD funcOffset = (DWORD)newAddr - patchAddr - 5;
	
	*(unsigned*)(&jumpCode[1]) = funcOffset;

	doPatch(patchAddr, jumpCode, sizeof(jumpCode));	
}

void patchByte(DWORD addr, const u8 code) {
	
	u8 temp[] = {code};

	doPatch(addr, temp, 1);
}

// -----

void threadFunc() {
	
	srand(time(NULL));
	
	HMODULE hModule = GetModuleHandle("hook.dll"); // cccaster hook offset
	
	if(hModule == NULL) {
		system("echo hook dll addr was null, what? >> ReduceAttackInfoDisplay.log");
	}
	
	DWORD casterOffset = (DWORD)hModule;
	
	patchByte(INPUTDISPLAYTOGGLE, 1);
	
	//LPDIRECT3DDEVICE9 d3dDevice;
	//
	//d3dDevice->SetStreamSource(0, vb, 0, sizeof(CUSTOMVERTEX));
	//d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	//d3dDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, 1);
	
	//HookDirectX();
	
}

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	(void)hinstDLL;
	(void)lpReserved;
	
	if(fdwReason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)threadFunc, 0, 0, 0);
	}
	
	return TRUE;
}

