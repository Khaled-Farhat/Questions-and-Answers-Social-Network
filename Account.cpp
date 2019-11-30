// Member-function definition for class Account
#include "Account.h"
#include "Country.h"
#include "Thread.h"
#include "Question.h"
#include "Answer.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// constructor takes usrname, password, first name, last name, a pointer to country and interests list
Account::Account(const string& username, const string& userPassword, const string& firstName,
	const string& lastName, shared_ptr<Country> countryPtr, const vector<string>& interestsList)
	: IAccount(username, firstName, lastName, countryPtr, interestsList),
	  password(userPassword), coins(0), threadCounter(0), answersCounter(0) {}

// takes password; returns true if the password is correct; false otherwise
bool Account::validatePIN(const string& passwordToValidate) {
	if (passwordToValidate == password) {
		return true;
	}
	else {
		return false;
	}
}

// returns friends names list
vector<string> Account::getFriendsList() const {
	vector<string> friendList;

	for (size_t i{ 0 }; i < friends.size(); ++i) {
		friendList.push_back(friends[i]->getUsername());
	}

	return friendList;
}

// ask user
void Account::ask(shared_ptr<Question> questionToAskPtr) {
	if (isBlocked(questionToAskPtr->getAskedBy())) { // user is blocked; do not add the answer
		return;
	}
	else {
		questionsInbox.push_back(questionToAskPtr);
	}
}

// takes username of the asker and thread id
void Account::askInThread(shared_ptr<Question> questionToAskPtr, int threadID) {
	shared_ptr<Thread> threadPtr = getThread(threadID);
	questionToAskPtr->setThread(threadPtr);
	ask(questionToAskPtr);
}

// return last 10 answers
vector<shared_ptr<IAnswer>> Account::getFeed() const {
	vector<shared_ptr<IAnswer>> feed;

	int index{ static_cast<int>(answers.size()) - 1 };

	for (int i{ 0 }; i < 10; ++i) {
		if (index < 0) {
			break;
		}
		else {
			feed.push_back(answers[index]);
			--index;
		}
	}

	return feed;
}

// takes account username and answer id
void Account::switchLikeAnswer(const string& username, int answerID) {
	shared_ptr<Answer> answerPtr = getAnswer(answerID);
	answerPtr->switchLikeAnswer(username);

	if (isUserLikeMyAnswer(username, answerID)) {
		shared_ptr<Notification> likeNotificationPtr(new Notification(username, answerPtr, "likes your answer"));
		addLikeNotification(likeNotificationPtr);
		addLike();
	}
	else {
		cancelLike();
	}
}

// takes username and answer id; returns true if the user like that answer; false otherwise
bool Account::isUserLikeMyAnswer(const string& username, int answerID) {
	shared_ptr<Answer> answerPtr = getAnswer(answerID);
	return answerPtr->isUserLikeAnswer(username);
}

// takes username and answer id; add a reward from that user to answer
void Account::userRewardMyAnswer(const string& username, int answerID) {
	shared_ptr<Answer> answerPtr = getAnswer(answerID);
	answerPtr->userRewardMyAnswer(username);
	addReward();
}

// returns true if there are sufficient coins
bool Account::isSufficientCoins() const {
	if (coins > 0) {
		return true;
	}
	else {
		return false;
	}
}

// takes answer id; returns usernames of users who like the answer
vector<string> Account::getAnswerLikesUsers(int answerID) const {
	shared_ptr<Answer> answerPtr = getAnswer(answerID);
	return answerPtr->getLikesUsers();
}

// takes answer id; returns usernames of users who rewarded the answer
vector<string> Account::getAnswerRewardsUsers(int answerID) const {
	shared_ptr<Answer> answerPtr = getAnswer(answerID);
	return answerPtr->getRewardsUsers();
}

// set first name
void Account::setFirstName(const string& newFirstName) { IAccount::setFirstName(newFirstName); }

// set last name
void Account::setLastName(const string& newLastName) { IAccount::setLastName(newLastName); }

// set country
void Account::setCountry(std::shared_ptr<Country> countryPtr) { IAccount::setCountry(countryPtr); }

// block username
void Account::blockUser(shared_ptr<Account> user) {
	blockedAccounts.push_back(user);
}

// returns user latest answers
vector<shared_ptr<IAnswer>> Account::getProfileLatestAnswers() const {
	vector<shared_ptr<IAnswer>> latestAnswers;

	for (size_t i{ 0 }; i < answers.size(); ++i) {
		latestAnswers.push_back(answers[i]);
	}

	return latestAnswers;
}

