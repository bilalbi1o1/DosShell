#include <iostream>
#include "node.h"
#include <conio.h>
#include <sstream>

void profile()
{
	cout << "						Bilal Ahmad  2022-CS-170  Data Structures and Algorithms" << endl;
}

int main()
{
	profile();
	string input;
	string command;
	string source;
	string destination;
	node rootFolder = node("V:",nullptr, false);
	node* currFolder = &rootFolder;
	string commandPrompt = ">";

	while (true)
	{
		cout << currFolder->getPath() << commandPrompt;
		getline(cin, command);
		command = currFolder->removeLeadingSpaces(command);

		if (currFolder->isAttrib(command))
		{
			if(!currFolder->attrib(command))
				cout << "The system can not find the directory specified." << endl;

				continue;
		}
		else if (currFolder->ismkdir(command))
		{
			if (currFolder->isdirExist(command))
			{
				cout << "A subdirectory " << currFolder->getSubStrAftrSpaceN(command, 1) << " already exists."<<endl <<endl;
				continue;
			}
			currFolder->mkdir(command);
		}
		else if (currFolder->isrmdir(command))
		{
			if (!currFolder->isdirExist(command))
			{
				cout << "The system can not find the file specified." << endl << endl;
				continue;
			}
			currFolder->rmdir(command);
		}
		else if (currFolder->isFind(command))
		{
			if (currFolder->Find(currFolder->getSubStrAftrSpaceN(command, 1)))
			{
				cout << "File found.";
			}
			else
				cout << "The system can not find the file specified.";
		}
		else if (currFolder->isCopy(command))
		{
			cout << currFolder->copy(command)<<endl<<endl;
			continue;
		}
		else if (currFolder->isChangeDir(command) && currFolder->isdirExist(command))
		{
			currFolder = currFolder->changeDir(command);
			continue;
		}
		else if (currFolder->isPrevDir(command))
		{
			currFolder = currFolder->prevDir();
			continue;
		}
		else if (currFolder->isDir(command))
		{
			currFolder->dir();
			continue;
		}
		else if (currFolder->isCreate(command))
		{
			if (currFolder->isFileExist(command))
				cout << "File already exist" << endl;
			else if (!currFolder->isExtensionValid(command))
				cout << "Invalid File extension" << endl;
			else
				currFolder->createFile(command);

			continue;
		}
		else if (currFolder->isEdit(command))
		{
			currFolder->edit(command);
		}
		else if (currFolder->isRename(command))
		{
			cout << currFolder->renameFile(command) << endl << endl;
			continue;
		}
		else if (currFolder->isDel(command))
		{
			bool isDeleted = currFolder->del(command);
			if (!isDeleted)
				cout << "could not found " << currFolder->getPath() << command.substr(4) << endl << endl;

			continue;
		}
		else if (currFolder->isConvert(command))
		{
			currFolder->convert(command);
		}
		else if (currFolder->isGotoRoot(command))
		{
			currFolder = currFolder->getRoot();
			continue;
		}
		else if (currFolder->isCurrDir(command))
		{
			cout << endl;
			continue;
		}
		else if (currFolder->isFormat(command))
		{
			if (currFolder->isdirExist(command))
			{
				cout << currFolder->Format(command) << endl << endl;
				continue;
		}
			cout << "The system can not find the directory specified"<<endl<<endl;
		}
		else if (currFolder->isTree(command))
		{
			currFolder->Tree(currFolder,0);
		}
		else if (currFolder->isPrompt(command))
		{
			char val;
			val = currFolder->prompt(command);
			if (val != NULL)
				commandPrompt = val;
			else
				cout << "Invalid choice." << endl << endl;
		}
		else if (currFolder->isExit(command))
		{
			break;
		}
		else if (currFolder->isHelp(command))
		{
			currFolder->help();
		}
		else if (currFolder->isCls(command))
		{
			currFolder->clearScreen();
		}
		else if (currFolder->isVer(command))
		{
			currFolder->ver();
		}
		else if (currFolder->isPprint(command))
		{
			File* file = currFolder->addToPriorityQueue(command);
			if (!file)
			{
				cout << "The system can not Find the file specified"<<endl<<endl;
				continue;
			}
			cout << "Priority Print Queue : "<<endl;
			currFolder->pprint();
			cout << "File " << "\'" << file->name << "\'" << " added to the priority print queue."<<endl << endl;

		}
		else if (currFolder->isPrint(command))
		{
			File* file = currFolder->addToQueue(command);
			if (!file)
			{
				cout << "The system can not Find the file specified" << endl << endl;
				continue;
			}
			cout << "Print Queue : " << endl;
			currFolder->print();
			cout << "File " << "\'" << file->name << "\'" << " added to the print queue." << endl << endl;
		}
		else if (currFolder->isQueue(command))
		{
			cout << "Print Queue : " << endl;
			currFolder->print();
			cout << endl << endl;
		}
		else if (currFolder->isPqueue(command))
		{
			cout << "Priority Print Queue : " << endl;
			currFolder->pprint();
			cout << endl << endl;
		}
		else if (currFolder->isSave(command))
		{
			currFolder->saveTree();
			currFolder->saveFiles();
		}
		else if (currFolder->isPwd(command))
		{
			currFolder->pwd(command);
		}
		else if (currFolder->isLoadTree(command))
		{
			currFolder->loadTree();
			currFolder->loadFiles();
		}
		else
		{
			cout << currFolder->getSubStrAftrSpaceN(command,0) << " is not recognized as an internal or external command," << endl;
			cout << "operable program or batch file." << endl << endl;
		}
	
	}
	return 0;
}