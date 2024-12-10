#include <iostream>
#include <libpq-fe.h>
#include <windows.h>
#include <ctime>
#include <string>
#include <cstring>
#include "Users.h"
#include "User.h"
#include <conio.h>

using namespace std;

//prototype
bool checkConn(PGconn* conn);

void user_functions(PGconn* conn, users& Accounts, u_node*& user, int choice);
void Signup_login(int choice, users& Accounts, PGconn*& conn);
void post_creation(users& Accounts,u_node*& user);
void Display_admin_menu();
void Display_login_menu(u_node*& user);
void handle_messages(u_node*& user, Node*& username, int option);
bool check_query(PGresult* res);
void Signup_preview(PGconn*& conn);
void Login_preview(PGconn* conn, u_node*& user, users& Accounts);

//new functions
void Load_users_to_App(PGconn* conn, users& Account);
void write_to_database(PGconn*& conn,users&Accounts, u_node*& user);
bool userExists(const string& username, PGconn* conn);
bool addUser(user*& newUser, PGconn* conn);
void updateUser(u_node*& user, PGconn* conn);
void updatePost(u_node*& user, PGconn* conn);
user* app_user(size_t ID, vector<info*>app);
void updateNotification(u_node*& user, PGconn* conn);
void updateMessage(u_node*& user, PGconn* conn);
void updateFriends(users& Accounts, u_node*& user, PGconn* conn);

int main() {

	const char* conninfo = "user=postgres host=localhost dbname=Projects password=Hello123 port=5000";

	// Connect to the database
	PGconn* conn = PQconnectdb(conninfo);
	if (!checkConn(conn)) {
		cout << " Data base can't be loaded." << endl;
		return 0;
	}

	//Main menu
	users Accounts;
	int choice{ -1 };
	while (true) {
		system("CLS");
		Display_admin_menu();
		cin >> choice; cin.ignore();
		if (choice == 0) break;
		Signup_login(choice,Accounts,conn);
	}

	PQfinish(conn);

	system("pause");
	return 0;
}


bool checkConn(PGconn* conn) {
	return (PQstatus(conn) == CONNECTION_OK);
}

void Display_admin_menu() {
	cout << "SignUp/Login Form" << endl;
	cout << "1. Create Account." << endl;
	cout << "2. Login Account." << endl;
	cout << "3. Load App." << endl;
	cout << "Exit (0)" << endl;
	cout << "Enter Choice : ";
}

void Signup_login(int choice,users &Accounts, PGconn*& conn) {
	string username, password, re_password, city;
	user* newUser{ new user() };
	u_node* user{ nullptr };
	int list_choice{ -1 };

	switch (choice) {
		//Sign-Up information
		case 1: 
			Signup_preview(conn);
			break;
			
		//login section
		case 2:
			Login_preview(conn, user, Accounts);
			if (user != nullptr) {
				while (true) {
					system("CLS");
					Display_login_menu(user);
					//User input
					cout << "Enter choice." << endl;
					cin >> list_choice; cin.ignore();
					if (list_choice == 0) {
						write_to_database(conn,Accounts, user);
						user = nullptr;
						break;
					}
					user_functions(conn,Accounts, user, list_choice);
				}
			}
			else {
				cout << "Issue in login." << endl;
			}
			break;
			
		case 3:
			cout << "Loading to App" << endl;
			Load_users_to_App(conn,Accounts);
			cin.get();
			break;

		default:
			cout << "Pick a correct once" << endl;
			cin.get();
	}
}

