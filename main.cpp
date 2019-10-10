#include "main.h"
#include "cwmods/cwmods.h"
#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

int serverAccessState = 0;
bool P2PRequestLogging = false;
bool blacklisting = false;
std::map<std::string, std::vector<long long>> home_map;

float GetMilliseconds(int hour, int minute) {
    return (hour * 60 * 60 * 1000) + (minute * 60 * 1000);
}

void CommandsModMessage(const wchar_t* message) {
    cube::Game* game = cube::GetGame();
    game->PrintMessage(L"[");
    game->PrintMessage(L"CommandsMod", 255, 140, 0);
    game->PrintMessage(L"] ");
    game->PrintMessage(message);
}

int DotsToMapCoord(long long coord) {
    return (coord - 0x2000000) / (cube::DOTS_PER_BLOCK * cube::BLOCKS_PER_MAP_CHUNK);
}

long long MapCoordToDots(int coord) {
    return ((long long)coord * cube::DOTS_PER_BLOCK * cube::BLOCKS_PER_MAP_CHUNK) + 0x2000000;
}

// https://stackoverflow.com/questions/9435385/split-a-string-using-c11
std::vector<std::string> split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

void InitHomeMap(){
    std::ifstream myfile("Mods\\CommandsMod\\home.txt");
    if(!myfile.is_open()) {
        return;
    }
    std::string line;

    while (getline(myfile, line)) {
        std::vector<std::string> elements = split(line, ' ');
        if (elements.size() == 4) {
            std::string name = elements.at(0);
            std::string x = elements.at(1);
            std::string y = elements.at(2);
            std::string z = elements.at(3);
            try {
                std::vector<long long> coords {std::stoll(x), std::stoll(y), std::stoll(z)};
                home_map[name] = coords;
            } catch ( ... ) {
                continue;
            }
        }
    }
}

void SetHomeMap(std::string alias, long long x, long long y, long long z) {
    std::vector<long long> coords {x, y, z};
    home_map[alias] = coords;
    std::ofstream outfile("Mods\\CommandsMod\\home.txt");
    if(outfile.is_open()) {
        for(auto item = home_map.begin();item != home_map.end(); item++) {
            auto value = item->second;
            outfile<<item->first<<' '<<item->second[0]<<' '<<item->second[1]<<' '<<item->second[2]<<std::endl;
        }
        outfile.close();
    }
}

std::vector<long long> GetHomePosition(std::string alias) {
    auto iter = home_map.find(alias);
    if(iter != home_map.end()) {
        return iter->second;
    }
    return std::vector<long long>();
}

long long GetSteamIDFromAlias(std::string alias) {
    std::ifstream myfile("Mods\\CommandsMod\\join.txt");
    if(!myfile.is_open()) {
        return -1;
    }
    std::string line;

    while (getline(myfile, line)) {
        std::vector<std::string> elements = split(line, ' ');
        if (elements.size() == 2) {
            std::string name = elements.at(0);
            if (name.compare(alias)) {
                continue;
            }
            std::string strID = elements.at(1);
            long long ID;
            try {
                ID = std::stoll(strID, nullptr);
            } catch ( ... ) {
                continue;
            }
            return ID;
        }
    }

    return 0;
}

bool IsBlockedID(long long steamID) {
    std::ifstream myfile("Mods\\CommandsMod\\blacklist.txt");
    if(!myfile.is_open()) {
        return false;
    }
    std::string line;

    while (getline(myfile, line)) {
        long long ID;
        try {
            ID = std::stoll(line, nullptr);
        } catch ( ... ) {
            continue;
        }
        if (steamID == ID) {
            return true;
        }
    }
    return false;
}

void PrintCommandName(const wchar_t* name) {
    cube::Game* game = cube::GetGame();
    game->PrintMessage(name, 66, 161, 245);
}

void PrintCommandArg(const wchar_t* arg) {
    cube::Game* game = cube::GetGame();
    game->PrintMessage(arg, 139, 209, 42);
}

void PrintCommand(const wchar_t* command, const wchar_t* args, const wchar_t* message) {
    cube::Game* game = cube::GetGame();
    PrintCommandName(command);
    game->PrintMessage(L" ");
    if (wcslen(args) > 0) {
        PrintCommandArg(args);
        game->PrintMessage(L" ");
    }
    game->PrintMessage(L"- ");
    game->PrintMessage(message);
    game->PrintMessage(L"\n");
}

