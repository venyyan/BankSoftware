#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <fstream>
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
const string MESSAGE_WITHDRAW = "You successfully withdrew ";
const string MESSAGE_NOT_VALID_COMMAND = "Not a valid command!";

const string REGEX_USERNAME = "^[A-Za-z\!-/\:-@\[-_]+$";
const string REGEX_PASSWORD = "[A-Za-z\\\\0-9\!\@\#\$\%\^\&\*]+$";
const string REGEX_PASSWORD_CONTAINS_AT_LEAST_ONE_UPPERCASE_LOWERCASE_SYMBOL =
"(?=.*[a-z])(?=.*[A-Z])(?=.*\\W)^[^ ]+$";

const unsigned MINIMUM_PASSWORD_LENGTH = 5;

const int MAX_OVERDRAFT = -10000;
string registerUser(bool&);
string loginUser(bool&);
string cancelAccount();
string deposit();
string hashedPassword(string);
string transfer();
string withdraw();
int returnUserIdByUsername(string, string&);
bool isValidUsername(string);
bool isValidPassword(string);
bool userAlreadyExists(string);
bool passwordMatchesUsername(string, string);
bool isPositiveAmount(double);
vector<string> returnUser(string, string, size_t&);
vector<string> splitLineFromFile(string);
void getInformationFromFile();
void updateFile();


vector<vector<string>> userData;
vector<string> loggedUser;
size_t userId;

int main()
{
	getInformationFromFile();
	cout << MESSAGE_OPENING << endl;

	char command = ' ';
	bool isLogged = false;

	while (command != COMMAND_QUIT) {
		cin >> command;
		string username;
		string password;

		if (!isLogged) {
			switch (command) {
			case COMMAND_LOGIN: cout << loginUser(isLogged);
				break;
			case COMMAND_REGISTER: cout << registerUser(isLogged);
				break;
			case COMMAND_QUIT: return 0;
				break;
			default: cout << MESSAGE_NOT_VALID_COMMAND;
				break;
			}
		}
		else {
			switch (command) {
			case COMMAND_CANCEL_ACCOUNT: cout << cancelAccount();
				break;
			case COMMAND_DEPOSIT: cout << deposit();
				break;
			case COMMAND_LOGOUT: cout << MESSAGE_OPENING; 
				isLogged = false;
				break;
			case COMMAND_TRANSFER: cout << transfer();
				break;
			case COMMAND_WITHDRAW: cout << withdraw();
				break;
			default: cout << MESSAGE_NOT_VALID_COMMAND;
				break;
			}
			
		}
		updateFile();
	}
}

string registerUser(bool& isLogged) {
	string username, password;
	cin >> username >> password;
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
	if (isLogged) {
		loggedUser = returnUser(username, password, userId);
	}

	return MESSAGE_AFTER_LOGGING_FIRST + "0" + MESSAGE_AFTER_LOGGING_SECOND;
}

string loginUser(bool& isLogged) {
	string username, password;
	cin >> username >> password;
	if (!userAlreadyExists(username)) {
		isLogged = false;
		return MESSAGE_USER_DOES_NOT_EXIST;
	}

	if (!passwordMatchesUsername(username, hashedPassword(password))) {
		isLogged = false;
		return MESSAGE_PASSWORD_DOES_NOT_MATCH_USERNAME;
	}

	isLogged = true;
	if (isLogged) {
		loggedUser = returnUser(username, password, userId);

	}
	return MESSAGE_AFTER_LOGGING_SECOND;
}

string cancelAccount() {
	cout << MESSAGE_ENTER_PASSWORD;
	string password;
	cin >> password;

	string username = loggedUser[0];
	if (!passwordMatchesUsername(username, hashedPassword(password))) {
		return MESSAGE_PASSWORD_DOES_NOT_MATCH_USERNAME;
	}

	userData.erase(userData.begin() + userId);
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

string transfer() {
	double amount;
	string usernameToSend;
	cin >> amount >> usernameToSend;
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

	string balanceOfUserToSend;
	int idOfUsernameToSend = returnUserIdByUsername(usernameToSend, balanceOfUserToSend);


	double balanceAfterReceivingMoney = stod(balanceOfUserToSend) + amount;
	string balanceAfterReceivingMoneyStr = to_string(balanceAfterReceivingMoney);
	userData.at(idOfUsernameToSend)[2] = balanceAfterReceivingMoneyStr;

	return MESSAGE_TRANSFER + to_string(amount) + " BGN!";
}

string withdraw() {
	double amount;
	cin >> amount;

	double balance = stof(loggedUser[2]);
	if (balance - amount < MAX_OVERDRAFT) {
		return MESSAGE_EXCEEDED_OVERDRAFT + to_string(abs(balance - amount + MAX_OVERDRAFT)) + " BGN";
	}

	double balanceAfterWithdrawing = stod(loggedUser[2]) - amount;
	string balanceAfterWithdrawingStr = to_string(balanceAfterWithdrawing);
	loggedUser.at(2) = balanceAfterWithdrawingStr;
	userData.at(userId) = loggedUser;
	return MESSAGE_TRANSFER + to_string(amount) + " BGN!";
}

vector<string> returnUser(string username, string password, size_t& userId) {
	vector<string> user;
	for (size_t i = 0; i < userData.size(); i++)
	{
		for (int j = 0; j < userData[i].size(); j++) {

			if (userData[i][0] == username && userData[i][1] == hashedPassword(password)) {
				user = { username, hashedPassword(password), userData[i][2] };
				userId = i;
				break;
			}
			break;
		}
	}
	return user;
}

int returnUserIdByUsername(string username, string& balanceOfUserToSend) {
	int idOfUsernameToSend = 0;
	for (int i = 0; i < userData.size(); i++) {
		for (int j = 0; j < userData[i].size(); j++) {
			if (userData[i][0] == username) {
				idOfUsernameToSend = i;
				balanceOfUserToSend = userData[i][2];
				break;
			}
		}
	}
	return idOfUsernameToSend;
}

void getInformationFromFile() {
	fstream file;
	file.open("BankSoftwareDataBase.txt", fstream::in);
	string buffer;

	while (getline(file, buffer)) {
		userData.push_back(splitLineFromFile(buffer));
	}
	file.close();
}

void updateFile() {
	fstream file;

	file.open("BankSoftwareDataBase.txt", fstream::out);
	for (int i = 0; i < userData.size(); i++) {
		string input;
		for (int j = 0; j < userData[i].size(); j++) {
			input = userData[i][0] + ':' + userData[i][1] + ':' + userData[i][2];
			file << input << "\n";
			break;
		}
	}
	file.close();
}
vector<string> splitLineFromFile(string buffer) {
	vector<string> splitted;
	int start = 0;
	int end = 0;
	string token;
	while ((end = buffer.find(':', start)) != string::npos) {
		token = buffer.substr(start, end - start);
		start = end + 1;
		splitted.push_back(token);
	}
	splitted.push_back(buffer.substr(start));
	return splitted;
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
		for (int j = 0; j < userData[i].size(); j++) {
			if (userData[i][0] == username) {
				return true;
			}
		}
	}
	return false;
}

bool passwordMatchesUsername(string username, string password) {
	for (int i = 0; i < userData.size(); i++)
	{
		for (int j = 0; j < userData[i].size(); j++) {

			if (userData[i][0] == username && userData[i][1] == password) {
				return true;
			}
		}
	}
	return false;
}

bool isPositiveAmount(double amount) {
	return amount > 0;
}