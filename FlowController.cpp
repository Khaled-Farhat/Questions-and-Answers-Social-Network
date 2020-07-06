// Member-function definition for class FlowController.
#include "FlowController.h"
#include "Database.h"
#include "IAccount.h"
#include "IAnswer.h"
#include "Notification.h"
#include "IQuestion.h"
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <tuple>
#include <iomanip>

using namespace std;

// enumeration constants represent main menu options
enum class MainMenuOption {ASK = 1, FEED, LEADER, INBOX, MYPROFILE, FRIENDS, SEARCH, NOTIFICATIONS, EDIT, LOGOUT};

// enumeration constants represent ask friends menu options
enum class AskFriendsOption {SELECT = 1, UNSELECT, ASK, BACK};

// enumeration constants represent feed menu optoins
enum class FeedOption {MORE = 1, VIEW, BACK};

// enumeration constants represent view answer menu options
enum class ViewAnswerOption { LIKE = 1, REWARD, THREAD, SHOWLIKES, SHOWREWARDS, FORWARD, REPORT, BACK };

// enumeration constants represent view people menu options
enum class ViewPeopleOption { MORE = 1, VIEW, BACK };

// enumeration constants represent view profile menu options
enum class ViewProfileOption { ASK = 1, FOLLOW, LATEST, TOP, BACK, BLOCK };

// enumeration constants represent view thread menu options
enum class ViewThreadOption { ASK = 1, SHOW, BACK };

// enumeration constants represent edit profile menu
enum class EditProfileOption { FIRST = 1, LAST, COUNTRY, BACK };

// enumeration constants represent view notification menu options
enum class ViewNotificationsOption { ANSWERS = 1, LIKES, BACK};

// enumeration constants represent list notification menu options
enum class ListNotificationOption { MORE = 1, SELECT, BACK };

// enumeration constants represent view notification options
enum class ViewNotificationOption { PROFILE = 1, ANSWER, BACK };

// enumeration constants represent questions inbox options
enum class QuestionsInboxOption { MORE = 1, ANSWER, REMOVE, BACK };

// constructor takes username
FlowController::FlowController(const string& username, Database& theDatabase)
	: currentUsername{ username }, database{ theDatabase } {}

// start FlowController
void FlowController::run() {
	vector<string> mainMenu{ "Ask Friends", "Show Feed", "Show Leaderboard", "Questions Inbox", "My Profile",
		"Friends", "Search people", "My Notifications", "Edit profile", "Log out"};
	bool userExited = false; // whether user has exited

	while (userExited == false) {
		displayMenu(mainMenu);
		int choice = inputInteger(1, 10);
		MainMenuOption option = static_cast<MainMenuOption>(choice);

		switch (option) {
			case MainMenuOption::ASK: { // ask friends
				cout << "Enter Question: ";
				string question;

				while (true) {
					getline(cin, question);

					if (question.size() != 0) {
						break;
					}
				}

				askFriends(question);
				break;
			}

			case MainMenuOption::FEED: // view answers feed
				myFeed();
				break;

			case MainMenuOption::LEADER: // view leaderboard
				viewLeaderboard();
				break;

			case MainMenuOption::INBOX: // show question inbox
				viewQuestionsInbox();
				break;

			case MainMenuOption::MYPROFILE: // view my profule
				viewProfile(currentUsername);
				break;

			case MainMenuOption::FRIENDS: // list friends
				listFriends();
				break;

			case MainMenuOption::SEARCH: // search people by username or interest
				searchPeople();
				break;

			case MainMenuOption::NOTIFICATIONS: // view notifications
				viewNotifications();
				break;

			case MainMenuOption::EDIT: // edit profile info (first name - last name - country)
				editProfile();
				break;

			case MainMenuOption::LOGOUT: // log out
				userExited = true;
				break;
		}
	}
}

