// Member-function definition for class Database.
#include "Database.h"
#include "IAnswer.h"
#include "IAccount.h"
#include "Account.h"
#include "Country.h"
#include "Question.h"
#include "Thread.h"
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <exception>
#include <tuple>
#include <algorithm>
#include <stack>

using namespace std;

// default constructor
Database::Database() {
	countries.push_back(shared_ptr<Country>(new Country("syria")));
}

// takes username; returns true if there is no account with that username; false otherwise
bool Database::isValidUsername(const string& username) const {
	shared_ptr<Account> accountPtr = getAccount(username);

	if (accountPtr == nullptr) { // no user with that username; so the username is valid
		return true;
	}
	else {
		return false;
	}
}

// returns vector of country names
vector<string> Database::getCountryList() const {
	vector<string> names;

	for (size_t i{ 0 }; i < countries.size(); ++i) {
		names.push_back(countries[i]->getName());
	}

	return names;
}

// create a new account; takes usrname, password, first name, last name, counrty and interests list
void Database::createAccount(const string& username, const string& password, const string& firstName,
	const string& lastName, const string& countryName, const vector<std::string>& interestsList) {
	shared_ptr<Country> countryPtr = getCountry(countryName);

	if (countryPtr == nullptr) {
		throw std::runtime_error("\nCountry was not found.");
	}

	shared_ptr<Account> accountPtr(new Account(username, password, firstName, lastName, countryPtr, interestsList));
	countryPtr->addAccount(accountPtr);

	for (size_t i{ 0 }; i < interestsList.size(); ++i) {
		interests[interestsList[i]].push_back(accountPtr);
	}

	accounts.push_back(accountPtr);
}

// takes username and password; returns true if username and password are correct
bool Database::authenticateUser(const string& username, const string& password) const {
	shared_ptr<Account> accountPtr = getAccount(username);

	if (accountPtr == nullptr) {
		throw runtime_error("\nAccount was not found.");
	}

	if (accountPtr->validatePIN(password) == true) {
		return true;
	}
	else {
		return false;
	}
}

// takes username; returns list of friends of that user
vector<string> Database::getFriendsList(const string& username) const {
	shared_ptr<Account> accountPtr = getAccount(username);

	if (accountPtr == nullptr) {
		throw runtime_error("\nAccount was not found.");
	}

	return accountPtr->getFriendsList();
}

// ask multiple friends; takes account username, friends list and takes question type (openly - anonymously)
void Database::askMultipleUsers(const string& askedBy, const vector<string>& usersList, const string& question, QuestionType questionType) {
	shared_ptr<Account> askedByPtr = getAccount(askedBy);

	bool anonymous = (questionType == QuestionType::ANONYMOUSLY) ? true : false; // aske openly or anonymously
	shared_ptr<Question> questionPtr(new Question(askedByPtr, question, anonymous, questionsCount, nullptr));
	++questionsCount;

	for (size_t i{ 0 }; i < usersList.size(); ++i) {
		shared_ptr<Account> userToAskPtr = getAccount(usersList[i]);
		userToAskPtr->ask(questionPtr);
	}
}

// ask user; takes account username and takes question type (openly - anonymously)
void Database::askUser(const string& askedBy, const string& userToAsk, const string& question, QuestionType questionType) {
	shared_ptr<Account> askedByPtr = getAccount(askedBy);
	shared_ptr<Account> userToAskPtr = getAccount(userToAsk);

	bool anonymous = (questionType == QuestionType::ANONYMOUSLY) ? true : false; // aske openly or anonymously
	shared_ptr<Question> questionPtr(new Question(askedByPtr, question, anonymous, questionsCount, nullptr));
	++questionsCount;

	userToAskPtr->ask(questionPtr);
}

