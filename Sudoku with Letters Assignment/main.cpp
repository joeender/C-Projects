#include "lpclib.h"
#include "vector.h"
#include "solvers.h"
using namespace std;

int main()
{
  cout << "===== Domino Test =====\n";
  Vector<Domino> myDominos;
  myDominos.add(Domino(5, 2));
  myDominos.add(Domino(1, 6));
  myDominos.add(Domino(5, 6));
  myDominos.add(Domino(3, 3));
  myDominos.add(Domino(1, 3));


    Vector<Domino> myChain; /// blank chain of dominos
    if (SolveDominoChain(myChain, myDominos))
    {
        cout << "Yes, a complete chain can be formed:\n";
        foreach (Domino d in myChain)
        {
            cout << d;
        }
    }
    else
    {
        cout << "No, a complete chain cannot be formed.\n";
    }

  cout << "\n";
  cout << "===== Sudoku Test =====\n";

  Sudoku mySudoku (
		"DC B     ",
		" B       ",
		"      A  ",
		"         ",
		"       D ",
		"A   C   E",
		"         ",
		"    F    ",
		"         ",
		"");


  cout << "Starting sudoku arrangement:\n";
  mySudoku.Draw();

  if (SolveSudoku(mySudoku))
    {
      cout << "\nThere is a solution:\n";
      mySudoku.Draw();
    }
  else
    {
      cout << "\nThere is NO solution.\n";
    }

    /// My tests
    cout << "My tests:" << endl;
    Sudoku test1 (
        "B  C",
        "D   ",
        "   B",
        "    ",
        ""       );
    Sudoku test2 (
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "              A ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        "                ",
        ""       );

    cout << mySudoku.IsLegal();
    cout << test1.IsLegal();
    cout << endl;
    Position p(8,8);
    cout << mySudoku.IsLegal() << endl;
    //mySudoku.Set(p, 'F');
    SolveSudoku(mySudoku);
    mySudoku.Draw();
    SolveSudoku(test1);
    test1.Draw();
    SolveSudoku(test2);
    test2.Draw();
    return 0;
}


