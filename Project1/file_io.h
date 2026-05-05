#pragma once
#include <string>
#include <fstream>
#include "CampusMap.h"

using namespace std;

// 使用命名空间封装 IO 操作
namespace FileIO {

    // 保存系统到文件。成功返回 true，失败返回 false
    inline bool saveToFile(const string& path, AppState& appState) {
        if (appState.maps.size() == 0) return false;

        ofstream ofs(path);
        if (!ofs.is_open()) return false;

        ofs << appState.maps.size() << "\n";
        for (int i = 0; i < appState.maps.size(); ++i) {
            CampusMap& map = appState.maps[i];
            ofs << map.getID() << " " << map.getLength() << " " << map.getWidth() << " " << map.getNextID() << "\n";
            ofs << map.getName() << "\n";
            ofs << map.getBuildingCount() << "\n";
            for (int j = 0; j < map.getBuildingCount(); ++j) {
                const Building& b = map.getBuilding(j);
                ofs << b.id << " " << b.type << " " << b.x << " " << b.y << " " << b.length << " " << b.width << "\n";
                ofs << b.name << "\n";
                ofs << b.description << "\n";
            }
        }
        ofs.close();
        return true;
    }

    // 从文件读取系统。成功返回 true，失败返回 false
    inline bool loadFromFile(const string& path, AppState& appState) {
        ifstream ifs(path);
        if (!ifs.is_open()) return false;

        appState.maps.clear();
        int mapCount;
        if (!(ifs >> mapCount)) return false;

        for (int i = 0; i < mapCount; ++i) {
            int id, len, wid, nextId;
            ifs >> id >> len >> wid >> nextId;
            ifs.ignore();
            string name; getline(ifs, name);

            CampusMap map(name, len, wid);
            map.setID(id); map.setNextID(nextId);

            int bCount; ifs >> bCount;
            for (int j = 0; j < bCount; ++j) {
                int bId, bType; double bx, by, blen, bwid;
                ifs >> bId >> bType >> bx >> by >> blen >> bwid;
                ifs.ignore();
                string bName, bDesc;
                getline(ifs, bName); getline(ifs, bDesc);
                Building b(bx, by, blen, bwid, bId, bName, bType, bDesc);
                map.AddBuilding(b);
            }
            appState.maps.add(map);
        }
        ifs.close();
        return true;
    }
}