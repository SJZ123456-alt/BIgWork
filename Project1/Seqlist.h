#pragma once
#include <iostream>
#include <string>

//------------------------------------------------顺序表形式的线性表--------------------------------------------//
template <typename T>
class Seqlist {
protected:
    T* elements;
    int size;
    int capacity;

public:
    Seqlist();
    Seqlist(int cap);                               //数入一个capacity确定数组大小
    Seqlist(const Seqlist& s);                      //拷贝构造函数
    ~Seqlist();
    Seqlist<T>& operator=(const Seqlist& s);        
    void push_back(const T& e);                     //在列表尾部添加元素

    void remove();                                  //去除列表尾部最后一个元素
    void remove(T& value);                          //去除值为value的元素
    void remove(int index);                         //去除索引为index的元素

    void update(int index, T& value);               //在index位置添加一个值为value的值
    void update(T& target, T& value);               //将原本值为target的元素替换成值为value的元素

    void read() const;                              //打印所有元素

    int get_size() const;                           //获取列表大小

    T& operator[](int i)const;                      //用索引获取元素
};


template<typename T>
Seqlist<T>::Seqlist() {
    elements = nullptr;
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
Seqlist<T>::Seqlist(const Seqlist& s) {
    capacity = s.capacity;
    elements = new T[capacity];
    size = s.size;
    for (int i = 0; i < size; i++) {
        elements[i] = s.elements[i];
    }
}

template<typename T>
Seqlist<T>::~Seqlist() {
    delete[] elements;
}

template<typename T>
Seqlist<T>& Seqlist<T>::operator=(const Seqlist& s) {
    if (this != &s) { 
        delete[] elements; 
        capacity = s.capacity;
        size = s.size;
        elements = new T[capacity];
        for (int i = 0; i < size; i++) {
            elements[i] = s.elements[i];
        }
    }
    return *this;
}

template<typename T>
void Seqlist<T>::push_back(const T& e) {
    if (capacity == size) {
        int newCapacity = capacity * 2 + 1;
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
        std::cout << "当前已经没有建筑！\n" << "删除失败\n";
        return;
    }
    size--;
    std::cout << "删除成功！\n";
}

template<typename T>
void Seqlist<T>::remove(T& value) {
    int pos = -1;
    for (int i = 0; i < size; i++) {
        if (elements[i] == value) {
            pos = i;
            break;
        }
    }
    remove(pos);
}

template<typename T>
void Seqlist<T>::remove(int index) {
    if (size == 0) {
        std::cout << "列表为空，删除失败\n";
        return;
    }
    if (index < 0 || index >= size) {
        std::cout << "位置非法，删除失败\n";
        return;
    }
    for (int i = index; i < size - 1; i++) {
        elements[i] = elements[i + 1];
    }
    size--;
    std::cout << "成功删除\n";
}

template<typename T>
void Seqlist<T>::update(int index, T& value) {
    if (size == 0) {
        std::cout << "列表为空，重建失败\n";
        return;
    }
    if (index < 0 || index >= size) {
        std::cout << "位置非法，重建失败\n";
        return;
    }
    elements[index] = value;
    std::cout << "重建成功！\n";
}

template<typename T>
void Seqlist<T>::update(T& target, T& value) {
    int pos = -1;
    for (int i = 0; i < size; i++) {
        if (elements[i] == target) {
            pos = i;
            break;
        }
    }
    if (size == 0) {
        std::cout << "重建失败\n";
        return;
    }
    if (pos == -1) {
        std::cout << "这个建筑不存在！请重新输入。\n";
        return;
    }
    elements[pos] = value;
    std::cout << "重建成功！\n";
}

template<typename T>
void Seqlist<T>::read() const {
    if (size == 0) {
        std::cout << "列表为空\n";
        return;
    }
    for (int i = 0; i < size; i++) {
        std::cout << elements[i] << std::endl;
    }
}

template<typename T>
inline int Seqlist<T>::get_size() const
{
    return size;
}

template<typename T>
T& Seqlist<T>::operator[](int i) const{
    return elements[i];
}