// takes a question; manages Ask Friends interface
void FlowController::askFriends(const string& question) {
	// ask openly or anonymously
	vector<string> questionTypeMenu{ "Ask openly", "Ask anonymously" };
	cout << "\nAsk openly and get + 5 coins for each answer you’ll receive.Ask openly?";
	displayMenu(questionTypeMenu);

	int choice = inputInteger(1, 2);
	QuestionType questionType = static_cast<QuestionType>(choice);

	// get friends list
	vector<string> friendsList = database.getFriendsList(currentUsername);

	if (friendsList.size() == 0) { // no friends to ask
		cout << "\nYou have not followed any user yet. follow users to ask them.";
		return;
	
	}

	// output friends list
	for (size_t i{ 0 }; i < friendsList.size(); ++i) {
		cout << "\n" << i + 1 << " - " << friendsList[i];
	}

	vector<string> friendsToAsk;
	bool userExited = false;

	while (userExited == false) {
		vector<string> askFriendsMenu{ "Select friend", "Unselect friend", "Ask selected friends", "Back" };
		displayMenu(askFriendsMenu);
		choice = inputInteger(1, 4);
		AskFriendsOption option = static_cast<AskFriendsOption>(choice);

		switch (option) {
			case AskFriendsOption::SELECT: { // select more friends to ask
				cout << "Enter friend number: ";
				choice = inputInteger(1, friendsList.size());
				bool selected = false;

				for (size_t i{ 0 }; i < friendsToAsk.size(); ++i) {
					if (friendsToAsk[i] == friendsList[choice - 1]) {
						selected = true;
						break;
					}
				}

				if (selected == false) {
					if (friendsToAsk.size() == 50) {
						cout << "\nCan not ask more than 50 user";
					}
					else {
						friendsToAsk.push_back(friendsList[choice - 1]);
						cout << "\nFriend " << friendsList[choice - 1] << " was selected.";
					}
				}

				break;
			}

			case AskFriendsOption::UNSELECT: // un select friends to ask
				cout << "Enter friend number: ";
				choice = inputInteger(1, friendsList.size());
				cout << "\nFriend " << friendsList[choice - 1] << "was not selected.";

					for (size_t i{ 0 }; i < friendsToAsk.size(); ++i) {
						if (friendsToAsk[i] == friendsList[choice - 1]) {
							friendsToAsk.erase(friendsToAsk.begin() + i);
							break;
						}
					}

				break;

			case AskFriendsOption::ASK: // ask selected friends
				if (friendsToAsk.size() == 0) {
					cout << "\nYou have not selected any friends to ask.";
				}
				else {
					database.askMultipleUsers(currentUsername, friendsToAsk, question, questionType);
					cout << "\nSuccessfully asked selected users.";
					userExited = true;
				}

				break;

			case AskFriendsOption::BACK: // back
				userExited = true;
				break;
		}
	}
}

// show user feed
void FlowController::myFeed() {
	vector<string> menu{ "Friends feed", "Country feed"};
	displayMenu(menu);
	int choice = inputInteger(1, 2);
	vector<shared_ptr<IAnswer>> feedAnswers;

	if (choice == 1) { // friends feed
		menu = { "Also show answers my friends like", "Do not show answers my friends like" };
		displayMenu(menu);
		choice = inputInteger(1, 2);

		if (choice = 1) { // show friends like
			feedAnswers = database.getFriendsFeedAndLike(currentUsername);
		}
		else { // do not show friends like
			feedAnswers = database.getFriendsFeed(currentUsername);
		}
	}
	else { // country feed
		feedAnswers = database.getCountryFeed(currentUsername);
	}

	if (feedAnswers.size() == 0) {
		cout << "\nNo answers to show. follow some friends.";
		return;
	}

	showAnswersHelper(feedAnswers);
}

// takes answers and output them
void FlowController::showAnswersHelper(const vector<shared_ptr<IAnswer>>& answers) {
	vector<string> feedMenu{ "Show more answers", "View answer", "Back" };

	size_t index = 0;
	bool endFeed = false;
	bool userExited = false;

	cout << "\n------------------------------";

	// output first 10 answers
	for (int i{ 0 }; i < 10; ++i) {
		cout << "\n" << index +1 << " - " << *answers[index];
		++index;

		if (index == answers.size()) {
			endFeed = true;
			break;
		}
	}

	while (userExited == false) {
		displayMenu(feedMenu);
		int choice = inputInteger(1, 3);
		FeedOption option = static_cast<FeedOption>(choice);

		switch (option) {
			case FeedOption::MORE: // show more answers
								   // output 10 more answers
				if (endFeed == true) {
					cout << "\nNo more answers to show.";
					break;
				}
				else {
					for (int i{ 0 }; i < 10; ++i) {
						cout << "\n" << index + 1 << " - " << *answers[index];
						++index;

						if (index == answers.size()) {
							endFeed = true;
							break;
						}
					}
				}

				break;

			case FeedOption::VIEW: { // view answer
				cout << "\nEnter answer number: ";
				int choice = inputInteger(1, index);
				viewAnswer(answers[choice - 1]);
				break;
			}

			case FeedOption::BACK: // back
				userExited = true;
				break;
		}
	}
}

