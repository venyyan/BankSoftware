#include <iostream>
#include <string>
#include <regex>
#include <sstream>
using namespace std;

const char COMMAND_LOGIN = 'L';
const char COMMAND_REGISTER = 'R';
const char COMMAND_QUIT = 'Q';
const char COMMAND_CANCEL_ACCOUNT = 'C';
const char COMMAND_DEPOSIT = 'D';
const char COMMAND_LOGOUT = 'L';
const char COMMAND_TRANSFER = 'T';
const char COMMAND_WITHDRAW = 'W';


const string MESSAGE_OPENING = "Hello! You are now using BankSoftware.\nIf you want to Log In type \"L\"\n"
"If you want to Register type \"R\" \nIf you want to Quit type \"Q\"";
const string MESSAGE_NOT_VALID_USERNAME = "Not a valid username!";
const string MESSAGE_NOT_VALID_PASSWORD = "Not a valid password!";
const string MESSAGE_USER_ALREADY_EXISTS = "User already exists!";
const string MESSAGE_PASSWORD_DOES_NOT_MATCH_USERNAME = "Password does not match username. Try again.";
const string MESSAGE_USER_DOES_NOT_EXIST = "User does not exist! Try again or register.";
const string MESSAGE_AFTER_LOGGING_FIRST = "Registered successfully!\nYou have ";
const string MESSAGE_AFTER_LOGGING_SECOND = "Choose one of"
"the following options: \nC - cancel account\nD - deposit\nL - logout\nT - transfer\nW - withdraw";
const string MESSAGE_ENTER_PASSWORD = "Please, enter your password: ";
const string MESSAGE_DEPOSIT = "Please enter deposit amount: ";
const string MESSAGE_NOT_POSITIVE_AMOUNT = "You entered a non-positive amount!";
const string MESSAGE_SUCCESSFUL_DEPOSIT = "You successfully deposited ";
const string MESSAGE_EXCEEDED_OVERDRAFT = "The maximal overdraft is 10000! You exceed it by ";
const string MESSAGE_TRANSFER = "You successfully transfered ";

const string REGEX_USERNAME = "^[A-Za-z\!-/\:-@\[-_]+$";
const string REGEX_PASSWORD = "[A-Za-z\\\\0-9\!\@\#\$\%\^\&\*]+$";
const string REGEX_PASSWORD_CONTAINS_AT_LEAST_ONE_UPPERCASE_LOWERCASE_SYMBOL =
"(?=.*[a-z])(?=.*[A-Z])(?=.*\\W)^[^ ]+$";

const unsigned MINIMUM_PASSWORD_LENGTH = 5;

const int MAX_OVERDRAFT = -10000;
string registerUser(string, string, bool&);
string loginUser(string, string, bool&);
string cancelAccount();
string deposit();
string hashedPassword(string);
string transfer(string, double);
bool isValidUsername(string);
bool isValidPassword(string);
bool userAlreadyExists(string);
bool passwordMatchesUsername(string, string);
bool isPositiveAmount(double);
vector<string> returnUser(string, string, size_t&);


vector<vector<string>> userData;
vector<string> loggedUser;
size_t userId;

int main()
{
	cout << MESSAGE_OPENING;

	char command = ' ';
	bool isLogged = false;

	while (command != COMMAND_QUIT) {
		cin >> command;
		string username;
		string password;

		if (command == COMMAND_LOGIN) {
			cin >> username >> password;
			cout << loginUser(username, password, isLogged);
			if (isLogged) {
				loggedUser = returnUser(username, password, userId);
			}
		}
		else if (command == COMMAND_REGISTER) {
			cin >> username >> password;
			cout << registerUser(username, password, isLogged);
			if (isLogged) {
				loggedUser = returnUser(username, password, userId);
			}
		}
		else if (command == COMMAND_CANCEL_ACCOUNT) {
			cout << MESSAGE_ENTER_PASSWORD;
			cin >> password;
			cout << cancelAccount();
		}
		else if (command == COMMAND_DEPOSIT) {
			cout << deposit();
		}
		else if (command == COMMAND_LOGOUT) {
			cout << MESSAGE_OPENING;
			isLogged = false;
		}
		else if (command == COMMAND_TRANSFER) {
			double amount;
			string username;
			cin >> amount >> username;
			cout << transfer(username, amount);
		}
	}
}

