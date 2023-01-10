#include <string>
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
const string MESSAGE_ENTER_USERNAME = "Please enter your username: ";
const string MESSAGE_ENTER_PASSWORD = "Please enter your password: ";
const string MESSAGE_USERNAME_REGISTER = "Please enter username. It should contain only latin letters or symbols: ";
const string MESSAGE_PASSWORD_REGISTER = "Please enter password. It should contain the symbols !@#$%^&* "
"and must have at least 1 lowercase, 1 uppercase, and be at least 5 characters: ";
const string MESSAGE_ENTER_AMOUNT = "Please enter amount: ";
const string MESSAGE_ENTER_USERNAME_TO_SEND = "Please enter username to send the amount to: ";
const string MESSAGE_NOT_VALID_USERNAME = "Not a valid username!";
const string MESSAGE_NOT_VALID_PASSWORD = "Not a valid password!";
const string MESSAGE_USER_ALREADY_EXISTS = "User already exists!";
const string MESSAGE_PASSWORD_DOES_NOT_MATCH_USERNAME = "Password does not match username. Try again.";
const string MESSAGE_USER_DOES_NOT_EXIST = "User does not exist! Try again or register.";
const string MESSAGE_AFTER_LOGGING_FIRST = "Registered successfully!\nYou have ";
const string MESSAGE_AFTER_LOGGING_SECOND = " BGN. Choose one of "
"the following options: \nC - cancel account\nD - deposit\nL - logout\nT - transfer\nW - withdraw";
const string MESSAGE_DEPOSIT = "Please enter deposit amount: ";
const string MESSAGE_NOT_POSITIVE_AMOUNT = "You entered a non-positive amount!";
const string MESSAGE_SUCCESSFUL_DEPOSIT = "You successfully deposited ";
const string MESSAGE_EXCEEDED_OVERDRAFT = "The maximal overdraft is 10000! You exceed it by ";
const string MESSAGE_TRANSFER = "You successfully transfered ";
const string MESSAGE_WITHDRAW = "You successfully withdrew ";
const string MESSAGE_NOT_VALID_COMMAND = "Not a valid command!";
const string MESSAGE_BALANCE_IS_NOT_ZERO = "You still have money in your account!";
const string ZERO_BALANCE = "0";

const string REGEX_USERNAME = "^[A-Za-z\!-/\:-@\[-_]+$";
const string REGEX_PASSWORD = "[A-Za-z\\\\0-9\!\@\#\$\%\^\&\*]+$";
const string REGEX_PASSWORD_CONTAINS_AT_LEAST_ONE_UPPERCASE_LOWERCASE_SYMBOL =
"(?=.*[a-z])(?=.*[A-Z])(?=.*\\W)^[^ ]+$";

const unsigned MINIMUM_PASSWORD_LENGTH = 5;

const int MAX_OVERDRAFT = -10000;