// view answer; takes profile username and answer id
void FlowController::viewAnswer(shared_ptr<IAnswer> answer) {
	vector<string> viewAnswerMenu{ "Like answer", "Reward answer", "Show complete thread", "Show likes", "Show rewards",
		"Forward the question", "Report answer", "Back" };
	bool userExited = false;
	string user = answer->getAnsweredBy();
	int answerId = answer->getAnswerID();
	bool isCurrentLikeAnswer = database.isUserLikeAnswer(currentUsername, user, answerId);

	if (isCurrentLikeAnswer == true) {
		viewAnswerMenu[0] = "Unlike answer";
	}

	while (userExited == false) {
		displayMenu(viewAnswerMenu);
		int choice = inputInteger(1, 8);
		ViewAnswerOption option = static_cast<ViewAnswerOption>(choice);

		switch (option) {
			case ViewAnswerOption::LIKE: // like - unlike answer
				database.switchLikeAnswer(currentUsername, user, answerId);
				break;

			case ViewAnswerOption::REWARD: // reward answer
				if (database.rewardAnswer(currentUsername, user, answerId) == false) {
					cout << "\nYou’re out of coins :( Answer and earn more!";
				}

				break;

			case ViewAnswerOption::THREAD: { // show complete thread
				int threadID = answer->getThreadID();
				viewThread(user, threadID);
				break;
			}

			case ViewAnswerOption::SHOWLIKES: { // show answer likes
				vector<string> likes = database.getAnswerLikesUsers(user, answerId);
				
				if (likes.size() == 0) {
					cout << "\nThere are no likes on this answer.";
				}
				else {
					listAccounts(likes);
				}
				
				break;
			}

			case ViewAnswerOption::SHOWREWARDS: { // show answer rewards
				vector<string> rewards = database.getAnswerLikesUsers(user, answerId);

				if (rewards.size() == 0) {
					cout << "\nThere are no rewards on this answer.";
				}
				else {
					listAccounts(rewards);
				}

				break;
			}

			case ViewAnswerOption::FORWARD: // forward the question
				askFriends(answer->getQuestion());
				break;

			// TO DO: REPORT ANSWER
			case ViewAnswerOption::REPORT: // report question to admin
				cout << "This feauture is not available now.";
				//database.reportAnswer(user, answerId);
				break;

			case ViewAnswerOption::BACK: // back
				userExited = true;
				break;
		}
	}
}

// list people
void FlowController::listAccounts(const vector<string>& people) {
	vector<string> viewReactantsMenu{ "Show more people", "View profile", "back" };
	bool userExited = false;
	bool endList = false;
	size_t index{ 0 };

	// output first 10 people
	for (int i{ 0 }; i < 10; ++i) {
		cout << "\n" << index + 1 << " - " << people[index];
		++index;

		if (index == people.size()) {
			endList = true;
			break;
		}
	}

	while (userExited == false) {
		displayMenu(viewReactantsMenu);
		int choice = inputInteger(1, 3);
		ViewPeopleOption option = static_cast<ViewPeopleOption>(choice);

		switch (option) {
			case ViewPeopleOption::MORE: // show more people
				if (endList == true) {
					cout << "\nNo more people to show.";
					break;
				}
				else {
					for (int i{ 0 }; i < 10; ++i) {
						cout << "\n" << index + 1 << " - " << people[i];
						++index;

						if (index == people.size()) {
							endList = true;
							break;
						}
					}
				}

				break;

			case ViewPeopleOption::VIEW: { // view profile
				cout << "\nEnter profile number: ";
				int position = inputInteger(1, index);
				viewProfile(people[position - 1]);
				break;
			}
			case ViewPeopleOption::BACK: // back
				userExited = true;
				break;
		}
	}
}

