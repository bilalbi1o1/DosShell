#pragma once
#include <fstream>
#include "File.h"
#include <list>
#include <cctype>
#include <stack>
#include "textEditor.h"
#include <queue>
#include <tuple>

queue<File*> pq;
queue<File*> que;

struct FilePriorityComparator
{
	bool operator()(File* file1, File* file2)
	{
		return file1->priorityValue() < file2->priorityValue();
	}
};

class node
{
	friend class textEditor;
public:
	string name;
	string creationTime;
	string owner;
	bool readOnly;

	node* parent;
	list<node*> subFolders;
	list<File*> files;

	node(string foldername, node* parentFolder, bool isReadOnly)
	{
		name = foldername;
		creationTime = File::setCreationTime();
		owner = "Bilal";
		parent = parentFolder;
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
		string name = getStrAfterSpaceN(input, 1);

		for (node* n : subFolders)
		{
			if (n->name == name)
				return n;
		}
		return nullptr;
	}
	bool isFileExist(string input)
	{
		string name = getStrAfterSpaceN(input, 1);

		bool isExist = Find(name);
		return isExist;
	}

	bool attrib(string input)
	{
		string fileName = lowerCase(getStrAfterSpaceN(input, 1));
		
		if (File* file = Find(fileName))
		{
			cout << "              Details of the given file are  " << endl << endl;
			cout << "	Name            : " << file->name << endl;
			cout << "	Owner           : " << file->owner << endl;
			cout << "	Read Only        : " << file->readOnly << endl;
			cout << "	Creation Time      : " << file->creationTime << endl;
			cout << "	Number of Lines     : " << file->numberOfLines << endl;
			cout << "	Number of Characters   : " << file->numberOfCharacters << endl;
			cout << "	Avg num of char per line : " << file->avgNumOfCharPerLine << endl;
			cout << "	Priority Level      : " << file->priorityString() << endl;
			cout << "	Time takes to Print    : " << file->timeTakesForPrint << "seconds" << endl << endl;
			return true;
		}
		return false;
	}

	void printFoldersWithPath()
	{
		for (auto folder : subFolders)
		{
			cout << setw(10) << "" << folder->getPath() << endl;
		}
	}
	void printFilesWithPath()
	{
		for (auto file : files)
		{
			cout << setw(10) << "" << getPath(file) << endl;
		}

	}
	void printFoldersWithPath(node* folderName)
	{
		for (auto folder : folderName->subFolders)
		{
			cout << setw(10) << "" << folder->getPath() << endl;
		}
	}
	void printFilesWithPath(node* folderName)
	{
		for (auto file : folderName->files)
		{
			cout << setw(10) << "" << folderName->getPath(file) << endl;
		}

	}

