#pragma once
#include <iostream>
#include <string>
using namespace std;

template <typename T>
class Seqlist {
protected:
	T* elements;
	int size;
	int capacity;
public:

	Seqlist();
	Seqlist(int cap);
	~Seqlist();

	void insert(const T& e);

	void remove();
	void remove(T value);
	void remove(int index);

	void change(int index, T value);
	void change(T target, T value);							//target指原本的数据

	T& find() {}

};

template<typename T>
Seqlist<T>::Seqlist() {
	size = 0;
	capacity = 0;
}

template<typename T>
Seqlist<T>::Seqlist(int cap) {
	elements = new T[cap];
	size = 0;
	capacity = cap;
}

template<typename T>
Seqlist<T>::~Seqlist() {
	delete[] elements;
}

template<typename T>
void Seqlist<T>::insert(const T& e) {
	if (capacity == size) {
		int newCapacity = capacity * 2;
		T* newElements = new T[newCapacity];
		for (int i = 0; i < size; i++) {
			newElements[i] = elements[i];
		}
		delete[] elements;
		elements = newElements;
		capacity = newCapacity;
	}
	elements[size] = e;
	size++;
}

template<typename T>
void Seqlist<T>::remove() {
	if (size == 0) {
		cout << "当前已经没有建筑！\n" << "删除失败\n";
		return;
	}
	size--;
	cout << "删除成功！\n";
}

template<typename T>
void Seqlist<T>::remove(T value) {
	int pos = -1;
	for (int i = 0; i < size; i++) {
		if (elements[i] == value) {
			pos = i;
			break;
		}
	}
	if (size == 0) {
		cout << "列表为空，删除失败\n";
		return;
	}
	if (pos == -1) {
		cout << "这个建筑不存在！请重新输入。\n";
		return;
	}
	if (pos == size - 1) {
		size--;
		cout << "删除成功！\n";
		return;
	}
	for (int i = pos; i < size; i++) {
		elements[i] = elements[i + 1];
	}
	size--;
	cout << "删除成功！\n";
}

template<typename T>
void Seqlist<T>::remove(int index) {
	if (size == 0) {
		cout << "列表为空，删除失败\n";
		return;
	}
	if (index < 0 || index >= size) {
		cout << "位置非法，删除失败\n";
		return;
	}
	for (int i = index; i < size - 1; i++) {
		elements[i] = elements[i + 1];
	}
	size--;
	cout << "索引 " << index << " 处的建筑已删除\n";
}

template<typename T>
void Seqlist<T>::change(int index, T value) {
	if (size == 0) {
		cout << "列表为空，重建失败\n";
		return;
	}
	if (index < 0 || index >= size) {
		cout << "位置非法，重建失败\n";
		return;
	}
	elements[index] = value;
	cout << "重建成功！\n";
}

template<typename T>
void Seqlist<T>::change(T target, T value) {			//target指原本的数据
	int pos = -1;
	for (int i = 0; i < size; i++) {
		if (elements[i] == tar) {
			pos = i;
			break;
		}
	}
	if (size == 0) {
		cout << "列表为空，删除失败\n";
		return;
	}
	if (pos == -1) {
		cout << "这个建筑不存在！请重新输入。\n";
		return;
	}
	elements[pos] = value;
	cout << "重建成功！\n";
}