// takes profile username; manages view profile menu
void FlowController::viewProfile(const string& username) {
	shared_ptr<IAccount> account = database.getProfileInfo(username);
	cout << "\n" << *account;

	vector<string> viewProfileMenu{ "Ask", "Follow", "Show latest answers", "Show top answers", "Back", "Block this user" };
	int optionsCount{ 6 };
	bool userExited{ false };

	if (username == currentUsername) {
		viewProfileMenu[0] = "Ask my self";
		optionsCount = 4; // user can not block himself
		
		for (size_t i{ 0 }; i < viewProfileMenu.size(); ++i) {
			if (viewProfileMenu[i] == "Block this user") {
				viewProfileMenu.erase(viewProfileMenu.begin() + i);
			}
		}
	}

	while (userExited == false) {
		displayMenu(viewProfileMenu);
		int choice = inputInteger(1, optionsCount);
		ViewProfileOption option = static_cast<ViewProfileOption>(choice);

		switch (option) {
			case ViewProfileOption::ASK: { // ask user
				cout << "\nEnter the question: ";
				string question;
				
				while (true) {
					getline(cin, question);

					if (question.size() != 0) {
						break;
					}
				}

				// ask openly or anonymously
				vector<string> questionTypeMenu{ "Ask openly", "Ask anonymously" };
				cout << "\nAsk openly and get + 5 coins for each answer you’ll receive.Ask openly?";
				displayMenu(questionTypeMenu);
				int choice = inputInteger(1, 2);
				QuestionType questionType = static_cast<QuestionType>(choice);

				database.askUser(currentUsername, username, question, questionType);
				cout << "\nSuccessfully asked the question.";
				break;
			}

			case ViewProfileOption::FOLLOW: // follow user
				if (username == currentUsername) {
					cout << "You cannot follow yourself!\n";
				}
				else {
					database.followUser(currentUsername, username);
					cout << "Successfully followed " << username << "\n";
				}

				break;

			case ViewProfileOption::LATEST: { // show latest answers
				vector < shared_ptr<IAnswer>> latestAnswers = database.getProfileLatestAnswers(username);
				
				if (latestAnswers.size() == 0) {
					cout << "\nThere are no answers to show.";
				}
				else {
					showAnswersHelper(latestAnswers);
				}

				break;
			}

			case ViewProfileOption::TOP: { // show top answers
				vector < shared_ptr<IAnswer>> topAnswers = database.getProfileTopAnswers(username);

				if (topAnswers.size() == 0) {
					cout << "\nThere are no answers to show.";
				}
				else {
					showAnswersHelper(topAnswers);
				}

				break;
			}

			case ViewProfileOption::BLOCK: // block user
				database.blockUser(currentUsername, username);
				break;

			case ViewProfileOption::BACK: // back
				userExited = true;
				break;
		}
	}
}

// takes username and thread id; manages view thread menu
void FlowController::viewThread(const string& username, int threadID) {
	vector<string> viewThreadMenu{ "Keep Asking in this thread", "Show answers in this thread", "Back" };
	bool userExited = false;

	int likes = database.getThreadTotalLikes(username, threadID);
	int rewards = database.getThreadTotalRewards(username, threadID);

	cout << "\nThread total likes: " << likes
		<< "\nThread total rewards: " << rewards;

	while (userExited == false) {
		displayMenu(viewThreadMenu);
		int choice = inputInteger(1, 3);
		ViewThreadOption option = static_cast<ViewThreadOption>(choice);

		switch (option) {
		case ViewThreadOption::ASK: { // ask in thread
			cout << "\nEnter question: ";
			string question = inputString(35);

			// ask openly or anonymously
			vector<string> questionTypeMenu{ "Ask openly", "Ask anonymously" };
			cout << "\nAsk openly and get + 3 coins for each answer you’ll receive.Ask openly?";
			displayMenu(questionTypeMenu);

			choice = inputInteger(1, 2);
			QuestionType questionType = static_cast<QuestionType>(choice);

			database.askInThread(currentUsername, username, question, threadID, questionType);
			cout << "Qestion asked successfully.";
			break;
		}

		case ViewThreadOption::SHOW: { // show thread answers
			vector<shared_ptr<IAnswer>> threadAnswers = database.getThreadAnswers(username, threadID);
			showAnswersHelper(threadAnswers);
			break;
		}

		case ViewThreadOption::BACK: // back
			userExited = true;
			break;
		}
	}
}

