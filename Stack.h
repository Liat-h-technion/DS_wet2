#ifndef DS_WET2_STACK_H
#define DS_WET2_STACK_H

template<typename T>
class Node {
public:
    T data;
    Node<T>* next;

    Node(const T& data) : data(data), next(nullptr) {}
};

template<typename T>
class Stack {
private:
    Node<T>* top;
    int size;

public:
    Stack() : top(nullptr), size(0) {}

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(const T& element) {
        Node<T>* newNode = new Node<T>(element);
        newNode->next = top;
        top = newNode;
        size++;
    }

    T pop() {
        if (!isEmpty()) {
            T topData = top->data;
            Node<T>* temp = top;
            top = top->next;
            delete temp;
            size--;
            return topData;
        }
    }

    T& getTop() {
        if (isEmpty()) {
            return top->data;
        }
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    int getSize() const {
        return size;
    }

};

#endif //DS_WET2_STACK_H
