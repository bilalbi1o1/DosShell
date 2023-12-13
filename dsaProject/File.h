#pragma once
#include <string>
#include<chrono>
#include<sstream>
#include<iomanip>

using namespace std;

enum priorityLevel
{
	High, Normal, Low
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
		priority = Normal;
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
};