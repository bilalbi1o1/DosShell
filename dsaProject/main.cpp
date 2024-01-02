#include <iostream>
#include "node.h"
#include <conio.h>
#include <sstream>

void profile()
{
	cout << "						Bilal Ahmad  2022-CS-170  Data Structures and Algorithms" << endl;
}
string lowerCase(string input)
{
	string lower = "";
	for (char c : input)
		lower += tolower(c);
	return lower;
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

int main()
{
	profile();
	string input;
	string command;
	string source;
	string destination;
	node rootFolder = node("V:", nullptr, false);
	node* currFolder = &rootFolder;
	string commandPrompt = ">";

	while (true)
	{
		source = "";
		destination = "";
		cout << currFolder->getPath() << commandPrompt;
		getline(cin, input);
		if (input == "")
			continue;
		input = currFolder->removeLeadingSpaces(input);
		stringstream strStream(input);
		strStream >> command;
		command = lowerCase(command);
		strStream >> source;
		strStream >> destination;

		if (command == "attrib")
		{
			if (source == "")
			{
				cout << "The syntax of the command is incorrect" << endl << endl;
				continue;
			}
			if (!currFolder->attrib(input))
				cout << "The system can not find the file specified." << endl << endl;

			continue;
		}
		else if (command == "mkdir")
		{
			if (source == "")
			{
				cout << "The syntax of the command is incorrect" << endl << endl;
				continue;
			}
			if (currFolder->isdirExist(input))
			{
				cout << "A subdirectory " << currFolder->getStrAfterSpaceN(input, 1) << " already exists." << endl << endl;
				continue;
			}
			currFolder->mkdir(input);
		}
		else if (command == "rmdir")
		{
			if (source == "")
			{
				cout << "The syntax of the command is incorrect" << endl << endl;
				continue;
			}
			if (!currFolder->isdirExist(input))
			{
				cout << "The system can not find the directory specified." << endl << endl;
				continue;
			}
			currFolder->rmdir(input);
		}
		else if (command == "find")
		{
			if (source == "")
			{
				cout << "The syntax of the command is incorrect" << endl << endl;
				continue;
			}
			if (currFolder->Find(currFolder->getStrAfterSpaceN(input, 1)))
			{
				cout << "File found." << endl << endl;
			}
			else
				cout << "The system can not find the file specified." << endl << endl;
		}
		else if (command == "findf")
		{
			if (source == "")
			{
				cout << "The syntax of the command is incorrect" << endl << endl;
				continue;
			}
			File* file = currFolder->Find(currFolder->getStrAfterSpaceN(input, 1));
			if (!file)
			{
				cout << "The system can not find the file specified." << endl << endl;
				continue;
			}

			cout << "Enter string you want to search in the provided file : ";
			getline(cin, input);

			bool isFound = currFolder->findF(file->name, input);
			if (isFound)
				cout << "provided String found";
			else
				cout << "The system can not find the provided string in the file specified";
			cout << endl << endl;

		}
		else if (command == "findstr")
		{
			cout << "Enter string you want to search in the current directory : ";
			getline(cin, input);

			cout << currFolder->findstr(input) << endl;
		}
		else if (command == "copy")
		{
			if (source == "" || destination == "")
			{
				cout << "The syntax of the command is incorrect" << endl << endl;
				continue;
			}

			cout << currFolder->copy(input) << endl << endl;
		}
		else if (command == "move")
		{
			if (source == "" || destination == "")
			{
				cout << "The syntax of the command is incorrect" << endl << endl;
				continue;
			}

			cout << currFolder->move(input) << endl << endl;
		}
		else if (command == "cd")
		{
			if (currFolder->isdirExist(input))
			{
				currFolder = currFolder->changeDir(input);
				continue;
			}
			cout << "The system can not find the directory specified" << endl << endl;
		}
		else if (command == "cd..")
		{
			currFolder = currFolder->prevDir();
			continue;
		}
		else if (command == "dir")
		{
			currFolder->dir();
			continue;
		}
		else if (command == "create")
		{
			if (source == "")
			{
				cout << "The syntax of the command is incorrect" << endl << endl;
				continue;
			}

			if (currFolder->isFileExist(input))
				cout << "File already exist" << endl;
			else if (!currFolder->isExtensionValid(input))
				cout << "Invalid File extension" << endl;
			else
				currFolder->createFile(input);
		}
		else if (command == "edit")
		{
			if (source == "")
			{
				cout << "The syntax of the command is incorrect" << endl << endl;
				continue;
			}
			currFolder->edit(input);
			cout << endl << endl;
		}
		else if (command == "rename")
		{
			if (source == "" || destination == "")
			{
				cout << "The syntax of the command is incorrect" << endl << endl;
				continue;
			}

			cout << currFolder->renameFile(input) << endl << endl;
		}
		else if (command == "del")
		{
			if (source == "")
			{
				cout << "The syntax of the command is incorrect" << endl << endl;
				continue;
			}
			bool isDeleted = currFolder->del(input);
			if (!isDeleted)
				cout << "could not found " << currFolder->getPath() << input.substr(4) << endl << endl;

		}
		else if (command == "convert")
		{
			if (source == "" || destination == "")
			{
				cout << "The syntax of the command is incorrect" << endl << endl;
				continue;
			}
			currFolder->convert(source, destination);
		}
		else if (command == "cd\\")
		{
			currFolder = currFolder->getRoot();
			continue;
		}
		else if (command == "cd.")
		{
			cout << endl;
			continue;
		}
		else if (command == "format")
		{
			if (currFolder->isdirExist(input))
			{
				cout << currFolder->Format(input) << endl << endl;
				continue;
			}
			cout << "The system can not find the directory specified" << endl << endl;
		}
		else if (command == "tree")
		{
			currFolder->Tree(currFolder, 0);
		}
		else if (command == "prompt")
		{
			char val;
			val = currFolder->prompt(command, source);
			if (val != NULL)
				commandPrompt = val;
			else
				cout << "Invalid choice." << endl << endl;
		}
		else if (command == "exit")
		{
			break;
		}
		else if (command == "help")
		{
			currFolder->help();
		}
		else if (command == "cls")
		{
			currFolder->clearScreen();
		}
		else if (command == "ver")
		{
			currFolder->ver();
		}
		else if (command == "pprint")
		{
			if (source == "")
			{
				cout << "The syntax of the command is incorrect" << endl << endl;
				continue;
			}
			File* file = currFolder->addToPriorityQueue(input);
			if (!file)
			{
				cout << "The system can not Find the file specified" << endl << endl;
				continue;
			}
			cout << "Priority Print Queue : " << endl;
			currFolder->pprint();
			cout << "File " << "\'" << file->name << "\'" << " added to the priority print queue." << endl << endl;

		}
		else if (command == "print")
		{
			if (source == "")
			{
				cout << "The syntax of the command is incorrect" << endl << endl;
				continue;
			}
			File* file = currFolder->addToQueue(input);
			if (!file)
			{
				cout << "The system can not Find the file specified" << endl << endl;
				continue;
			}
			cout << "Print Queue : " << endl;
			currFolder->print();
			cout << "File " << "\'" << file->name << "\'" << " added to the print queue." << endl << endl;
		}
		else if (command == "queue")
		{
			cout << "Print Queue : " << endl;
			currFolder->print();
			cout << endl << endl;
		}
		else if (command == "pqueue")
		{
			cout << "Priority Print Queue : " << endl;
			currFolder->pprint();
			cout << endl << endl;
		}
		else if (command == "save")
		{
			currFolder->saveTree();
			currFolder->saveFiles();
		}
		else if (command == "pwd")
		{
			currFolder->pwd();
		}
		else if (command == "loadtree")
		{
			currFolder->loadTree();
			currFolder->loadFiles();
		}
		else
		{
			cout << command << " is not recognized as an internal or external command," << endl;
			cout << "operable program or batch file." << endl << endl;
		}

	}
	return 0;
}