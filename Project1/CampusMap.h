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

inline void CampusMap::PrintAll(){
	for (int i = 0; i < building.get_size(); i++) {
		std::cout << building[i];
	}
}

inline void CampusMap::RemoveLast(){
	building.remove();
}

bool CampusMap::is_not_conflict(const Building& a, const Building& b) {
	return !((a.x + a.length <= b.x) || 
		(a.x >= b.x + b.length) || 
		(a.y + a.width <= b.y) || 
		(a.y >= b.y + b.width)    
		);
}

bool CampusMap::AddBuilding(const Building& b) {
	if (b.x < 0 || b.y < 0 || b.x + b.length > L || b.y + b.width > W) {
		return false;
	}
	for (int i = 0; i < building.get_size(); i++) {
		if (is_not_conflict(building[i], b)) {
			return false; 
		}
	}
	building.push_back(b);
	return true;
}
inline CampusMap::CampusMap(std::string name, int L, int W){
	this->name = name;
	this->L = L;
	this->W = W;
}
