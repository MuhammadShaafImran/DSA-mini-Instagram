#pragma once 
#include "u_node.h"
#include <vector>
#include <list>

using namespace std;

class UserHashTable {
private:
    static const int TABLE_SIZE = 100;
    vector<list<pair<string, u_node*>>> table;

    int hash(const string& username) {
        int hashValue = 0;
        for (char ch : username) {
            hashValue = (hashValue * 31 + ch) % TABLE_SIZE;
        }
        return hashValue;
    }

public:
    UserHashTable() : table(TABLE_SIZE) {}

    void insert(const string& username, u_node* userNode) {
        int index = hash(username);
        table[index].emplace_back(username, userNode);
    }

    u_node* search(const string& username) {
        int index = hash(username);
        for (auto& pair : table[index]) {
            if (pair.first == username) {
                return pair.second;
            }
        }
        return nullptr;
    }

    void remove(const std::string& username) {
        int index = hash(username);
        table[index].remove_if([&username](const std::pair<std::string, u_node*>& pair) {
            return pair.first == username;
        });
    }
};