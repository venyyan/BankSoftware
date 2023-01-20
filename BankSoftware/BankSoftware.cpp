/**
*
* Solution to course project #09
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2022/2023
*
* @author Venelina Andreeva
* @idnumber 0MI0600184
* @compiler <VC>
*
* Realize BankSoftware
*
*/

#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <fstream>
#include "global_constants.h"
#include <iomanip>  
using namespace std;

string registerUser(bool&);
string loginUser(bool&);
string cancelAccount();
string deposit();
string hashedPassword(string);
string transfer();
string withdraw();
string logout(bool&);
string roundDouble(string);
string takeMoneyFromLoggedUser(string, bool&);
double stringToDouble(string);
int returnUserIdByUsername(string, string&);
bool isValidUsername(string);
bool isValidPassword(string);
bool userAlreadyExists(string);
bool passwordMatchesUsername(string, string);
bool isPositiveAmount(string);
bool isValidNumeral(string);
bool exceededOverdraft(double, double);
string validateInputData(string);
vector<string> returnUser(string, string, size_t&);
vector<string> splitLineFromFile(string);
void getInformationFromFile(bool&);
void updateFile();


vector<vector<string>> userData;
vector<string> loggedUser;
size_t userId;

int main()
{
	bool isFileOpened = true;
	getInformationFromFile(isFileOpened);

	if (!isFileOpened) {
		return 0;
	}

	cout << MESSAGE_OPENING << endl;

	char command = ' ';
	bool isLogged = false;

	while (command != COMMAND_QUIT) {
		cin >> command;
		string username;
		string password;

		if (!isLogged) {
			switch (command) {
			case COMMAND_LOGIN: cout << loginUser(isLogged) << endl;
				break;
			case COMMAND_REGISTER: cout << registerUser(isLogged) << endl;
				break;
			case COMMAND_QUIT: return 0;
				break;
			default: cout << MESSAGE_NOT_VALID_COMMAND;
				break;
			}
		}
		else {
			switch (command) {
			case COMMAND_CANCEL_ACCOUNT: cout << cancelAccount() << endl;
				break;
			case COMMAND_DEPOSIT: cout << deposit() << endl;
				break;
			case COMMAND_LOGOUT: cout << logout(isLogged) << endl;
				break;
			case COMMAND_TRANSFER: cout << transfer() << endl;
				break;
			case COMMAND_WITHDRAW: cout << withdraw() << endl;
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

	cout << MESSAGE_USERNAME_REGISTER;
	cin >> username;
	cout << MESSAGE_PASSWORD_REGISTER;
	cin >> password;

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
	vector<string> user = { username, hashedPassword(password), ZERO_BALANCE };
	userData.push_back(user);
	if (isLogged) {
		loggedUser = returnUser(username, password, userId);
	}
	system("cls");
	return MESSAGE_AFTER_REGISTERING + ZERO_BALANCE + " " + "BGN. " + MESSAGE_AFTER_LOGGING;
}

string loginUser(bool& isLogged) {
	string username, password;

	cout << MESSAGE_ENTER_USERNAME;
	cin >> username;
	cout << MESSAGE_ENTER_PASSWORD;
	cin >> password;

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
	system("cls");
	return MESSAGE_AFTER_LOGGING_FIRST + loggedUser[2] + " BGN. " + MESSAGE_AFTER_LOGGING;
}

string logout(bool& isLogged) {
	isLogged = false;
	loggedUser.clear();
	userId = NULL;

	system("cls");
	return MESSAGE_AFTER_LOGOUT + "\n" + MESSAGE_OPENING;
}

string cancelAccount() {
	double balance = stringToDouble(loggedUser[2]);

	if (balance > 0) {
		system("cls");
		return MESSAGE_BALANCE_IS_NOT_ZERO + MESSAGE_REMAINING_AMOUNT
			+ roundDouble(to_string(balance)) + " BGN." + "\n" + MESSAGE_AFTER_LOGGING;
	}

	cout << MESSAGE_ENTER_PASSWORD;
	string password;
	cin >> password;

	string username = loggedUser[0];
	if (!passwordMatchesUsername(username, hashedPassword(password))) {
		return MESSAGE_PASSWORD_DOES_NOT_MATCH_USERNAME;
	}
	userData.erase(userData.begin() + userId);

	system("cls");
	return MESSAGE_CANCEL_ACCOUNT + "\n" + MESSAGE_OPENING;
}

string deposit() {
	string amountStr;
	cout << MESSAGE_DEPOSIT;
	cin >> amountStr;

	if (!isPositiveAmount(amountStr)) {
		return MESSAGE_NOT_POSITIVE_AMOUNT;
	}

	if (!isValidNumeral(amountStr)) {
		return MESSAGE_NOT_VALID_AMOUNT;
	}

	amountStr = roundDouble(amountStr);

	double amount = stringToDouble(amountStr);
	double currentBalance = stringToDouble(loggedUser[2]);

	double balanceAfterDeposit = currentBalance + amount;
	string balanceAfterDepositStr = roundDouble(to_string(balanceAfterDeposit));

	loggedUser.at(2) = balanceAfterDepositStr;
	userData.at(userId) = loggedUser;

	system("cls");
	return MESSAGE_SUCCESSFUL_DEPOSIT + roundDouble(to_string(amount)) + " BGN!\n"
		+ MESSAGE_REMAINING_AMOUNT + balanceAfterDepositStr + " BGN.\n" + MESSAGE_AFTER_LOGGING;
}

string transfer() {
	string amountStr;
	string usernameToSend;
	cout << MESSAGE_ENTER_AMOUNT;
	cin >> amountStr;
	cout << MESSAGE_ENTER_USERNAME_TO_SEND;
	cin >> usernameToSend;

	if (!isPositiveAmount(amountStr)) {
		return MESSAGE_NOT_POSITIVE_AMOUNT;
	}

	if (!isValidNumeral(amountStr)) {
		return MESSAGE_NOT_VALID_AMOUNT;
	}

	if (!userAlreadyExists(usernameToSend)) {
		return MESSAGE_NOT_VALID_USERNAME;
	}
	double amount = stringToDouble(amountStr);

	bool isExceededOverdraft = false;
	string balanceAfterSendingMoneyStr = takeMoneyFromLoggedUser(amountStr, isExceededOverdraft);
	if (isExceededOverdraft) {
		return balanceAfterSendingMoneyStr;
	}

	string balanceOfUserToSend;
	int idOfUsernameToSend = returnUserIdByUsername(usernameToSend, balanceOfUserToSend);

	double balanceAfterReceivingMoney = stringToDouble(balanceOfUserToSend) + stringToDouble(amountStr);
	string balanceAfterReceivingMoneyStr = roundDouble(to_string(balanceAfterReceivingMoney));
	
	userData.at(idOfUsernameToSend)[2] = balanceAfterReceivingMoneyStr;

	system("cls");
	return MESSAGE_TRANSFER + roundDouble(to_string(amount)) + " BGN to " + usernameToSend + "!\n" +
		MESSAGE_REMAINING_AMOUNT + balanceAfterSendingMoneyStr + " BGN.\n" + MESSAGE_AFTER_LOGGING;
}

string withdraw() {
	string amountStr;
	cout << MESSAGE_ENTER_AMOUNT;
	cin >> amountStr;

	if (!isPositiveAmount(amountStr)) {
		return MESSAGE_NOT_POSITIVE_AMOUNT;
	}

	if (!isValidNumeral(amountStr)) {
		return MESSAGE_NOT_VALID_AMOUNT;
	}

	double amount = stringToDouble(amountStr);
	bool isExceededOverdraft = false;
	string balanceAfterWithdrawingStr = takeMoneyFromLoggedUser(amountStr, isExceededOverdraft);
	if (isExceededOverdraft) {
		return balanceAfterWithdrawingStr;
	}

	system("cls");
	return MESSAGE_WITHDRAW + roundDouble(to_string(amount)) + " BGN!\n" + MESSAGE_REMAINING_AMOUNT
		+ balanceAfterWithdrawingStr + " BGN.\n" + MESSAGE_AFTER_LOGGING;
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

void getInformationFromFile(bool& fileIsOpened) {
	fstream file;
	file.open(FILE_NAME, fstream::in);

	if (!file.is_open()) {
		cout << MESSAGE_NOT_OPENED_FILE << endl;
		fileIsOpened = false;
		return;
	}

	string buffer;

	while (getline(file, buffer)) {
		userData.push_back(splitLineFromFile(buffer));
	}
	file.close();
}

void updateFile() {
	fstream file;

	file.open(FILE_NAME, fstream::out);
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

string roundDouble(string number) {
	int index = 0;
	while (number[index] != '\0') {
		if (number[index] == '.') {
			break;
		}
		index++;
	}
	number = number.substr(0, index + 3);
	return number;
}

bool isValidNumeral(string amount) {
	int index = 0;
	while (amount[index] != '\0') {
		if (!(amount[index] >= '0' && amount[index] <= '9') && !(amount[index] == '.')) {
			return false;
			break;
		}
		index++;
	}
	return true;
}

bool isPositiveAmount(string amount) {
	return amount[0] != '-';
}

bool exceededOverdraft(double amount, double balance) {
	if (balance - amount < MAX_OVERDRAFT) {
		return true;
	}
	return false;
}
size_t numberSize(int number) {

	int numberSize = 0;

	if (number == 0) {
		numberSize = 1;
	}
	while (number > 0) {
		number /= 10;
		numberSize++;
	}
	return numberSize;
}

double stringToDouble(string str) {
	double result = 0;
	bool isNegative = false;

	int mantissaInt = 0;
	double mantissaDouble = 0;

	bool hasDot = false;
	size_t strLength = str.size();

	int index = 0;
	int indexOfDot = 0;
	while (str[index] != '\0') {
		if (str[index] == '.') {
			indexOfDot = index;
			hasDot = true;
		}
		index++;
	}

	if (!hasDot) {
		indexOfDot = str.size();
	}

	index = 0 + isNegative;
	int counter = indexOfDot - 1 - isNegative;
	while (index != indexOfDot) {
		result += (str[index] - '0') * pow(10, counter);
		counter--;
		index++;
	}
	if (!hasDot) {
		return result;
	}

	index = indexOfDot + 1;
	counter = strLength - indexOfDot - 2;
	while (str[index] != '\0') {
		mantissaInt += (str[index] - '0') * pow(10, counter);
		counter--;
		index++;
	}

	int mantissaCopy = mantissaInt;
	size_t mantissaSize = numberSize(mantissaCopy);
	mantissaDouble = mantissaInt;
	mantissaDouble /= pow(10, mantissaSize);

	result += mantissaDouble;

	cout << setprecision(2);
	return result;
}

string takeMoneyFromLoggedUser(string amountStr, bool& isExceededOverdraft) {
	double amount = stringToDouble(amountStr);
	double balance = stringToDouble(loggedUser[2]);

	if (exceededOverdraft(amount, balance)) {
		isExceededOverdraft = true;
		string exceededAmount = to_string(abs(balance - amount + MAX_OVERDRAFT));
		return MESSAGE_EXCEEDED_OVERDRAFT + roundDouble(exceededAmount) + " BGN.\n"
			+ MESSAGE_AFTER_LOGGING;
	}

	double balanceAfterSendingMoney = balance - amount;
	string balanceAfterSendingMoneyStr = roundDouble(to_string(balanceAfterSendingMoney));

	loggedUser.at(2) = balanceAfterSendingMoneyStr;
	userData.at(userId) = loggedUser;

	return balanceAfterSendingMoneyStr;
}
