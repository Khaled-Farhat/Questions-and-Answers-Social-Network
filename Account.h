// Account class definition.
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "IAccount.h"
#include "IAnswer.h"
#include "Thread.h"
#include "Question.h"
#include "Notification.h"
#include "Answer.h"
#include <string>
#include <vector>
#include <memory>
#include <stack>

class Country; // forward declaration of class Country

class Account : public IAccount {
public:
	// constructor takes usrname, password, first name, last name, a pointer to country and interests list
	Account(const std::string&, const std::string&, const std::string&,
		const std::string&, std::shared_ptr<Country>, const std::vector<std::string>&);
	bool validatePIN(const std::string&); // takes password; returns true if the password is correct; false otherwise
	std::vector<std::string> getFriendsList() const; // returns friends names list
	void ask(std::shared_ptr<Question>); // ask user
	void askInThread(std::shared_ptr<Question>, int); // takes pointer to question and thread id
	std::vector<std::shared_ptr<IAnswer>> getFeed() const; // return last 10 answers
	void switchLikeAnswer(const std::string&, int); // takes account username and answer id
	bool isUserLikeMyAnswer(const std::string&, int); // takes username and answer id; returns true if the user like that answer; false otherwise
	void userRewardMyAnswer(const std::string&, int); // takes username and answer id; add a reward from that user to answer
	bool isSufficientCoins() const; // returns true if there are sufficient coins
	std::vector<std::string> getAnswerLikesUsers(int) const; // takes answer id; returns usernames of users who like the answer
	std::vector<std::string> getAnswerRewardsUsers(int) const; // takes answer id; returns usernames of users who rewarded the answer
	void setFirstName(const std::string&); // set first name
	void setLastName(const std::string&); // set last name
	void setCountry(const std::shared_ptr<Country>); // set country
	void blockUser(std::shared_ptr<Account>); // block username
	std::vector<std::shared_ptr<IAnswer>> getProfileLatestAnswers() const; // returns user latest answers
	std::vector<std::shared_ptr<IAnswer>> getProfileTopAnswers() const; // returns user Top answers
	std::vector<std::shared_ptr<Notification>> getLikesNotification() const; // returns user likes notifications
	std::vector<std::shared_ptr<Notification>> getAnswersNotification() const; // returns user answers notifications
	std::vector<std::shared_ptr<IAnswer>> getThreadAnswers(int) const; // takes thread id; returns vector of that thread answers
	int getThreadTotalLikes(int) const; // takes thread id; returns thread total likes
	int getThreadTotalRewards(int) const; // takes thread id; returns thread total rewards
	std::vector<std::shared_ptr<IQuestion>> getUserQuestions() const; // returns vecotr of IQuestion objects
	void answerQuestion(int, const std::string&); // takes question id and questoin answer; implements: user answers question
	void removeQuestion(int); // takes question id; remove question from questions inbox list
	void addAnswerNotification(std::shared_ptr<Notification>); // if user answered my question; add notificatin
	void followUser(std::shared_ptr<Account>); // takes a pointer to account and adds that accounts to the friends list

private:
	std::vector<std::shared_ptr<Account>> friends;
	std::vector<std::shared_ptr<Account>> blockedAccounts;
	std::vector<std::shared_ptr<Question>> questionsInbox;
	std::vector<std::shared_ptr<Answer>> answers;
	std::vector<std::shared_ptr<Notification>> likesNotifications;
	std::vector<std::shared_ptr<Notification>> answersNotifications;
	std::vector<std::shared_ptr<Thread>> threads;
	const std::string& password;
	int coins;
	int threadCounter;
	int answersCounter;

	std::shared_ptr<Answer> getAnswer(int) const; // utility function; takes answer id; returns pointer to Answer object;
												  // returns nullptr if answer not found
	std::shared_ptr<Thread> getThread(int) const; // utility function; takes thread id; returns pointer to Thread object;
												  // returns nullptr if thread not found
	std::shared_ptr<Question> getQuestion(int) const; // utility function; takes question id; returns pointer to Thread object;
													  // returns nullptr if question not found
	std::shared_ptr<Thread> newThead(); // creates a new thread
	bool isBlocked(const std::string&); // takes username; returns true if it is blocked
	void addLikeNotification(std::shared_ptr<Notification>); // add like notification
};

#endif
