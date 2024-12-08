#pragma once
#include <iostream>
#include <algorithm>
#include "User.h"
#include "Followers.h"
#include "stack.h"
#include "Queue.h"
#include "Message.h"
#include "vector"
#include "Notification.h"

using namespace std;

struct info{
    user* user_details;
    Followers* followers;
    stack<notification> notifications;
    Queue<string>requests;
    info():user_details(new user),followers(new Followers) {};
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

class users {
private:
    u_node* root_node;

    // Insertion Function
    void insert_function(u_node*& root, user* value) {
        if (root == nullptr) {
            root = new u_node(new info(value));
            return;
        }
        if (root->data->user_details->get_username() > value->get_username()) {
            insert_function(root->left, value);
        }
        if (root->data->user_details->get_username() < value->get_username()) {
            insert_function(root->right, value);
        }
        BalancingFactor_function(root);
    }

    void BalancingFactor_function(u_node*& root) {
        if (root == nullptr) return;
        int LeftBalancingFactor = maxDepth_function(root->left);
        int RightBalancingFactor = maxDepth_function(root->right);
        if (LeftBalancingFactor - RightBalancingFactor >= 2) {
            if (maxDepth_function(root->left->right) > maxDepth_function(root->left->left)) {
                root->left = rotateLeft_function(root->left);
            }
            root = rotateRight_function(root);
        }
        if (RightBalancingFactor - LeftBalancingFactor >= 2) {
            if (maxDepth_function(root->right->left) > maxDepth_function(root->right->right)) {
                root->right = rotateRight_function(root->right);
            }
            root = rotateLeft_function(root);
        }
    }

    // Helper functions for rotations
    u_node* rotateLeft_function(u_node*& root) {
        u_node* newRoot = root->right;
        root->right = newRoot->left;
        newRoot->left = root;
        return newRoot;
    }

    u_node* rotateRight_function(u_node*& root) {
        u_node* newRoot = root->left;
        root->left = newRoot->right;
        newRoot->right = root;
        return newRoot;
    }

    // Height Function
    int maxDepth_function(u_node* root) {
        if (root == nullptr) return 0;
        return max(maxDepth_function(root->left), maxDepth_function(root->right)) + 1;
    }

    // Display Function
    void Display_function(u_node* root) {
        if (root != nullptr) {
            Display_function(root->left);
            root->data->Display_Details();
            Display_function(root->right);
        }
    }

    void Display_users_function(u_node* root) {
        if (root != nullptr) {
            Display_users_function(root->left);
            cout << root->data->user_details->get_username() << endl;
            Display_users_function(root->right);
        }
    }

    // Deleting node 
    void deleteNode_function(u_node*& root, string username) {
        if (root == nullptr) { return; }
        if (username < root->data->user_details->get_username()) {
            deleteNode_function(root->left, username);
        }
        else if (username > root->data->user_details->get_username()) {
            deleteNode_function(root->right, username);
        }
        else {
            if (root->left == nullptr) {
                u_node* temp = root->right;
                delete root;
                root = temp;
            }
            else if (root->right == nullptr) {
                u_node* temp = root->left;
                delete root;
                root = temp;
            }
            else {
                u_node* temp = minValueNode(root->right);
                root->data = temp->data;
                deleteNode_function(root->right, temp->data->user_details->get_username());
            }
            BalancingFactor_function(root);
        }
    }

    // Small in Right sub tree
    u_node* minValueNode(u_node* temp) {
        u_node* current = temp;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    u_node* maxValueNode(u_node* temp) {
        u_node* current = temp;
        while (current && current->right != nullptr) {
            current = current->right;
        }
        return current;
    }

    // Find data
    u_node* find_function(u_node* root, std::string username) {
        if (root == nullptr) return nullptr;
        if (root->data->user_details->get_username() > username) {
            return find_function(root->left, username);
        }
        else if (root->data->user_details->get_username() < username) {
            return find_function(root->right, username);
        }
        else {
            return root;
        }
    }
    
public:
    users() : root_node(nullptr) {};

    void insert(user*& data) {
        insert_function(root_node, data);
    }

    void Display() {
        Display_function(root_node);
    }

    int maxHeight() {
        return maxDepth_function(root_node);
    }

    void deleteNode(string value) {
        if (find_function(root_node, value)) {
            deleteNode_function(root_node, value);
        }
        else {
            std::cout << "NOT FOUND !!" << std::endl;
        }
    }

    u_node* find(std::string username) {
        return find_function(root_node, username);
    }

    void Display_users() {
        Display_users_function(root_node);
    }

    //Send requests
    void send_request_function(u_node*& sender_, string receiver_) {
        u_node* receiver = find_function(root_node, receiver_);
        notification* sender_notification = new notification, *receiver_notification = new notification;
        if (receiver != nullptr) {
            receiver->data->requests.enqueue(sender_->data->user_details->get_username());
            sender_notification->setNotification("Friend Request", "Friend request has send to " + receiver->data->user_details->get_username());
            receiver_notification->setNotification("Friend Request", "Friend request has received from " + sender_->data->user_details->get_username());

            sender_->data->notifications.push(sender_notification);
            receiver->data->notifications.push(receiver_notification);
        }
    }

    //Accept requests
    void accept_reject_list(u_node*& user_, string friend_, string command) {
        string user_friend_name = user_->data->requests.find(friend_); //remove the friend name from queue;
        if (user_friend_name == "None") return;
        u_node* Friend = find_function(root_node, user_friend_name);
        notification* sender_notification = new notification, * receiver_notification = new notification;
        if (Friend != nullptr) {
            if (command == "accept") {
                user_->data->followers->insert(Friend->data->user_details);
                Friend->data->followers->insert(user_->data->user_details);

                sender_notification->setNotification("Request Accepted",Friend->data->user_details->get_username()+" Add to Friend List.");
                receiver_notification->setNotification("Friend Added ", user_->data->user_details->get_username() + " Add to Friend List.");
            }
            else if (command == "rejected"|| command == "reject") {
                sender_notification->setNotification("Request rejected", Friend->data->user_details->get_username() );
                receiver_notification->setNotification("Friend removed", user_->data->user_details->get_username() + " removed from Friend queue.");
            }
        }

        std::cout << user_friend_name << std::endl;
    }

    //push messages
    void send_message(u_node*& user,string receiver_ ,string title_,string context_) {
        u_node* receiver = find_function(root_node, receiver_);
        if (!user->data->followers->find(receiver->data->user_details)) {
            cout << " user is not in your follower list." << endl;
            return;
        }
        user->data->followers->push_messages(receiver->data->user_details->get_username(), title_, context_);
        
        notification* sender_notification = new notification, * receiver_notification = new notification;
        sender_notification->setNotification("Message send", "to : " + receiver->data->user_details->get_username() + " Title :"+ title_);
        receiver_notification->setNotification("Message received", "from : " + user->data->user_details->get_username() + " Title :" + title_);

        user->data->notifications.push(sender_notification);
        receiver->data->notifications.push(receiver_notification);
    }

    //view messages
    void view_message(u_node*& user,string friend_) {
        user->data->followers->view_Messages(friend_);
    }

    //view notification
    void view_notifications(u_node*&user) {
        while (!user->data->notifications.isEmpty()) {
            notification *view = user->data->notifications.pop();
            cout << view->getNotification_title() << ": " << view->getNotification_content() << " | Time Stamp: " << view->getNotification_timePublished()<< endl;
        }
    }

};

