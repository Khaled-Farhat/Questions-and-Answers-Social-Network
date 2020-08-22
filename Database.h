// Database class definition.
#ifndef DATABASE_H
#define DATABASE_H

#include "Account.h"
#include "IAccount.h"
#include "IAnswer.h"
#include "IQuestion.h"
#include "Country.h"
#include "Question.h"
#include "Notification.h"
#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include <stack>
#include <unordered_map>

// enumeration constants represent question type
enum class QuestionType { OPENLY = 1, ANONYMOUSLY };

class Database {
public:
	Database(); // default constructor
	bool isValidUsername(const std::string&) const; // takes username; returns true if there is no account with that username; false otherwise
	std::vector<std::string> getCountryList() const; // returns vector of country names
	void createAccount(const std::string&, const std::string&, const std::string&,
		const std::string&, const std::string&, const std::vector<std::string>&); // create a new account; takes usrname, password, first name, last name, country and interests
	bool authenticateUser(const std::string&, const std::string&) const; // takes username and password; returns true if username and password are correct
	std::vector<std::string> getFriendsList(const std::string&) const; // takes username; returns list of friends of that user
	void askMultipleUsers(const std::string&, const std::vector<std::string>&, const std::string&, QuestionType); // ask multiple friends; takes account username, friends list;
																												  // question and takes question type (openly - anonymously)
	std::vector<std::shared_ptr<IAnswer>> getFriendsFeed(const std::string&) const; // takes username; returns friends feed
	std::vector<std::shared_ptr<IAnswer>> getFriendsFeedAndLike(const std::string&); // takes username; returns friends feed and their answers like
	std::vector<std::shared_ptr<IAnswer>> getCountryFeed(const std::string&) const; // takes username; returns country feed
	void switchLikeAnswer(const std::string&, const std::string&, int); // takes first account username second account username and answer id;
																		// switch like status of the first user to the second user answer (like -> unlike) or (unlike -> like)
	bool isUserLikeAnswer(const std::string&, const std::string&, int) const; // takes first account username second account username and answer id; returns true if the first user
																			  // liked the answer of the second user; false otherwise
	bool rewardAnswer(const std::string&, const std::string&, int); // takes first account username, second account username and answer id;
																	// add reward from the first user to second user answer
																	// return true if success; false otherwise (first user has no sufficient coins)
	// TO DO: void reportAnswer(const std::string&, int); // takes username of answer owner and answer id; report the answer to admin
	std::vector<std::string> getAnswerLikesUsers(const std::string&, int) const; // takes username and answer id; returns usernames of users who liked the answer
	std::vector<std::string> getAnswerRewardsUsers(const std::string&, int) const; // takes username and answer id; returns usernames of users who rewarded the answer
	void setAccountFirstName(const std::string&, const std::string&); // edit account first name; takes username and new firt name
	void setAccountLastName(const std::string&, const std::string&); // edit account last name; takes username and new last name
	void setAccountCountry(const std::string&, const std::string&); // ecit account country; takes username and new country name
	std::vector<std::tuple<std::string, int, int>> getFriendsLeaderboard(const std::string&) const; // takes username; returns friends leaderboard as vector of tuple of form (username, rewards, liks)
	std::vector<std::tuple<std::string, int, int>> getCountryLeaderboard(const std::string&) const; // takes username; returns country leaderboard as vector of tuple of form (username, rewards, liks)
	std::shared_ptr<IAccount> getProfileInfo(const std::string&) const; // takes username; returns IAccount object
	void blockUser(const std::string&, const std::string&); // takes two usernames; first username blocks second username
	void askUser(const std::string&, const std::string&, const std::string&, QuestionType); // ask user; takes two accounts usernames, question and takes question type (openly - anonymously)
	std::vector<std::shared_ptr<IAnswer>> getProfileLatestAnswers(const std::string&) const; // takes usrname; returns user latest answers
	std::vector<std::shared_ptr<IAnswer>> getProfileTopAnswers(const std::string&) const; // takes username; returns user top answers (answers with greater likes)
	bool searchUsername(const std::string&) const; // takes username and search it; returns true if found; false otherwise
	std::vector<std::string> searchPeopleByInterest(const std::string&); // takes interest; returns vector of username with that interest
	std::vector<std::shared_ptr<Notification>> getLikesNotification(const std::string&) const; // takes username; returns user likes notifications
	std::vector<std::shared_ptr<Notification>> getAnswersNotification(const std::string&) const; // takes username; returns user answers notifications
	std::vector<std::shared_ptr<IAnswer>> getThreadAnswers(const std::string&, int) const; // takes username and thread id; returns vector of that thread answers
	void askInThread(const std::string&, const std::string&, const std::string&, int, QuestionType); // takes two usernames, thread id and questin type; implements: first user ask the second user in a thread
	int getThreadTotalLikes(const std::string&, int) const; // takes thread id and username of the usr who owns that thread; returns thread total likes
	int getThreadTotalRewards(const std::string&, int) const; // takes thread id and username of the usr who owns that thread; returns thread total rewards
	std::vector<std::shared_ptr<IQuestion>> getUserQuestions(const std::string&) const; // takes username; returns vecotr of IQuestion objects
	void answerQuestion(const std::string&, int, const std::string&); // takes username, question id and answer; implements: user answers question
	void removeQuestion(const std::string&, int); // takes username and question id; remove question
	void followUser(const std::string&, const std::string&); // takes usernames of the first and second user and add the second user to the friends of the first user

private:
	std::unordered_map<std::string, std::shared_ptr<Account>> accounts;
	std::vector<std::shared_ptr<Country>> countries;
	std::unordered_map<std::string, std::vector<std::shared_ptr<Account>>> interests;
	int questionsCount{ 0 };

	std::shared_ptr<Account> getAccount(const std::string&) const; // takes username; returns a pointer to Account object; returns nullptr if not found
	std::shared_ptr<Country> getCountry(const std::string&) const; // takes country name; returns a pointer to country object; returns nullptr if not found
};

#endif