// takes username; returns friends feed
vector<shared_ptr<IAnswer>> Database::getFriendsFeed(const string& username) const {
	shared_ptr<Account> accountPtr = getAccount(username);
	vector<string> friendsList = accountPtr->getFriendsList();
	vector<shared_ptr<IAnswer>> friendsFeed;

	for (size_t i{ 0 }; i < friendsList.size(); ++i) {
		shared_ptr<Account> friendPtr = getAccount(friendsList[i]);
		vector<shared_ptr<IAnswer>> friendFeed = friendPtr->getFeed();

		for (size_t j{ 0 }; j < friendFeed.size(); ++j) {
			friendsFeed.push_back(friendFeed[j]);
		}
	}

	return friendsFeed;
}

// takes username; returns friends feed and their answers like
vector<shared_ptr<IAnswer>> Database::getFriendsFeedAndLike(const string& username) {
	shared_ptr<Account> accountPtr = getAccount(username);
	vector<string> friendsList = accountPtr->getFriendsList();
	vector<shared_ptr<IAnswer>> feed;

	for (size_t i{ 0 }; i < friendsList.size(); ++i) {
		shared_ptr<Account> friendPtr = getAccount(friendsList[i]);
		vector<shared_ptr<IAnswer>> friendFeed = friendPtr->getFeed();

		for (size_t j{ 0 }; j < friendFeed.size(); ++j) {
			bool inserted = false;

			for (size_t k{ 0 }; k < feed.size(); ++k) {
				if (*friendFeed[j] == *feed[k]) {
					inserted = true;
				}
			}

			if (inserted == false) {
				feed.push_back(friendFeed[j]);
			}
		}
	}

	return feed;
}

// takes username; returns country feed
vector<shared_ptr<IAnswer>> Database::getCountryFeed(const string& username) const {
	shared_ptr<Account> accountPtr = getAccount(username);
	shared_ptr<Country> countryPtr = getCountry(accountPtr->getCountryName());
	return countryPtr->getCountryFeed();
}

// takes first account username second account username and answer id; switch like status of the first user to the second user answer (like -> unlike) or (unlike -> like)
void Database::switchLikeAnswer(const string& firstUsername, const string& answerOwner, int answerID) {
	shared_ptr<Account> answerOwnerPtr = getAccount(answerOwner);
	answerOwnerPtr->switchLikeAnswer(firstUsername, answerID);
}

// takes first account username second account username and answer id; returns true if the first user liked the answer of the second user; false otherwise
bool Database::isUserLikeAnswer(const string& firstUsername, const string& answerOwner, int answerID) const {
	shared_ptr<Account> accountPtr = getAccount(answerOwner);
	return accountPtr->isUserLikeMyAnswer(firstUsername, answerID);
}

// takes first account username, second account username and answer id;
// add reward from the first user to second user answer
// return true if success; false otherwise (first user has no sufficient coins)
bool Database::rewardAnswer(const string& firstUsername, const string& answerOwner, int answerID) {
	shared_ptr<Account> accountPtr = getAccount(answerOwner);

	if (accountPtr->isSufficientCoins()) {
		accountPtr->userRewardMyAnswer(firstUsername, answerID);
		return true;
	}
	else {
		return false;
	}
}

// takes username and answer id; returns usernames of users who liked the answer
vector<string> Database::getAnswerLikesUsers(const string& username, int answerID) const {
	shared_ptr<Account> accountPtr = getAccount(username);
	return accountPtr->getAnswerLikesUsers(answerID);
}

// takes username and answer id; returns usernames of users who rewarded the answer
vector<string> Database::getAnswerRewardsUsers(const string& username, int answerID) const {
	shared_ptr<Account> accountPtr = getAccount(username);
	return accountPtr->getAnswerRewardsUsers(answerID);
}

// edit account first name; takes username and new firt name
void Database::setAccountFirstName(const string& username, const string& firstName) {
	shared_ptr<Account> accountPtr = getAccount(username);
	accountPtr->setFirstName(firstName);
}