// returns user Top answers
vector<shared_ptr<IAnswer>> Account::getProfileTopAnswers() const {
	vector<shared_ptr<IAnswer>> topAnswers = getProfileLatestAnswers();

	sort(topAnswers.begin(), topAnswers.end(),
		[](auto i, auto j) {
			return (i->getLikes() > j->getLikes());
		}
	);

	return topAnswers;
}

// returns user likes notifications
vector<shared_ptr<Notification>> Account::getLikesNotification() const {
	return likesNotifications;
}

// returns user answers notifications
vector<shared_ptr<Notification>> Account::getAnswersNotification() const {
	return answersNotifications;
}

// takes thread id; returns thread total likes
int Account::getThreadTotalLikes(int threadID) const {
	shared_ptr<Thread> threadPtr = getThread(threadID);
	return threadPtr->getTotalLikes();
}

// takes thread id; returns thread total rewards
int Account::getThreadTotalRewards(int threadID) const {
	shared_ptr<Thread> threadPtr = getThread(threadID);
	return threadPtr->getTotalRewards();
}

// returns vecotr of IQuestion objects
vector<shared_ptr<IQuestion>> Account::getUserQuestions() const {
	vector<shared_ptr<IQuestion>> questionsList;

	for (size_t i{ 0 }; i < questionsInbox.size(); ++i) {
		questionsList.push_back(questionsInbox[i]);
	}

	return questionsList;
}

// takes thread id; returns vector of that thread answers
vector<shared_ptr<IAnswer>> Account::getThreadAnswers(int threadID) const {
	shared_ptr<Thread> threadPtr = getThread(threadID);
	return threadPtr->getThreadAnswers();
}

// takes  question id; implements: user answers a question
void Account::answerQuestion(int questionID, const string& questionAnswer) {
	shared_ptr<Question> questionPtr = getQuestion(questionID);
	shared_ptr<Thread> threadPtr = nullptr;

	if (questionPtr->noThread() == true) {
		threadPtr = newThead();
	}
	else {
		threadPtr = getThread(questionPtr->getThreadID());
	}

	shared_ptr<IAccount> answererPtr(this);

	++answersCounter;
	shared_ptr<Answer> answerPtr(new Answer(questionPtr, threadPtr, answererPtr, questionAnswer, answersCounter));

	threadPtr->addAnswer(answerPtr);
	questionPtr->sendNotification(answerPtr);

	answers.push_back(answerPtr);
	removeQuestion(questionID);
}

// takes question id; remove question from questions inbox list
void Account::removeQuestion(int questionID) {
	for (size_t i{ 0 }; i < questionsInbox.size(); ++i) {
		if (questionsInbox[i]->getQuestionID() == questionID) {
			questionsInbox.erase(questionsInbox.begin() + i);
			break;
		}
	}
}

// utility function; takes answer id; returns pointer to Answer object;
// returns nullptr if answer not found
shared_ptr<Answer> Account::getAnswer(int answerID) const {
	for (size_t i{ 0 }; i < answers.size(); ++i) {
		if (answers[i]->getAnswerID() == answerID) {
			return answers[i];
		}
	}

	// answer not found
	return nullptr;
}

// utility function; takes thread id; returns pointer to Thread object;
// returns nullptr if thread not found
shared_ptr<Thread> Account::getThread(int threadID) const {
	for (size_t i{ 0 }; i < threads.size(); ++i) {
		if (threads[i]->getThreadID() == threadID) {
			return threads[i];
		}
	}

	// thread not found
	return nullptr;
}

// utility function; takes question id; returns pointer to Thread object;
// returns nullptr if question not found
shared_ptr<Question> Account::getQuestion(int questionID) const {
	for (size_t i{ 0 }; i < questionsInbox.size(); ++i) {
		if (questionsInbox[i]->getQuestionID() == questionID) {
			return questionsInbox[i];
		}
	}

	// question not found
	return nullptr;
}

// creates a new thread
shared_ptr<Thread> Account::newThead() {
	++threadCounter;
	shared_ptr<Thread> newThreadPtr(new Thread(threadCounter));
	threads.push_back(newThreadPtr);
	return newThreadPtr;
}

// takes username; returns true if it is blocked
bool Account::isBlocked(const string& user) {
	for (size_t i{ 0 }; i < blockedAccounts.size(); ++i) {
		if (blockedAccounts[i]->getUsername() == user) {
			return true;
		}
	}

	return false;
}

// add like notification
void Account::addLikeNotification(shared_ptr<Notification> notification) {
	likesNotifications.push_back(notification);
}

// if user answered my question; add notificatin
void Account::addAnswerNotification(shared_ptr<Notification> notification) {
	answersNotifications.push_back(notification);
}
