//
// animal_guessing.h
// Created by Jiahao Liang on 11/24/21.

#pragma once
#ifndef _ANIMAL_GUESSING_H_
#define _ANIMAL_GUESSING_H_

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <climits>

class AniamlGuessingTree;

class AniamlGuessingNode
{
public:
	friend class AniamlGuessingTree;
	AniamlGuessingNode();
	AniamlGuessingNode(const std::string& = "");
	~AniamlGuessingNode();

private:
	AniamlGuessingNode* yes;
	AniamlGuessingNode* no;
	std::string text;
};


class AniamlGuessingTree
{
public:
	AniamlGuessingTree();
	~AniamlGuessingTree();
	void playGame();
	
private:
	void printGreetingMessage();
	void createDecisionTree();
	void remove(AniamlGuessingNode*);
	AniamlGuessingNode* buildDecisionTree(const int&, const std::string&);
	AniamlGuessingNode* guess(AniamlGuessingNode*, bool&);
	void addBranch(AniamlGuessingNode**, const std::string&, const std::string&);
	bool playAgain();
	int selectPlayStyle();
	void openFile(std::ofstream&);
	void openFile(std::ifstream&);
	template<typename T> void closeFile(T&);
	void readDataFromFile();
	AniamlGuessingNode* readDataFromFile(std::ifstream&);
	void writeDataToFile();
	void writeDataToFile(AniamlGuessingNode*, std::ofstream&);
	char toUper(const char&);
	void toUper(std::string&);
	template<typename T> inline T getUserInput(const std::string&);
	template<typename T> inline T getUserLineInput(const std::string&);
	void clearInstream();
	void isValidInput(std::string&);
	
private:
	AniamlGuessingNode* root;
};

/* 
** for extra credit, for extra credit, close the file.
*/
template<typename T> 
void AniamlGuessingTree::closeFile(T &file)
{
	file.close();
	if(file.fail()) 
	{
		throw std::bad_exception();
	}
}

template <typename T>
inline T AniamlGuessingTree::getUserInput(const std::string &prompt)
{
	T input;
	std::cout << prompt;
	std::cin >> input;
	return input;
}

template<typename T>
inline T AniamlGuessingTree::getUserLineInput(const std::string &prompt)
{
	T input;
	std::cout << prompt;
	getline(std::cin, input);
	return input;
}

#endif
