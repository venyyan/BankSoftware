#include <iostream>
#include <string>
#include <regex>
using namespace std;

const char COMMAND_LOGIN = 'L';
const char COMMAND_REGISTER = 'R';
const char COMMAND_QUIT = 'Q';


const string MESSAGE_OPENING = "Hello! You are now using BankSoftware.\nIf you want to Log In type \"L\"\n"
"If you want to Register type \"R\" \nIf you want to Quit type \"Q\"";
const string MESSAGE_NOT_VALID_USERNAME = "Not a valid username!";
const string MESSAGE_NOT_VALID_PASSWORD = "Not a valid password!";
const string MESSAGE_USER_ALREADY_EXISTS = "User already exists!";
const string MESSAGE_AFTER_LOGGING = "Registered successfully!\nYou have 0 BGN. Choose one of"
"the following options: \nC - cancel account\nD - deposit\nL - logout\nT - transfer\nW - withdraw";



const string REGEX_USERNAME = "^[A-Za-z\!-/\:-@\[-_]+$";
const string REGEX_PASSWORD = "[A-Za-z\\\\0-9\!\@\#\$\%\^\&\*]+$";
const string REGEX_PASSWORD_CONTAINS_AT_LEAST_ONE_UPPERCASE_LOWERCASE_SYMBOL =
"(?=.*[a-z])(?=.*[A-Z])(?=.*\\W)^[^ ]+$";

const unsigned MINIMUM_PASSWORD_LENGTH = 5;


string registerUser(string, string);
bool isValidUsername(string);
bool isValidPassword(string);
bool userAlreadyExists(string);

vector<vector<string>> userData;

int main()
{
	cout << MESSAGE_OPENING;

	char command = ' ';

	while (command != COMMAND_QUIT) {
		cin >> command;

		if (command == COMMAND_LOGIN) {
			//login logic here
		}
		else if (command == COMMAND_REGISTER) {
			string username;
			string password;
			cin >> username >> password;
			cout << registerUser(username, password);
		}
	}
}

string registerUser(string username, string password) {
	if (!isValidUsername(username)) {
		return MESSAGE_NOT_VALID_USERNAME;
	}
	if (!isValidPassword(password)) {
		return MESSAGE_NOT_VALID_PASSWORD;
	}
	if (userAlreadyExists(username)) {
		return MESSAGE_USER_ALREADY_EXISTS;
	}
	hash<string> passwordHasher;
	string hashedPassword = to_string(passwordHasher(password));

	vector<string> user = { username, hashedPassword, "0" };
	userData.push_back(user);

	return MESSAGE_AFTER_LOGGING;
}

bool isValidUsername(string username) {
	if (!regex_match(username, regex(REGEX_USERNAME))) {
		return false;
	}
}

bool isValidPassword(string password) {
	if (!regex_match(password, regex(REGEX_PASSWORD))) {
		return false;
	}

	if (password.length() < MINIMUM_PASSWORD_LENGTH) {
		return false;
	}

	if (!regex_match(password, regex(REGEX_PASSWORD_CONTAINS_AT_LEAST_ONE_UPPERCASE_LOWERCASE_SYMBOL))) {
		return false;
	}
}

bool userAlreadyExists(string username) {
	for (int i = 0; i < userData.size(); i++)
	{
		if (userData[i][0] == username) {
			return true;
		}
	}

	return false;
}