void Signup_preview(PGconn*& conn) {
	string username, password, re_password, city;

	cout << "Create Account" << endl;

	// Username
	while (true) {
		cout << "Username: ";
		getline(cin, username);

		if (username.empty()) {
			cout << "Username cannot be empty. Please try again." << endl;
			continue;
		}
		if (username.length() > 50) {
			cout << "Username is too long (max 50 characters). Please try again." << endl;
			continue;
		}
		if (userExists(username, conn)) {
			cout << "User already exists. Please choose a different username." << endl;
			continue;
		}

		break;
	}

	//Password
	while (true) {
		cout << "Password: ";
		getline(cin, password);

		cout << "Confirm Password: ";
		getline(cin, re_password);

		if (password != re_password) {
			cout << "Passwords do not match. Please try again." << endl;
			continue;
		}

		if (password.length() < 8) {
			cout << "Password must be at least 8 characters long." << endl;
			continue;
		}

		break;
	}
	//newUser->password = hashPassword(password); // Hash the password
	
	//City
	while (true) {
		cout << "City: ";
		getline(cin, city);

		if (city.empty()) {
			cout << "City cannot be empty. Please try again." << endl;
			continue;
		}

		if (city.length() > 50) {
			cout << "City name is too long (max 50 characters). Please try again." << endl;
			continue;
		}

		break;
	}

	user* newUser{ new user(username,password,city) };
	// Add user to the database
	if (addUser(newUser, conn)) {
		cout << "Account created successfully!" << endl;
		cin.get();
	}
	else {
		cout << "An error occurred while creating your account. Please try again." << endl;
	}
}

void Login_preview(PGconn* conn,u_node*& user,users& Accounts) {
	string username, password;
	cout << "Login " << endl;
	while (true) {
		char ch = _getch();
		system("CLS");
		cout << "Press any key to contiune or esc to Exit." << endl;
		cout << "Username : "; getline(cin, username);
		cout << "password : "; getline(cin, password);
		if (ch == 27) break;
		user = Accounts.find(username);
		if (user == nullptr || user->data->user_details->get_password()!= password) {
			cout << "Incorrect username or password." << endl;
			cin.get();
		}
		else {
			cin.get();
			break;
		}
	}
}

void Display_login_menu(u_node*& user) {
	cout << "Welcome " << user->data->user_details->get_username() << "!" << endl;
	cout << "1. NewsFeed" << endl;
	cout << "2. Posts" << endl;
	cout << "3. Notifications" << endl;
	cout << "4. Search Users " << endl;
	cout << "5. Messaging " << endl;
	cout << "6. Followers" << endl;
	cout << "7. Follow Request" << endl;
	cout << "8. Cancel and Accept Requests" << endl;
	cout << "Exit (0)" << endl;
}

