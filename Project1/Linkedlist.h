#pragma once
#include <iostream>

template<class T>
class LinkedList {
public:
    struct Node {
        T data;
        Node* next;
        Node(T d) : data(d), next(nullptr) {}
        Node() : next(nullptr) {}
    };

    Node* head;
    int length;

public:
    LinkedList();
    ~LinkedList();

    void add(T e);
    void remove(T e);
    void remove(int index);
    void clear();
    void display() const;
    int size() const;

    T& operator[](int index)const;
};

template<class T>
LinkedList<T>::LinkedList() {
    head = new Node(); 
    length = 0;
}

template<class T>
LinkedList<T>::~LinkedList() {
    clear();
    delete head;
}

template<class T>
void LinkedList<T>::add(T e) {
    Node* newNode = new Node(e);
    Node* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }
    current->next = newNode;
    length++;
}

template<class T>
void LinkedList<T>::remove(T e) {
    Node* current = head;
    while (current->next != nullptr) {
        if (current->next->data == e) {
            Node* temp = current->next;
            current->next = temp->next;
            delete temp;
            length--;
            return;
        }
        current = current->next;
    }
}

template<class T>
inline void LinkedList<T>::remove(int index) {
    if (index < 0 || index >= length) { 
        return;
    }
    Node* cur = head; 
    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }
    Node* temp = cur->next;
    cur->next = temp->next; 
    delete temp;
    length--;
}

template<class T>
void LinkedList<T>::clear() {
    Node* current = head->next;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
    head->next = nullptr;
    length = 0;
}

template<class T>
void LinkedList<T>::display() const {
    Node* current = head->next;
    while (current != nullptr) {
        std::cout << current->data << " -> ";
        current = current->next;
    }
    std::cout << "NULL" << std::endl;
}

template<class T>
int LinkedList<T>::size() const {
    return length;
}

template<class T>
T& LinkedList<T>::operator[](int index) const{
    if (index > length || index < 0) {
        throw std::out_of_range("您输入的节点不存在，请重新输入！\n");
    }
    Node* cur = head->next;
    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }
    return cur->data;
}