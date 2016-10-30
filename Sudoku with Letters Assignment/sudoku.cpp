
/// This is the implementation of the Domino class.
/// You may ONLY change the IsLegal() method and the IsFull() method.
/// You may ADD additional methods as needed.

#include "sudoku.h"
#include "vector.h"
#include <cstdarg>
#include <cmath>
int IntSqrt(int input);

bool Sudoku::IsFull()
{
    /// YOUR CODE HERE
    /// Checks each cell for a letter.
    for (int i = 0; i < this->BoardSize(); i++)
    {
        for (int j = 0; j < this->BoardSize(); j++)
        {
            /// Returns false when a space char is found.
            if (this->board[i][j] == ' ')
            {
                return false;
            }
        }
    }
    /// Else return true when all of the grid has been filled with letters.
    return true;
}

bool Sudoku::IsLegal()
{
    /// YOUR CODE HERE
    /// Initialize the variables to contain information about current cell.
    char compare = ' ';
    int rowCompare = 0;
    int colCompare = 0;
    /// Finds the side of length of the mini-boxes.
    int subSquareN = IntSqrt(this->BoardSize());

    /// Nested loops goes through each cell.
    for (int i = 0; i < this->BoardSize(); i++)
    {
        for (int j = 0; j < this->BoardSize(); j++)
        {
            /// To find an occupied cell.
            if (this->board[i][j] != ' ')
            {
                /// Saves the letter and its location to compare with the letters
                /// in its row and column.
                compare = this->board[i][j];
                rowCompare = i;
                colCompare = j;

                /// Comparing letter in its row.
                for (int k = 0; k < this->BoardSize(); k++)
                {
                    /// Skips over its own index.
                    if (rowCompare == i && colCompare == k)
                    {
                        continue;
                    }
                    /// If matching letter found, return false.
                    if (compare == this->board[i][k])
                    {
                        return false;
                    }
                }
                /// Comparing letter in its column.
                for (int l = 0; l < this->BoardSize(); l++)
                {
                    /// Skips over its own index.
                    if (rowCompare == l && colCompare == j)
                    {
                        continue;
                    }
                    /// If matching letter found, return false.
                    if (compare == this->board[l][j])
                    {
                        return false;
                    }
                }

                /// Another set of nested loops to find cells within the compare cell's mini-box.
                for (int m = 0; m < this->BoardSize(); m++)
                {
                    for (int n = 0; n < this->BoardSize(); n++)
                    {
                        ///  This checks if a given cell is part of the mini-box set of boxes.
                        if ((m/subSquareN == rowCompare/subSquareN) && (n/subSquareN == colCompare/subSquareN))
                        {
                            /// Skips over its own index.
                            if (rowCompare == m && colCompare == n)
                            {
                                continue;
                            }
                            /// Returns false once the same letter has been found.
                            if (compare == this->board[m][n])
                            {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    /// If each cell in the grid passes the above tests, return true.
    return true;
}

/// Method to return the char value of a given box.
char Sudoku::GetLetter(int row, int col)
{
    return (this->board[row][col]);
}

/// Function I wrote to return the square root of an int as an int.  To
/// be used to find the size of the side of a mini-box.
int IntSqrt(int input)
{
    int root = 1;
    while(input != (root * root))
    {
        if (input <= root)
        {
            cout << "Error! Square root not found." << endl;
            return root;
        }
        root++;
    }
    return root;
}



/// =======================================
/// DO NOT CHANGE anything below this line.
/// =======================================

Sudoku::Sudoku(char * contents, ...)
{
  Vector<string> lines;
  va_list arguments;
  va_start ( arguments, contents );
  string line = contents;
  int len = (int) line.length();
  while (1)
    {
      lines.add(line);
      line = va_arg (arguments, char *);
      if (line == "")
	break;
      if (line.length() != len)
	Error("Strings provided to initialize the board must be the same length");
    }
  board.resize(lines.size(), len);
  for (int r = 0; r < board.numRows(); r++)
    {
      for (int c = 0; c < board.numCols(); c++)
        {
	  board.setAt(r, c, lines[r][c]);
        }
    }

  boardSize = board.numRows();
  if (boardSize != board.numRows())
    Error ("Sudoku board number of rows and columns must match");
  if (int(sqrt(boardSize)) * int(sqrt(boardSize)) != boardSize)
    Error ("Sudoku board must be NxN where N is a perfect square (4, 9, etc.)");
}

#define Horiz(n)  { cout << "+"; for (int k = 1; k <= (2*n - 1); k++) cout << "-"; cout << "+\n"; }
void Sudoku::Draw()
{
  int N = boardSize;
  int subN = (int) sqrt(N);
  Horiz(N);
  for (int r = 0; r < N; r++)
    {
      cout << "|";
      for (int c = 0; c < N; c++)
        {
	  cout << board.getAt(r, c);
	  if ((c % subN) < (subN - 1))
	    cout << " ";
	  else
	    cout << "|";
        }
      cout << "\n";
      if ((r % subN) == (subN - 1))
	  Horiz(N);
    }
}

static void RangeCheck(char x, int n)
{
  if (x < 'A' || x > ('A' + n - 1))
    {
      string msg = "Illegal letter, must be ";
      msg += "A to " + ('A' + n - 1);
      Error(msg);
    }
}

void Sudoku::Set(Position p, char letter)
{
  if (!board.inBounds(p.row, p.col))
    Error(string("Position ") + IntToString(p.row) + ", " + IntToString(p.col)
	  + " is not a legal position on this sudoku board.");
  if (letter != ' ')
    RangeCheck(letter, boardSize);

  board.setAt(p.row, p.col, letter);
}

int Sudoku::BoardSize()
{
  return boardSize;
}
