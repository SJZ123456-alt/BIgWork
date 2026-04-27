#include "CampusMap.h"
#include "Linkedlist.h"
class AppState {
public:
    LinkedList<CampusMap> maps;
    CampusMap* currentMap = nullptr;

    void createNewMap();
};

void AppState::createNewMap() {
    std::string mName;
    int mL, mW;
    std::cout << "--- 创建新地图 ---\n";
    std::cout << "请输入学校名称: "; std::cin >> mName;
    std::cout << "请输入地图长度 L: "; std::cin >> mL;
    std::cout << "请输入地图宽度 W: "; std::cin >> mW;

    CampusMap newMap(mName, mL, mW);
    maps.add(newMap);
    std::cout << "地图 [" << mName << "] 创建成功！\n";
}