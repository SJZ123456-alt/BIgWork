#pragma once
#include "CampusMap.h"
#include "Linkedlist.h"
class AppState {
public:
    LinkedList<CampusMap> maps;
    CampusMap* currentMap = nullptr;

    void addMap(std::string name, int l, int w);                //在链表尾部添加一个学校
    void selectMap(int index);                                  //选择一个索引为index的学校
                                                                //一个学校可以直接用[]返回
};

inline void AppState::addMap(std::string name, int l, int w) {
    CampusMap newMap(name, l, w);
    maps.add(newMap);
}

inline void AppState::selectMap(int index)
{
    currentMap = &maps[index];
}
