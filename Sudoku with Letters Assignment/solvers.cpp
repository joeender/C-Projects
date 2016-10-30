#include "lpclib.h"
#include "vector.h"
#include "domino.h"
#include "sudoku.h"

/// This is the recursive solver function for dominos
/// It should use recursive backtracking to
/// explore possible solutions and determine if
/// all remaining dominoes can form a legal chain.
bool SolveDominoChain(Vector<Domino> & chain, Vector<Domino> & remaining)
{
    /// YOUR CODE HERE
    /// If there are no dominoes left to be placed, the chain has been completed.
    if (remaining.isEmpty())
    {
        return true;
    }

    /// For each domino in remaining...
    for(int i = 0; i < remaining.size(); i++)
    {
        /// If the chain is empty, add the current domino selected in remaining.
        if (chain.isEmpty())
        {
            /// Adds the domino to chain.
            chain.add(remaining[i]);
            /// Removes the domino in remaining.
            remaining.removeAt(i);
            /// Runs the function again and returns true if functions returns true.
            if (SolveDominoChain(chain, remaining))
            {
                return true;
            }
            /// If the domino placed could not be used to solve the chain, add it back to remaining.
            remaining.insertAt(i, chain[0]);
            /// Then remove it from the chain.
            chain.removeAt(0);

        }
        /// If there is already a domino in the chain, check the last one there and match its
        /// bottom value to the selected remaining domino's top value.
        else if (chain[chain.size() - 1].getBottom() == remaining[i].getTop())
        {
            /// If they match, add it to the chain.
            chain.add(remaining[i]);
            /// Remove it from the remaining.
            remaining.removeAt(i);
            /// Run the function again, returns true if domino placed was part of the solution.
            if (SolveDominoChain(chain, remaining))
            {
                return true;
            }
            /// Else, add it back to the remaining, then remove it from the chain.
            remaining.insertAt(i, chain[chain.size() - 1]);
            chain.removeAt(chain.size() - 1);
        }
        /// If the selected domino's top value does not match the last chain's bottom value,
        /// flip it.
        else
        {
            /// Declare a new domino that has the values of the selected remaining domino flipped.
            Domino flipped(remaining[i].getBottom(), remaining[i].getTop());
            /// Same process as the above except with the flipped domino.
            if (chain[chain.size() - 1].getBottom() == flipped.getTop())
            {
                chain.add(flipped);
                remaining.removeAt(i);
                if (SolveDominoChain(chain, remaining))
                {
                    return true;
                }
                /// Note that if the flipped domino was not part of the solution, it will be replaced back
                /// to remaining as the flipped version of the domino.
                remaining.insertAt(i, chain[chain.size() - 1]);
                chain.removeAt(chain.size() - 1);
            }
        }
    }
    return false;
}



/// This is the recursive solver function for sudoku.
/// It should use recursive backtracking to
/// explore possible solutions and determine if
/// a given puzzle is solvable.
bool SolveSudoku (Sudoku & puzzle)
{
    /// YOUR CODE HERE
    /// Creates an array of char to be used to insert letters into cells.
    static char letterList[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    /// Initialize Position variable to be used to locate and set char values within the Sudoku board grid.
    Position p(0,0);

    /// Base case:  If the board is full, return true.
    if(puzzle.IsFull())
    {
        return true;
    }
    else
    {
        ///  Goes through each cell of the board.
        for (int i = 0; i < puzzle.BoardSize(); i++)
        {
            for (int j = 0; j < puzzle.BoardSize(); j++)
            {
                /// Saves the location of the current position to Position struct.
                p.row = i;
                p.col = j;
                /// If the cell is empty.
                if (puzzle.GetLetter(i, j) == ' ')
                {
                    /// For every letter to be used in the board.
                    for (int k = 0; k < puzzle.BoardSize(); k++)
                    {
                        /// Places the letter onto the current location.
                        puzzle.Set(p,letterList[k]);
                        /// If the placement was legal and was part of a legal solved board, return true.
                        if(puzzle.IsLegal() && SolveSudoku(puzzle))
                        {
                            return true;
                        }
                        /// Else, remove the letter.
                        puzzle.Set(p,' ');
                    }
                    /// If no letter was legal, return false and the previous letter placement cannot be
                    /// part of the solved board.
                    return false;
                }
            }
        }
    }
    /// If this is reached, then the board cannot be solved.
    return false;
}



