#pragma once
#include "File.h"
#include <list>
#include <queue>
#include <stack>

struct state
{
	int rows, cols;
	list<list<char>> text;
	list<char>::iterator colItr;
	list<list<char>>::iterator rowItr;
};
class textEditor
{
public:
	int currRow, currCol;
	list<list<char>> text;
	list<char>::iterator colItr;
	list<list<char>>::iterator rowItr;
	deque<state> undo;
	stack<state> redo;

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
				(*row).push_back(ch);
			}
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
};