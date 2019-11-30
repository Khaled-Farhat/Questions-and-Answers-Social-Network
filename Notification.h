// Notification class definition.
#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "IAnswer.h"
#include <iostream>
#include <string>
#include <memory>

class IAnswer; // forward delaration of class IAnswer
class IAccount; // forward declatation of class IAccount

class Notification {
	friend std::ostream& operator<<(std::ostream&, const Notification&);

public:
	// constructor takes username, pointer to IAnswer object and notification message
	Notification(const std::string& username, std::shared_ptr<IAnswer>, const std::string&);
	const std::string& getUser() const; // returns username of notifier
	std::shared_ptr<IAnswer> getAnswer() const; // returns a pointer to IAnswer object
	const std::string& getMessage() const; // returns notification message

private:
	std::string username;
	std::shared_ptr<IAnswer> iAnswerPtr;
	std::string message;
};

#endif