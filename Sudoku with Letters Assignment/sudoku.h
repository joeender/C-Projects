/// This is the interface to the Sudoku class,
/// representing a Sudoku board.
/// DO NOT CHANGE any EXISTING code here.
/// You may ADD additional methods if needed.

#include "lpclib.h"
#include "grid.h"

/// This is a helper struct representing one board position
struct Position
{
  int row;
  int col;
  Position(int r, int c) : row(r), col(c) { }
};

class Sudoku
{
 public:

  /// Initializes the sudoku board, specified by a sequence
  /// of strings (one per row) and ending with an empty string
  Sudoku(char * contents, ...);

  /// "Draws" the sudoku board by printing a text representation of it.
  void Draw();

  /// Sets the given position to the given letter
  /// Use ' ' instead of a letter to set a position to empty
  void Set(Position p, char letter);

  /// Determines if the current board arrangement is legal
  bool IsLegal();

  /// Determines if the current board arrangement is full
  /// (all cells set to a letter)
  bool IsFull();

  /// Returns the board size (number of letters per row)
  int BoardSize();

  /// Returns the letter at given index.
  char GetLetter(int row, int col);

 private:
  Grid<char> board;
  int boardSize;
};


