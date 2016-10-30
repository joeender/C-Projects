/// This is the interface to the Domino class,
/// representing a standard domino oriented vertically.
/// DO NOT CHANGE any of this code.

#include "lpclib.h"
using namespace std;

class Domino
{
public:
  Domino(unsigned int topNum=0, unsigned int bottomNum=0);
  unsigned int getTop();
  unsigned int getBottom();
friend ostream& operator<<(ostream& out, const Domino & d);

private:
  unsigned int top;
  unsigned int bottom;
  static string squares[7][3];

};



