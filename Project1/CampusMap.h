#pragma once
#include <iostream>
#include <string>
#include "Linkedlist.h"


//-------------------------------------对于建筑的定义-----------------------------------------------//


class Building {
public:
	double x;
	double y;
	double length;
	double width;
	int id;
	std::string name;
	int type;						// 1教学 2食堂 3图书 4体育 5湖泊 6宿舍
	std::string description;
public:
	Building();
	Building(double x, double y, double l, double w, int id, std::string name, int type, std::string desc);

	Building& operator=(const Building& b);										//重定义赋值=
	bool operator==(const Building& b)const;									//重定义相等==
	bool contains(double px, double py)const;									//判断鼠标是否在建筑内部
	friend std::ostream& operator<<(std::ostream& cout, const Building& b);		//重定义输出<<
};

//---------------------------------------实现一个校园里不同建筑的储存------------------------------//


class CampusMap {
protected:
	int ID;
	std::string name;
	double L;
	double W;
	Seqlist<Building> building;
	int nextID;
public:
	CampusMap();
	CampusMap(std::string name, double L, double W);
	CampusMap(const CampusMap& other);											//拷贝构造函数

	double getWidth() const { return W; }										//获取宽度
	double getLength() const { return L; }										//获取长度
	int getID() const { return ID; }											//获取当前ID
	void setID(int id) { ID = id; }												//设置当前ID
	int getNextID() const { return nextID; }									//获取下一个ID
	void setNextID(int nid) { nextID = nid; }									//设置下一个ID
	std::string getName() { return name; }										//获取学校名字

	bool AddBuilding(const Building& b);										//添加建筑
	bool removeBuildingById(int id);											//按ID删除
	Building* getBuildingById(int id);											//按ID查找
	bool updateBuilding(int id, const Building& newB);							//修改建筑
	void RemoveLast();															//去除builidng中最后一个元素

	bool is_not_conflict(const Building& a, const Building& b);					//判断新添加进来的建筑是否与原来的建筑有重合
	Building* getBuildingAt(double px, double py);								//根据点击/悬停坐标找建筑
	bool isNameExist(const std::string& checkName, int excludeId = -1) const;	// 检查建筑名字是否已经存在（excludeId 用于修改时排除自身）

	void PrintAll();															//打印building里所有元素			
	bool operator==(const CampusMap& c)const;									//重定义相等==
	
	inline int getBuildingCount() const { return building.get_size(); }			//获取当前地图中有建筑数量
	inline const Building& getBuilding(int index) const { return building[index]; }
};


//---------------------实现不同学校的在链表中的储存-------------------------------//


class AppState {
public:
	LinkedList<CampusMap> maps;
	CampusMap* currentMap = nullptr;

	void addMap(std::string name, int l, int w);                //在链表尾部添加一个学校
	void selectMap(int index);                                  //选择一个索引为index的学校
	//一个学校可以直接用[]返回
};


//------------------------------------------------------------------------//

inline Building::Building() :x(0), y(0), length(0), width(0), id(0), name(""), type(0),description("") {}

inline Building::Building(double x, double y, double l, double w, int id, std::string name, int type, std::string desc) :
	x(x), y(y), length(l), width(w), id(id), name(name), type(type),description(desc) {
}

inline Building& Building:: operator=(const Building& b) {
	x = b.x;
	y = b.y;
	length = b.length;
	width = b.width;
	id = b.id;
	name = b.name;
	type = b.type;
	description = b.description;
	return *this;
}

inline bool Building::operator==(const Building& b)const {
	return x == b.x && y == b.y;
}

inline std::ostream& operator<<(std::ostream& cout, const Building& b) {
	cout << "ID:" << b.id << "Name:" << b.name << "Pos:(" << b.x << "," << b.y << ")" << std::endl;
	return cout;
}

inline bool Building::contains(double px, double py) const {
	return (px >= x && px <= x + length &&
		py >= y && py <= y + width);
}


//------------------------------------------------------------------------//

inline CampusMap::CampusMap() : ID(0), name(""), L(0), W(0), nextID(1) {}

inline CampusMap::CampusMap(std::string name, double L, double W) : name(name), L(L), W(W), ID(0), nextID(1) {}

inline CampusMap::CampusMap(const CampusMap& other) {
	this->ID = other.ID;
	this->name = other.name;
	this->L = other.L;
	this->W = other.W;
	this->building = other.building;
	this->nextID = other.nextID;
}

inline bool CampusMap::is_not_conflict(const Building& a, const Building& b) {
	return ((a.x + a.length <= b.x) || (a.x >= b.x + b.length) || (a.y + a.width <= b.y) || (a.y >= b.y + b.width));
}

inline bool CampusMap::AddBuilding(const Building& b) {
	Building temp = b;
	if (b.x < 0 || b.y < 0 || b.x + b.length > L || b.y + b.width > W) return false;
	for (int i = 0; i < building.get_size(); i++) {
		if (!is_not_conflict(building[i], b)) return false;
	}
	if (isNameExist(b.name)) return false;

	if (temp.id == 0) temp.id = nextID++;
	building.push_back(temp);
	return true;
}

inline bool CampusMap::removeBuildingById(int id) {
	for (int i = 0; i < building.get_size(); i++) {
		if (building[i].id == id) {
			building.remove(i);
			return true;
		}
	}
	return false;
}

inline Building* CampusMap::getBuildingById(int id) {
	for (int i = 0; i < building.get_size(); i++) {
		if (building[i].id == id) return &building[i];
	}
	return nullptr;
}

inline bool CampusMap::updateBuilding(int id, const Building& newB) {
	if (newB.x < 0 || newB.y < 0 || newB.x + newB.length > L || newB.y + newB.width > W) return false;
	for (int i = 0; i < building.get_size(); i++) {
		if (building[i].id != id && !is_not_conflict(building[i], newB)) return false;
	}

	if (isNameExist(newB.name, id)) return false;

	Building* target = getBuildingById(id);
	if (target) {
		*target = newB;
		target->id = id;
		return true;
	}
	return false;
}

inline bool CampusMap::operator==(const CampusMap& c) const { return ID == c.ID; }

inline Building* CampusMap::getBuildingAt(double px, double py) {
	for (int i = building.get_size() - 1; i >= 0; i--) { 
		if (building[i].contains(px, py)) return &building[i];
	}
	return nullptr;
}

inline bool CampusMap::isNameExist(const std::string & checkName, int excludeId) const {
	for (int i = 0; i < building.get_size(); i++) {
		if (building[i].name == checkName && building[i].id != excludeId) {
			return true;
		}
	}
	return false;
}

inline void CampusMap::PrintAll() {
	for (int i = 0; i < building.get_size(); i++) std::cout << building[i];
}
inline void CampusMap::RemoveLast() { building.remove(); }
//------------------------------------------------------------------------//


inline void AppState::addMap(std::string name, int l, int w) {
	CampusMap newMap(name, l, w);
	maps.add(newMap);
}

inline void AppState::selectMap(int index){
	currentMap = &maps[index];
}
