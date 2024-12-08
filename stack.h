#pragma once
#include <iostream>
template<typename T>
struct node {
    node* next;
    T* data;

    node(T* data) : data(data), next(nullptr) {}
};
template<typename T>
class stack {
private:
    node<T>* top;

public:
    stack() : top(nullptr) {}

    ~stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(T* value) {
        node<T>* news_node = new node<T>(value);
        news_node->next = top;
        top = news_node;
        //std::cout << "Post pushed to stack." << std::endl;
    }

    T* pop() {
        if (isEmpty()) {
            std::cout << "stack is empty. Cannot pop." << std::endl;
            return nullptr;
        }

        node<T>* temp = top;
        T* poppedValue = top->data;
        top = top->next;
        delete temp;
        //std::cout << "Post popped from stack." << std::endl;
        return poppedValue;
    }

    T* peek() const {
        if (isEmpty()) {
            //std::cout << "stack is empty. Cannot peek." << std::endl;
            return nullptr;
        }

        return this->top->data;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    void Display() {
        //Display function
        node<T>* current = top;
        while (current != nullptr) {
            std::cout << current->data<<" ";
            current = current->next;
        }
    }
};