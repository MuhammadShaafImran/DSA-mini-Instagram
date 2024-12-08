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
void login_function(int choice, users& Accounts, PGconn*& conn);
void post_creation(u_node*& user);
void Display_admin_menu();
void Display_login_menu(u_node*& user);
void handle_messages(u_node*& user, Node*& username, int option);
void insert_users_to_App(PGconn* conn, users& Account);
bool check_query(PGresult* res);
void Signup_preview(users& Accounts, PGconn*& conn);
void Login_preview(PGconn* conn, u_node*& user, users& Accounts);
void write_to_database(PGconn* conn, u_node*& user);
size_t generateUniqueID(PGconn* conn);

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
		login_function(choice,Accounts,conn);
	}

	PQfinish(conn);

	system("pause");
	return 0;
}


bool checkConn(PGconn* conn) {
	return (PQstatus(conn) == CONNECTION_OK);
}
//
void Display_admin_menu() {
	cout << " SignUp/Login Form" << endl;
	cout << "1. Create Account." << endl;
	cout << "2. Login Account." << endl;
	cout << "Exit (0)" << endl;
	cout << "Enter Choice : ";
}
void login_function(int choice,users &Accounts, PGconn*& conn) {
	string username, password, re_password, city;
	user* newUser{ new user() };
	u_node* user{ nullptr };
	int list_choice{ -1 };

	switch (choice) {
		//Sign-Up information
		case 1: 
			Signup_preview(Accounts, conn);
			break;
		//login section
		case 2:
			Login_preview(conn,user, Accounts);
			if (user != nullptr) {
				while (true) {
					system("CLS");
					Display_login_menu(user);
					//User input
					cout << "Enter choice." << endl;
					cin >> list_choice; cin.ignore();
					if (list_choice == 0) break;
					user_functions(conn,Accounts, user, list_choice);
					write_to_database(conn, user);
				}
			}
			else {
				cout << "Issue in login." << endl;
			}
			break;
		default:
			cout << "Pick a correct once" << endl;
			cin.get();
	}
}
void Signup_preview(users& Accounts, PGconn*& conn) {
	string username, password, re_password, city;
	unique_ptr<user> newUser{ new user() }; 

	cout << "Create Account" << endl;
	cout << "Username: "; getline(cin, username);

	while (true) {
		cout << "Password: "; getline(cin, password);
		cout << "Confirm Password: "; getline(cin, re_password);
		if (password == re_password) break;
		else {
			cout << "Passwords do not match. Please try again." << endl;
		}
	}

	cout << "City: "; getline(cin, city);

	// Check for existing user in the database
	string ch_query = "SELECT username FROM user_information WHERE username = $1";
	const char* const user_name[] = { username.c_str() };

	PGresult* check_res = PQexecParams(conn, ch_query.c_str(), 1, NULL, user_name, NULL, NULL, 0);

	if (check_query(check_res)) {
		if (PQntuples(check_res) != 0) {
			cout << "User already exists in the database." << endl;
			PQclear(check_res);
			cin.get();
			return;
		}
	}
	else {
		cout << "Error occurred: " << PQerrorMessage(conn) << endl;
		PQclear(check_res);
		cin.get();
		return;
	}

	PQclear(check_res);

	// Hash password before storing (pseudo-code)
	// string hashed_password = hash_function(password);

	// Adding user to database using parameterized query
	string sub_query = "INSERT INTO user_information (user_id, username, password, city, created_at) VALUES ($1, $2, $3, $4, $5)";
	size_t user_id = generateUniqueID(conn);
	cout << to_string(user_id).c_str() << endl;
	cin.get();

	const char* params[5] = {
		to_string(user_id).c_str(),
		username.c_str(),
		password.c_str(),
		city.c_str(),
		to_string(time(0)).c_str()
	};
	PGresult* res = PQexecParams(conn, sub_query.c_str(), 5, NULL, params, NULL, NULL, 0);

	if (check_query(res)) {
		cout << "User added to database." << endl;
		cin.get();
	}
	else {
		cout << "Error occurred: " << PQerrorMessage(conn) << endl;
		cin.get();
	}

	PQclear(res);
}
void Login_preview(PGconn* conn,u_node*& user,users& Accounts) {
	insert_users_to_App(conn,Accounts);
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
		if (user == nullptr) {
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
	cout << "6. Followers List " << endl;
	cout << "7. Follow list" << endl;
	cout << "8. Cancel and Accept Requests" << endl;
	cout << "Exit (0)" << endl;
}
//
void user_functions(PGconn* conn, users &Accounts,u_node*& user,int choice) {
	string username,helper;
	int option{ -1 };
	system("CLS");
	switch (choice) {
	case 1:
		cout << "News Feed" << endl;
		user->data->user_details->newsFeed_function();
		cin.ignore();
		break;
	
	case 2:
		while (true) {
			cout << "1.Create posts" << endl;
			cout << "2.View posts" << endl;
			cout << "Esc to exits." << endl;
			cout << "Enter choice: "; cin >> option; cin.ignore();
			if (option == 27) break;
			switch (option) {
				case 1:
					post_creation(user);
					break;
				case 2:
					user->data->followers->Display_user_friend_posts(); //Post of user friend's
					break;
				default:
					cout << "Enter correct key" << endl;
			}
		}
		break;
	case 3:
		cout << "Notifications:" << endl;
		Accounts.view_notifications(user);
		cin.get();
		break;
	case 4:
		cout << "Search user:" << endl;
		//TODO: implement search user
		break;
	case 5:
		while (true) {
			cout << "Messaging portal" << endl;
			user->data->followers->Display_Friends();
			cout << "username : "; getline(cin, username);
			//Check is the user exists
			Node* Friend = user->data->followers->find_friend(username);
			if (Friend != nullptr) {
				//TODO : Push a message 
				//TODO : View a message		
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
			Accounts.send_request_function(user, username);
		}

		break;
	case 8:
		cout << "Requests List: " << endl;
		user->data->requests.display();
		cout << "Enter username:"; getline(cin,username);
		cout << "Command(accept, reject):"; getline(cin, helper);
		Accounts.accept_reject_list(user,username,helper);
		cin.get();
		break;
	default:
		cout << "Pick a correct choice" << endl;
		cin.get();
	}
}
void post_creation(u_node*& user) {
	string title_, content_;
	cout << "Enter title : "; getline(cin, title_);
	cout << "Enter content : "; getline(cin, content_);
	user->data->user_details->add_post(title_,content_);
}
void handle_messages(u_node*&user, Node*& username ,int option) {
	string title_, conversation;
	switch (option) {
	case (1):
		cout << "Enter info: "; getline(cin, conversation);
		username->Conversation.push(new message(username->user_info->get_username(), conversation));
		break;
	case (2):
		cout << "Conversation with" << username->user_info->get_username() << endl;
		username->Conversation.Display();
		cin.get();
	default:
		cout << "Enter a correct key" << endl;

	}
}
void insert_users_to_App(PGconn* conn , users& Account) {
	if (!conn) return;
	// Query to fetch data from the "user_information" table
	const char* query = "SELECT user_id, username, password, city , created_at FROM user_information;";
	PGresult* res = PQexec(conn, query);
	vector<info*>result;
	if (check_query(res)) {
		for (size_t i = 0; i < PQntuples(res); i++) {
			user* person = new user;
			person->set_info(size_t(PQgetvalue(res,i,0)), PQgetvalue(res, i, 1), PQgetvalue(res, i, 2), PQgetvalue(res, i, 3),size_t(PQgetvalue(res, i, 4)));
			result.push_back(new info(person));
		}
	}
	PQclear(res);
	for (size_t i = 0; i < result.size(); i++) {
		const char* post_query = "SELECT pt.post_title,pt.post_content FROM posts AS pt FULL OUTER JOIN user AS ui WHERE ui.user_id = "+ result[i]->user_details->get_user_id(); //+ char("; ")
		PGresult* post_res = PQexec(conn, post_query);
		if (check_query(post_res)) {
			for (size_t j = 0; j < PQntuples(post_res); j++) {
				result[i]->user_details->add_post(PQgetvalue(post_res, j, 0), PQgetvalue(post_res, j, 1));
			}
		}
		PQclear(post_res);
		//const char* friend_query = ("SELECT f.friend_id FROM friend AS f WHERE (" + to_string(result[i]->user_details->get_user_id()) + " = f.user_id ) AND (f.status = 'accepted') ").c_str();
		//TODO: find the friend with the help of user_id
	}

}
void write_to_database(PGconn*conn,u_node*& user) {
	if (!conn) return;

	//user information table

	char ui_query[256];
	snprintf(ui_query, sizeof(ui_query), "SELECT user_id, username, password, city , created_at FROM user_information WHERE %d = user_id;",int(user->data->user_details->get_user_id()));
	PGresult* res = PQexec(conn, ui_query);
	if (PQntuples(res) == 0) {
		char input_query[256];
		snprintf(input_query, sizeof(input_query), "INSERT INTO user_information (user_id,username,password,city,created_at) VALUES (%d ,'%s','%s','%s' ,%d)", int(user->data->user_details->get_user_id()), user->data->user_details->get_username(), user->data->user_details->get_password(), user->data->user_details->get_city(), int(user->data->user_details->get_Time()));
		PGresult* input_res = PQexec(conn, input_query);
		PQclear(input_res);
	}
	else {
		char input_query[256];
		snprintf(input_query, sizeof(input_query), "UPDATE user_information SET username = '%s', password = '%s' , city = '%s',created_at = %d WHERE user_id = %d", user->data->user_details->get_username(), user->data->user_details->get_password(), user->data->user_details->get_city(), time(0), int(user->data->user_details->get_user_id()));
		PGresult* input_res = PQexec(conn, input_query);
		PQclear(input_res); 
	}

	////post table

	//char pt_query[256];
	//snprintf(pt_query, sizeof(pt_query), "SELECT user_id, username, password, city , created_at FROM user_information WHERE %d = user_id;", int(user->data->user_details->get_user_id()));
	//PGresult* res = PQexec(conn, pt_query);
	//if (PQntuples(res) == 0) {
	//	char input_query[256];
	//	snprintf(input_query, sizeof(input_query), "INSERT INTO user_information (user_id,username,password,city,created_at) VALUES (%d ,'%s','%s','%s' ,%d)", int(user->data->user_details->get_user_id()), user->data->user_details->get_username(), user->data->user_details->get_password(), user->data->user_details->get_city(), int(user->data->user_details->get_Time()));
	//	PGresult* input_res = PQexec(conn, input_query);
	//	PQclear(input_res);
	//}
	//else {
	//	char input_query[256];
	//	snprintf(input_query, sizeof(input_query), "UPDATE user_information SET username = '%s', password = '%s' , city = '%s',created_at = %d WHERE user_id = %d", user->data->user_details->get_username(), user->data->user_details->get_password(), user->data->user_details->get_city(), time(0), int(user->data->user_details->get_user_id()));
	//	PGresult* input_res = PQexec(conn, input_query);
	//	PQclear(input_res);
	//}


}
bool check_query(PGresult* res) {
	if (res == NULL) {
		return false;
	}
	return PQresultStatus(res) == PGRES_TUPLES_OK;
}
// Function to generate a unique ID
size_t generateUniqueID(PGconn*conn) {
	const char* ID_query = "SELECT user_id FROM user_information";
	PGresult* ID_res = PQexec(conn, ID_query);
	vector<size_t>generated_ids;
	size_t newID;

	if (PQntuples(ID_res) == 0) {
		PQclear(ID_res);
	 	return 1;
	}
	for (size_t i = 0; i < PQntuples(ID_res); i++) {
		generated_ids.push_back(size_t(PQgetvalue(ID_res, i, 0)));
	}
	//Check for the next uinque ID
	do {
		newID = std::rand() % 10000;
	} while (find(generated_ids.begin(),generated_ids.end(),newID) != generated_ids.end()); 
	
	return newID;
}