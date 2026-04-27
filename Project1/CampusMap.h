#include "Building.h"
#include "Seqlist.h"

class CampusMap {
protected:
	int ID;
	std::string name;
	int L;
	int W;
	Seqlist<Building> building;
	int nextID;
public:
	CampusMap(std::string name,int L,int W);
	bool AddBuilding(const Building& b);
	bool is_not_conflict(const Building& a, const Building& b);
	void PrintAll();
	void RemoveLast();
};

bool CampusMap::is_not_conflict(const Building& a, const Building& b) {
	return (a.x + a.length <= b.x) || (a.x >= b.x + b.length) ||
		(a.y + a.width <= b.y) || (a.y >= b.y + b.width);
}

inline void CampusMap::PrintAll(){
	for (int i = 0; i < building.get_size(); i++) {
		std::cout << building[i];
	}
}

inline void CampusMap::RemoveLast(){
	building.remove();
}

bool CampusMap::AddBuilding(const Building& b) {
	if (b.x < 0 || b.y < 0 || b.x + b.length > L || b.y + b.width > W) {
		std::cout << "错误：建筑位置超出了地图边界！\n";
		return false;
	}
	for (int i = 0; i < building.get_size(); i++) {
		if (!is_not_conflict(building[i], b)) {
			std::cout << "冲突：该位置已存在建筑 [" << building[i].name << "]！\n";
			return false;
		}
	}
	building.push_back(b);
	std::cout << "添加成功：[" << b.name << "] 已加入地图。\n";
	return true;
}

inline CampusMap::CampusMap(std::string name, int L, int W){
	this->name = name;
	this->L = L;
	this->W = W;
}
