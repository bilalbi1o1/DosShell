#pragma once
#include <fstream>
#include "File.h"
#include <list>
#include <queue>
#include <stack>
#include "node.h"
#include <conio.h>
#include<Windows.h>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

struct state
{
	int rows, cols;
	list<list<char>> text;
	list<char>::iterator colItr;
	list<list<char>>::iterator rowItr;
};
class textEditor
{
	friend class node;
public:
	int currRow, currCol;
	list<list<char>> text;
	list<char>::iterator colItr;
	list<list<char>>::iterator rowItr;
	deque<state> undo;
	stack<state> redo;

	textEditor()
	{
		text.push_back(list<char>());
		rowItr = text.begin();
		(*rowItr).push_back(' ');
		colItr = (*rowItr).begin();
		currRow = 0, currCol = 0;
	}
	state saveState()
	{
		state* st = new state;
		st->text.push_back(list<char>());
		auto rItr = st->text.begin();
		for (auto row = text.begin(); row != text.end(); row++)
		{
			st->text.push_back(list<char>());
			for (auto col = (*row).begin(); col != (*row).end(); col++)
			{
				char ch = *col;
				(*rItr).push_back(ch);
			}
			rItr++;
		}
		st->rowItr = st->text.begin();
		st->colItr = (*st->rowItr).begin();
		st->cols = currCol;
		st->rows = currRow;
		return *st;
	}
	void loadState(state st)
	{
		text = st.text;
		rowItr = text.begin();
		currCol = st.cols;
		currRow = st.rows;
		for (int r = 0; r < st.rows; r++)
			rowItr++;
		colItr = (*rowItr).begin();
		for (int c = 0; c < st.cols; c++)
			colItr++;
	}
	void updateUndo()
	{
		if (undo.size() > 5)
			undo.erase(undo.begin());

		state st = saveState();
		undo.push_back(st);
	}
	void gotoRowCol(int rowPos, int colPos)
	{
		COORD coordinates;
		coordinates.X = colPos;
		coordinates.Y = rowPos;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
	}

	/*void editFile(ofstream& wrt)
	{
		system("color F0");
		char ch;
		ch = _getch();
		gotoRowCol(currRow, currCol);
		cout << ch;
		(*colItr) = ch;
		currCol++;

		while (true)
		{
				gotoRowCol(currRow, currCol);

			ch = _getch();

			if (ch == -32)//if arrow keys or del is pressed
			{
				A:
				ch = _getch();

				if (ch == 72)//up
				{
					if (currRow == 0)
						continue;

					rowItr--;
					colItr = (*rowItr).begin();
					currRow--;currCol = 0;
					continue;
				}
				else if (ch == 80)//down
				{
						if (rowItr != --text.end())
							{
								rowItr++;
								colItr = (*rowItr).begin();
								currRow++; currCol = 0;
							}
					continue;
				}
				else if (ch == 75)//left
				{
					if (currCol - 1 > 0)
					{
						colItr--;
						currCol--;
					}
					continue;
				}
				else if (ch == 77)//right
				{
					if (currCol == 159 || colItr == (*rowItr).end())
						continue;

					colItr++;
					currCol++;
					continue;
				}
				else if (ch == 83)//del
				{
					auto temp = colItr;
					colItr++;
					(*rowItr).erase(colItr);
					colItr = temp;
					system("cls");
					print();
					updateUndo();
				}
			}
			else if (ch == 13)//Enter
			{
				text.push_back(list<char>());
				rowItr++;

				currRow++;
				currCol = 0;
				gotoRowCol(currRow,currCol);
				ch = _getch();
				if (ch == -32)//if arrow key of del pressed
					goto A;
				(*rowItr).push_back(ch);
				colItr = (*rowItr).begin();
				system("cls");
				print();
				updateUndo();
				continue;
			}
			else if (ch == 8)//back space
			{
				if (currCol == 0)
						continue;

				if (colItr != (*rowItr).begin())
				{
					auto temp = --colItr;
					colItr++;
					(*rowItr).erase(colItr);
					colItr = temp;
					currCol--;
				}
				else
				{
					(*rowItr).erase(colItr);
					currCol--;
				}
				system("cls");
				print();
				updateUndo();
				continue;
			}
			else if (ch == 26) // undo
			{
				//undo.popback();
				if (!undo.empty())
				{
					state st = undo.back();
					loadState(st);
					redo.push(undo.back());
					undo.pop_back();
					//curRow = 0; curCol = 0;
					system("cls");
					print();
				}
				continue;
			}
			else if (ch == 25)//redo
			{
				if (!redo.empty())
				{
					undo.push_back(redo.top());
					state st = redo.top();
					redo.pop();
					loadState(st);
					//currRow = 0; currCol = 0;

					system("cls");
					print();
				}
			}
			else if (ch == 27)//esc to close editor
			{
				writeTextToFile(wrt);
				break;
			}

			if (currCol == 159)
			{
				text.push_back(list<char>());
				rowItr++;
				currCol = 0;
				currRow++;
				(*rowItr).push_back(ch);
				colItr = (*rowItr).begin();
			}
			else
			{
				auto temp = colItr;
				colItr++;
				colItr = (*rowItr).insert(colItr,ch);
				//colItr = ++temp;
				currCol++;
			}

			system("cls");
			print();
			updateUndo();
		}
		system("color 07");
	}*/
	void editFile(ofstream& wrt)
	{
		system("color F0");
		char ch;

		while (true)
		{
			gotoRowCol(currRow, currCol);

			ch = _getch();

			if (ch == -32)  // Handle special keys
			{
				ch = _getch();  // Get the extended key code

				if (ch == 72) // Up arrow key
				{
					if (currRow == 0)
						continue;

					rowItr--;
					colItr = (*rowItr).begin();
					currRow--;currCol = 0;
				}
				else if (ch == 80) // Down arrow key
				{
					if (rowItr != --text.end())
					{
						rowItr++;
						colItr = (*rowItr).begin();
						currRow++; currCol = 0;
					}
				}
				else if (ch == 75) // Left arrow key
				{
					if (currCol - 1 >= 0)
					{
						colItr--;
						currCol--;
					}
				}
				else if (ch == 77) // Right arrow key
				{
					if (currCol == 159 || colItr == (*rowItr).end())
						continue;

					colItr++;
					currCol++;
				}
				else if (ch == 83) // del key
				{
				}
			}
			else if (ch == 13)  // Enter key
			{
				handleEnter(wrt);
			}
			else if (ch == 8)  // Backspace key
			{
				handleBackspace();
			}
			else if (ch == 26)  // Undo key
			{
				handleUndo();
			}
			else if (ch == 25)  // Redo key
			{
				handleRedo();
			}
			else if (ch == 27)  // Esc key to close editor
			{
				writeTextToFile(wrt);
				break;
			}
			else
			{
				handleCharacterInput(ch, wrt);
			}

			system("cls");
			print();
			updateUndo();
		}

		system("color 07");
	}

