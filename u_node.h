#pragma once
#include <string>
#include "User.h"
#include "Followers.h"
#include "stack.h"
#include "Queue.h"
#include "Notification.h"

struct info {
    user* user_details;
    Followers* followers;
    stack<notification> notifications;
    Queue<string>requests;
    info() :user_details(new user), followers(new Followers) {};
    info(user* user) :user_details(user) {
        this->followers = new Followers;
    }
    void Display_Details() {
        user_details->Display();
        followers->Display_Friends();
        notifications.Display();
    }
};

struct u_node {
    info* data;
    u_node* left;
    u_node* right;

    u_node(info* new_data) : data(new_data), left(nullptr), right(nullptr) {};
};