// edit profile
void FlowController::editProfile() {
	vector<string> editProfileMenu{ "Edit first name", "Edit last name", "Edit country", "Back"};
	bool userExited = false;

	while (userExited == false) {
		displayMenu(editProfileMenu);
		int choice = inputInteger(1, 4);
		EditProfileOption option = static_cast<EditProfileOption>(choice);

		switch (option) {
			case EditProfileOption::FIRST: { // edit frist name
				cout << "\nEnter new first name: ";
				string firstName = inputString(12);
				database.setAccountFirstName(currentUsername, firstName);
				break;
			}

			case EditProfileOption::LAST: { // edit last name
				cout << "\nEnter new first name: ";
				string lastName = inputString(12);
				database.setAccountLastName(currentUsername, lastName);
				break;
			}

			case EditProfileOption::COUNTRY: { // edit country
				cout << "\nEnter new first name: ";
				string country = inputCountrySelection();
				database.setAccountCountry(currentUsername, country);
				break;
			}

			case EditProfileOption::BACK:
				userExited = true;
				break;
		}
	}
}

// view leaderboard
void FlowController::viewLeaderboard() {
	vector<string> leaderboardMenu{ "Friends leaderboard", "Country leaderboard" };
	displayMenu(leaderboardMenu);
	int choice = inputInteger(1, 2);
	vector<tuple<string, int, int>> leaderboard;

	if (choice == 1) { // friends leaderboard
		leaderboard = database.getFriendsLeaderboard(currentUsername);
	}
	else { // country leaderboard
		leaderboard = database.getCountryLeaderboard(currentUsername);
	}

	if (leaderboard.size() == 0) {
		cout << "\nNo people to show";
		return;
	}

	vector<string> viewReactantsMenu{ "Show more people", "View profile", "back" };
	bool userExited = false;
	bool endList = false;
	size_t index{ 0 };

	// output first 10 people
	for (int i{ 0 }; i < 10; ++i) {
		cout << "\n" << index + 1 << " - " 
			<< left << setw(16) << get<0>(leaderboard[index])
			<< "Likes: " << setw(4) << get<1>(leaderboard[index])
			<< "Rewards: " << setw(4) << get<2>(leaderboard[index]);
		++index;

		if (index == leaderboard.size()) {
			endList = true;
			break;
		}
	}

	while (userExited == false) {
		displayMenu(viewReactantsMenu);
		int choice = inputInteger(1, 3);
		ViewPeopleOption option = static_cast<ViewPeopleOption>(choice);

		switch (option) {
			case ViewPeopleOption::MORE: // show more people
				if (endList == true) {
					cout << "\nNo more people to show.";
					break;
				}
				else {
					for (int i{ 0 }; i < 10; ++i) {
						cout << "\n" << index + 1 << " - "
							<< left << setw(16) << get<0>(leaderboard[index])
							<< "Likes: " << setw(4) << get<1>(leaderboard[index])
							<< "Rewards: " << setw(4) << get<2>(leaderboard[index]);
						++index;

						if (index == leaderboard.size()) {
							endList = true;
							break;
						}
					}
				}

				break;

			case ViewPeopleOption::VIEW: { // view profile
				cout << "\nEnter profile number: ";
				int position = inputInteger(1, index);
				viewProfile(get<0>(leaderboard[position - 1]));
				break;
			}
			case ViewPeopleOption::BACK: // back
				userExited = true;
				break;
			}
	}
}

// manages search people manu
void FlowController::searchPeople() {
	vector<string> searchMenu{ "Search by username", "Search by interests" };
	displayMenu(searchMenu);
	int choice = inputInteger(1, 2);

	if (choice == 1) { // search by username
		cout << "\nEnter username: ";
		string username = inputString(12);
		
		bool result = database.searchUsername(username);

		if (result == true) {
			viewProfile(username);
		}
		else {
			cout << "\nUser was not found.";
		}
	}
	else { // search by interest
		cout << "\nEnter one interest: ";
		string interest = inputString(12);

		vector<string> accounts = database.searchPeopleByInterest(interest);

		if (accounts.size() == 0) {
			cout << "\nNo user was found.";
			return;
		}

		listAccounts(accounts);
	}
}

