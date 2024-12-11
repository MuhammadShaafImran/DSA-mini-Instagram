#pragma once
#include <iostream>
#include <algorithm>
#include "User.h"
#include "Message.h"
#include "stack.h"
#include <string>

struct Node {
    user* user_info;
    stack<message> Conversation;

    Node* left;
    Node* right;

    Node(user* new_data) : user_info(new_data), left(nullptr), right(nullptr) {};
};

class Followers {
private:
    Node* root_node;

    void insert_function(Node*& root, user*& value) {
        if (root == nullptr) {
            root = new Node(value);
            return;
        }
        if (value->get_username() < root->user_info->get_username()) {
            insert_function(root->left, value);
        }
        else if (value->get_username() > root->user_info->get_username()) {
            insert_function(root->right, value);
        }
        else {
            return;
        }
        balancingFactor_function(root);
    }

    void balancingFactor_function(Node*& root) {
        if (root == nullptr) return;

        int leftHeight = maxDepth_function(root->left);
        int rightHeight = maxDepth_function(root->right);

        if (leftHeight - rightHeight >= 2) {
            if (maxDepth_function(root->left->right) > maxDepth_function(root->left->left)) {
                root->left = rotateLeft_function(root->left);
            }
            root = rotateRight_function(root);
        }
        else if (rightHeight - leftHeight >= 2) {
            if (maxDepth_function(root->right->left) > maxDepth_function(root->right->right)) {
                root->right = rotateRight_function(root->right);
            }
            root = rotateLeft_function(root);
        }
    }

    void deleteNode_function(Node*& root, user*& key) {
        if (root == nullptr) return;

        if (key->get_username() < root->user_info->get_username()) {
            deleteNode_function(root->left, key);
        }
        else if (key->get_username() > root->user_info->get_username()) {
            deleteNode_function(root->right, key);
        }
        else {
            if (root->left == nullptr) {
                Node* temp = root->right;
                delete root;
                root = temp;
            }
            else if (root->right == nullptr) {
                Node* temp = root->left;
                delete root;
                root = temp;
            }
            else {
                Node* temp = minValueNode(root->right);
                root->user_info = temp->user_info;
                deleteNode_function(root->right, temp->user_info);
            }
        }
        balancingFactor_function(root);
    }

    Node* rotateLeft_function(Node* root) {
        Node* newRoot = root->right;
        root->right = newRoot->left;
        newRoot->left = root;
        return newRoot;
    }

    Node* rotateRight_function(Node* root) {
        Node* newRoot = root->left;
        root->left = newRoot->right;
        newRoot->right = root;
        return newRoot;
    }

    int maxDepth_function(Node* root) const {
        if (root == nullptr) return 0;
        return max(maxDepth_function(root->left), maxDepth_function(root->right)) + 1;
    }

    void Display_Friends_function(Node* root) const {
        if (root != nullptr) {
            Display_Friends_function(root->left);
            std::cout << root->user_info->get_username() << std::endl;
            Display_Friends_function(root->right);
        }
    }

    Node* minValueNode(Node* node) const {
        while (node && node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    // Find data
    bool find_function(Node* root, user* person) const {
        if (root == nullptr) return false;

        if (person->get_username() < root->user_info->get_username()) {
            return find_function(root->left, person);
        }
        else if (person->get_username() > root->user_info->get_username()) {
            return find_function(root->right, person);
        }
        else {
            return true;
        }
    }

    //Search Friend
    
    Node* find_friend_function(Node * root, string person) {
        if (root == nullptr) return nullptr;
        if (person < root->user_info->get_username()) {
            return find_friend_function(root->left, person);
        }
        else if (person > root->user_info->get_username()) {
            return find_friend_function(root->right, person);
        }
        else {
            return root;
        }
    }

    void Display_user_friend_posts_function(Node*root) const {
        if (root != nullptr) {
            Display_user_friend_posts_function(root->left);
            cout << "----------------------------------------------------------" << endl;
            cout << root->user_info->get_username() + " Posts:" << endl;
            root->user_info->Display_posts();
            Display_user_friend_posts_function(root->right);
        }
    }

public:
    Followers() : root_node(nullptr) {}

    void insert(user*& data) {
        insert_function(root_node, data);
    }

    void Display_Friends() const {
        Display_Friends_function(root_node);
        std::cout << std::endl;
    }

    int maxHeight() const {
        return maxDepth_function(root_node);
    }

    void deleteNode(user* value) {
        deleteNode_function(root_node, value);
    }

    bool find(user* title) const {
        return find_function(root_node, title);
    }
    Node* find_friend(string friend_) {
        return find_friend_function(root_node,friend_);
    }
    void view_Messages(string search_friend) {
        Node* Friend = find_friend_function(this->root_node,search_friend);
        stack<message> temp;
        while (Friend->Conversation.peek() != nullptr) {
            message* temp_post = Friend->Conversation.peek();
            cout << temp_post->getUser() << " : " << temp_post->getUserMessage() << " |Time Stamp:" << temp_post->getTimeTalk() << endl;
            temp.push(temp_post);
            Friend->Conversation.pop();
        }
        //moving back the stack items
        while (!temp.isEmpty()) {
            Friend->Conversation.push(temp.peek());
            temp.pop();
        }
    }

    void push_messages(string sender_,string receiver_, string context_) {
        Node* receiver = find_friend_function(this->root_node, receiver_);
        receiver->Conversation.push(new message(sender_, context_));
    }

    void Display_user_friend_posts() {
        Display_user_friend_posts_function(root_node);
    }

    Node* get_root_node() {
        return root_node;
    }
};