#pragma once
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <random>
#include <filesystem>

using namespace std;

enum priorityLevel
{
	Low,Normal,High
};
class File
{
	friend class node;
public:
	string name;
	string type;
	node* parentFolder;
	int numberOfCharacters;
	int numberOfLines;
	double avgNumOfCharPerLine;
	string creationTime;
	string owner;
	priorityLevel priority;
	float timeTakesForPrint;
	bool readOnly;

	File(string fileName, bool isReadOnly)
	{
		name = fileName;
		type = getExtension();
		creationTime = setCreationTime();
		owner = "Bilal";
		priority = getRandomPriority();
		numberOfCharacters = 0;
		numberOfLines = 0;
		avgNumOfCharPerLine = 0;
		timeTakesForPrint = 0;
		readOnly = isReadOnly;
	}
	
	static string setCreationTime()
	{
		// Get the current time in seconds
		time_t currentTime = time(0);

		// Declare and initialize a struct tm variable
		tm timeStruct = {};

		// Use localtime_s for safer version
		if (localtime_s(&timeStruct, &currentTime) != 0) {
			// Handle the error, for example, throw an exception or return an error string
			return "Error getting local time";
		}

		// Format the time into a string
		ostringstream oss;
		oss << put_time(&timeStruct, "%m/%d/%Y %I:%M %p");

		// Return the formatted time as a string
		return oss.str();
	}
	void setTimeToPrint()
	{
		int charactersInFile = numberOfCharacters;
		float timeInSeconds = charactersInFile * 10;
		timeTakesForPrint = timeInSeconds;
	}
	void setcountLinesInFile() {
		ifstream file(name);

		if (!file.is_open()) {
			cout << "Error opening file: " << name << std::endl;
			return;
		}

		int lineCount = 0;
		string line;

		while (std::getline(file, line))
			lineCount++;

		file.close();
		numberOfLines = lineCount;
	}
	void setcountCharactersInFile() {
		ifstream file(name);

		if (!file.is_open()) {
			cout << "Error opening file: " << name << endl;
			return; // Return -1 to indicate an error
		}

		int characterCount = 0;
		char c;

		while (file.get(c)) {
			characterCount++;
		}

		file.close();
		numberOfCharacters = characterCount;
	}
	void setaverageCharactersPerLine() {
		ifstream file(name);

		if (!file.is_open()) {
			cout << "Error opening file: " << name << std::endl;
			return;
		}

		int totalCharacters = 0;
		int lineCount = 0;
		string line;

		while (getline(file, line)) {
			totalCharacters += line.length();
			lineCount++;
		}

		file.close();

		if (lineCount == 0) {
			cout << "File is empty." << endl;
			avgNumOfCharPerLine = 0;
			return;
		}

		avgNumOfCharPerLine = static_cast<double>(totalCharacters) / lineCount;
	}

	string getExtension()
	{
		if (name.substr(name.length() - 4) == ".txt")
			return ".txt";
		else
			return ".text";
	}
	priorityLevel getRandomPriority() {
		
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> distribution(0, 2);

		int randomPriorityValue = distribution(gen);

		// Convert the random value to the Priority enum
		switch (randomPriorityValue) {
		case 1:
			return Low;
		case 2:
			return Normal;
		case 3:
			return High;
		default:
			return Normal; // Default to Normal if something unexpected happens
		}
	}
	int priorityValue()
	{
		if (priority == High)
			return 2;
		else if (priority == Normal)
			return 1;
		else
			return 0;
	}
	string priorityString()
	{
		if (priority == 2)
			return "High";
		else if (priority == 1)
			return "Normal";
		else
			return "Low";
	}
};