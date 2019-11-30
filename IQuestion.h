// IQuestion class definition.
#ifndef IQUESTION_H
#define IQUESTION_H

#include <iostream>
#include <string>
#include <memory>

class Account; // forward declaration of class Account

class IQuestion {
	friend std::ostream& operator<<(std::ostream&, const IQuestion&);
	
public:
	// constructor takes pointer to IAccount object, question, boolean variable (is question anonymous or open) and question id
	IQuestion(std::shared_ptr<Account>, const std::string&, bool, int);
	int getQuestionID() const; // returns question id
	const std::string getAskedBy() const; // returns the username of the user who asked the question
	const std::string& getQuestion() const; // return question string

protected:
	std::shared_ptr<Account> getAskerPtr() const; // returns a pointer to the asker ptr

private:
	std::shared_ptr<Account> askerPtr;
	std::string question;
	int id;
	bool anosymous;
};

#endif