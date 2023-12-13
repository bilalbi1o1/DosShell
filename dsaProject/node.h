#pragma once
#include "File.h"
#include<list>
#include<cctype>
#include<stack>

class node
{
public:
	string name;
	string creationTime;
	string owner;
	bool readOnly;

	node* parent;
	list<node*> subFolders;
	list<File*> files;

	node(string foldername, bool isReadOnly)
	{
		name = foldername;
		creationTime = File::setCreationTime();
		owner = "Bilal";
		readOnly = isReadOnly;
	}

	string getPath()
	{
		string path = "";
		node* itr = this;
		stack<string> folders;
		while (itr != nullptr)
		{
			folders.push(itr->name);
			itr = itr->parent;
		}
		while (!folders.empty())
		{
			path = path + folders.top() + "\\";
			folders.pop();
		}
		if (path.length() > 4)
			path = path.substr(0, path.length() - 1);
		return path;
	}

	node* isdirExist(string input)
	{
		string name = getSubStrAftrSpaceN(input, 1);

		for (node* n : subFolders)
		{
			if (n->name == name)
				return n;
		}
		return nullptr;
	}
	File* isFileExist(string input)
	{
		string name = getSubStrAftrSpaceN(input, 1);

		for (File* n : files)
		{
			if (n->name == name)
				return n;
		}
		return nullptr;
	}

	bool ismkdir(string input)
	{
		string command = lowerCase(input.substr(0, 5));
		if (command == "mkdir")
			return true;

		return false;
	}
	void mkdir(string input)
	{

		string subFolderName = input.substr(6, input.length());
		node* subFolder = new node(subFolderName, false);
		subFolder->parent = this;

		this->subFolders.push_back(subFolder);
	}

	bool isrmdir(string input)
	{
		string command = lowerCase(input.substr(0, 5));
		if (command == "rmdir")
			return true;

		return false;
	}
	void rmdir(string input)
	{

		string subFolderName = input.substr(6, input.length());
		node* subFolder = new node(subFolderName, false);
		subFolder->parent = this;

		this->subFolders.push_back(subFolder);
	}

	bool isChangeDir(string input)
	{
		string cd = lowerCase(input.substr(0, 2));
		if (cd == "cd")
		{
			return true;
		}
		return false;
	}
	node* changeDir(string input)
	{
		string chDir = input.substr(3, input.length());
		node* curr = isdirExist(input);
		return curr;
	}

	bool isPrevDir(string input)
	{
		input = lowerCase(input);
		if (input == "cd..")
			return true;
		return false;
	}
	node* prevDir()
	{
		node* curr = this->parent;
		return curr;
	}

	bool isCurrDir(string input)
	{
		input = lowerCase(input);
		if (input == "cd.")
			return true;
		return false;
	}

	bool isGotoRoot(string input)
	{
		input = lowerCase(input);
		if (input == "cd\\")
			return true;
		return false;
	}
	node* getRoot()
	{
		node* itr = this;
		while (itr->parent != nullptr)
		{
			itr = itr->parent;
		}
		return itr;
	}

	bool isCreate(string input)
	{
		string command = lowerCase(input.substr(0, 6));
		if (command == "create")
		{
			return true;
		}
		return false;
	}
	bool isExtensionValid(string input)
	{
		string name = getSubStrAftrSpaceN(input,1);
		if (name.length() >= 4 && name.substr(name.length() - 4) == ".txt")
			return true;
		if(name.length() >= 5 && name.substr(name.length() - 5) == ".text")
			return true;

		return false;
	}
	File* createFile(string input)
	{
		string name = getSubStrAftrSpaceN(input,1);
		File* newFile = new File(name,false);
		files.push_back(newFile);
		return newFile;
	}

	bool isDel(string input)
	{
		string command = lowerCase(input.substr(0, 3));
		if (command == "del")
		{
			return true;
		}
		return false;
	}
	bool del(string input)
	{
		bool del = false;
		string name = getSubStrAftrSpaceN(input,1);
		for (auto itr = files.begin(); itr != files.end();itr++)
		{
			if ((*itr)->name == name)
			{
				itr = files.erase(itr);
				del = true;
				if (files.size() == 0)
					break;
			}
		}
		return del;
	}
	
	bool isConvert(string input)
	{
		string command = lowerCase(input.substr(0, 7));
		if (command == "convert")
		{
			return true;
		}
		return false;
	}

	bool isExit(string input)
	{
		input = lowerCase(input);
		if (input == "exit")
			return true;
		return false;
	}
	void convert(string input)
	{
		string ext1 = getSubStrAftrSpaceN(input, 1);
		string ext2 = getSubStrAftrSpaceN(input, 2);

		if (isExtensionValid(ext1) && isExtensionValid(ext2))
		{
			changeExtension(ext1, ext2);
		}
		else
			cout << "Invalid Extension" << endl;

	}
	void changeExtension(string ext1, string ext2)
	{
		list<File*>::iterator i;
		for (i = files.begin(); i != files.end(); i++)
		{
			if ((*i)->getExtension() == ext1)
			{
				int dotPos = findDotPosition((*i)->name);
				(*i)->name.replace(dotPos,ext1.length(), ext2);
				(*i)->type = ext2;
			}
			else if ((*i)->getExtension() == ext2)
			{
				int dotPos = findDotPosition((*i)->name);
				(*i)->name.replace(dotPos, ext2.length(), ext1);
				(*i)->type = ext1;
			}
		}
	}

	bool isDir(string input)
	{
		input = lowerCase(input);
		if (input == "dir")
			return true;
		return false;
	}
	void dir()
	{
		printSubFolders();
		printFiles();
		cout << endl << "\t\t" << files.size() << " File(s)" << endl;
		cout << "\t\t" << subFolders.size() << " Dir(s)" << endl << endl;

	}
	void printSubFolders()
	{
		list<node*>::iterator i;
		cout << "Directory of " << getPath() << "\n\n";
		for (i = subFolders.begin(); i != subFolders.end(); i++)
		{
			cout << left << setw(30) << (*i)->creationTime << "<DIR>      " << (*i)->name << endl;
		}
	}
	void printFiles()
	{
		list<File*>::iterator i;
		for (i = files.begin(); i != files.end(); i++)
		{
			cout << left << setw(30) << (*i)->creationTime << "<File>     " << (*i)->name /* << (*i)->type*/ << endl;
		}
	}

	string lowerCase(string input)
	{
		string lower = "";
		for (char c : input)
			lower += tolower(c);
		return lower;
	}
	string getSubStrAftrSpaceN(string input, int numSpaces) {
		numSpaces--;
		int spacePos = input.find(' ');

		// Iterate until we find the desired number of spaces
		for (int i = 0; i < numSpaces; ++i) {
			if (spacePos == string::npos)
				return input.substr(spacePos,input.length()-1);  // Return an empty string if there are fewer spaces than expected
			spacePos = input.find(' ', spacePos + 1);
		}

		// If we found the desired number of spaces, return the substring after the last one
		if (spacePos != string::npos)
		{
			// Find the position of the second space after the first one
			int secondSpacePos = input.find(' ', spacePos + 1);
			if (secondSpacePos != string::npos)
				return input.substr(spacePos + 1, secondSpacePos - spacePos - 1);
			else
				return input.substr(spacePos + 1);
		}
		else
			return input;
	}
	int findDotPosition(const std::string& input) {
		int dotPos = input.find('.');

		// Check if a dot is found
		if (dotPos != std::string::npos) {
			return static_cast<int>(dotPos);  // Convert size_t to int for consistency
		}
		else {
			return -1;  // Return -1 to indicate that the dot is not found
		}
	}
};