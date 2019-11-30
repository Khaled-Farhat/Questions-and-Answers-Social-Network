// Member-functio definition for class IAccount.
#include "IAccount.h"
#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

// constructor takes usrname, first name, last name, a pointer to country and interests list
IAccount::IAccount(const string& user, const string& first, const string& last,
	shared_ptr<Country> countryPtr, const vector<string>& interestsList)
	: username(user), firstName(first), lastName(last), country(countryPtr),
	  interests(interestsList), likes(0), followers(0) {}

// returns username
const string& IAccount::getUsername() const { return username; }

// returns country name
const string& IAccount::getCountryName() const { return country->getName(); }

// returns total likes number
int IAccount::getLikes() const { return likes; }

// returns total rewards number
int IAccount::getRewards() const { return rewards; }

// set first name
void IAccount::setFirstName(const string& first) { firstName = first; }

// set last name
void IAccount::setLastName(const string& last) { lastName = last; }

// set country
void IAccount::setCountry(shared_ptr<Country> countryPtr) { country = countryPtr; }

// add like
void IAccount::addLike() { ++likes; }

// cancel like
void IAccount::cancelLike() { --likes; }

// add reward
void IAccount::addReward() { ++rewards; }

ostream& operator<<(ostream& output, const IAccount& iAccount) {
	cout << "\nUsername: " << iAccount.username
		<< "\nName: " << iAccount.firstName << " " << iAccount.lastName
		<< "\nCountry: " << (iAccount.country)->getName()
		<< "\nLikes: " << iAccount.getLikes() << "\tRewards: " << iAccount.getRewards() << "\tFollowers: " << iAccount.followers << "\n";

	return output;
}

