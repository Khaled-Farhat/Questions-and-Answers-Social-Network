// FlowController class definition.
#ifndef FLOWCONTROLLER_H
#define FLOWCONTROLLER_H

#include "Database.h"
#include "Notification.h"
#include <string>
#include <vector>
#include <stack>

class FlowController {
public:
	FlowController(const std::string&, Database&); // constructor takes username and reference to database
	void run(); // start Flow Controller

private:
	std::string currentUsername;
	Database& database; // reference to database
	
	void askFriends(const std::string&); // takes a question; manages ask friends interface
	void myFeed(); // show user feed
	void viewAnswer(std::shared_ptr<IAnswer>); // view answer; takes a pointer to answer info object
	void listAccounts(const std::vector<std::string>&); // list people who have reacted on the answer
	void viewProfile(const std::string&); // takes profile username; manages view profile menu
	void viewThread(const std::string&, int); // takes username and thread id; manages view thread menu
	void editProfile(); // edit profile
	void viewLeaderboard(); // view leaderboard
	void showAnswersHelper(const std::vector<std::shared_ptr<IAnswer>>&); // takes answers and output them
	void searchPeople(); // manages search people menu
	void listFriends(); // list use friends
	void viewNotifications(); // view notifications
	void viewQuestionsInbox(); // view question inbox
	void listNotifications(const std::vector<std::shared_ptr<Notification>>&); // list notifications
	void viewNotification(std::shared_ptr<Notification>);

	std::string inputCountrySelection(); // helper to input country selection
	void displayMenu(const std::vector<std::string>&) const; // utility function to display meny
	int inputInteger(int, int); // utility function to input integer in range [low - high]
	std::string inputString(size_t); // utility function to input string
};

#endif