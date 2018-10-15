#undef __STRICT_ANSI__
#include "main.h"
#include <iostream>
#include <fstream>


UINT_PTR base;


_declspec(naked) void DLL_EXPORT ASMHandleMessage(){

    //asm("lea eax, [ebp - 0x50]");
    asm("lea eax, [ebp- 0x128 + 0x4]");
    asm("mov eax, [eax]"); //get message
    asm("push eax");
    asm("call [_HandleMessagePtr]");
    asm("cmp eax, 0"); //message ptr
    asm("je 0f");

    asm("1:");
    asm("push eax");
    asm("lea ecx, [ebp - 0x68]");

    asm("mov eax, [_base]");
    asm("add eax, 0xEB60");
    asm("call eax");

    asm("mov ecx, [edi+0x800A14]");
    asm("lea eax, [ebp - 0x38]");
    asm("push eax");
    asm("lea eax, [ebp - 0x68]");
    asm("push eax");

    asm("mov byte ptr [ebp - 0x4], 0x8");
    asm("mov dword ptr [ebp - 0x38], 0x3F800000");
    asm("mov dword ptr [ebp - 0x34], 0x3F25A1CB");
    asm("mov dword ptr [ebp - 0x30], 0x3F800000");
    asm("mov dword ptr [ebp - 0x2C], 0x3F800000");

    asm("mov eax, [_base]");
    asm("add eax, 0x3AB30");
    asm("call eax");

    asm("lea ecx, [ebp - 0x68]");

    asm("mov eax, [_base]");
    asm("add eax, 0x193E50");
    asm("call eax");

    asm("lea ecx, [ebp - 0x28]");
    asm("mov byte ptr [ebp - 0x4], 0x4");

    asm("mov eax, [_base]");
    asm("add eax, 0x193E50");
    asm("call eax");

    asm("mov ecx, [_base]"); //jump to end
    asm("add ecx, 0x7E6BF");
    asm("jmp ecx");


    asm("0:"); //exit normally
    asm("mov eax, [_base]"); //jump back
    asm("add eax, 0x7E621");
    asm("cmp dword ptr [edi + 0x8006CC], 0"); //original comparison
    asm("jmp eax");
}

wchar_t* DLL_EXPORT HandleMessage(wchar_t msg[]){
    wchar_t response[255];
    DWORD entityaddr = (DWORD)(base + 0x36b1c8);
    entityaddr = *(DWORD*)entityaddr;
    entityaddr += 0x39C;
    entityaddr = *(DWORD*)entityaddr;

    long long unsigned int* x = (long long unsigned int*)(entityaddr+0x10);
    long long unsigned int* y = (long long unsigned int*)(entityaddr+0x18);
    long long unsigned int* z = (long long unsigned int*)(entityaddr+0x20);

    if(!wcscmp(msg, L"/coords")){
        swprintf(response, L"X: %llu\nY: %llu\nZ: %llu\n", *x, *y, *z);
        return response;
    }

    else if(!wcscmp(msg, L"/chunks")){
        unsigned int chunkx = *x / 0x1000000;
        unsigned int chunky = *y / 0x1000000;
        swprintf(response, L"X: Chunk %u\nY: Chunk %u\nZ: %llu\n", chunkx, chunky, *z);
        return response;

    }

    else{
        long long unsigned int targetx, targety, targetz;
        if ( swscanf(msg, L"/tp %llu %llu %llu", &targetx, &targety, &targetz) == 3){
            *x = targetx;
            *y = targety;
            *z = targetz;
            swprintf(response, L"Teleporting.", targetx);
            return response;
        }
        else if ( swscanf(msg, L"/tpch %llu %llu", &targetx, &targety) == 2){
            *x = targetx * 0x1000000;
            *y = targety * 0x1000000;
            swprintf(response, L"Teleporting.", targetx);
            return response;
        }

    }

    return NULL;



}
DWORD HandleMessagePtr = (DWORD)&HandleMessage;


void WriteJMP(BYTE* location, BYTE* newFunction){
	DWORD dwOldProtection;
	VirtualProtect(location, 5, PAGE_EXECUTE_READWRITE, &dwOldProtection);
    location[0] = 0xE9; //jmp
    *((DWORD*)(location + 1)) = (DWORD)(( (unsigned INT32)newFunction - (unsigned INT32)location ) - 5);
	VirtualProtect(location, 5, dwOldProtection, &dwOldProtection);
}


extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    base = (UINT_PTR)GetModuleHandle(NULL);
    switch (fdwReason)
    {

        case DLL_PROCESS_ATTACH:
            WriteJMP((BYTE*)(base + 0x7E61A), (BYTE*)&ASMHandleMessage); //This code forces the server to generate the correct chunk

            break;
;
    }
    return TRUE;
}
