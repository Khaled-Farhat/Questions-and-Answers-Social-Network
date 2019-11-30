// Country class definition.
#ifndef COUNTRY_H
#define COUNTRY_H

#include "IAnswer.h"
#include <string>
#include <memory>
#include <vector>

class Account; // forward declaration of class Account

class Country {
public:
	Country(const std::string&); // constructor takes counrty name
	const std::string& getName() const; // returns country name
	std::vector<std::shared_ptr<IAnswer>> getCountryFeed() const; // returns country feed
	void addAccount(std::shared_ptr<Account>); // add new account to country accounts list
	void removeAccount(std::shared_ptr<Account>); // remove account from country accounts list
	std::vector<std::tuple<std::string, int, int>> getCountryLeaderboard() const; // returns country leaderboard as vector of tuple of form (username, rewards, liks)

private:
	std::string name; // counrty name
	std::vector<std::shared_ptr<Account>> accounts; // store pointers to each account in this country
};

#endif