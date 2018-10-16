#undef __STRICT_ANSI__
#include "main.h"
#include <iostream>
#include <fstream>


UINT_PTR base;

class Color{
    public:
    float red, green, blue, alpha;
    Color(float r, float g, float b, float a){
        red = r;
        green = g;
        blue = b;
        alpha = a;
    }

};
Color defaultColor = Color(1.0, 1.0, 1.0, 1.0);
DWORD defaultColorPtr = (DWORD)&defaultColor;

wchar_t defaultMessage[255];
DWORD defaultMessagePtr = (DWORD)&defaultMessage;

char msgObject[255];
DWORD msgObjectPtr = (DWORD)&msgObject;

_declspec(naked) void DLL_EXPORT ASMHandleMessage(){

    //asm("lea eax, [ebp - 0x50]");
    asm("lea eax, [ebp- 0x128 + 0x4]");
    asm("mov eax, [eax]"); //get message
    asm("push eax");
    asm("call [_HandleMessagePtr]");
    asm("cmp eax, 0"); //message ptr
    asm("je 0f");

    asm("1:");
    asm("mov ecx, [_base]"); //jump to end
    asm("add ecx, 0x7E6BF");
    asm("jmp ecx");


    asm("0:"); //exit normally
    asm("mov eax, [_base]"); //jump back
    asm("add eax, 0x7E621");
    asm("cmp dword ptr [edi + 0x8006CC], 0"); //original comparison
    asm("jmp eax");
}

void DLL_EXPORT ASMPrintMessage(){

    asm("push [_defaultMessagePtr]");
    asm("mov ecx, [_msgObjectPtr]");

    asm("mov eax, [_base]");
    asm("add eax, 0x0EB60");
    asm("call eax"); //call some message constructing function

    asm("mov ecx, [_base]");
    asm("add ecx, 0x36B1C8");
    asm("mov ecx, [ecx]"); //ecx points to gamecontroller
    asm("mov ecx, [ecx + 0x800A14]"); //ecx points to chatwidget

    asm("push [_defaultColorPtr]");
    asm("push [_msgObjectPtr]");
    asm("mov edx, [_base]");
    asm("add edx, 0x3AB30");
    asm("call edx"); //prints message


    asm("mov ecx, [_msgObjectPtr]");

    asm("mov eax, [_base]");
    asm("add eax, 0x193E50");
    asm("call eax"); //destructor for that message object

}

void DLL_EXPORT PrintMessage(wchar_t message[]){

    wcsncpy(defaultMessage, message, 255);
    defaultColor.red = 1.0;
    defaultColor.blue = 1.0;
    defaultColor.green = 1.0;
    defaultColor.alpha = 1.0;
    ASMPrintMessage();
}
void DLL_EXPORT PrintMessage(wchar_t message[], int r, int g, int b){
    wcsncpy(defaultMessage, message, 255);
    defaultColor.red = r / 255.0;
    defaultColor.green = g / 255.0;
    defaultColor.blue = b / 255.0;
    ASMPrintMessage();
}


void CommandsModMessage(wchar_t message[]){
    PrintMessage(L"[");
    PrintMessage(L"CommandsMod", 255, 140, 0);
    PrintMessage(L"] ");
    PrintMessage(message);
}

bool DLL_EXPORT HandleMessage(wchar_t msg[]){
    wchar_t response[255];
    DWORD entityaddr = (DWORD)(base + 0x36b1c8);
    entityaddr = *(DWORD*)entityaddr;
    entityaddr += 0x39C;
    entityaddr = *(DWORD*)entityaddr;

    long long unsigned int* x = (long long unsigned int*)(entityaddr+0x10);
    long long unsigned int* y = (long long unsigned int*)(entityaddr+0x18);
    long long unsigned int* z = (long long unsigned int*)(entityaddr+0x20);

    //Display commands
    if (!wcscmp(msg, L"/help")){
        CommandsModMessage(L"List of commands:\n");
        PrintMessage(L"/help - displays this\n");
        PrintMessage(L"/coords - displays your absolute coordinates\n");
        PrintMessage(L"/chunks - displays your coordinates in terms of chunks\n");
        PrintMessage(L"/tp <x> <y> <z> - teleports you in terms of absolute coordinates\n");
        PrintMessage(L"/tpch <chunk x> <chunk y> - teleports you in terms of chunks\n");
        return true;
    }
    else if(!wcscmp(msg, L"/coords")){
        swprintf(response, L"World coordinates:\nX: %llu\nY: %llu\nZ: %llu\n", *x, *y, *z);
        CommandsModMessage(response);
        return true;
    }

    else if(!wcscmp(msg, L"/chunks")){
        unsigned int chunkx = *x / 0x1000000;
        unsigned int chunky = *y / 0x1000000;
        swprintf(response, L"Chunk coordinates:\nX: Chunk %u\nY: Chunk %u\nZ: %llu\n", chunkx, chunky, *z);
        CommandsModMessage(response);
        return true;

    }

    //Argument commands
    else{
        long long unsigned int targetx, targety, targetz;

        if ( swscanf(msg, L"/tp %llu %llu %llu", &targetx, &targety, &targetz) == 3){
            *x = targetx;
            *y = targety;
            *z = targetz;
            swprintf(response, L"Teleporting.\n");
            CommandsModMessage(response);
            return true;
        }

        else if ( swscanf(msg, L"/tpch %llu %llu", &targetx, &targety) == 2){
            *x = targetx * 0x1000000;
            *y = targety * 0x1000000;
            swprintf(response, L"Teleporting.\n");
            CommandsModMessage(response);
            return true;
        }

    }

    return false;



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
            WriteJMP((BYTE*)(base + 0x7E61A), (BYTE*)&ASMHandleMessage);

            break;
;
    }
    return TRUE;
}