void user_functions(PGconn* conn, users &Accounts,u_node*& user,int choice) {
	string username,helper;
	int option{ -1 };
	system("CLS");
	switch (choice) {
	case 1:
		cout << "News Feed" << endl;
		user->data->user_details->newsFeed_function();
		cin.get();
		break;
	
	case 2:
		while (true) {
			system("CLS");
			cout << "1.Create posts" << endl;
			cout << "2.View posts" << endl;
			cout << "Exits (0)" << endl;
			cout << "Enter choice: "; cin >> option; cin.ignore();
			if (option == 0) break;
			switch (option) {
				case 1:
					cout << "-------------------------------------------------------" << endl;
					post_creation(Accounts,user);
					break;
				case 2:
					user->data->followers->Display_user_friend_posts(); //Post of user friend's
					cin.get();
					break;
				default:
					cout << "Enter correct key" << endl;
			}
		}
		break;
	case 3:
		cout << "Notifications:" << endl;
		Accounts.view_notifications(conn,user);
		cin.get();
		break;
	case 4:
		while (true) {
			system("CLS");
			Accounts.Display_users();
			cout << "Search user: "; getline(cin, username);
			if (username == "") break;
			u_node* searched_user = Accounts.find(username);
			if (searched_user != nullptr) {
				system("CLS");
				cout << searched_user->data->user_details->get_username() + "Posts : " << endl;
				searched_user->data->user_details->Display_posts();
			}
			else {
				cout << "some error occured in searching." << endl;
			}
			cin.get();
		}
		break;
	case 5:
		while (true) {
			system("CLS");
			cout << "Messaging portal" << endl;
			user->data->followers->Display_Friends();
			cout << "username : "; getline(cin, username);
			if (username == "") break;
			//Check is the user exists
			Node* Friend = user->data->followers->find_friend(username);
			if (Friend != nullptr) {	
				while (true) {
					system("CLS");
					cout << "1.Send a message." << endl;
					cout << "2.view messages." << endl;
					cout << "Exit(0)" << endl;
					cout << "Enter option : "; cin >> option; cin.ignore();
					if (option == 0) break;
					handle_messages(user, Friend ,option);
				}
			}
		}
		break;
	case 6:
		cout << "Followers List: " << endl;
		user->data->followers->Display_Friends();
		cin.get();
		break;
	case 7:
		while (true) {
			system("CLS");
			Accounts.Display_users();
			cout << "Send request to : "; getline(cin, username);
			if (username == "") break;
			//Find the selected user 
			if (Accounts.send_request_function(user, username)) {
				u_node* friend_id = Accounts.find(username);
				string check_query = "INSERT INTO friends (user_id,user_friend_id,status,created_at) VALUES ($1,$2,'pending',NOW())";
				string sender = to_string(user->data->user_details->get_user_id());
				string receiver = to_string(friend_id->data->user_details->get_user_id());
				const char* params[] = {
					sender.c_str(),
					receiver.c_str(),
				};
				PGresult* check_res = PQexecParams(conn, check_query.c_str(), 2, nullptr, params, nullptr, nullptr, 0);
			}
		}

		break;
	case 8:
		cout << "Requests List: " << endl;
		user->data->requests.display();
		cout << "Enter username:"; getline(cin,username);
		cout << "Command(accept, reject):"; getline(cin, helper);
		if (Accounts.accept_reject_list(user, username, helper)) {
			u_node* friend_id = Accounts.find(username);
			string check_query = "UPDATE friends SET status = 'accepted' WHERE user_id = $1 AND user_friend_id = $2 ;";
			string receiver = to_string(user->data->user_details->get_user_id());
			string sender = to_string(friend_id->data->user_details->get_user_id());
			const char* params[] = {
				sender.c_str(),
				receiver.c_str(),
			};
			PGresult* check_res = PQexecParams(conn, check_query.c_str(), 2, nullptr, params, nullptr, nullptr, 0);
		}
		cin.get();

		break;
	default:
		cout << "Pick a correct choice" << endl;
		cin.get();
	}
}

void post_creation(users&Accounts,u_node*& user) {
	string title_, content_;
	cout << "Enter title : "; getline(cin, title_);
	cout << "Enter content : "; getline(cin, content_);
	user->data->user_details->add_post(title_,content_);

	for (Node* friend_node = user->data->followers->get_root_node(); friend_node != nullptr;) {
		u_node* friend_ = Accounts.find(friend_node->user_info->get_username());
		friend_->data->notifications.push(new notification( "Post added by "+ user->data->user_details->get_username(), "Related to " + title_ ));
		if (friend_node->left) {
			friend_node = friend_node->left;
		}
		else if (friend_node->right) {
			friend_node = friend_node->right;
		}
		else {
			break;
		}
	}
	

}

void handle_messages(u_node*&user, Node*& username ,int option) {
	string conversation;
	switch (option) {
	case (1):
		cout << "Enter info: "; getline(cin, conversation);
		username->Conversation.push(new message(username->user_info->get_username(), conversation));
		user->data->notifications.push(new notification("Send to : " + username->user_info->get_username(), "chat : " + conversation));
		break;
	case (2):
		cout << "Conversation with " << username->user_info->get_username() << endl;
		username->Conversation.Display();
		cin.get();
	default:
		cout << "Enter a correct key" << endl;

	}
}

