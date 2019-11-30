// Member-function definition for class Answer
#include "Answer.h"
#include "IAccount.h"
#include "Thread.h"
#include <memory>
#include <string>
#include <vector>

using namespace std;

// constructor takes a pointer to question, a pointer to thread and a pointer to the user who snwered the qusetion
// constructor takes a pointer to question, a pointer to thread, a pointer to the answerer, the answer and answer id
Answer::Answer(shared_ptr<Question> questionPtr, shared_ptr<Thread> threadPtr, shared_ptr<IAccount> answererPtr, const string& answer, int answerID)
	: IAnswer(questionPtr, threadPtr, answererPtr, answer, answerID) {}

// takes account username
void Answer::switchLikeAnswer(const string& username) {
	if (isUserLikeAnswer(username)) {
		removeLike(username);
		decreaseLikes();
	}
	else {
		likesUsers.push_back(username);
		increaseLikes();
	}
}

// takes username; returns true if the user like answer; false otherwise
bool Answer::isUserLikeAnswer(const string& username) {
	for (size_t i{ 0 }; i < likesUsers.size(); ++i) {
		if (likesUsers[i] == username) {
			return true;
		}
	}

	return false;
}

// takes username; returns true if the user reward answer; false otherwise
bool Answer::isUserRewardAnswer(const string& username) {
	for (size_t i{ 0 }; i < rewardsUsers.size(); ++i) {
		if (rewardsUsers[i] == username) {
			return true;
		}
	}

	return false;
}

// takes username; add a reward from that user to answer
void Answer::userRewardMyAnswer(const string& username) {
	if (isUserRewardAnswer(username) == false) {
		rewardsUsers.push_back(username);
	}

	increaseRewards();
}

// returns usernames of users who like the answer
vector<string> Answer::getLikesUsers() const {
	return likesUsers;
}

// returns usernames of users who rewarded the answer
vector<string> Answer::getRewardsUsers() const {
	return rewardsUsers;
}

// takes username; removes like from that username
void Answer::removeLike(const string& username) {
	for (size_t i{ 0 }; i < likesUsers.size(); ++i) {
		if (likesUsers[i] == username) {
			likesUsers.erase(likesUsers.begin() + i);
			break;
		}
	}
}
