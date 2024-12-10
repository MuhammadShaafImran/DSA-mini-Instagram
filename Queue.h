#pragma once
#include <iostream>

template<typename T>
class q_node {
public:
    T data;
    q_node* next;

    q_node(T value) : data(value), next(nullptr) {}
};

template<typename T>
class Queue {
private:
    q_node<T>* front;
    q_node<T>* rear;

    size_t count;

public:
    Queue() : front(nullptr), rear(nullptr), count(0) {}

    // Enqueue operation
    void enqueue(T value) {
        q_node<T>* newnode = new q_node<T>(value);
        if (rear == nullptr) {
            front = rear = newnode;
            count++;
            return;
        }
        rear->next = newnode;
        rear = newnode;
        count++;
    }

    // Dequeue operation
    void dequeue() {
        if (front == nullptr) {
            std::cout << "Queue is empty. Cannot dequeue." << std::endl;
            return;
        }
        q_node<T>* temp = front;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        count--;
        delete temp;
    }

    // Display function
    void display() const {
        if (front == nullptr) {
            std::cout << "Queue is empty." << std::endl;
            return;
        }
        q_node<T>* current = front;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    // Destructor to free memory
    ~Queue() {
        while (front != nullptr) {
            dequeue();
        }
    }

    //Top
    q_node<T>* Top() {
        return front;
    }

    //is Empty
    bool isEmpty() {
        return (front == nullptr && rear == nullptr);
    }

    //Find funciton
    T find(std::string username) {
        T temp = "None";
        size_t index = 0;
        while (index < count) {
            if (Top()->data == username) {
                temp = username;
            }
            else {
                enqueue(Top()->data);
            }
            dequeue();
            index++;
        }
        return temp;
    }
};