void Load_users_to_App(PGconn* conn, users& Accounts) {
	if (!conn) return;

	const char* user_query = "SELECT user_id, username, password, city, created_at FROM user_information;";
	PGresult* res = PQexec(conn, user_query);
	vector<info*> result;

	//adding user,posts,notifications
	if (check_query(res)) {
		for (int i = 0; i < PQntuples(res); i++) {
			info* person = new info();

			size_t user_id = size_t(stoul(PQgetvalue(res, i, 0)));
			string username = PQgetvalue(res, i, 1);
			string password = PQgetvalue(res, i, 2);
			string city = PQgetvalue(res, i, 3);
			size_t created_at = size_t(stoul(PQgetvalue(res, i, 4)));

			//user* person = new user();
			person->user_details->set_info(user_id, username, password, city, created_at);

			string post_query =
				"SELECT title, content FROM posts WHERE user_id = " + to_string(user_id) + ";";
			PGresult* post_res = PQexec(conn, post_query.c_str());

			// Check if the query executed successfully
			if (check_query(post_res)) {
				// Add posts to the user
				for (int j = 0; j < PQntuples(post_res); j++) {
					string title = PQgetvalue(post_res, j, 0);
					string content = PQgetvalue(post_res, j, 1);
					person->user_details->add_post(title, content);
				}
			}
			PQclear(post_res);

			// Query to fetch notification for this user
			string notification_query =
				"SELECT topic,topic_info FROM notification WHERE user_id = " + to_string(user_id) + "; ";
			PGresult* notification_res = PQexec(conn, notification_query.c_str());

			// Check if the query executed successfully
			if (check_query(notification_res)) {
				// Add friends to the user
				for (int j = PQntuples(notification_res) - 1 ; j >=0 ; j--) {
					string topic_title = PQgetvalue(notification_res, j, 0);
					string topic_info = PQgetvalue(notification_res, j, 1);
					person->notifications.push(new notification(topic_title,topic_info)); 
				}
			}
			PQclear(notification_res);
			result.push_back(person);
		}
	}
	PQclear(res);

	//Add friends,friend request ,messages
	for (size_t i = 0; i < result.size(); i++) {

		// Add friend
		string friend_query1 =
			"SELECT user_friend_id FROM friends WHERE user_id = " + to_string(result[i]->user_details->get_user_id()) +
			" AND status = 'accepted';";
		PGresult* friend_res1 = PQexec(conn, friend_query1.c_str());
		if (check_query(friend_res1)) {
			for (int j = 0; j < PQntuples(friend_res1); j++) {
				size_t friend_id = static_cast<size_t>(stoul(PQgetvalue(friend_res1, j, 0)));
				user* friend_ = app_user(friend_id, result);
				result[i]->followers->insert(friend_);
			}
		}
		PQclear(friend_res1);

		string friend_query2 =
			"SELECT user_id FROM friends WHERE user_friend_id = " + to_string(result[i]->user_details->get_user_id()) +
			" AND status = 'accepted';";
		PGresult* friend_res2 = PQexec(conn, friend_query2.c_str());
		if (check_query(friend_res2)) {
			for (int j = 0; j < PQntuples(friend_res2); j++) {
				size_t friend_id = static_cast<size_t>(stoul(PQgetvalue(friend_res2, j, 0)));
				user* friend_ = app_user(friend_id, result);
				result[i]->followers->insert(friend_);
			}
		}
		PQclear(friend_res2);

		//Add request
		string f_queue_query =
			"SELECT user_friend_id FROM friends WHERE user_id = " + to_string(result[i]->user_details->get_user_id()) +
			" AND status = 'pending';";
		PGresult* f_queue_res = PQexec(conn, f_queue_query.c_str());
		if (check_query(f_queue_res)) {
			for (int j = 0; j < PQntuples(f_queue_res); j++) {
				size_t friend_id = static_cast<size_t>(stoul(PQgetvalue(f_queue_res, j, 0)));
				user* friend_ = app_user(friend_id, result);
				result[i]->requests.enqueue(friend_->get_username());
			}
		}
		PQclear(f_queue_res);

	}

	for (size_t i = 0; i < result.size(); i++) {
		result[i]->followers->Display_Friends();
		cin.get();
	}

	for (size_t i = 0; i < result.size(); i++) {
		// Add messages from friends where the user is person1
		string message_query1 =
			"SELECT person2_id, conversation FROM message WHERE person1_id = "
			+ to_string(result[i]->user_details->get_user_id()) + ";";

		PGresult* message_res1 = PQexec(conn, message_query1.c_str());
		if (check_query(message_res1)) {
			user* sender = app_user(result[i]->user_details->get_user_id(), result);
			for (int j = 0; j < PQntuples(message_res1); j++) {
				size_t friend_id = static_cast<size_t>(stoul(PQgetvalue(message_res1, j, 0)));
				std::string conversation = PQgetvalue(message_res1, j, 1);
				user* friend_ = app_user(friend_id, result);
				result[i]->followers->push_messages(sender->get_username(), friend_->get_username(), conversation);
			}
		}
		PQclear(message_res1);

		// Add messages from friends where the user is person2
		string message_query2 =
			"SELECT person1_id, conversation FROM message WHERE person2_id = "
			+ to_string(result[i]->user_details->get_user_id()) + ";";

		PGresult* message_res2 = PQexec(conn, message_query2.c_str());
		if (check_query(message_res2)) {
			user* recipient = app_user(result[i]->user_details->get_user_id(), result);
			for (int j = 0; j < PQntuples(message_res2); j++) {
				size_t friend_id = static_cast<size_t>(stoul(PQgetvalue(message_res2, j, 0)));
				std::string conversation = PQgetvalue(message_res2, j, 1);
				user* friend_ = app_user(friend_id, result);
				cout << "current_user = " << recipient->get_username() << "other = " << friend_->get_username() << endl;
				result[i]->followers->push_messages(recipient->get_username(), friend_->get_username(), conversation);
			}
		}
		PQclear(message_res2);
	}

	//Adding the result to App
	for (size_t i = 0; i < result.size(); i++) {
		Accounts.insert(result[i]);
	}
}

