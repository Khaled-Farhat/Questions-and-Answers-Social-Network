// Member-function definition for clsss IQuestion
#include "IQuestion.h"
#include "IAccount.h"
#include "Account.h"
#include <string>
#include <memory>

using namespace std;

// constructor takes pointer to IAccount object, question, boolean variable (is question anonymous or open) and question id
IQuestion::IQuestion(shared_ptr<Account> askerUserPtr, const string& questionString, bool isAnonymous, int questionID)
	: askerPtr(askerUserPtr), question(questionString), anosymous(isAnonymous), id(questionID) {}

// returns question id
int IQuestion::getQuestionID() const { return id; }

// returns the username of the user who asked the question
const string IQuestion::getAskedBy() const {
	if (anosymous == true) {
		return "Anonymous";
	}
	else {
		return askerPtr->getUsername();
	}
}

// return question string
const string& IQuestion::getQuestion() const { return question; }

// returns a pointer to the asker ptr
shared_ptr<Account> IQuestion::getAskerPtr() const { return askerPtr; }

ostream& operator<<(ostream& output, const IQuestion& question) {
	cout << "\n\tAsked by: " << question.getAskedBy()
		<< "\n\tQuestion: " << question.question
		<< "\n------------------------------";

	return output;
}
