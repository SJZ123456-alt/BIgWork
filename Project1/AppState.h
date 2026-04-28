#pragma once
#include "CampusMap.h"
#include "Linkedlist.h"
class AppState {
public:
    LinkedList<CampusMap> maps;
    CampusMap* currentMap = nullptr;

    void addMap(std::string name, int l, int w);
    void selectMap(int index);
};

void AppState::addMap(std::string name, int l, int w) {
    CampusMap newMap(name, l, w);
    maps.add(newMap);
}

inline void AppState::selectMap(int index)
{
    currentMap = &maps[index];
}
