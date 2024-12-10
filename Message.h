#pragma once 
#include <ctime>
class message {
private:
	std::string user, info;
	std::size_t time_talk;
public:
	//constructor
	message() :info("none"),time_talk(0) {};
	message(std::string user_, std::string info_) {
		this->user = user_;
		this->info = info_;
		this->time_talk = unsigned int(time(0));
	}
	//setter methods
	void setPost(std::string user_, std::string info_) {
		this->user = user_;
		this->info = info_;
		this->time_talk = unsigned int(time(0));
	}
	//getting methods
	std::string getUser() const {
		return this->user;
	}
	std::string getUserMessage() const {
		return this->info;
	}
	std::size_t getTimeTalk() const {
		return this->time_talk;
	}
	//updating
	void setuser(std::string user_) {
		this->user = user_;
	}
	void setInfo(std::string content_) {
		this->info = content_;
	}
	void setTime() {
		this->time_talk = unsigned int(time(0));
	}
};