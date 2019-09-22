#include "main.h"
#include "cwmods/cwmods.h"
#include <wchar.h>
#include <stdio.h>


float GetMilliseconds(int hour, int minute){
    return (hour * 60 * 60 * 1000) + (minute * 60 * 1000);
}

void CommandsModMessage(wchar_t message[]){
    cube::Game* game = cube::GetGame();
    game->chat_widget->PrintMessage(L"[");
    game->chat_widget->PrintMessage(L"CommandsMod", 255, 140, 0);
    game->chat_widget->PrintMessage(L"] ");
    game->chat_widget->PrintMessage(message);
}

int DotsToMapCoord(long long coord) {
    return (coord - 0x2000000) / (cube::DOTS_PER_BLOCK * cube::BLOCKS_PER_MAP_CHUNK);
}

long long MapCoordToDots(int coord) {
    return ((long long)coord * cube::DOTS_PER_BLOCK * cube::BLOCKS_PER_MAP_CHUNK) + 0x2000000;
}

EXPORT int HandleChat(wchar_t* msg) {
    cube::Game* game = cube::GetGame();
    cube::Host* host = &game->host;
    cube::World* world = &host->world;

    wchar_t response[256];

    int targetx, targety;

    int targetHour, targetMinute;

    if ( swscanf(msg, L"/settime %d:%d", &targetHour, &targetMinute) == 2){
        printf("%ld:%ld\n", targetHour, targetMinute);
        float targetMS = GetMilliseconds(targetHour, targetMinute);

        float maxTime = GetMilliseconds(24, 0);
        float minTime = GetMilliseconds(0, 0);

        if (targetMS > maxTime) targetMS = maxTime;
        else if (targetMS < minTime) targetMS = minTime;

        world->time = targetMS;

        swprintf(response, L"Changing time to %d:%02d.\n", targetHour % 24, targetMinute % 60);
        CommandsModMessage(response);
        return 1;
    }
    else if (!wcscmp(msg, L"/coords")){
        cube::Creature* player = game->GetPlayer();
        swprintf(response, L"World coordinates:\nX: %lld\nY: %lld\nZ: %lld\n", player->position.x, player->position.y, player->position.z);
        CommandsModMessage(response);
        return 1;
    }
    else if ( swscanf(msg, L"/tp %d %d", &targetx, &targety) == 2){
            cube::Creature* player = game->GetPlayer();
            player->position.x = MapCoordToDots(targetx);
            player->position.y = MapCoordToDots(targety);
            swprintf(response, L"Teleporting.\n");
            CommandsModMessage(response);
            return 1;
        }
    return 0;
}
