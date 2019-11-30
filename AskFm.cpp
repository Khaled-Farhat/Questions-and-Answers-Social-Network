// Member-function definition for class AskFm.
#include "AskFm.h"
#include "Database.h"
#include "FlowController.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// enumeration constants represent system main menu options
enum class SystemMainMenuOption { SIGNUP = 1, LOGIN, EXIT };

// constructor takes a reference to database
AskFm::AskFm(Database& theDatabase)
	: database(theDatabase) {}

// start Ask Fm
void AskFm::run() {
	cout << "Console Implementation of Ask.fm - Designed and implemented by Khaled Farhat -"
		"github.com/Khaled-Farhat/\n";

	bool userExited = false;
	
	while (userExited == false) {
		displayMainMenu();
		int choice = inputInteger(1, 3);
		SystemMainMenuOption option = static_cast<SystemMainMenuOption>(choice);

		switch (option) {
			case SystemMainMenuOption::SIGNUP:
				signup();
				break;

			case SystemMainMenuOption::LOGIN:
				login();
				break;

			case SystemMainMenuOption::EXIT:
				userExited = true;
				break;
		}
	}
}

// display main menu
void AskFm::displayMainMenu() const {
	cout << "\n1 - Signup"
		<< "\n2 - Login"
		<< "\n3 - Exit"
		<< "\nMake a choice: ";
}

// perform sign up
void AskFm::signup() {
	string username;
	string password;
	string firstName;
	string lastName;
	string country;
	vector<string> interests;

	while (true) {
		// loop until user enters a unique username
		cout << "Enter username: ";
		username = inputString(12);

		if (database.isValidUsername(username)) {
			break;
		}
		else {
			cout << "Username " << username << " is not available. Enter another username: ";
		}
	}

	cout << "Enter password: ";
	password = inputString(12);

	cout << "Enter first name: ";
	firstName = inputString(12);

	cout << "Enter last name: ";
	lastName = inputString(12);

	country = inputCountrySelection();

	cout << "\nEnter three interests: ";

	for (int i{ 0 }; i < 3; ++i) {
		cout << "Enter interest: ";
		string interest = inputString(12);
		interests.push_back(interest);
	}
	
	database.createAccount(username, password, firstName, lastName, country, interests);
}

// perform log in
void AskFm::login() {
	string username;
	string password;

	cout << "Enter username: ";
	username = inputString(12);
	
	cout << "Enter password: ";
	password = inputPassword();

	if (database.authenticateUser(username, password)) {
		cout << "\n\nWelcome " << username;
		unique_ptr<FlowController> flowController = make_unique<FlowController>(username, database);
		flowController->run();
	}
	else {
		cout << "\nInvalid username or password.";
	}
}

// helper to input country selection
string AskFm::inputCountrySelection() {
	vector<string> countryList = database.getCountryList();
	size_t index = 0;
	bool userSelected = false; // whether user has made a choice
	string countrySelection;

	while (index < countryList.size()) {
		// output 10 more countries
		for (int i{ 0 }; i < 10; ++i) {
			if (index == countryList.size()) { // no mpre countries to show
				break;
			}
			
			cout << "\n" << index + 1 << " - " << countryList[index];
			++index;
		}

		// no more countries to show
		if (index == countryList.size()) {
			break;
		}

		cout << "\n1 - Show more"
			<< "\n2 - Select a country"
			<< "\nMake a choice: ";
		int choice = inputInteger(1, 2);

		switch (choice) {
			case 1:
				break;
			
			case 2:
				int position = inputInteger(1, index);
				userSelected = true;
				countrySelection = countryList[position - 1];
				break;
		}

		if (userSelected) {
			break;
		}
	}

	if (userSelected) {
		return countrySelection;
	}
	else {
		cout << "\nNo more countries to show. Select a country: ";
		int position = inputInteger(1, index);
		userSelected = true;
		countrySelection = countryList[position - 1];
		return countrySelection;
	}
}

// utility function to input integer in range [low - high]
int AskFm::inputInteger(int low, int high) {
	int integer = 0;

	while (1) {
		string input;
		getline(cin, input);

		if (input.size() == 0) {
			continue;
		}

		bool validInput = 1;

		for (size_t i{ 0 }; i < input.size(); ++i) {
			if (isdigit(input[i]) == false) {
				validInput = 0;
				break;
			}
		}

		if (input.size() > 9) {
			validInput = 0;
		}

		if (validInput) {
			integer = stoi(input);

			if (integer < low || integer > high) {
				validInput = false;
			}
		}

		if (validInput) {
			break;
		}
		else {
			cout << "Invalid input. Enter integer in range [" << low << " - " << high << "]: ";
		}
	}

	return integer;
}

// utility function to input string with size <= stringSize
string AskFm::inputString(size_t stringSize) {
	string inputString;

	while (true) {
		string input;
		getline(cin, input);

		if (input.size() == 0) {
			continue;
		}

		bool validInput = 1;

		for (size_t i{ 0 }; i < input.size(); ++i) {
			if (input[i] == ' ' || ispunct(input[i])) {
				validInput = 0;
			}
		}

		if (input.size() > stringSize) {
			validInput = 0;
		}

		if (validInput) {
			inputString = input;
			break;
		}
		else {
			cout << "Invalid input. Enter a string without spaces, without special characters and with size less than "
				<< stringSize << ": ";
		}
	}

	return inputString;
}

// utility function to input password
string AskFm::inputPassword() {
	string inputString;
	const size_t stringSize = 12;

	while (true) {
		string input;
		getline(cin, input);

		if (input.size() == 0) {
			continue;
		}

		bool validInput = 1;

		for (size_t i{ 0 }; i < input.size(); ++i) {
			if (input[i] == ' ') {
				validInput = 0;
			}
		}

		if (input.size() > stringSize) {
			validInput = 0;
		}

		if (validInput) {
			inputString = input;
			break;
		}
		else {
			cout << "Invalid input. Enter a string without spaces, without special characters and with size less than "
				<< stringSize << ": ";
		}
	}

	return inputString;
}
