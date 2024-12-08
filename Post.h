#pragma once 
#include <string>
#include <ctime>
class post {
private:
	std::string title, content;
	std::size_t time_publishing;
public:
	//constructor
	post() :title("none"), content("none"), time_publishing(0) {};
	post(std::string title_, std::string content_) {
		this->title = title_;
		this->content = content_;
		this->time_publishing = unsigned int(time(0));
	}
	//setter methods
	void setPost(std::string title_, std::string content_) {
		this->title = title_;
		this->content = content_;
		this->time_publishing = unsigned int(time(0));
	}
	//getting methods
	std::string getPost_title() const {
		return this->title;
	}
	std::string getPost_content() const {
		return this->content;
	}
	std::size_t getPost_timePublished() const {
		return this->time_publishing;
	}
	//updating
	void setPost_title(std::string title_) {
		this->title = title_;
	}
	void setPost_content(std::string content_) {
		this->content = content_;
	}
	void setPost_time() {
		this->time_publishing = unsigned int(time(0));
	}
};