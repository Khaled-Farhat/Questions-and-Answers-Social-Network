// Member-function definition for class Country.
#include "Country.h"
#include "Account.h"
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

// constrcutor takes country name
Country::Country(const std::string& countryName)
	: name(countryName) {}

// returns country name
const string& Country::getName() const { return name; }

// returns country feed
vector<shared_ptr<IAnswer>> Country::getCountryFeed() const {
	vector<shared_ptr<IAnswer>> feed;

	for (size_t i{ 0 }; i < accounts.size(); ++i) {
		vector<shared_ptr<IAnswer>> accountFeed = accounts[i]->getFeed();

		for (size_t j{ 0 }; j < accountFeed.size(); ++j) {
			feed.push_back(accountFeed[j]);
		}
	}

	return feed;
}

// add new account to country accounts list
void Country::addAccount(shared_ptr<Account> accountPtr) {
	accounts.push_back(accountPtr);
}

// remove account from country accounts list
void Country::removeAccount(shared_ptr<Account> accountPtr) {
	for (size_t i{ 0 }; i < accounts.size(); ++i) {
		if (accounts[i]->getUsername() == accountPtr->getUsername()) {
			accounts.erase(accounts.begin() + i);
			break;
		}
	}
}

// returns country leaderboard as vector of tuple of form (username, rewards, liks)
vector<tuple<string, int, int>> Country::getCountryLeaderboard() const {
	vector<tuple<string, int, int>> leaderboard;

	for (size_t i{ 0 }; i < accounts.size(); ++i) {
		shared_ptr<Account> accountPtr = accounts[i];

		leaderboard.push_back(make_tuple(
			accountPtr->getUsername(),
			accountPtr->getLikes(),
			accountPtr->getRewards()));
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