	void handleUpArrowKey()
	{
		// Move the cursor up
		if (currRow > 0) {
			currRow--;
		}

		// Update the console cursor position
		gotoRowCol(currRow, currCol);
	}
	void handleEnter(ofstream& wrt)
	{
		rowItr = text.begin();
		advance(rowItr, currRow);

		colItr = (*rowItr).begin();
		advance(colItr, currCol);

		// Insert a newline character
		auto newRowItr = text.insert(++rowItr, list<char>(colItr, (*rowItr).end()));

		// Erase the content after the cursor in the current line using iterators from newRowItr
		if (newRowItr != text.end()) {
			(*newRowItr).erase((*newRowItr).begin(), (*newRowItr).end());
		}

		// Move to the next line
		rowItr = newRowItr;
		currRow++;
		colItr = (*rowItr).begin();
		currCol = 0;

		system("cls");
		print();

		// Update undo and clear redo stack
		state currentState = saveState();

		undo.push_back(currentState);
		while (!redo.empty()) {
			redo.pop();
		}

		updateUndo();
	}
	void handleBackspace()
	{
		if (currCol == 0 && currRow == 0) {
			// Nothing to delete at the beginning
			return;
		}

		rowItr = text.begin();
		advance(rowItr, currRow);

		colItr = (*rowItr).begin();
		advance(colItr, currCol);

		if (currCol == 0) {
			// If at the beginning of a line, remove the newline character
			auto prevRowItr = rowItr;
			advance(prevRowItr, -1);
			auto lastCharItr = (*prevRowItr).end();
			advance(lastCharItr, -1);

			// Merge the two lines
			(*prevRowItr).splice(lastCharItr, (*rowItr));

			// Remove the current line
			text.erase(rowItr);
			rowItr = prevRowItr;
			currRow--;
			currCol = lastCharItr != (*prevRowItr).end() ? distance((*prevRowItr).begin(), lastCharItr) : 0;
		}
		else {
			// Delete the character to the left
			auto temp = colItr;
			colItr--;
			(*rowItr).erase(colItr);
			colItr = temp;
			currCol--;
		}

		system("cls");
		print();

		// Update undo and clear redo stack
		state currentState = saveState();

		undo.push_back(currentState);
		while (!redo.empty()) {
			redo.pop();
		}

		updateUndo();
	}

	void handleUndo()
	{
		// Handle Undo key press
		// ...
	}

	void handleRedo()
	{
		// Handle Redo key press
		// ...
	}

	void handleCharacterInput(char ch, ofstream& wrt)
	{
		rowItr = text.begin();
		advance(rowItr, currRow);

		colItr = (*rowItr).begin();
		advance(colItr, currCol);

		if (currCol == 159)// if end of line is reached
		{
			text.push_back(std::list<char>());
			rowItr++;
			currCol = 0;
			currRow++;
		}

		auto temp = colItr;
		colItr = (*rowItr).insert(colItr, ch);
		currCol++;

		system("cls");
		print();

		state currState = saveState();

		// Update undo and clear redo stack
		undo.push_back(currState);
		while (!redo.empty()) {
			redo.pop();
		}

		updateUndo();
	}


	void print()
	{
		for (auto r = text.begin();r != text.end();r++)
		{
			for (auto c = (*r).begin();c != (*r).end();c++)
			{
				cout << *c;
			}
			cout << endl;
		}
	}
	void writeTextToFile(ofstream& wtr)
	{
		for (auto r = text.begin(); r != text.end(); r++)
		{
			for (auto c = (*r).begin(); c != (*r).end(); c++)
			{
				wtr << (*c);
			}
			wtr << '\n';
		}
	}
	void openSavedFile(ifstream& rdr)
	{
		char ch;

		while (rdr.get(ch))
		{
			if (ch != '\n')
				(*rowItr).push_back(ch);
			else
			{
				text.push_back(list<char>());
				rowItr++;
			}
		}
		rowItr = text.begin();
		colItr = (rowItr != text.end()) ? rowItr->begin() : std::list<char>::iterator();
		currCol = 0, currRow = 0;
		print();
	}
	/*static void closing()
	{
		ofstream wrt;
		wrt.open("fileNames.txt");

		for (auto i = Files.begin(); i != Files.end(); i++)
		{
			wrt << (*i) << endl;
		}
		wrt.close();
	}*/
};