//
// maze.cpp
// Jay Zhong
//
// CS15 Assignment 7
//
// Using a recursive function to solve a maze and prints out the solution path.
//

#include <iostream>
#include <fstream>
using namespace std;

// Maximum size of the maze 2-d array.
const int MAXROW = 24;
const int MAXCOL = 81;

void ReadMaze(istream &, char inputmaze[][MAXCOL], int &, int &);
void PrintMaze(ostream &, char inputmaze[][MAXCOL], int, int);
void RunMaze(char inputmaze[][MAXCOL], int, int);
bool RunMazeHelper(char inputmaze[][MAXCOL], int &, int &, int &, int &);



int main(int argc, char * argv[])
{
	if (argc != 3)
	{
		cout << "Please use the command line for input and output file names" << endl;
		return 0;
	}

	ifstream fin(argv[1]);

	if (!fin)
	{
		cout << "There was a problem opening the input file." << endl;
		return 0;
	}

	ofstream fout(argv[2]);

	if (!fout)
	{
		cout << "There was a problem opening the output file." << endl;
	}

	char maze[MAXROW][MAXCOL];

	// Will pass by reference to store the size of the maze.
	int rows;
	int cols;

	fout << "Input file name: " << argv[1] << endl;
	fout << "Output file name: " << argv[2] << endl << endl;

	ReadMaze(fin, maze, rows, cols);
	RunMaze(maze, rows, cols);
	PrintMaze(fout, maze, rows, cols);

	return 0;
}

void ReadMaze(istream & in, char inputmaze[][MAXCOL], int & inputrows, int & inputcols)
{
	// Read the size of the map and store it through reference variables.
	in >> inputrows;
	in >> inputcols;

	char buffer;

	int i = 0;
	int j = 0;

	// I used a while loop instead of nested for loops because I wanted to skip advancing the count
	// if the character read was something I did not want to store.
	while(!in.eof())
	{
		// Using get() to store white spaces.
		in.get(buffer);

		// I want to ignore the newline character.

		if (buffer != '\n')
		{
			inputmaze[i][j] = buffer;
			// Advances the count.  This mimics the behavior of a nested for-loop.
			if (++j >= inputcols)
			{
				i++;
				j = 0;
			}
		}
	}

	return;
}

// Prints the array.
void PrintMaze(ostream & out, char inputmaze[][MAXCOL], int inputrows, int inputcols)
{
	for (int i = 0; i < inputrows; i++)
	{
		for (int j = 0; j < inputcols; j++)
		{
			// Prevents the printing of the dead-end character.
			if (inputmaze[i][j] == '@')
			{
				out << ' ' << flush;
			}
			else
			{
				out << inputmaze[i][j] << flush;
			}
		}
		out << endl;
	}
}

void RunMaze(char inputmaze[][MAXCOL], int inputrows, int inputcols)
{
	// Using local varibles to store the current position while running the maze.
	int currentrow = 0;
	int currentcol = 0;

	// Passing through the maze array, current position and maze size in this recursive helper function.
	// This test will return true if a solution is found.
	if(!RunMazeHelper(inputmaze, currentrow, currentcol, inputrows, inputcols))
	{
		cout << "There is no solution to this maze" << endl;
	}
	return;
}

bool RunMazeHelper(char inputmaze[][MAXCOL], int & currentrow, int & currentcol, int & inputrows, int & inputcols)
{
	// Places a # character at current location.
	inputmaze[currentrow][currentcol] = '#';

	// Check if current location is the exit.
	if(currentrow == (inputrows - 1) && currentcol == (inputcols - 1))
	{
		return true;
	}

	// Checks if going UP is possible.
	if((currentrow > 0) && (inputmaze[currentrow - 1][currentcol] == ' '))
	{
		// If so, move up.
		currentrow--;

		// Run this function again with updated position.
		RunMazeHelper(inputmaze, currentrow, currentcol, inputrows, inputcols);

		// Since this step will be reached only if the current position is at a dead-end, this will
		// check if the current dead end is the exit.
		if(currentrow == (inputrows - 1) && currentcol == (inputcols - 1))
		{
			// Replaces the dead-end character with a path character.
			inputmaze[currentrow][currentcol] = '#';
			return true;
		}
		// If not the exit, go back a step..
		currentrow++;
	}

	// Checks if going RIGHT is possible.
	if((currentcol < inputcols - 1) && (inputmaze[currentrow][currentcol + 1] == ' '))
	{
		currentcol++;
		RunMazeHelper(inputmaze, currentrow, currentcol, inputrows, inputcols);
		if(currentrow == (inputrows - 1) && currentcol == (inputcols - 1))
		{
			inputmaze[currentrow][currentcol] = '#';
			return true;
		}
		currentcol--;
	}

	// Checks if going DOWN is possible.
	if((currentrow < inputrows - 1) && (inputmaze[currentrow + 1][currentcol] == ' '))
	{
		currentrow++;
		RunMazeHelper(inputmaze, currentrow, currentcol, inputrows, inputcols);
		if(currentrow == (inputrows - 1) && currentcol == (inputcols - 1))
		{
			inputmaze[currentrow][currentcol] = '#';
			return true;
		}
		currentrow--;
	}

	// Checks if going LEFT is possible.
	if((currentcol > 0) && (inputmaze[currentrow][currentcol - 1] == ' '))
	{
		currentcol--;
		RunMazeHelper(inputmaze, currentrow, currentcol, inputrows, inputcols);
		if(currentrow == (inputrows - 1) && currentcol == (inputcols - 1))
		{
			inputmaze[currentrow][currentcol] = '#';
			return true;
		}
		currentcol++;
	}

	// If none of the above directions are possible, then the current location is marked
	// as a dead end.
	inputmaze[currentrow][currentcol] = '@';

	// If this is the last iteration of the function to be resolved then this function will return
	// to the wrapper with a false, indicating that there was no solution found for this maze.
	return false;
}

