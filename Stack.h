#ifndef DS_WET2_STACK_H
#define DS_WET2_STACK_H


template<typename T>
class Stack {
private:
    T* data;
    int capacity;
    int top;

public:
    Stack(int capacity = 10) : capacity(capacity), top(0) {
        data = new T[capacity];
    }

    ~Stack() {
        delete[] data;
    }

    void push(const T& element) {
        if (top == capacity) {
            resize(capacity * 2);
        }
        data[top++] = element;
    }

    T pop() {
        if (!isEmpty()) {
            return data[--top];
        }
    }

    T& getTop() {
        if (!isEmpty()) {
            return data[top - 1];
        }
    }

    bool isEmpty() const {
        return top == 0;
    }

    int size() const {
        return top;
    }

private:
    void resize(int newCapacity) {
        T* newData = new T[newCapacity];
        for (int i = 0; i < top; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }
};





#endif //DS_WET2_STACK_H
