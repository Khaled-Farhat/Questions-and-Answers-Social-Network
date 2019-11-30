// Member-function definition for class Thread.
#include "Thread.h"
#include <vector>
#include <memory>

using namespace std;

// constructor takes thread id
Thread::Thread(int threadID)
	: id(threadID) {}

// get thread total likes
int Thread::getTotalLikes() const {
	int likes{ 0 };

	for (size_t i{ 0 }; i < answers.size(); ++i) {
		likes += answers[i]->getLikes();
	}

	return likes;
}

// get thread total rewards
int Thread::getTotalRewards() const {
	int rewards{ 0 };

	for (size_t i{ 0 }; i < answers.size(); ++i) {
		rewards += answers[i]->getRewards();
	}

	return rewards;
}

// returns vector of that thread answers
vector<shared_ptr<IAnswer>> Thread::getThreadAnswers() const {
	vector<shared_ptr<IAnswer>> threadAnswers;

	for (size_t i{ 0 }; i < answers.size(); ++i) {
		threadAnswers.push_back(answers[i]);
	}

	return threadAnswers;
}

// return thread id
int Thread::getThreadID() const { return id; }

// add answer to the thread
void Thread::addAnswer(shared_ptr<Answer> answer) {
	answers.push_back(answer);
}
