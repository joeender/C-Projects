/// This is the implementation of the Domino class.
/// You should NOT need to read any of this to do the assignment.
/// DO NOT CHANGE any of this code.

#include "domino.h"

Domino::Domino(unsigned int t, unsigned int b) : top(t), bottom(b)
{ 
  if (t > 6 || b > 6)  Error("Illegal domino specification - only 0 to 6 dots allowed");
}
unsigned int Domino::getTop() { return top; }
unsigned int Domino::getBottom() { return bottom; }
ostream& operator<<(ostream& out, const Domino & d)
{
  static const string hl = " --- "; 
  int val;
  string s = "";
  for (int r = 0; r < 9; r++)
    {
      val = (r > 4) ? d.bottom : d.top;
      if (r%4)
	{
	  s += "|" + Domino::squares[val][r%4-1] + "|\n";
	}
      else
	s += hl + "\n";
    }
  return (out << s);
}

string Domino::squares[7][3] = {
  { "   ",
    "   ",
    "   " },
  { "   ",
    " * ",
    "   " },
  { "*  ",
    "   ",
    "  *" },
  { "*  ",
    " * ",
    "  *" },
  { "* *",
    "   ",
    "* *" },
  { "* *",
    " * ",
    "* *" },
  { "* *",
    "* *",
    "* *" } 
};


