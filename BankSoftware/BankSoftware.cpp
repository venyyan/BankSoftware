#include <iostream>
#include <string>
using namespace std;

const char COMMAND_LOGIN = 'L';
const char COMMAND_REGISTER = 'R';
const char COMMAND_QUIT = 'Q';


const string OPENING_MESSAGE = "Hello! You are now using BankSoftware.\nIf you want to Log In type \"L\"\n"
"If you want to Register type \"R\" \nIf you want to Quit type \"Q\"";

int main()
{
	cout << OPENING_MESSAGE;

	char command = ' ';

	while (command != COMMAND_QUIT) {
		cin >> command;

		
	}
}
