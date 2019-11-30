// Question class definition.
#ifndef QUESTION_H
#define QUESTION_H

#include"IQuestion.h"
#include <string>
#include <memory>

class Account; // forward declaration of class Account
class Thread; // forward declaration of class Thread
class IAnswer; // forward declaration of class IAnswer

class Question : public IQuestion {
public:
	// consrtuctor takes a pointer to askedBy user, question string, boolean variable (open = 0, anonymous = 1);
	// quetion id and pointer to thread
	Question(std::shared_ptr<Account>, const std::string&, bool, int, std::shared_ptr<Thread>);
	void setThread(std::shared_ptr<Thread>); // set thread
	bool noThread() const; // returns true if the question has no thread
	int getThreadID() const; // returns thred id; -1 no thread
	void sendNotification(std::shared_ptr<IAnswer>); // new answer to the question; send notification to the asker

private:
	std::shared_ptr<Thread> threadPtr;
};

#endif