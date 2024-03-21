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
    /* Complexity: time: O(1), space: O(1)*/
    Stack() : top(nullptr), size(0) {}

    /* Complexity: time: O(n), space: O(1)*/
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    /* Complexity: time: O(1), space: O(1)*/
    void push(const Pair& data) {
        Node* newNode = new Node(data);
        newNode->next = top;
        top = newNode;
        size++;
    }

    /* Complexity: time: O(1), space: O(1)*/
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

    /* Complexity: time: O(1), space: O(1)*/
    bool isEmpty() const {
        return top == nullptr;
    }

    /* Complexity: time: O(1), space: O(1)*/
    int getSize() const {
        return size;
    }
};

#endif //DS_WET2_STACK_H
