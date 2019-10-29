#include "CandyGrid.h"
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>

CandyGrid::CandyGrid(int rows, int cols) : rows(rows), cols(cols) {

	grid.reserve(rows);

	// Init grid with file
	std::string line;
	std::ifstream file;
	file.open("Game/level1.txt");

	int i, j;
	i = 0;
	if (file.is_open()) { 

		while (std::getline(file, line)) {
			std::vector<Candy*> temp;
			std::string token;
			std::istringstream tokenStream(line);
			j = 0;

			while (std::getline(tokenStream, token, ' ')) {
				CandyType type = CandyType(std::stoi(token));
				Candy *newCandy = new Candy(iPoint(i, j), type);
				temp.push_back(newCandy);
				j++;
			}

			grid.push_back(temp);
			i++;
			
		}
		file.close();
	}
}

CandyGrid::~CandyGrid() {
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			delete grid[i][j];
		}
		grid[i].clear();
	}
	grid.clear();
}

void CandyGrid::ChangeType(CandyType type, iPoint pos) {
	grid[pos.x][pos.y]->SetType(type);
}

Candy* CandyGrid::Get(iPoint pos) {
	return grid[pos.x][pos.y];
}

void CandyGrid::Move(iPoint pos, iPoint newPos) {
	grid[newPos.x][newPos.y] = grid[pos.x][pos.y];
}

void CandyGrid::Swap(iPoint pos, iPoint newPos) {
	grid[newPos.x][newPos.y]->SetPos(pos);
	grid[pos.x][pos.y]->SetPos(newPos);

	Candy *temp = grid[newPos.x][newPos.y];
	grid[newPos.x][newPos.y] = grid[pos.x][pos.y];
	grid[pos.x][pos.y] = temp;
}

CandyMatch CandyGrid::CheckMatch(CandyType type, iPoint pos) {
	CandyMatch match;

	int xBegin;
	int xEnd;
	int yBegin;
	int yEnd;

	yBegin = yEnd = pos.y;

	// CHECK LEFT
	int matches = 1;
	for (int j = pos.y - 1; j >= 0; --j) {
		if (grid[pos.x][j]->GetType() == type) {
			++matches;
			yBegin = j;
		}
		else {
			break;
		}
	}

	// CHECK RIGHT
	for (int j = pos.y + 1; j < cols; ++j) {
		if (grid[pos.x][j]->GetType() == type) {
			++matches;
			yEnd = j;
		}
		else {
			break;
		}
	}

	if (matches >= MIN_MATCHES) {
		match = CandyMatch(Match::ROW, -1, -1, yBegin, yEnd);
	}

	xBegin = xEnd = pos.x;

	// CHECK UP
	matches = 1;
	for (int j = pos.x - 1; j >= 0; --j) {
		if (grid[j][pos.y]->GetType() == type) {
			++matches;
			xBegin = j;
		}
		else {
			break;
		}
	}

	// CHECK DOWN
	for (int j = pos.x + 1; j < rows; ++j) {
		if (grid[j][pos.y]->GetType() == type) {
			++matches;
			xEnd = j;
		}
		else {
			break;
		}
	}

	if (matches >= MIN_MATCHES) {
		if (match.GetMatch() == Match::ROW)
			match = CandyMatch(Match::BOTH, xBegin, xEnd, yBegin, yEnd);
		else match = CandyMatch(Match::COL, xBegin, xEnd, -1, -1);
	}

	return match;
}

void CandyGrid::ClearMatchedCol(int col, int xBegin, int xEnd) {
	
	// Copy upper candies
	int pos = xEnd;
	for (int i = xBegin - 1; i >= 0; i--) {
		Move(iPoint(i, col), iPoint(pos, col));
		pos--;
	}

	// Create new candies
	for (; pos >= 0; pos--) {
		CandyType type = CandyType(rand() % CANDY_TYPES);
		ChangeType(type, iPoint(pos, col));
	}

}

void CandyGrid::ClearMatchedRow(int row, int yBegin, int yEnd) {

	for(int pos = yBegin; pos <= yEnd; pos++){
		ClearMatchedCol(pos, row, row);
	}
}