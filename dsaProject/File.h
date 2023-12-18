#pragma once
#include <string>
#include<chrono>
#include<sstream>
#include<iomanip>
#include<random>

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
	float avgNumOfCharPerLine;
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