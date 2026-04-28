#pragma once
#include "Seqlist.h"
#include <iostream>
#include <string>

class Building {
public:
	int x;
	int y;
	int length;
	int width;
	int id;
	std::string name;
	std::string type;
public:
	Building();
	Building(int x, int y, int l, int w, int id, std::string name,std::string type);

	Building& operator=(const Building& b);
	bool operator==(const Building& b)const;
	friend std::ostream& operator<<(std::ostream& cout,const Building& b);
};

Building::Building():x(0),y(0),length(0),width(0),id(0),name(""),type(""){}

Building::Building(int x, int y, int l, int w, int id, std::string name, std::string type):
	x(x),y(y),length(l),width(w),id(id),name(name),type(type){ }

Building& Building:: operator=(const Building& b) {
	x = b.x;
	y = b.y;
	length = b.length;
	width = b.width;
	id = b.id;
	name = b.name;
	type = b.type;
	return *this;
}

bool Building::operator==(const Building& b)const {
	return x == b.x && y == b.y;
}

std::ostream& operator<<(std::ostream& cout, const Building& b) {
	cout << "该建筑在(" << b.x << "," << b.y << ")" << "\n";
	cout << "该建筑长" << b.length << "宽" << b.width << "\n";
	cout << "该建筑是" << b.id << "号楼，叫做" << b.name << "，类型是" << b.type << "\n";
	return cout;
}