// list user friends
void FlowController::listFriends() {
	vector<string> friends = database.getFriendsList(currentUsername);

	if (friends.size() == 0) {
		cout << "\nYou have not followed any user yet.";
	}
	else {
		listAccounts(friends);
	}
}

// helper to input country selection
string FlowController::inputCountrySelection() {
	vector<string> countryList = database.getCountryList();
	size_t index = 0;
	bool userSelected = false; // whether user has made a choice
	string countrySelection;

	while (index < countryList.size()) {
		// output 10 more countries
		for (int i{ 0 }; i < 10; ++i) {
			if (index == countryList.size()) { // no mpre countries to show
				break;
			}

			cout << "\n" << index + 1 << " - " << countryList[index];
			++index;
		}

		// no more countries to show
		if (index == countryList.size()) {
			break;
		}

		cout << "\n1 - Show more"
			<< "\n2 - Select a country"
			<< "\nMake a choice: ";
		int choice = inputInteger(1, 2);

		switch (choice) {
		case 1:
			break;

		case 2:
			int position = inputInteger(1, index);
			userSelected = true;
			countrySelection = countryList[position - 1];
			break;
		}

		if (userSelected) {
			break;
		}
	}

	if (userSelected) {
		return countrySelection;
	}
	else {
		cout << "\nNo more countries to show. Select a country: ";
		int position = inputInteger(1, index);
		userSelected = true;
		countrySelection = countryList[position - 1];
		return countrySelection;
	}
}

// view notifications
void FlowController::viewNotifications() {
	vector<string> viewNotificationMenu{ "Answers notifications", "Likes notifications", "Back" };
	bool userExited = false;

	while (userExited == false) {
		displayMenu(viewNotificationMenu);
		int choice = inputInteger(1, 5);
		ViewNotificationsOption option = static_cast<ViewNotificationsOption>(choice);

		switch (option) {
			case ViewNotificationsOption::ANSWERS: { // get answers notifications
				vector<shared_ptr<Notification>> notifications = database.getAnswersNotification(currentUsername);
				listNotifications(notifications);
				break;
			}

			case ViewNotificationsOption::LIKES: { // get likes notification
				vector<shared_ptr<Notification>> notifications = database.getLikesNotification(currentUsername);
				listNotifications(notifications);
				break;
			}

			case ViewNotificationsOption::BACK: // back
				userExited = true;
				break;
		}
	}
}

// view questions inbox
void FlowController::viewQuestionsInbox() {
	vector<shared_ptr<IQuestion>> questions = database.getUserQuestions(currentUsername);

	cout << "\n------------------------------";

	if (questions.size() == 0) {
		cout << "\nNo questions to show.";
		return;
	}

	vector<string> questionsInboxMenu{ "Show more questions", "Answer question", "Remove question", "Back" };
	bool userExited = false;
	bool endList{ false };
	size_t index{ 0 };

	for (int i{ 0 }; i < 10; ++i) {
		cout << "\n" << index + 1 << " - " << *questions[index];
		++index;

		if (index == questions.size()) {
			endList = true;
			break;
		}
	}

	while (userExited == false) {
		displayMenu(questionsInboxMenu);
		int choice = inputInteger(1, 4);
		QuestionsInboxOption option = static_cast<QuestionsInboxOption>(choice);

		switch (option) {
			case QuestionsInboxOption::MORE: // show more questions
				if (endList == true) {
					cout << "\nNo more questions to show";
				}
				else {
					for (int i{ 0 }; i < 10; ++i) {
						cout << "\n" << index + 1 << " - " << questions[index];
						++index;

						if (index == questions.size()) {
							endList = true;
							break;
						}
					}
				}

				break;

			case QuestionsInboxOption::ANSWER: { // answer question
				cout << "\nEnter question number: ";
				choice = inputInteger(1, index);

				cout << "Enter answer: ";
				string answer;

				while (true) {
					getline(cin, answer);

					if (answer.size() != 0) {
						break;
					}
				}

				database.answerQuestion(currentUsername, questions[choice - 1]->getQuestionID(), answer);
				cout << "\nQuestion answered successfully.";
				break;
			}

			case QuestionsInboxOption::REMOVE: // remove question
				cout << "\nEnter question number to remove: ";
				choice = inputInteger(1, index);
				database.removeQuestion(currentUsername, questions[choice - 1]->getQuestionID());
				break;

			case QuestionsInboxOption::BACK: // back
				userExited = true;
				break;
		}
	}
}

