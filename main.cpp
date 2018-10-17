#undef __STRICT_ANSI__
#include "main.h"
#include <iostream>


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

wchar_t defaultMessage[1024];
DWORD defaultMessagePtr = (DWORD)&defaultMessage;

char msgObject[255];
DWORD msgObjectPtr = (DWORD)&msgObject;

_declspec(naked) void DLL_EXPORT ASMHandleMessage(){

    asm("mov eax, [_base]");
    asm("add eax, 0x36B1C8");
    asm("mov eax, [eax]"); //eax points to gamecontroller
    asm("mov eax, dword ptr [eax + 0x800A14]"); //eax points to ChatWidget
    asm("mov eax, dword ptr [eax + 0x178]"); //get message size
    asm("push eax");

    asm("lea eax, [ebp - 0x128 + 0x4]");
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

void SetTime(signed int hour, signed int minute){
    DWORD worldTimePtr = (DWORD)(base + 0x36B1C8);
    worldTimePtr = *(DWORD*)worldTimePtr; //cube::GameController
    worldTimePtr += 0x2E4; //cube::World
    worldTimePtr += 0x80015C; //cube::World::time

    unsigned int* worldTime = (unsigned int*) worldTimePtr;
    *worldTime = (hour * 60 * 60 * 1000) + (minute * 60 * 1000);// convert to miliseconds
}

void CommandsModMessage(wchar_t message[]){
    PrintMessage(L"[");
    PrintMessage(L"CommandsMod", 255, 140, 0);
    PrintMessage(L"] ");
    PrintMessage(message);
}
void CommandHelpMessage(wchar_t* command, wchar_t* details){
    PrintMessage(command, 50, 255, 127);
    PrintMessage(L" - ");
    PrintMessage(details);
    PrintMessage(L"\n");
}

bool DLL_EXPORT HandleMessage(wchar_t buf[], unsigned int msg_size){
    wchar_t response[255];
    wchar_t msg[1024] = { 0 };
    memcpy(msg, buf, msg_size * 2); //the message should be null terminated

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
        CommandHelpMessage(L"/help", L"displays this");
        CommandHelpMessage(L"/coords", L"displays your absolute coordinates");
        CommandHelpMessage(L"/chunks", L"displays your coordinates in terms of chunks");
        CommandHelpMessage(L"/tp <x> <y> <z>", L"teleports you in terms of absolute coordinates");
        CommandHelpMessage(L"/tpch <chunk x> <chunk y>", L"teleports you in terms of chunks");
        CommandHelpMessage(L"/move <x> <y> <z>", L"teleports you with relative coordinates");
        CommandHelpMessage(L"/settime <hour>:<minute>", L"Sets the time of day");
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
        long long unsigned int delta_x, delta_y, delta_z;
        signed int targetHour;
        signed int targetMinute;

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

        else if ( swscanf(msg, L"/move %llu %llu %llu", &delta_x, &delta_y, &delta_z) == 3){
            *x += delta_x;
            *y += delta_y;
            *z += delta_z;
            swprintf(response, L"Teleporting.\n");
            CommandsModMessage(response);
            return true;
        }

        else if ( swscanf(msg, L"/settime %d:%d", &targetHour, &targetMinute) == 2){
            SetTime(targetHour, targetMinute);
            swprintf(response, L"Changing time to %d:%02d.\n", targetHour % 24, targetMinute % 60);
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
