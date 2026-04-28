#pragma once
#include "Building.h"
#include "Seqlist.h"

class CampusMap {
protected:
	int ID;
	std::string name;
	double L;
	double W;
	Seqlist<Building> building;
	int nextID;
public:
	double getWidth() const { return W; }
	double getLength() const { return L; }
	CampusMap();
	CampusMap(std::string name,double L, double W);
	bool AddBuilding(const Building& b);
	bool is_not_conflict(const Building& a, const Building& b);
	void PrintAll();
	void RemoveLast();
	CampusMap(const CampusMap& other);
	bool operator==(const CampusMap& c)const;
	Building* getBuildingAt(double px, double py);
};

inline void CampusMap::PrintAll(){
	for (int i = 0; i < building.get_size(); i++) {
		std::cout << building[i];
	}
}

inline void CampusMap::RemoveLast(){
	building.remove();
}

inline CampusMap::CampusMap(const CampusMap& other){
		this->ID = other.ID;
		this->name = other.name;
		this->L = other.L;
		this->W = other.W;
		this->building = other.building;
		this->nextID = other.nextID;
}

inline bool CampusMap::is_not_conflict(const Building& a, const Building& b) {
	return ((a.x + a.length <= b.x) || 
		(a.x >= b.x + b.length) || 
		(a.y + a.width <= b.y) || 
		(a.y >= b.y + b.width)    
		);
}

inline bool CampusMap::AddBuilding(const Building& b) {
	Building temp = b;
	if (b.x < 0 || b.y < 0 || b.x + b.length > L || b.y + b.width > W) {
		return false;
	}
	for (int i = 0; i < building.get_size(); i++) {
		if (!is_not_conflict(building[i], b)) {
			return false; 
		}
	}
	temp.id = nextID++;
	building.push_back(temp);
	return true;
}

inline CampusMap::CampusMap(){
	ID = 0;
	name = "";
	L = 0;
	W = 0;
	nextID = 0;
}

inline CampusMap::CampusMap(std::string name, double L, double W){
	this->name = name;
	this->L = L;
	this->W = W;
	this->ID = 0;
}

inline bool CampusMap::operator==(const CampusMap& c)const {
	return ID == c.ID && name == c.name;
}

//根据点击/悬停坐标找建筑
inline Building* CampusMap::getBuildingAt(double px, double py) {
	for (int i = 0; i < building.get_size(); i++) {
		if (building[i].contains(px, py)) {
			return &building[i];
		}
	}
	return nullptr;
}