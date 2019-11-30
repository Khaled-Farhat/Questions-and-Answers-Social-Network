// AskFm class definition.
#ifndef ASKFM_H
#define ASKFM_H

#include "Database.h"
#include <string>

class AskFm {
public:
	AskFm(Database&); // constructor takes a reference to Database
	void run();	// start Ask Fm

private:
	Database& database; // reference to database
	void displayMainMenu() const; // display main menu
	void signup(); // perform signup
	void login(); // perform login

	std::string inputCountrySelection(); // helper to input country selection
	int inputInteger(int, int); // utility function to input integer in range [low - high]
	std::string inputString(size_t); // utility function to input string
	std::string inputPassword(); // utility function to input password
};

#endif