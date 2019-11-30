// Member-function definition for class IAnswer
#include "IAnswer.h"
#include "Question.h"
#include "Thread.h"
#include "IAccount.h"
#include "Notification.h"
#include <string>
#include <iostream>
#include <memory>

using namespace std;

// returns a pointer to IThread object
IAnswer::IAnswer(shared_ptr<Question> question, shared_ptr<Thread> threadPtr,
	shared_ptr<IAccount> answerer, const string& answerString, int id)
	: questionPtr(question), thread(threadPtr), answererPtr(answerer), 
	answer(answerString), answerId(id) {}

// return answer id
int IAnswer::getAnswerID() const { return answerId; }

// return thread id
int IAnswer::getThreadID() const { return thread->getThreadID(); }

// return answeredBy
const string& IAnswer::getAnsweredBy() const { return answererPtr->getUsername(); }

// return question
const string& IAnswer::getQuestion() const { return questionPtr->getQuestion(); }

// returns number answer likes
int IAnswer::getLikes() const { return likes; }

// returns number of answer rewards
int IAnswer::getRewards() const { return rewards; }

bool IAnswer::operator==(const IAnswer& right) {
	if (answerId == right.getAnswerID()) {
		return true;
	}
	else {
		return false;
	}
}

// decrease likes by 1
void IAnswer::decreaseLikes() { --likes; }

// increase likse by 1
void IAnswer::increaseLikes() { ++likes; }

// increase rewards by 1
void IAnswer::increaseRewards() { ++rewards; }

// returns answer string
const string& IAnswer::getAnswerString() const { return answer; }

// returns a pointer to IQuestion object
shared_ptr<IQuestion> IAnswer::getQuestionPtr() const { return questionPtr; }


ostream& operator<<(ostream& output, const IAnswer& iAnswer) {
	shared_ptr<IQuestion> iQuestionPtr(iAnswer.getQuestionPtr());

	cout << "\n\tAsked by: " << iQuestionPtr->getAskedBy() << "\tAnswered by: " << iAnswer.getAnsweredBy()
		<< "\n\tQuestion: " << iQuestionPtr->getQuestion()
		<< "\n\tAnswer: " << iAnswer.getAnswerString()
		<< "\n\tLikes: " << iAnswer.getLikes() << "\tRewards: " << iAnswer.getRewards()
		<< "\n------------------------------";

	return output;
}

