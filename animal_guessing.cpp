//
// animal_guessing.cpp
// Created by Jiahao Liang on 11/24/21.

#include "animal_guessing.h"

AniamlGuessingNode::AniamlGuessingNode():yes(nullptr), no(nullptr){}

AniamlGuessingNode::AniamlGuessingNode(const std::string &text):text(text) ,yes(nullptr), no(nullptr){}

AniamlGuessingNode::~AniamlGuessingNode()
{
}

AniamlGuessingTree::AniamlGuessingTree():root(nullptr){}

AniamlGuessingTree::~AniamlGuessingTree()
{
	if(root != nullptr)
	{
		remove(root);
	}
}

void AniamlGuessingTree::remove(AniamlGuessingNode* leaf)
{
	if(leaf != nullptr)
	{
		remove(leaf->yes);
		remove(leaf->no);
		delete leaf;
		leaf = nullptr;
	}
}

void AniamlGuessingTree::createDecisionTree()
{
	std::cout << "\nInput 0 to stop" << std::endl;
	clearInstream();
	this->root = buildDecisionTree(1, "root");
}

AniamlGuessingNode* AniamlGuessingTree::buildDecisionTree(const int &level, const std::string &pos)
{
	std::string text;
	if(pos == "root")
	{
		std::cout << "(level " << std::to_string(level) << ") (" << pos << ") Enter a QUESTION: ";
		getline(std::cin, text);
		
	}
	else
	{
		std::cout << "(level " << std::to_string(level) << ") (" + pos << ") Enter a QUESTION or an ANSWER: ";
		getline(std::cin, text);
	}
		if(text == "0")
		{
			return nullptr;
		}
		AniamlGuessingNode* newNode = new AniamlGuessingNode(text);
		newNode->yes = buildDecisionTree(level+1, "YES");
		newNode->no = buildDecisionTree(level+1, "NO");
		return newNode;
}

/* 
** for extra credit, for extra credit, open the file to write.
*/
void AniamlGuessingTree::openFile(std::ofstream &outFile)
{
	//const std::string fileName = getUserInput<std::string>("Enter the file-path: ");
	const std::string fileName = "animals_data.txt";
	outFile.open(fileName.c_str(), std::ios::out);
	if (!outFile.is_open())
	{
		std::cout << "ERROR: INVALID INPUT, PLEASE RE-ENTER" << std::endl;
		clearInstream();
		openFile(outFile);
	}
}

/* 
** for extra credit, open the file to read.
*/
void AniamlGuessingTree::openFile(std::ifstream &inFile)
{
	//const std::string fileName = getUserInput<std::string>("Enter the file-path: ");
	std::string fileName = "animals_data.txt";
	inFile.open(fileName.c_str(), std::ios::in);
	if (!inFile.is_open())
	{
		std::cout << "ERROR: INVALID INPUT, PLEASE RE-ENTER" << std::endl;
		clearInstream();
		openFile(inFile);
	}
}

void AniamlGuessingTree::readDataFromFile()
{
	std::ifstream inFile;
	openFile(inFile);
	this->root = readDataFromFile(inFile);
	closeFile(inFile);
}

AniamlGuessingNode* AniamlGuessingTree::readDataFromFile(std::ifstream& inFile)
{
	std::string buffer;
	getline(inFile, buffer);
	if(buffer != "#NULL")
	{
		AniamlGuessingNode* newNode;
		newNode = new AniamlGuessingNode(buffer);
		newNode->yes = readDataFromFile(inFile);
		newNode->no = readDataFromFile(inFile);
		return newNode;
	}
	else
	{
		return nullptr;
	}
}

void AniamlGuessingTree::writeDataToFile()
{
	std::ofstream outFile;
	openFile(outFile);
	writeDataToFile(root, outFile);
	closeFile(outFile);
}

void AniamlGuessingTree::writeDataToFile(AniamlGuessingNode* leaf, std::ofstream& outFile)
{
	if(leaf == nullptr)
	{
		outFile << "#NULL" << "\n";
		return;
	}
	outFile << leaf->text << "\n";
	writeDataToFile(leaf->yes, outFile);
	writeDataToFile(leaf->no, outFile);
}

void AniamlGuessingTree::printGreetingMessage()
{
	std::cout << "Welcome to Animal Guessing Game." << std::endl;
}

