#pragma once
#include "Seqlist.h"
#include <iostream>
#include <string>

class Building {
public:
	double x;
	double y;
	double length;
	double width;
	int id;
	std::string name;
	std::string type;
public:
	Building();
	Building(double x, double y, double l, double w, int id, std::string name,std::string type);

	Building& operator=(const Building& b);
	bool operator==(const Building& b)const;
	bool contains(double px, double py)const;
	friend std::ostream& operator<<(std::ostream& cout,const Building& b);
};

inline Building::Building():x(0),y(0),length(0),width(0),id(0),name(""),type(""){}

inline Building::Building(double x, double y, double l, double w, int id, std::string name, std::string type):
	x(x),y(y),length(l),width(w),id(id),name(name),type(type){ }

inline Building& Building:: operator=(const Building& b) {
	x = b.x;
	y = b.y;
	length = b.length;
	width = b.width;
	id = b.id;
	name = b.name;
	type = b.type;
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