// list notifications
void FlowController::listNotifications(const vector<shared_ptr<Notification>>& notifications) {
	if (notifications.size() == 0) {
		cout << "\nThere are no notifications to show";
		return;
	}

	bool endList{ false };
	bool userExited{ false };
	size_t index{ 0 };

	for (int i{ 0 }; i < 10; ++i) {
		cout << "\n" << index + 1 << " - " << *notifications[i];
		++index;

		if (index == notifications.size()) {
			endList = true;
			break;
		}
	}

	vector<string> listNotificationsMenu{ "Show more notifications", "Select notification", "Back" };

	while (userExited == false) {
		displayMenu(listNotificationsMenu);
		int choice = inputInteger(1, 3);
		ListNotificationOption option = static_cast<ListNotificationOption>(choice);

		switch (option) {
			case ListNotificationOption::MORE: // show more notifications
				if (endList == true) {
					cout << "\nNo more notifications to show.";
				}
				else {
					for (int i{ 0 }; i < 10; ++i) {
						cout << "\n" << index + 1 << " - " << *notifications[i];
						++index;

						if (index == notifications.size()) {
							endList = true;
							break;
						}
					}
				}

				break;

			case ListNotificationOption::SELECT:
				cout << "\nEnter notification number: ";
				choice = inputInteger(1, index);
				viewNotification(notifications[choice - 1]);
				break;

			case ListNotificationOption::BACK: // back
				userExited = true;
				break;
		}
	}

}

// view notification
void FlowController::viewNotification(shared_ptr<Notification> notification) {
	cout << "\n" << *notification;
	
	vector<string > notificationMenu{ "View user profile", "View answer", "Back" };
	bool userExited = false;

	while (userExited == false) {
		displayMenu(notificationMenu);
		int choice = inputInteger(1, 3);
		ViewNotificationOption option = static_cast<ViewNotificationOption>(choice);

		switch (option) {
			case ViewNotificationOption::PROFILE: { // view profile
				string user = notification->getUser();
				viewProfile(user);
				break;
			}

			case ViewNotificationOption::ANSWER: // view answer
				cout << "\n------------------------------";
				cout << *(notification->getAnswer());
				viewAnswer(notification->getAnswer());
				break;

			case ViewNotificationOption::BACK: // back
				userExited = true;
				break;
		}
	}
}

// utility function to display menu
void FlowController::displayMenu(const vector<string>& menu) const {
	cout << "\n";

	for (size_t i{ 0 }; i < menu.size(); ++i) {
		cout << "\n" << i + 1 << " - " << menu[i];
	}
	cout << "\nMake a choice: ";
}

// utility function to input integer in range [low - high]
int FlowController::inputInteger(int low, int high) {
	int integer = 0;

	while (1) {
		string input;
		getline(cin, input);

		if (input.size() == 0) {
			continue;
		}

		bool validInput = 1;

		for (size_t i{ 0 }; i < input.size(); ++i) {
			if (isdigit(input[i]) == false) {
				validInput = 0;
				break;
			}
		}

		if (input.size() > 9) {
			validInput = 0;
		}

		if (validInput) {
			integer = stoi(input);

			if (integer < low || integer > high) {
				validInput = false;
			}
		}

		if (validInput) {
			break;
		}
		else {
			cout << "Invalid input. Enter integer in range [" << low << " - " << high << "]: ";
		}
	}

	return integer;
}

// utility function to input string with size <= stringSize
string FlowController::inputString(size_t stringSize) {
	string inputString;

	while (true) {
		string input;
		getline(cin, input);

		if (input.size() == 0) {
			continue;
		}

		bool validInput = 1;

		for (size_t i{ 0 }; i < input.size(); ++i) {
			if (input[i] == ' ' || ispunct(input[i])) {
				validInput = 0;
			}
		}

		if (input.size() > stringSize) {
			validInput = 0;
		}

		if (validInput) {
			inputString = input;
			break;
		}
		else {
			cout << "Invalid input. Enter a string without spaces, without special characters and with size less than "
				<< stringSize << ": ";
		}
	}

	return inputString;
}