void Help(int page) {
    wchar_t response[256];
    switch (page) {
    case 1:
        swprintf(response, L"Help %d\n", page);
        CommandsModMessage(response);
        PrintCommand(L"/help", L"[number]", L"get help");
        PrintCommand(L"/coords", L"", L"display world coords");
        PrintCommand(L"/tp", L"<x> <y>", L"teleport in terms of map coords");
        PrintCommand(L"/tp", L"<name>", L"teleport to the specified player location");
        PrintCommand(L"/settime", L"<hour>:<minute>", L"set time");
        PrintCommand(L"/name", L"<name>", L"change your name");
        break;
    case 2:
        swprintf(response, L"Help %d\n", page);
        CommandsModMessage(response);
        PrintCommand(L"/join", L"<Steam ID>", L"Attempt to connect by ID");
        PrintCommand(L"/join", L"<alias>", L"Attempt to connect by alias");
        PrintCommand(L"/server", L"open", L"Allow anyone to connect");
        PrintCommand(L"/server", L"close", L"Stop allowing anyone to connect");
        PrintCommand(L"/server", L"block", L"Refuse all new sessions");
        PrintCommand(L"/server", L"blacklist", L"Enable blacklisting of new sessions");
        PrintCommand(L"/server", L"log", L"Log new session requests");
        break;
    case 3:
        swprintf(response, L"Help %d\n", page);
        CommandsModMessage(response);
        PrintCommand(L"/tpmap", L"", L"Teleport to cursor position on map");
        PrintCommand(L"/sethome", L"<alias>", L"Set player position as home");
        PrintCommand(L"/home", L"<alias>", L"Teleport to home position by alias");
        PrintCommand(L"/gui", L"chat <width> <height>", L"Set GUI Widget size");
        break;
    default:
        CommandsModMessage( L"Invalid page number!\n");
        break;
    }

}