int AniamlGuessingTree::selectPlayStyle()
{
	std::cout << "\n\t[1]. Load Questions From a Text File to Play\n"
			  << "\t[2]. Create Your Own Questions to Play\n";
	std::string playStyle = getUserInput<std::string>("\nPlease Select [1/2]: ");
	if(playStyle == "1")
	{
		return 1;
	}
	else if(playStyle == "2")
	{
		return 2;
	}
	else 
	{
		std::cout << "ERROR: INVALID ARGUMENT" << std::endl;
		clearInstream();
		return selectPlayStyle();
	}
}

void AniamlGuessingTree::playGame()
{

	printGreetingMessage();
	switch(selectPlayStyle()) 
	{
		case 1:
			readDataFromFile();
			break;
		case 2:
			createDecisionTree();
			break;
	}
	do
	{
		bool guessIsTrue = false;
		AniamlGuessingNode** lastNode;
		AniamlGuessingNode* temp;
		temp = guess(root, guessIsTrue);
		lastNode = &temp;
		if(guessIsTrue)
		{
			std::cout << "I guessed right." << std::endl;
		}
		else 
		{
			std::string previousAnimal = (*lastNode)->text;
			std::string newAnimal;
			std::cout << "I give up. What are you?\n";
			getline(std::cin, newAnimal);
			addBranch(lastNode, previousAnimal, newAnimal);
		}
	}while(playAgain());
	writeDataToFile();
	std::cout << "Thank you for teaching me a thing or two.";
}

AniamlGuessingNode* AniamlGuessingTree::guess(AniamlGuessingNode* leaf, bool &isTure)
{
	std::string input;
	if(leaf->yes == nullptr && leaf->no == nullptr)
	{
		std::cout << "My guess is a " << leaf->text << ". Am I right? [Yes/No]:\n";
		std::cin >> input;
		clearInstream();
		toUper(input);
		isValidInput(input);
		input == "YES" || input == "Y" ? isTure = true : isTure = false;
		return leaf;
	}
	else
	{
		std::cout << leaf->text << std::endl;
		std::cin >> input;
		toUper(input);
		isValidInput(input);
		return input == "YES" || input == "Y" ? guess(leaf->yes, isTure) : guess(leaf->no, isTure);
	}
	if(leaf == nullptr)
	{
		throw std::out_of_range("ERROR: OUT OF RANGE");
	}
}

void AniamlGuessingTree::addBranch(AniamlGuessingNode** leaf, const std::string &previousAnimal, const std::string &newAnimal)
{
	std::string newQuestion;
	std::string response;
	std::cout << "Please type a [YES/NO] question that will distinguish a " << previousAnimal << " from a " << newAnimal << ".\n";
	getline(std::cin, newQuestion);
	(*leaf)->text = newQuestion;
	std::cout << "As a " << newAnimal << ", " << newQuestion << " Please answer [Yes or No]:\n";
	std::cin >> response;
	toUper(response);
	isValidInput(response);
	if(response == "YES" || response == "Y")
	{
		(*leaf)->yes = new AniamlGuessingNode(newAnimal);
		(*leaf)->no = new AniamlGuessingNode(previousAnimal);
	}
	else 
	{
		(*leaf)->yes = new AniamlGuessingNode(previousAnimal);
		(*leaf)->no = new AniamlGuessingNode(newAnimal);
	}
}

bool AniamlGuessingTree::playAgain()
{
	std::string continuePlay = getUserInput<std::string>("Shall we play again? [YES/NO]:\n");
	toUper(continuePlay);
	isValidInput(continuePlay);
	return continuePlay == "YES" || continuePlay == "Y" ? true : false;
}

char AniamlGuessingTree::toUper(const char &symbol)
{
	return (symbol >= 'a' && symbol <= 'z') ? symbol - 'a' + 'A' : symbol;
}

void AniamlGuessingTree::toUper(std::string &str)
{
	for(auto i = str.begin(); i != str.end(); ++i)
	{
		*i = toUper(*i);
	}
}

void AniamlGuessingTree::clearInstream()
{
	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
}

void AniamlGuessingTree::isValidInput(std::string &str)
{
	if(str == "YES" ||
		str == "NO" ||
		str == "Y"  ||
		str == "N")
	{
		return;
	}
	clearInstream();
	str = getUserInput<std::string>("ERROR: INVALID INPUT, PLEASE RE-ENTER [YES/NO]: ");
	toUper(str);
	return isValidInput(str);
}