// Answer class definition.
#ifndef ANSWER_H
#define ANSWER_H

#include "IAnswer.h"
#include "Question.h"
#include <string>
#include <vector>
#include <memory>

class Thread; // forward declaration of class Thread
class IAccount; // forward delcaration of class IAccount

class Answer : public IAnswer {
public:
	// constructor takes a pointer to question, a pointer to thread, a pointer to the answerer, the answer and answer id
	Answer(std::shared_ptr<Question>, std::shared_ptr<Thread>, std::shared_ptr<IAccount>, const std::string&, int);
	void switchLikeAnswer(const std::string&); // takes account username
	bool isUserLikeAnswer(const std::string&); // takes username; returns true if the user like answer; false otherwise
	bool isUserRewardAnswer(const std::string&); // takes username; returns true if the user like reward; false otherwise
	void userRewardMyAnswer(const std::string&); // takes username; add a reward from that user to answer
	std::vector<std::string> getLikesUsers() const; // returns usernames of users who like the answer
	std::vector<std::string> getRewardsUsers() const; // returns usernames of users who rewarded the answer

private:
	std::vector<std::string> likesUsers;
	std::vector<std::string> rewardsUsers;

	void removeLike(const std::string&); // takes username; removes like from that username
};

#endif