// edit account last name; takes username and new last name
void Database::setAccountLastName(const string& username, const string& lastName) {
	shared_ptr<Account> accountPtr = getAccount(username);
	accountPtr->setLastName(lastName);
}

// edit account counrty; takes username and new country name
void Database::setAccountCountry(const string& username, const string& countryName) {
	shared_ptr<Account> accountPtr = getAccount(username);
	shared_ptr<Country> newCountryPtr = getCountry(countryName);

	if (newCountryPtr == nullptr) {
		throw runtime_error("\nNo country with that name.");
	}

	shared_ptr<Country> oldCountryPtr = getCountry(accountPtr->getCountryName());

	newCountryPtr->addAccount(accountPtr);
	oldCountryPtr->removeAccount(accountPtr);
	accountPtr->setCountry(newCountryPtr);
}

// takes username; returns friends leaderboard as vector of tuple of form (username, rewards, liks)
vector<tuple<string, int, int>> Database::getFriendsLeaderboard(const string& username) const {
	shared_ptr<Account> accountPtr = getAccount(username);
	vector<string> friendsList = accountPtr->getFriendsList();
	vector<tuple<string, int, int>> leaderboard;

	for (size_t i{ 0 }; i < friendsList.size(); ++i) {
		shared_ptr<Account> friendPtr = getAccount(username);

		leaderboard.push_back(make_tuple(
			friendPtr->getUsername(),
			friendPtr->getLikes(),
			friendPtr->getRewards()));
	}

	// sort first by likes, second by rewards and third by username
	sort(leaderboard.begin(), leaderboard.end(),
		[](auto i, auto j) {
			if (get<1>(i) != get<1>(j)) { // we can sort by likes
				return (get<1>(i) > get<1>(j));
			}

			if (get<2>(i) != get<2>(j)) { // we can sort by rewards
				return (get<2>(i) > get<2>(j));
			}

			return (get<0>(i) < get<0>(j)); // sort by username
		}
	);

	return leaderboard;
}

// takes username; returns country leaderboard as vector of tuple of form (username, rewards, liks)
vector<tuple<string, int, int>> Database::getCountryLeaderboard(const string& username) const {
	shared_ptr<Account> accountPtr = getAccount(username);
	shared_ptr<Country> countryPtr = getCountry(accountPtr->getCountryName());

	if (countryPtr == nullptr) {
		throw("\nCountry was not found.");
	}

	return countryPtr->getCountryLeaderboard();
}

// takes username; returns IAccount object
shared_ptr<IAccount> Database::getProfileInfo(const string& username) const {
	shared_ptr<Account> accountPtr = getAccount(username);

	if (accountPtr == nullptr) {
		throw("\nAccount was not found.");
	}

	return accountPtr;
}

// takes two usernames; first username blocks second username
void Database::blockUser(const string& firstUsername, const string& secondUsername) {
	shared_ptr<Account> firstAccountPtr = getAccount(firstUsername);
	shared_ptr<Account> secondAccountPtr = getAccount(secondUsername);

	firstAccountPtr->blockUser(secondAccountPtr);
}

// takes usrname; returns user latest answers
vector<shared_ptr<IAnswer>> Database::getProfileLatestAnswers(const string& username) const {
	shared_ptr<Account> accountPtr = getAccount(username);
	return accountPtr->getProfileLatestAnswers();
}

// takes usrname; returns user top answers
vector<shared_ptr<IAnswer>> Database::getProfileTopAnswers(const string& username) const {
	shared_ptr<Account> accountPtr = getAccount(username);
	return accountPtr->getProfileTopAnswers();
}

// takes username and search it; returns true if found; false otherwise
bool Database::searchUsername(const string& username) const {
	shared_ptr<Account> accountPtr = getAccount(username);

	if (accountPtr != nullptr) { // found
		return true;
	}
	else {
		return false;
	}
}

