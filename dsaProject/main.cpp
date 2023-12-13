#include <iostream>
#include "node.h"
#include<conio.h>
#include<Windows.h>

void profile()
{
	cout << "						Bilal Ahmad  2022-CS-170  Data Structures and Algorithms" << endl;
}

int main()
{
	profile();
	string command;
	node rootFolder = node("V:", false);
	node* currFolder = &rootFolder;
	string commandPrompt = ">";

	while (true)
	{
		cout << currFolder->getPath() << commandPrompt;
		getline(cin, command);

		if (currFolder->ismkdir(command))
		{
			if (currFolder->isdirExist(command))
			{
				cout << "A subdirectory " << currFolder->getSubStrAftrSpaceN(command, 1) << " already exists."<<endl <<endl;
				continue;
			}
			currFolder->mkdir(command);
		}
		if (currFolder->isrmdir(command))
		{
			if (!currFolder->isdirExist(command))
			{
				cout << "The system can not find the file specified." << endl << endl;
				continue;
			}
			currFolder->rmdir(command);
		}
		if (currFolder->isChangeDir(command) && currFolder->isdirExist(command))
		{
			currFolder = currFolder->changeDir(command);
			continue;
		}
		if (currFolder->isPrevDir(command))
		{
			currFolder = currFolder->prevDir();
			continue;
		}
		if (currFolder->isDir(command))
		{
			currFolder->dir();
			continue;
		}
		if (currFolder->isCreate(command))
		{
			if (currFolder->isFileExist(command))
				cout << "File already exist" << endl;
			else if (!currFolder->isExtensionValid(command))
				cout << "Invalid File extension" << endl;
			else
				currFolder->createFile(command);

			continue;
		}
		if (currFolder->isDel(command))
		{
			bool isDeleted = currFolder->del(command);
			if (!isDeleted)
				cout << "could not fount " << currFolder->getPath() << command.substr(4) << endl << endl;

			continue;
		}
		if (currFolder->isConvert(command))
		{
			currFolder->convert(command);
		}
		if (currFolder->isGotoRoot(command))
		{
			currFolder = currFolder->getRoot();
			continue;
		}
		if (currFolder->isCurrDir(command))
		{
			cout << endl;
			continue;
		}
		if (currFolder->isExit(command))
		{
			break;
		}
	}
	return 0;
}