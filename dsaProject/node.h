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
	string getPath(File* file)
	{
		string path = file->parentFolder->getPath();
		path = path + '\\' + file->name;
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

	bool isAttrib(string input)
	{
		string command1 = lowerCase(getSubStrAftrSpaceN(input, 0));
		string command2 = lowerCase(getSubStrAftrSpaceN(input, 1));
		if (command1 == "attrib" && command1 == command2)
			return true;
		else if (command1 == "attrib" && input[6] == ' ')
			return true;
			
		return false;
	}
	bool attrib(string input)
	{
		string command1 = lowerCase(getSubStrAftrSpaceN(input, 0));
		string command2 = lowerCase(getSubStrAftrSpaceN(input, 1));
		if (command1 == command2)
		{
			printFoldersWithPath();
			printFilesWithPath();
			return true;
		}
		else if (node* folder = isdirExist(command2))
		{
			printFoldersWithPath(folder);
			printFilesWithPath(folder);
			return true;
		}
		return false;
	}

	void printFoldersWithPath()
	{
		for (auto folder : subFolders)
		{
			cout<< setw(10) << "" << folder->getPath() << endl;
		}
	}
	void printFilesWithPath()
	{
		for (auto file : files)
		{
			cout << setw(10)<< "" << getPath(file) << endl;
		}

	}
	void printFoldersWithPath(node* folderName)
	{
		for (auto folder : folderName->subFolders)
		{
			cout << setw(10)<<"" << folder->getPath() << endl;
		}
	}
	void printFilesWithPath(node* folderName)
	{
		for (auto file : folderName->files)
		{
			cout << setw(10) << "" << folderName->getPath(file) << endl;
		}

	}
	
	bool isCopy(string input)
	{
		string command = lowerCase(input.substr(0, 4));
		if (command == "copy")
			return true;

		return false;
	}
	string copy(string input)
	{
		string sourcePath = getSubStrAftrSpaceN(input, 1);
		string destinationPath = getSubStrAftrSpaceN(input, 2);
		node* source = isPathValid(sourcePath);
		node* destination = isPathValid(destinationPath);

		if (source != nullptr && destination != nullptr)
		{
			int count = specificCharCount(sourcePath, '\\');
			string fileName = getSubStrAftrNslashes(sourcePath, count);
			File* fileToBeCopied = Find(fileName, source);

			if (fileToBeCopied != nullptr)
			{
				File* file = new File(fileToBeCopied->name, false);

				destination->files.push_back(file);
				return "Copied.";
			}
			return "File with this name does not exist in the specified directory.";
		}
		else
			return "Invalid Path(s).";
	}

	bool isFind(string input)
	{
		string command = lowerCase(input.substr(0, 4));
		if (command == "find")
			return true;

		return false;
	}
	File* Find(string fileName)
	{
		for (auto itr = files.begin(); itr != files.end(); itr++)
		{
			if ((*itr)->name == fileName)
				return (*itr);
		}
		return nullptr;
	}
	File* Find(string fileName, node* folder)
	{
		for (auto itr = folder->files.begin(); itr != folder->files.end(); itr++)
		{
			if ((*itr)->name == fileName)
				return (*itr);
		}
		return nullptr;
	}
	node* isPathValid(string path)
	{
		node* curr = getRoot();
		int numSlashes = specificCharCount(path, '\\');
		int count = 1;

		while (numSlashes)
		{
			string folderName = getSubStrAftrNslashes(path, count);
			for (auto itr = curr->subFolders.begin();itr != curr->subFolders.end();itr++)
			{
				if ((*itr)->name == folderName)
				{
					curr = (*itr);
					count++;
					break;
				}
			}
			numSlashes--;
		}
		if (count == specificCharCount(path, '\\'))
			return curr;

		return nullptr;
	}

	bool isTree(string input)
	{
		string command = lowerCase(input);
		if (command == "tree")
			return true;

		return false;
	}
	void Tree(node* currFolder, int depth)
	{
		if (currFolder == nullptr)
			return;

		cout << setw(depth * 4) << "" << currFolder->name << endl;
		for (auto subfolder : currFolder->subFolders)
		{
			Tree(subfolder, depth + 1);
		}
		for (auto file : currFolder->files)
		{
			cout << setw((depth + 1) * 4) << "" << file->name << endl;
		}
	}

	bool isPrompt(string input)
	{
		string command = lowerCase(input.substr(0, 6));
		int length = input.length();
		if (command == "prompt")
		{
			if (length == 6)
				return true;
			if (length > 7 && input[6] == ' ')
				return true;
		}

		return false;
	}
	char prompt(string input)
	{
		string command = lowerCase(getSubStrAftrSpaceN(input, 1));
		if (command == "prompt")
			return '>';
		else if (command.length() == 1)
		{
			char cmdPrompt = command[0];
			if (!(cmdPrompt >= 'a' && cmdPrompt <= 'z' || cmdPrompt >= '0' && cmdPrompt <= '9'))
				return cmdPrompt;
		}
		return NULL;
	}

	bool isFormat(string input)
	{
		string command = lowerCase(input.substr(0, 6));
		if (command == "format")
			return true;

		return false;
	}
	string Format(string input)
	{
		node* curr = isdirExist(input);

		if (curr == nullptr)
			return "subdirectory with the given name does not exist";

		for (auto itr = curr->subFolders.begin(); itr != curr->subFolders.end();itr++)
		{
			curr->subFolders.erase(itr);
			if (curr->subFolders.size() == 0)
				break;
		}
		for (auto itr = curr->files.begin(); itr != curr->files.end();itr++)
		{
			curr->files.erase(itr);
			if (curr->files.size() == 0)
				break;
		}
		return "Formatted";
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
		for (auto i = subFolders.begin(); i != subFolders.end();i++)
		{
			if ((*i)->name == subFolderName)
			{
				subFolders.erase(i);
				break;
			}
		}
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
		string name = getSubStrAftrSpaceN(input, 1);
		if (name.length() >= 4 && name.substr(name.length() - 4) == ".txt")
			return true;
		if (name.length() >= 5 && name.substr(name.length() - 5) == ".text")
			return true;

		return false;
	}
	File* createFile(string input)
	{
		string name = getSubStrAftrSpaceN(input, 1);
		File* newFile = new File(name, false);
		newFile->parentFolder = this;
		files.push_back(newFile);
		return newFile;
	}

	bool isRename(string input)
	{
		string command = lowerCase(input.substr(0, 6));
		int spaceCount = specificCharCount(input, ' ');
		if (command == "rename" && spaceCount == 2)
		{
			return true;
		}
		return false;
	}
	string renameFile(string input)
	{
		string oldName = getSubStrAftrSpaceN(input, 1);
		string newName = getSubStrAftrSpaceN(input, 2);

		File* file = Find(oldName);
		File* file2 = Find(newName);
		bool isExtValid = isExtensionValid(newName);

		if (file == nullptr)
				return "The system can not find the file specified.";
		else if (!isExtValid)
				return "Extension of new file is not valid.";
		else if (file2 != nullptr)
				return "File with this name already exist.";

			file->name = newName;
			
			return "renamed successfully";

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
		string name = getSubStrAftrSpaceN(input, 1);
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
				(*i)->name.replace(dotPos, ext1.length(), ext2);
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

	bool isHelp(string input)
	{
		string command = lowerCase(input);
		if (input == "help")
			return true;

		return false;
	}
	void help()
	{
		system("cls");
		cout << "COMMAND                                            DESCRIPTION" << endl<< endl<< endl;
		cout << "ATTRIB  "
			<< "                "
			<< "Displays Files and Folders of Current or Provided Folder." << endl;
		cout << "CD      "
			<< "                "
			<< "Changes the current directory " << endl;
		cout << "CD.     "
			<< "                "
			<< "Prints working directory (i.e. your current directory in your tree structure) " << endl;
		cout << "CD..    "
			<< "                "
			<< "Change directory to previous directory " << endl;
		cout << "CD\\    "
			<< "                "
			<< " Changes directory to root directory (i.e. V:\\>)" << endl;
		cout << "CONVERT "
			<< "                "
			<< "Asks two types and converts extension of all files of one type to another type." << endl;
		cout << "COPY    "
			<< "                "
			<< "Copies one file in the current directory to another location (directory). See details below." << endl;
		cout << "CREATE  "
			<< "                "
			<< "Creates a file with the name provided and allows the user to enter contents of the file." << endl;
		cout << "DEL     "
			<< "                "
			<< "Delete a file whose name is provided in input." << endl;
		cout << "DIR     "
			<< "                "
			<< "Displays a list of files and subdirectories in a directory." << endl;
		cout << "EDIT    "
			<< "                "
			<< "Opens a file (loads into a linked list) and allows the user to edit and save the contents of the file." << endl;
		cout << "EXIT    "
			<< "                "
			<< "Quits the program" << endl;
		cout << "FIND    "
			<< "                "
			<< "Searches for a file in your current virtual directory whose name is provided as input." << endl;
		cout << "FINDF   "
			<< "                "
			<< "Searches for a text string in the currently open file or the file whose name is provided as input." << endl;
		cout << "FINDSTR "
			<< "                "
			<< "Searches for strings in all files of your current virtual directory, prints names of files with the string" << endl;
		cout << "FORMAT  "
			<< "                "
			<< "Formats the current virtual directory i.e. empties the current directory and all subdirectories." << endl;
		cout << "HELP    "
			<< "                "
			<< "Provides Help information for all these commands." << endl;
		cout << "LOADTREE"
			<< "                "
			<< "Load a given tree in your tree structure. Given tree is in a file named tree.txt	" << endl;
		cout << "MKDIR   "
			<< "                "
			<< "Creates a virtual directory." << endl;
		cout << "MOVE    "
			<< "                "
			<< "Moves one file (whose name is provided as input) from one directory to another               directory." << endl;
		cout << "PPRINT  "
			<< "                "
			<< "Adds a text file to the priority based print queue, and displays the current priority queue. " << endl;
		cout << "PROMPT  "
			<< "                "
			<< "Changes the Windows command prompt (for example from V:\\> to V$)." << endl;
		cout << "PRINT   "
			<< "                "
			<< "Adds a text file to the print queue, and displays the current queue. " << endl;
		cout << "PQUEUE  "
			<< "                "
			<< "Shows current state of the priority based print queue, with time left for each element" << endl;
		cout << "PWD     "
			<< "                "
			<< "Prints working directory" << endl;
		cout << "QUEUE   "
			<< "                "
			<< "Shows current state of the print queue, with time left for each element" << endl;
		cout << "RENAME  "
			<< "                "
			<< "Renames a file whose current and new name is provided as input." << endl;
		cout << "RMDIR   "
			<< "                "
			<< "Removes a directory along with its contents. Consider setting the respective sibling links " << endl;
		cout << "SAVE    "
			<< "                "
			<< "Saves the currently open file to disk.TREE Displays the complete directory structure.VER Displays the version of your program." << endl;
		cout << "TREE    "
			<< "                "
			<< "Displays the complete directory structure."<<endl;
		cout << "CLS     "
			<< "                "
			<< "Clears the screen." << endl;
		cout << "VER     "
			<< "                "
			<< "Displays the version of your program." <<endl <<endl;
	}

	bool isCls(string input)
	{
		string command = lowerCase(input);
		if (input == "cls")
			return true;

		return false;
	}
	void clearScreen()
	{
		system("cls");
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

		if (numSpaces < 0)
			return input.substr(0,spacePos);

		// Iterate until we find the desired number of spaces
		for (int i = 0; i < numSpaces; ++i) {
			if (spacePos == string::npos)
				return input.substr(spacePos, input.length() - 1);
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
	string getSubStrAftrNslashes(string input, int numSlashes) {
		numSlashes--;
		int spacePos = input.find('\\');

		// Iterate until we find the desired number of spaces
		for (int i = 0; i < numSlashes; ++i) {
			if (spacePos == string::npos)
				return input.substr(spacePos, input.length() - 1);  // Return an empty string if there are fewer spaces than expected
			spacePos = input.find('\\', spacePos + 1);
		}

		// If we found the desired number of spaces, return the substring after the last one
		if (spacePos != string::npos)
		{
			// Find the position of the second space after the first one
			int secondSpacePos = input.find('\\', spacePos + 1);
			if (secondSpacePos != string::npos)
				return input.substr(spacePos + 1, secondSpacePos - spacePos - 1);
			else
				return input.substr(spacePos + 1);
		}
		else
			return "";
	}
	int findDotPosition(string input) {
		int dotPos = input.find('.');

		// Check if a dot is found
		if (dotPos != std::string::npos) {
			return dotPos;  // Convert size_t to int for consistency
		}
		else {
			return -1;  // Return -1 to indicate that the dot is not found
		}
	}
	int specificCharCount(string input, char character) {
		int count = 0;
		for (char ch : input) {
			if (ch == character) {
				count++;
			}
		}
		return count;
	}
};