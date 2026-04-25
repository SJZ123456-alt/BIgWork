#include <iostream>

template<class T>
class LinkedList {
private:
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
    void clear();
    void display() const;
    int size() const;
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