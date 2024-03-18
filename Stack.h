#ifndef DS_WET2_STACK_H
#define DS_WET2_STACK_H

#include "Pair.h"

class Node {
public:
    Pair data;
    Node * next;

    explicit Node(const Pair& data) : data(data), next(nullptr) {}
};

class Stack {
private:
    Node* top;
    int size;

public:
    Stack() : top(nullptr), size(0) {}

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(const Pair& data) {
        Node* newNode = new Node(data);
        newNode->next = top;
        top = newNode;
        size++;
    }

    Pair pop() {
        if (!isEmpty()) {
            Pair topData = top->data;
            Node* temp = top;
            top = top->next;
            delete temp;
            size--;
            return topData;
        }
        return Pair();
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    int getSize() const {
        return size;
    }
};

#endif //DS_WET2_STACK_H
