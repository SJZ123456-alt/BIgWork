#pragma once
#include <string>
#include <fstream>
#include "CampusMap.h"

using namespace std;


//---------功能：实现系统数据的存盘与读取-------------//

namespace FileIO {

    // --- 将当前系统所有地图及建筑数据保存到文件 ---  //
    inline bool saveToFile(const string& path, AppState& appState) {
        // 若系统中没有地图，无需保存
        if (appState.maps.size() == 0) return false;

        ofstream ofs(path);
        if (!ofs.is_open()) return false;

        // 1. 写入地图总数（LinkedList 的长度）
        ofs << appState.maps.size() << "\n";

        for (int i = 0; i < appState.maps.size(); ++i) {
            CampusMap& map = appState.maps[i];

            // 2. 写入地图元数据：ID、长度、宽度、当前自增ID计数值
            ofs << map.getID() << " " << map.getLength() << " " << map.getWidth() << " " << map.getNextID() << "\n";

            // 3. 写入地图名称（独立一行，允许包含空格）
            ofs << map.getName() << "\n";

            // 4. 写入该地图下的建筑总数（Seqlist 的长度）
            ofs << map.getBuildingCount() << "\n";

            for (int j = 0; j < map.getBuildingCount(); ++j) {
                const Building& b = map.getBuilding(j);

                // 5. 写入建筑基础数值信息：ID、类型、坐标(x,y)、尺寸(l,w)
                ofs << b.id << " " << b.type << " " << b.x << " " << b.y << " " << b.length << " " << b.width << "\n";

                // 6. 写入建筑名称与描述
                ofs << b.name << "\n";
                ofs << b.description << "\n";
            }
        }

        ofs.close();
        return true;
    }

    // --- 从指定文件导入地图系统数据 --- //
    inline bool loadFromFile(const string& path, AppState& appState) {
        ifstream ifs(path);
        if (!ifs.is_open()) return false;

        appState.maps.clear();

        int mapCount;
        if (!(ifs >> mapCount)) return false;

        // dummy 变量用于吸收 ifs >> 操作后留在缓冲区里的换行符 \n
        string dummy;

        // 遍历读取每一个地图节点
        for (int i = 0; i < mapCount; ++i) {
            int id, len, wid, nextId;
            ifs >> id >> len >> wid >> nextId;
            getline(ifs, dummy); // 清理本行末尾换行符

            string name;
            getline(ifs, name); // 读取完整的地图名称

            // 创建地图实例并还原属性
            CampusMap map(name, len, wid);
            map.setID(id);
            map.setNextID(nextId);

            int bCount;
            ifs >> bCount;
            getline(ifs, dummy); // 清理换行符

            // 遍历读取该地图下的每一个建筑
            for (int j = 0; j < bCount; ++j) {
                int bId, bType;
                double bx, by, blen, bwid;

                // 读取建筑的 6 个数值参数
                ifs >> bId >> bType >> bx >> by >> blen >> bwid;
                getline(ifs, dummy); // 清理换行符

                string bName, bDesc;
                getline(ifs, bName); // 读取建筑名称（含空格）
                getline(ifs, bDesc); // 读取建筑描述
                Building b(bx, by, blen, bwid, bId, bName, bType, bDesc);
                map.AddBuilding(b);
            }

            appState.maps.add(map);
        }

        ifs.close();
        return true;
    }
}