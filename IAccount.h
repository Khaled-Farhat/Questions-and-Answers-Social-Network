// IAccount class definition
#ifndef IACCOUNT_H
#define IACCOUNT_H

#include "Country.h"
#include <string>
#include <memory>
#include <iostream>

class IAccount {
	friend std::ostream& operator<<(std::ostream&, const IAccount&);

public:
	// constructor takes usrname, first name, last name, a pointer to country and interests list
	IAccount(const std::string&,const std::string&,const std::string&,
		std::shared_ptr<Country>, const std::vector<std::string>&);
	const std::string& getUsername() const; // returns username
	const std::string& getCountryName() const; // returns country name
	int getLikes() const; // returns total likes number
	int getRewards() const; // returns total rewards number

protected:
	void setFirstName(const std::string&); // set first name
	void setLastName(const std::string&); // set last name
	void setCountry(std::shared_ptr<Country>); // set country
	void addLike(); // add like
	void cancelLike(); // cancel like
	void addReward(); // add reward

private:
	std::string username;
	std::string firstName;
	std::string lastName;
	std::shared_ptr<Country> country;
	std::vector<std::string> interests;
	int likes;
	int rewards;
	int followers;
};

#endif