	string copy(string input)
	{
		string sourceDestinationPath = getStrAfterSpaceN(input, 1);
		tuple<string, string > paths = getSourceDestinationPaths(sourceDestinationPath);
		string sourcePath = get<0>(paths);
		string destinationPath = get<1>(paths);
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
	string move(string input)
	{
		string sourceDestinationPath = getStrAfterSpaceN(input, 1);
		tuple<string, string > paths = getSourceDestinationPaths(sourceDestinationPath);
		string sourcePath = get<0>(paths);
		string destinationPath = get<1>(paths);
		node* source = isPathValid(sourcePath);
		node* destination = isPathValid(destinationPath);

		if (source != nullptr && destination != nullptr)
		{
			int count = specificCharCount(sourcePath, '\\');
			string fileName = getSubStrAftrNslashes(sourcePath, count);
			File* isFileCopied = Find(fileName, source);

			if (isFileCopied != nullptr)
			{
				File* file = new File(isFileCopied->name, false);
				destination->files.push_back(file);

				source->del(isFileCopied->name);
				return "File moved.";
			}
			return "File with this name does not exist in the specified directory.";
		}
		else
			return "Invalid Path(s).";
	}

	void loadTree()
	{
		ifstream file("tree.txt");
		if (file.is_open())
		{
			string s;
			while (getline(file, s))
			{
				stringstream path = stringstream(s);
				string folder;
				node* currFolder = getRoot();
				int count = 0;
				for (char c : s)
				{
					if (c == '\\')
						count++;
				}
				for (int i = 0; i < count; i++)
				{
					getline(path, folder, '\\');
					currFolder = currFolder->findFolder(folder);
				}
				getline(path, folder, '\\');
				node* newFolder = new node(folder, currFolder, "");
				currFolder->subFolders.push_back(newFolder);
			}
		}
		file.close();
	}
	void loadFiles()
	{
		ifstream file("fileNames.txt");
		string s;
		while (getline(file, s))
		{
			stringstream path = stringstream(s);
			string folder;
			node* currFolder = getRoot();
			int count = 0;
			for (char c : s)
			{
				if (c == '\\')
					count++;
			}
			for (int i = 0; i < count; i++)
			{
				getline(path, folder, '\\');
				currFolder = currFolder->findFolder(folder);
			}
			getline(path, folder, '\\');
			File* file = new File(folder, false);
			currFolder->files.push_back(file);
		}
	}
	node* findFolder(string name)
	{
		list<node*>::iterator i;
		for (i = subFolders.begin(); i != subFolders.end(); i++)
		{
			if ((*i)->name == name)
			{
				return (*i);
			}
		}
		return nullptr;
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

	char prompt(string input,string sign)
	{
		string command = lowerCase(getStrAfterSpaceN(input, 1));
		if (command == "prompt" && sign == "")
			return '>';
		else if (sign.length() == 1)
		{
			char cmdPrompt = sign[0];
			if (!(cmdPrompt >= 'a' && cmdPrompt <= 'z' || cmdPrompt >= '0' && cmdPrompt <= '9'))
				return cmdPrompt;
		}
		return NULL;
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

	void mkdir(string input)
	{

		string subFolderName = input.substr(6, input.length());
		node* subFolder = new node(subFolderName, this, false);

		this->subFolders.push_back(subFolder);
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

	node* changeDir(string input)
	{
		string chDir = input.substr(3, input.length());
		node* curr = isdirExist(input);
		return curr;
	}

	node* prevDir()
	{
		node* curr = this->parent;
		return curr;
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

	bool isExtensionValid(string input)
	{
		string name = getStrAfterSpaceN(input, 1);
		if (name.length() >= 4 && name.substr(name.length() - 4) == ".txt")
			return true;
		if (name.length() >= 5 && name.substr(name.length() - 5) == ".text")
			return true;

		return false;
	}
	File* createFile(string input)
	{
		string name = getStrAfterSpaceN(input, 1);
		File* newFile = new File(name, false);
		newFile->parentFolder = this;
		files.push_back(newFile);

		return newFile;
	}

	bool findF(string& filename, string& searchString)
	{
		ifstream file(filename);

		if (!file.is_open()) {
			cout << "Error opening file: " << filename << endl;
			return false;
		}

		string line;
		while (getline(file, line)) {
			if (line.find(searchString) != string::npos) {
				file.close(); // Close the file before returning
				return true;  // String found
			}
		}

		file.close(); // Close the file before returning
		return false;  // String not found
	}
	string findstr(string& searchString)
	{
		string output = "";
		for (auto file : files)
		{
			if (findF(file->name, searchString))
			{
				output = "String found in file: " + file->name + "\n";
			}
		}
		if (output == "")
			output = "String not found in any file in the current directory.\n";

		return output;
	}

	void edit(string input)
	{
		string filename = getStrAfterSpaceN(input, 1);
		File* file = Find(filename);

		if (file == nullptr)
		{
			cout << "File does not exist.";
			return;
		}
		clearScreen();
		ifstream rdr(filename.c_str());
		textEditor fileEditor;
		fileEditor.openSavedFile(rdr);
		rdr.close();

		ofstream wrt(filename.c_str(), ios_base::out);
		fileEditor.editFile(wrt);
		clearScreen();
		system("color 07");
		wrt.close();
		file->setTimeToPrint();
		file->setaverageCharactersPerLine();
		file->setcountCharactersInFile();
		file->setcountLinesInFile();
	}

	File* addToPriorityQueue(string input)
	{
		string fileName = getStrAfterSpaceN(input, 1);
		File* file = Find(fileName);
		if (file == nullptr)
			return nullptr;

		if (pq.empty())
		{
			pq.push(file);
		}
		else
		{
			File* currPrintingFile = pq.front();
			pq.pop();

			priority_queue<File*, vector<File*>, FilePriorityComparator> priorityQ;
			priorityQ.push(file);
			while (!pq.empty())
			{
				priorityQ.push(pq.front());
				pq.pop();
			}
			pq.push(currPrintingFile);
			while (!priorityQ.empty())
			{
				pq.push(priorityQ.top());
				priorityQ.pop();
			}
		}
		return file;
	}
	void pprint()
	{
		queue<File*> temp = pq;
		while (!temp.empty())
		{
			cout << temp.front()->name << "     " << temp.front()->priorityString() << endl;
			temp.pop();
		}
	}

	File* addToQueue(string input)
	{
		string fileName = getStrAfterSpaceN(input, 1);
		File* file = Find(fileName);
		if (file == nullptr)
			return nullptr;

		que.push(file);

		return file;
	}
	void print()
	{
		queue<File*> temp = que;
		while (!temp.empty())
		{
			cout << temp.front()->name << "     " << endl;
			temp.pop();
		}
	}

	string renameFile(string input)
	{
		string combineNames = getStrAfterSpaceN(input, 1);
		tuple<string,string> names = getSourceDestinationPaths(combineNames);

		File* file = Find(get<0>(names));
		File* file2 = Find(get<1>(names));
		bool isExtValid = isExtensionValid(get<1>(names));

		if (file == nullptr)
			return "The system can not find the file specified.";
		else if (!isExtValid)
			return "Extension of new file is not valid.";
		else if (file2 != nullptr)
			return "File with this name already exist.";

		file->name = get<1>(names);

		return "renamed successfully";

	}

	bool del(string input)
	{
		bool del = false;
		string name = getStrAfterSpaceN(input, 1);
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

	void convert(string ext1,string ext2)
	{
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

	void help()
	{
		system("cls");
		cout << "COMMAND                                            DESCRIPTION" << endl << endl << endl;
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
			<< "Displays the complete directory structure." << endl;
		cout << "CLS     "
			<< "                "
			<< "Clears the screen." << endl;
		cout << "VER     "
			<< "                "
			<< "Displays the version of your program." << endl << endl;
	}

	void clearScreen()
	{
		system("cls");
	}

	void ver()
	{
		cout << "Microsoft Windows [Version 10.0.19045.3803]" << endl << endl;
	}

	void saveFolderPaths(node* folder, queue<string>& q)
	{
		if (folder == nullptr)
			return;
		q.push(folder->getPath());
		for (auto i : folder->subFolders)
		{
			saveFolderPaths(i, q);
		}
	}
	void saveFilePaths(node* folder, queue<string>& q)
	{
		if (folder == nullptr)
			return;
		for (auto i : folder->files)
		{
			q.push(folder->getPath(i));
		}
		for (auto i : folder->subFolders)
		{
			saveFilePaths(i, q);
		}
	}
	void saveTree()
	{
		queue<string> q;
		saveFolderPaths(getRoot(), q);
		ofstream file("tree.txt");
		while (!q.empty())
		{
			string i = q.front();
			q.pop();
			if (i.length() < 3)
				continue;
			i = i.substr(3);
			file << i << endl;
		}
		file.close();
	}
	void saveFiles()
	{
		queue<string> q;
		saveFilePaths(getRoot(), q);
		ofstream file("fileNames.txt");
		while (!q.empty())
		{
			string i = q.front();
			q.pop();
			if (i.length() < 3)
				continue;
			i = i.substr(3);
			file << i << endl;
		}
		file.close();
	}

	void pwd()
	{
		cout << "Current Working directory is :" << this->name << endl << endl;
	}

	string lowerCase(string input)
	{
		string lower = "";
		for (char c : input)
			lower += tolower(c);
		return lower;
	}
	string getStrAfterSpaceN(string& input, int numSpaces) {
		numSpaces--;
		int spacePos = input.find(' ');

		// If no space found, or numSpaces is negative, return the original string
		if (spacePos == string::npos || numSpaces < 0)
			return input;

		// Iterate until we find the desired number of spaces
		for (int i = 0; i < numSpaces; ++i)
			spacePos = input.find(' ', spacePos + 1);

		// Return the substring after the last space found
		return input.substr(spacePos + 1);
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
	string removeLeadingSpaces(string input) {
		string result;

		// Find the position of the first non-space character
		int firstNonSpace = input.find_first_not_of(' ');

		// Check if the string is not empty and contains spaces before the first character
		if (firstNonSpace != string::npos && firstNonSpace > 0) {
			// Extract the substring starting from the first non-space character
			result = input.substr(firstNonSpace);
		}
		else {
			// No leading spaces found, return the original string
			result = input;
		}

		return result;
	}
	tuple<string , string> getSourceDestinationPaths(string sourceDestinationPath)
	{
		tuple<string, string> paths;
		
		int txtPos = sourceDestinationPath.find(".txt");
		int textPos = sourceDestinationPath.find(".text");

		if (txtPos != string::npos && textPos != string::npos)
		{
			if (txtPos < textPos)
			{
				get<0>(paths) = sourceDestinationPath.substr(0,txtPos+4);
				get<1>(paths) = sourceDestinationPath.substr(txtPos + 5);
			}
			else
			{
				get<1>(paths) = sourceDestinationPath.substr(0, textPos + 5);
				get<0>(paths) = sourceDestinationPath.substr(textPos +6);
			}
		}
		else if (txtPos != string::npos && textPos == string::npos)
		{
				get<0>(paths) = sourceDestinationPath.substr(0, txtPos + 4);
				int txtPos2 = sourceDestinationPath.find(".txt",txtPos + 5);
				get<1>(paths) = sourceDestinationPath.substr(txtPos + 5);
		}
		else if (txtPos == string::npos && textPos != string::npos)
		{
			get<0>(paths) = sourceDestinationPath.substr(0, textPos + 5);
			int textPos2 = sourceDestinationPath.find(".txt", textPos + 6);
			get<1>(paths) = sourceDestinationPath.substr(textPos + 6);
		}
		return paths;
	}
};