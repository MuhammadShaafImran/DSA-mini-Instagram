#pragma once
#include <iostream>
#include "Post.h"
#include "stack.h"
#include "Queue.h"
#include <libpq-fe.h>

using namespace std;


class user {
private:
	std::string username, password, city;
	std::size_t Lastlogin,user_id;
	stack<post>* Posts;

public:
	//constructor
	user() :username("none"), password("none"), city("none"), Lastlogin(0),user_id(0) {
		this->Posts = new stack<post>;
	};
	user(std::string username_, std::string password_, std::string city_) {
		this->username = username_;
		this->password = password_;
		this->city = city_;
		this->Lastlogin = 0;
		this->user_id = 0;
		this->Posts = new stack<post>;
	}
	//setter functions
	void set_info(std::size_t user_id_,std::string username_, std::string password_, std::string city_,std::size_t time) {
		this->username = username_;
		this->password = password_;
		this->city = city_;
		this->Lastlogin = time;
		this->user_id = user_id_;
	}
	
	void set_username(std::string user_name) {
		this->username = user_name;
	}
	
	void set_password(std::string previous, std::string new_password) {
		if (previous == this->password) {
			this->password = new_password;
		}
		else {
			std::cout << "Enter correct previous password." << std::endl;
		}
	}
	
	void set_city(std::string city_) {
		this->city = city_;
	}
	
	void add_post(std::string title_, std::string content_) {
		post* new_post = new post(title_, content_);
		this->Posts->push(new_post);
	}

	// getter functions 
	std::string get_username()const {
		return this->username;
	}
	std::string get_password()const {
		return this->password;
	}
	std::string get_city() const {
		return this->city;
	}
	std::size_t get_user_id() const {
		return this->user_id;
	}
	std::size_t get_Time()const {
		return this->Lastlogin;
	}
	post* get_post() {
		if (!Posts->isEmpty()) {
			return Posts->pop();
		}
		return nullptr;
	}
	//Display function
	void Display_posts() {
		stack<post> temp;
		while (Posts->peek() != nullptr) {
			post* temp_post = Posts->peek();
			cout << temp_post->getPost_title() << " : " << temp_post->getPost_content() << " " << temp_post->getPost_timePublished() << endl;
			temp.push(temp_post);
			Posts->pop();
		}
		//moving back the stack items
		while (!temp.isEmpty()) {
			Posts->push(temp.peek());
			temp.pop();
		}
	}

	void Display() {
		cout << "USER Info Detail" << endl;
		cout << "Name: " << username << endl;
		cout << "Password: " << password << endl;
		cout << "city: " << city << endl;
		cout << "TimeStamp:" << Lastlogin << endl << endl;
		cout << "Posts : " << endl;
		Display_posts(); cout << endl;
		cout << "-------------------------------------------------" << endl;
	}

	void newsFeed_function() {
		cout << "-------------------------------------------------" << endl;
		cout << "User:" << this->username << endl;
		Display_posts();
		cout << "\n-------------------------------------------------" << endl;
	}
};
