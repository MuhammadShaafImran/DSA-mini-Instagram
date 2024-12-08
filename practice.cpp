//#include<iostream>
//#include"Users.h"
//#include"User.h"
//#include"Message.h"
//#include<string>
//
//using namespace std;
//int main() {
//	users Accounts;
//
//	user* person1 = new user("Shaaf", "shaaf125", "Faisalabad");
//	person1->add_post("C++ Programing", "High level language.");
//	person1->add_post("python Programing", "High level language.");
//
//	user* person2 = new user("rasheed", "Naveed125", "Faisalabad");
//	person2->add_post("HTML Programing", "low level language.");
//	person2->add_post("Css Programing", "low level language.");
//
//	user* person3 = new user("imran", "imran467", "Faisalabad");
//	person3->add_post("java Programing", "low level language.");
//	person3->add_post("flutter Programing", "low level language.");
//
//	Accounts.insert(person1);
//	Accounts.insert(person2);
//	Accounts.insert(person3);
//
//	u_node* temp = Accounts.find("Shaaf");
//	u_node* temp2 = Accounts.find("rasheed");
//	//Sending friend request
//	Accounts.send_request_function(temp, "rasheed");
//	Accounts.accept_reject_list(temp2, "Shaaf", "accept");
//	
//
//	//cout << "Shaaf friends :" << endl;
//	//temp->user_Friends->display();
//	//cout << "rasheed friends :" << endl;
//	//temp2->user_Friends->display();
//
//	Accounts.Display();
//
//	////User newsFeed
//	//cout << "Shaaf Friend's posts :" << endl;
//	//Accounts.Display_user_posts(temp);
//
//	//message* person_info = new message("This is the message that was delivered.");
//	//message* person_info2 = new message("This is second the message that was delivered.");
//
//	//Accounts.add_message(temp,temp2->data->get_username(), person_info);
//	//Accounts.add_message(temp,temp2->data->get_username(), person_info2);
//
//	//Accounts.view_messages(temp,temp2->data->get_username());
//
//
//	Accounts.Display_notification(temp);
//
//
//	system("pause");
//	return 0;
//}