// takes interest; returns vector of username with that interest
vector<string> Database::searchPeopleByInterest(const string& interest) {
	vector<shared_ptr<Account>> interestsAccountsPtrs =  interests[interest];
	vector<string> interestsUsernamesList;

	for (size_t i{ 0 }; i < interestsAccountsPtrs.size(); ++i) {
		interestsUsernamesList.push_back(interestsAccountsPtrs[i]->getUsername());
	}

	return interestsUsernamesList;
}

// takes username; returns user likes notifications
vector<shared_ptr<Notification>> Database::getLikesNotification(const string& username) const {
	shared_ptr<Account> accountPtr = getAccount(username);
	return accountPtr->getLikesNotification();
}

// takes username; returns user answers notifications
vector<shared_ptr<Notification>> Database::getAnswersNotification(const string& username) const {
	shared_ptr<Account> accountPtr = getAccount(username);
	return accountPtr->getAnswersNotification();
}

// takes username and thread id; returns vector of that thread answers
vector<shared_ptr<IAnswer>> Database::getThreadAnswers(const string& username, int threadID) const {
	shared_ptr<Account> accountPtr = getAccount(username);
	return accountPtr->getThreadAnswers(threadID);
}

// takes two usernames and thread id; implements: first user ask the second user in a thread
void Database::askInThread(const string& asker, const string& userToAsk, const string& question,
	int threadID, QuestionType questionType) {
	shared_ptr<Account> askerPtr = getAccount(asker);
	shared_ptr<Account> userToAskPtr = getAccount(userToAsk);

	bool anonymous = (questionType == QuestionType::ANONYMOUSLY) ? true : false; // aske openly or anonymously
	shared_ptr<Question> questionPtr(new Question(askerPtr, question, anonymous, questionsCount, nullptr));
	++questionsCount;

	userToAskPtr->askInThread(questionPtr, threadID);
}

// takes thread id and username of the usr who owns that thread; returns thread total likes
int Database::getThreadTotalLikes(const string& username, int threadID) const {
	shared_ptr<Account> accountPtr = getAccount(username);
	return accountPtr->getThreadTotalLikes(threadID);
}

// takes thread id and username of the usr who owns that thread; returns thread total rewards
int Database::getThreadTotalRewards(const string& username, int threadID) const {
	shared_ptr<Account> accountPtr = getAccount(username);
	return accountPtr->getThreadTotalRewards(threadID);
}

// takes username; returns vecotr of IQuestion objects
vector<shared_ptr<IQuestion>> Database::getUserQuestions(const string& username) const {
	shared_ptr<Account> accountPtr = getAccount(username);
	return accountPtr->getUserQuestions();
}

// takes username and question id; implements: user answers question
void Database::answerQuestion(const string& username, int questionID, const string& answer) {
	shared_ptr<Account> accountPtr = getAccount(username);
	accountPtr->answerQuestion(questionID, answer);
}

// takes username and question id; remove question
void Database::removeQuestion(const string& username, int questionID) {
	shared_ptr<Account> accountPtr = getAccount(username);
	accountPtr->removeQuestion(questionID);
}

// returns a pointer to Account object; returns nullptr if not found
shared_ptr<Account> Database::getAccount(const string& username) const {
	for (size_t i{ 0 }; i < accounts.size(); ++i) {
		if (accounts[i]->getUsername() == username) {
			return accounts[i];
		}
	}

	return nullptr;
}

// returns a pointer to Country object; returns nullptr if not found
shared_ptr<Country> Database::getCountry(const string& countryName) const {
	for (size_t i{ 0 }; i < countries.size(); ++i) {
		if (countries[i]->getName() == countryName) {
			return countries[i];
		}
	}

	return nullptr;
}

// follow user
void Database::followUser(const string& firstUsername, const string& secondUsername) {
	shared_ptr<Account> firstUserPtr = getAccount(firstUsername);
	shared_ptr<Account> secondUserPtr = getAccount(secondUsername);
	firstUserPtr->followUser(secondUserPtr);
}