string registerUser(string username, string password, bool& isLogged) {
	if (!isValidUsername(username)) {
		isLogged = false;
		return MESSAGE_NOT_VALID_USERNAME;
	}
	if (!isValidPassword(password)) {
		isLogged = false;
		return MESSAGE_NOT_VALID_PASSWORD;
	}
	if (userAlreadyExists(username)) {
		isLogged = false;
		return MESSAGE_USER_ALREADY_EXISTS;
	}
	isLogged = true;
	vector<string> user = { username, hashedPassword(password), "0" };
	userData.push_back(user);

	return MESSAGE_AFTER_LOGGING_FIRST + "0" + MESSAGE_AFTER_LOGGING_SECOND;
}

string loginUser(string username, string password, bool& isLogged) {
	if (!userAlreadyExists(username)) {
		isLogged = false;
		return MESSAGE_USER_DOES_NOT_EXIST;
	}

	if (!passwordMatchesUsername(username, hashedPassword(password))) {
		isLogged = false;
		return MESSAGE_PASSWORD_DOES_NOT_MATCH_USERNAME;
	}
	return MESSAGE_AFTER_LOGGING_SECOND;
}

string cancelAccount() {
	
	string username = loggedUser[0];
	string password = loggedUser[1];
	if (!passwordMatchesUsername(username, hashedPassword(password))) {
		return MESSAGE_PASSWORD_DOES_NOT_MATCH_USERNAME;
	}

	userData.erase(userData.begin() + userId - 1);
	return MESSAGE_OPENING;
}

string deposit() {
	double depositAmount;
	cout << MESSAGE_DEPOSIT;
	cin >> depositAmount;

	if (!isPositiveAmount(depositAmount)) {
		return MESSAGE_NOT_POSITIVE_AMOUNT;
	}
	string depositAmountStr = to_string(ceil(depositAmount * 100.0) / 100.0);

	double balanceAfterDeposit = stof(loggedUser[2]) + depositAmount;
	string balanceAfterDepositStr = to_string(balanceAfterDeposit);

	loggedUser.at(2) = balanceAfterDepositStr;
	userData.at(userId) = loggedUser;

	return MESSAGE_SUCCESSFUL_DEPOSIT + depositAmountStr + " BGN";
}

string transfer(string usernameToSend, double amount) {
	if (!userAlreadyExists(usernameToSend)) {
		return MESSAGE_NOT_VALID_USERNAME;
	}
	double balance = stof(loggedUser[2]);
	if (balance - amount < MAX_OVERDRAFT) {
		return MESSAGE_EXCEEDED_OVERDRAFT + to_string(abs(balance - amount + MAX_OVERDRAFT)) + " BGN";
	}
	
	double balanceAfterSendingMoney = stod(loggedUser[2]) - amount;
	string balanceAfterSendingMoneyStr = to_string(balanceAfterSendingMoney);
	loggedUser.at(2) = balanceAfterSendingMoneyStr;
	userData.at(userId) = loggedUser;

	return MESSAGE_TRANSFER + to_string(amount) + " BGN!";
}

vector<string> returnUser(string username, string password, size_t& userId) {
	vector<string> user;
	for (size_t i = 0; i < userData.size(); i++)
	{
		if (userData[i][0] == username && userData[i][1] == password) {
			user = { username, password, userData[i][2] };
			userId = i;
			break;
		}
	}
	return user;
}

string hashedPassword(string password) {
	hash<string> passwordHasher;
	string hashedPassword = to_string(passwordHasher(password));
	return hashedPassword;
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

bool passwordMatchesUsername(string username, string password) {
	for (int i = 0; i < userData.size(); i++)
	{
		if (userData[i][0] == username && userData[i][1] == password) {
			return true;
		}
	}
	return false;
}

bool isPositiveAmount(double amount) {
	return amount > 0;
}