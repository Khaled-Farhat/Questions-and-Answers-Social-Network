// Thread class definition.
#ifndef THREAD_H
#define THREAD_H

#include "Answer.h"
#include <memory>
#include <vector>

class Thread {
public:
	Thread(int); // constructor takes thread id
	int getTotalLikes() const; // get thread total likes
	int getTotalRewards() const; // get thread total rewards
	std::vector<std::shared_ptr<IAnswer>> getThreadAnswers() const; // returns vector of that thread answers
	int getThreadID() const; // return thread id
	void addAnswer(std::shared_ptr<Answer>); // add answer to the thread

private:
	std::vector<std::shared_ptr<Answer>> answers;
	int id;
};

#endif