user* app_user(size_t ID,vector<info*>app) {
	for (int i = 0; i < app.size(); i++) {
		if (app[i]->user_details->get_user_id() == ID) {
			return app[i]->user_details;
		}
	}
	return nullptr;
}

void write_to_database(PGconn*&conn,users&Accounts,u_node*& user) {
	if (!conn) return;
	//user_information table
	updateUser(user, conn);
	cout << "user updated" << endl;
	cin.get();
	//posts table
	updatePost(user, conn);
	cout << "posts updated" << endl;
	cin.get();
	//notification table
	updateNotification(user, conn);
	cout << "notifications updated" << endl;
	cin.get();
	//friend
	updateFriends(Accounts, user, conn);
	cout << "friends updated" << endl;
	cin.get();
	//message table
	updateMessage(user, conn);
	cout << "messages updated" << endl;
	cin.get();
}

bool check_query(PGresult* result) {
	if (result == NULL) return false;
	ExecStatusType status = PQresultStatus(result);
	return (status == PGRES_TUPLES_OK || status == PGRES_COMMAND_OK);
}
// Check if a user exists in the database
bool userExists(const string& username, PGconn* conn) {
	string query = "SELECT username FROM user_information WHERE username = $1";
	const char* params[] = { username.c_str() };

	PGresult* res = PQexecParams(conn, query.c_str(), 1, NULL, params, NULL, NULL, 0);

	if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
		if (res) PQclear(res);
		return false;
	}

	bool exists = PQntuples(res) > 0;
	PQclear(res);
	return exists;
}
// Add a new user to the database
bool addUser(user*& newUser, PGconn* conn) {
	string query = "INSERT INTO user_information (username, password, city, created_at) VALUES ($1, $2, $3, NOW())";

	const string& username = newUser->get_username();
	const string& password = newUser->get_password();
	const string& city = newUser->get_city();

	if (username.empty() || password.empty() || city.empty()) {
		cerr << "Error: One or more fields are empty." << endl;
		return false;
	}

	const char* params[] = {
		username.c_str(),
		password.c_str(),
		city.c_str()
	};


	PGresult* res = PQexecParams(conn, query.c_str(), 3, NULL, params, NULL, NULL, 0);

	bool success = res && PQresultStatus(res) == PGRES_COMMAND_OK;
	if (res) PQclear(res);
	return success;
}
//update User function
void updateUser(u_node*& user, PGconn* conn) {
	string ui_query = "SELECT user_id, username, password, city , created_at FROM user_information WHERE user_id = " + int(user->data->user_details->get_user_id());
	PGresult* res = PQexec(conn, ui_query.c_str());
	string input_query = "UPDATE user_information SET username = '"
		+ user->data->user_details->get_username() + "', password = '"
		+ user->data->user_details->get_password() + "', city = '"
		+ user->data->user_details->get_city() + "', created_at = "
		+ to_string(long(time(0))) + " WHERE user_id = "
		+ to_string(int(user->data->user_details->get_user_id()));
	PGresult* input_res = PQexec(conn, input_query.c_str());
}
//update post function
void updatePost(u_node*& user, PGconn* conn) {
	string userId = to_string(user->data->user_details->get_user_id());
	post* Posts = user->data->user_details->get_post();
	while (Posts != nullptr) {
		string query = "SELECT title, content FROM posts WHERE (user_id =" + userId + ") AND (title = '" + Posts->getPost_title() + "') AND (content = '" + Posts->getPost_content() + "' )";
		PGresult* checkRes = PQexec(conn, query.c_str());

		if (PQresultStatus(checkRes) != PGRES_TUPLES_OK) {
			PQclear(checkRes);
			return;
		}

		if (PQntuples(checkRes) == 0) {
			string insertQuery = "INSERT INTO posts (user_id , title, content,created_at) VALUES ( "
				+ userId +", '"
				+ Posts->getPost_title() + "','"
				+ Posts->getPost_content() + "', NOW())";
			PGresult* input_res = PQexec(conn, insertQuery.c_str());

			PQclear(input_res);
		}

		PQclear(checkRes);

		Posts = user->data->user_details->get_post();
	}
}
//update notification
void updateNotification(u_node*& user, PGconn* conn) {
	notification* note = user->data->notifications.pop();
	while (note != nullptr) {
		// Prepare query to check if the notification already exists
		const char* query =
			"SELECT user_id, topic, topic_info FROM notification WHERE user_id = $1 AND topic = $2 AND topic_info = $3";
		string id = to_string(user->data->user_details->get_user_id());
		string title = note->getNotification_title();
		string content = note->getNotification_content();
		const char* params[] = { id.c_str(), title.c_str(), content.c_str() };

		PGresult* res = PQexecParams(conn, query, 3, NULL, params, NULL, NULL, 0);
		if (PQresultStatus(res) != PGRES_TUPLES_OK) {
			fprintf(stderr, "SELECT query failed: %s\n", PQerrorMessage(conn));
			PQclear(res);
			return;
		}
		if (PQntuples(res) == 0) {
			PQclear(res);

			const char* sub_query =
				"INSERT INTO notification (user_id, topic, topic_info, created_at) VALUES ($1, $2, $3, NOW())";
			PGresult* sub_res = PQexecParams(conn, sub_query, 3, NULL, params, NULL, NULL, 0);

			if (PQresultStatus(sub_res) != PGRES_COMMAND_OK) {
				fprintf(stderr, "INSERT query failed: %s\n", PQerrorMessage(conn));
				PQclear(sub_res);
				return;
			}

			PQclear(sub_res);
		}
		else {
			PQclear(res);
		}
		note = user->data->notifications.pop();
	}
}
//update message
void updateMessage(u_node*& user, PGconn* conn) {
	std::string query = "SELECT person2_id, conversation FROM message WHERE person1_id = "
		+ std::to_string(user->data->user_details->get_user_id());
	PGresult* res = PQexec(conn, query.c_str());

	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		std::cerr << "Database query failed: " << PQerrorMessage(conn) << std::endl;
		PQclear(res);
		return;
	}

	if (PQntuples(res) == 0) {
		Node* friend_node = user->data->followers->find_friend(user->data->followers->get_root_node()->user_info->get_username());
		if (friend_node && !friend_node->Conversation.isEmpty()) {
			message* info = friend_node->Conversation.pop();
			std::string insert_query = "INSERT INTO message (person1_id, person2_id, conversation) VALUES ("
				+ std::to_string(user->data->user_details->get_user_id()) + ", "
				+ std::to_string(friend_node->user_info->get_user_id()) + ", '"
				+ PQescapeLiteral(conn, info->getUserMessage().c_str(), info->getUserMessage().length()) + "')";
			PGresult* insert_res = PQexec(conn, insert_query.c_str());

			if (PQresultStatus(insert_res) != PGRES_COMMAND_OK) {
				std::cerr << "Failed to insert new message: " << PQerrorMessage(conn) << std::endl;
			}
			PQclear(insert_res);
		}
	}
	else {
		for (int i = 0; i < PQntuples(res); i++) {
			std::string friend_username = PQgetvalue(res, i, 0);
			std::string message_content = PQgetvalue(res, i, 1);

			Node* friend_node = user->data->followers->find_friend(friend_username);
			if (friend_node) {
				std::string check_query = "SELECT * FROM message WHERE person1_id = "
					+ std::to_string(user->data->user_details->get_user_id())
					+ " AND person2_id = " + std::to_string(friend_node->user_info->get_user_id())
					+ " AND conversation = '" + PQescapeLiteral(conn, message_content.c_str(), message_content.length()) + "'";
				PGresult* check_res = PQexec(conn, check_query.c_str());

				if (PQresultStatus(check_res) != PGRES_TUPLES_OK) {
					std::cerr << "Database check query failed: " << PQerrorMessage(conn) << std::endl;
					PQclear(check_res);
					continue;
				}

				if (PQntuples(check_res) == 0) {
					std::string insert_query = "INSERT INTO message (person1_id, person2_id, conversation) VALUES ("
						+ std::to_string(user->data->user_details->get_user_id()) + ", "
						+ std::to_string(friend_node->user_info->get_user_id()) + ", '"
						+ PQescapeLiteral(conn, message_content.c_str(), message_content.length()) + "')";
					PGresult* insert_res = PQexec(conn, insert_query.c_str());

					if (PQresultStatus(insert_res) != PGRES_COMMAND_OK) {
						std::cerr << "Failed to insert new message: " << PQerrorMessage(conn) << std::endl;
					}
					else {
						std::cout << "New message inserted for " << friend_username << ": " << message_content << std::endl;
					}
					PQclear(insert_res);
				}
				PQclear(check_res);
			}
		}
	}

	// Clear the result
	PQclear(res);
}
//update friends
void updateFriends(users& Accounts, u_node*& user, PGconn* conn) {

	//INSERTING THE user from the queue
	while (!(user->data->requests.isEmpty())) {
		q_node<string>* friend_name = user->data->requests.Top();
		u_node* friend_request = Accounts.find(friend_name->data);
		user->data->requests.dequeue();
		string check_query = "SELECT status FROM friends WHERE user_id = "
			+  to_string(user->data->user_details->get_user_id())
			+ " AND user_friend_id = " + to_string(friend_request->data->user_details->get_user_id());
		PGresult* check_res = PQexec(conn, check_query.c_str());
		if (PQntuples(check_res) == 0) {
			string check_query = "INSERT INTO friends (user_id,user_friend_id,status,created_at) VALUE($1,$2,'pending',NOW())";
			const char* params[] = {
				to_string(user->data->user_details->get_user_id()).c_str(),
				to_string(friend_request->data->user_details->get_user_id()).c_str(),
			};
			PGresult* check_res = PQexecParams(conn, check_query.c_str(), 2, nullptr, params, nullptr, nullptr, 0);
		}
	}

	/*for (Node* friend_node = user->data->followers->get_root_node(); friend_node != nullptr;) {

		string friend_username = friend_node->user_info->get_username();
		int friend_id = friend_node->user_info->get_user_id();

		string check_query = "UPDATE friends SET status = accepted WHERE (user_id = "
			+ to_string(friend_id)
			+ " AND user_friend_id = " + to_string(user->data->user_details->get_user_id()) + ") OR ( user_id = "
			+ to_string(user->data->user_details->get_user_id())
			+ " AND user_friend_id = " + to_string(friend_id) + ") ";
		PGresult* check_res = PQexec(conn, check_query.c_str());

		PQclear(check_res);

		if (friend_node->left) {
			friend_node = friend_node->left;
		}
		else if (friend_node->right) {
			friend_node = friend_node->right;
		}
		else {
			break;
		}
	}*/
}
