// Member-function definition for class Question
#include "Question.h"
#include "Thread.h"
#include "Account.h"
#include "Notification.h"
#include <string>
#include <memory>

using namespace std;

// consrtuctor takes a pointer to askedBy user, question string, boolean variable (open = 0, anonymous = 1);
// quetion id and pointer to thread
Question::Question(shared_ptr<Account> askerPtr, const string& question, bool isAnonymous,
	int questionID, shared_ptr<Thread> thread)
	: threadPtr(thread), IQuestion(askerPtr, question, isAnonymous, questionID) {}


// set thread
void Question::setThread(shared_ptr<Thread> thread) { threadPtr = thread; }


// returns true if the question has no thread
bool Question::noThread() const {
	if (threadPtr == nullptr) {
		return true;
	}
	else {
		return false;
	}
}


// returns thred id; -1 no thread
int Question::getThreadID() const { return threadPtr->getThreadID(); }

// new answer to the question; send notification to the asker
void Question::sendNotification(shared_ptr<IAnswer> answerPtr) {
	shared_ptr<Notification> answerNotificationPtr(new Notification(answerPtr->getAnsweredBy(), answerPtr, "answered your questionr"));
	shared_ptr<Account> askerPtr(getAskerPtr());

	askerPtr->addAnswerNotification(answerNotificationPtr);
}
