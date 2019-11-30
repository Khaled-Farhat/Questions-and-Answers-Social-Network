// IAnswer class definition.
#ifndef ANSWERINFO_H
#define	ANSWERINFO_H

#include <string>
#include <iostream>
#include <memory>

class Thread; // forward declaration of class Thread
class Question; // forward declaration of class Question
class IQuestion; // forward declaration of class IQuestion
class IAccount; // forward declaration of class IAccount

class IAnswer {
	friend std::ostream& operator<<(std::ostream&, const IAnswer&);

public:
	// constructor takes a pointer to question, a pointer to thread, a pointer to the answerer, the answer and answer id
	IAnswer(std::shared_ptr<Question>, std::shared_ptr<Thread>, std::shared_ptr<IAccount>, const std::string&, int);
	int getAnswerID() const; // return answer id
	int getThreadID() const; // return thread id
	const std::string& getAnsweredBy() const; // return answeredBy
	const std::string& getQuestion() const; // return question
	int getLikes() const; // returns number answer likes
	int getRewards() const;  // returns number of answer rewards
	bool operator==(const IAnswer&);
	const std::string& getAnswerString() const; // returns answer string
	std::shared_ptr<IQuestion> getQuestionPtr() const; // returns a pointer to IQuestion object

protected:
	void decreaseLikes(); // decrease likes by 1
	void increaseLikes(); // increase likse by 1
	void increaseRewards(); // increase rewards by 1

private:
	std::shared_ptr<Thread> thread;
	std::shared_ptr<Question> questionPtr;
	std::shared_ptr<IAccount> answererPtr;
	int answerId;
	std::string answer;
	int likes;
	int rewards;
};

#endif