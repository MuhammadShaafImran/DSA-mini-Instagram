#pragma once

#include <string>
#include <ctime>

class notification {
private:
	std::string title, content;
	std::size_t time_publishing;
public:
	//constructor
	notification() :title("none"), content("none"), time_publishing(0) {};
	notification(std::string title_, std::string content_) {
		this->title = title_;
		this->content = content_;
		this->time_publishing = unsigned int(time(0));
	}
	//setter methods
	void setNotification(std::string title_, std::string content_) {
		this->title = title_;
		this->content = content_;
		this->time_publishing = unsigned int(time(0));
	}
	//getting methods
	std::string getNotification_title() const {
		return this->title;
	}
	std::string getNotification_content() const {
		return this->content;
	}
	std::size_t getNotification_timePublished() const {
		return this->time_publishing;
	}
	//updating
	void setNotification_title(std::string title_) {
		this->title = title_;
	}
	void setNotification_content(std::string content_) {
		this->content = content_;
	}
	void setNotification_timePublished() {
		this->time_publishing = unsigned int(time(0));
	}
};