EXPORT int HandleChat(wchar_t* msg) {
    cube::Game* game = cube::GetGame();
    cube::Host* host = &game->host;
    cube::World* world = &host->world;

    wchar_t response[256];
    int targetx, targety;
    long int targetHour, targetMinute;
    int page;
    long long steamID;

    if (!wcscmp(msg, L"/help")) {
        Help(1);
        return 1;

    } else if ( swscanf(msg, L"/help %d", &page) == 1) {
        Help(page);
        return 1;

    } else if ( swscanf(msg, L"/settime %d:%d", &targetHour, &targetMinute) == 2) {
        printf("%ld:%ld\n", targetHour, targetMinute);
        float targetMS = GetMilliseconds(targetHour, targetMinute);

        float maxTime = GetMilliseconds(24, 0);
        float minTime = GetMilliseconds(0, 0);

        if (targetMS > maxTime)
            targetMS = maxTime;
        else if (targetMS < minTime)
            targetMS = minTime;

        world->time = targetMS;

        swprintf(response, L"Changing time to %d:%02d.\n", targetHour % 24, targetMinute % 60);
        CommandsModMessage(response);
        return 1;

    } else if (!wcscmp(msg, L"/coords")) {
        cube::Creature* player = game->GetPlayer();
        swprintf(response, L"World coordinates:\nX: %lld\nY: %lld\nZ: %lld\n", player->position.x, player->position.y, player->position.z);
        CommandsModMessage(response);
        return 1;

    } else if ( swscanf(msg, L"/tp %d %d", &targetx, &targety) == 2) {
        cube::Creature* player = game->GetPlayer();
        player->position.x = MapCoordToDots(targetx);
        player->position.y = MapCoordToDots(targety);
        CommandsModMessage(L"Teleporting.\n");
        return 1;

    } else if ( swscanf(msg, L"/gui chat %d %d", &targetx, &targety) == 2) {
        game->gui.chat_widget->width = targetx;
        game->gui.chat_widget->height = targety;
        CommandsModMessage(L"GUI Chat Widget size changed.\n");
        return 1;

    } else if ( !wcsncmp(msg, L"/tp ", 4) ) {
        wchar_t* wideName = &msg[4];
        char* cName = new char[wcslen(wideName)+1];
        int len = wcstombs(cName, wideName, 0x100);
        if (len < 1) {
            CommandsModMessage(L"Invalid name.\n");
            return 1;
        }
        for(cube::Creature* creature: game->world->creatures) {
            if (stricmp(creature->name, cName) == 0) {
                cube::Creature* player = game->GetPlayer();
                player->position.x = creature->position.x;
                player->position.y = creature->position.y;
                player->position.z = creature->position.z;
                CommandsModMessage(L"Teleporting.\n");
                return 1;

            }
        }
        CommandsModMessage(L"Target not found.\n");
        return 0;

    } else if (!wcscmp(msg, L"/tpmap")) {
        if(game->worldmap->cursor_position.x != 0x7FFFFFFF0000) {
            LongVector3 position = game->worldmap->cursor_position;
            cube::Creature* player = game->GetPlayer();
            player->position.x = position.x;
            player->position.y = position.y;
            player->position.z = position.z;
            CommandsModMessage(L"Teleporting.\n");
            return 1;
        }
        CommandsModMessage(L"Please open map.\n");
        return 0;

    } else if ( !wcsncmp(msg, L"/sethome ", 9) ) {
        wchar_t* wideAlias = &msg[9];
        char* cAlias = new char[wcslen(wideAlias)+1];
        int len = wcstombs(cAlias, wideAlias, 0x100);
        if (len < 1) {
            CommandsModMessage(L"Invalid alias.\n");
            return 1;
        }
        std::string alias(cAlias);
        cube::Creature* player = game->GetPlayer();
        SetHomeMap(alias, player->position.x, player->position.y, player->position.z);
        CommandsModMessage(L"Done.\n");
        return 1;

    } else if ( !wcsncmp(msg, L"/home ", 6) ) {
        wchar_t* wideAlias = &msg[6];
        char* cAlias = new char[wcslen(wideAlias)+1];
        int len = wcstombs(cAlias, wideAlias, 0x100);
        if (len < 1) {
            CommandsModMessage(L"Invalid alias.\n");
            return 1;
        }
        std::string alias(cAlias);
        std::vector<long long> position = GetHomePosition(alias);
        if(position.size() < 3) {
            CommandsModMessage(L"Invalid alias.\n");
            return 1;
        }
        cube::Creature* player = game->GetPlayer();
        player->position.x = position[0];
        player->position.y = position[1];
        player->position.z = position[2];
        CommandsModMessage(L"Teleporting.\n");
        return 1;

    } else if ( !wcsncmp(msg, L"/name ", 6) ) {
        cube::Creature* player = game->GetPlayer();
        wchar_t* wideName = &msg[6];
        char newName[18];
        memset(newName, 0, sizeof(newName));
        int len = wcstombs(newName, wideName, 16);
        if (len < 1) {
            CommandsModMessage(L"Invalid name.\n");
        } else if (len >= 16) {
            CommandsModMessage(L"Name too long!\n");
        } else {
            strcpy(player->name, newName);
            swprintf(response, L"You are now known as %s.\n", player->name);
            CommandsModMessage(response);
        }
        return 1;

    } else if (swscanf(msg, L"/join %lld", &steamID) == 1) {
        game->client.JoinSteamID(steamID);
        swprintf(response, L"Attempting to join:\n%lld\n", steamID);
        CommandsModMessage(response);
        return 1;

    } else if ( !wcsncmp(msg, L"/join ", 6) ) {
        wchar_t* wideAlias = &msg[6];
        char* cAlias = new char[wcslen(wideAlias)+1];
        int len = wcstombs(cAlias, wideAlias, 0x100);
        if (len < 1) {
            CommandsModMessage(L"Invalid alias.\n");
            return 1;
        }
        std::string alias(cAlias);
        steamID = GetSteamIDFromAlias(alias);
        if (steamID > 0) {
            swprintf(response, L"Attempting to join %ls\n(%lld)\n", wideAlias, steamID);
            CommandsModMessage(response);
            game->client.JoinSteamID(steamID);
        } else {
            swprintf(response, L"Unable to identify %ls\n", wideAlias);
            CommandsModMessage(response);
        }
        delete[] cAlias;
        return 1;

    } else if (!wcscmp(msg, L"/server open")) {
        serverAccessState = 2;
        CommandsModMessage(L"The server is now opened to the public.\n");
        return 1;

    } else if (!wcscmp(msg, L"/server block")) {
        serverAccessState = 1;
        CommandsModMessage(L"The server is now blocking all P2P requests.\n");
        return 1;

    } else if (!wcscmp(msg, L"/server close")) {
        serverAccessState = 0;
        CommandsModMessage(L"The server is now closed to the public.\n");
        return 1;

    } else if (!wcscmp(msg, L"/server log")) {
        P2PRequestLogging = !P2PRequestLogging;
        swprintf(response, L"Request logging is now %s.\n", P2PRequestLogging ? "enabled" : "disabled");
        CommandsModMessage(response);
        return 1;

    } else if (!wcscmp(msg, L"/server blacklist")) {
        /*
           TODO: Try to close existing blacklisted connections?
           HandleP2PRequest will only be triggered if the session is NEW.
           There is an implicit acceptance of the request if the server has already communicated
           with the client who is trying to connect. Therefore, we probably need to attempt to
           close any of the blacklisted connections in order to properly ban someone.
        */
        blacklisting = !blacklisting;
        swprintf(response, L"Blacklisting is now %s.\n", blacklisting ? "enabled" : "disabled");
        CommandsModMessage(response);
        return 1;
    }

    return 0;
}


EXPORT HandleP2PRequest(long long steamID) {
    wchar_t response[256];

    if (P2PRequestLogging) {
        swprintf(response, L"Incoming request from:\n%lld", steamID);
        CommandsModMessage(response);
    }

    if (blacklisting && IsBlockedID(steamID)) {
        return 1;
    }

    return serverAccessState;
}

EXPORT void ModInitialize() {
    InitHomeMap();
}
