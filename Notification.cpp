// Member-function definition for class Notification.
#include "Notification.h"
#include "IAnswer.h"
#include <string>
#include <memory>

using namespace std;

// constructor takes username, pointer to IAnswer object and notification message
Notification::Notification(const string& notifier, shared_ptr<IAnswer> iAnswer, const string& messageString)
	: username(notifier), iAnswerPtr(iAnswer), message(messageString) {}

// return username of notifier
const string& Notification::getUser() const { return username; }

// return pointer to IAnswer object
shared_ptr<IAnswer> Notification::getAnswer() const { return iAnswerPtr; }

// returns notification message
const string& Notification::getMessage() const {
	return message;
}

ostream& operator<<(ostream& output, const Notification& notification) {
	shared_ptr<IAnswer> iAnswerPtr = notification.getAnswer();

	cout << notification.getUser() << " " << notification.getMessage() << " " << iAnswerPtr->getAnswerString();

	return output;
}

