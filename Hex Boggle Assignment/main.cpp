#include "lpclib.h"
#include "hgraphics.h"
#include "vector.h"
#include "lexicon.h"
using namespace std;

void SetBoard(Vector<string>);
Vector<string> PlayerInput(Lexicon &);
bool PlayerWordSearch(string);
bool PlayerWordSearchHelper(string, int, int, Vector<int> &, Vector<int> &);
void ComputerWordSearch(Vector<string> &, Lexicon &);
void ComputerWordSearchHelper(Vector<string> &, int , int , string, Vector<int> &, Vector<int> &, Lexicon &);
void HighlightPath(Vector<int>, Vector<int>);
bool HexBoundsCheck(int, int);
void SetCase(Vector<int>, Vector<int>, bool);


int main()
{
    /// To hold the dice strings read from file.
    Vector<string> diceVector;

    /// List of words found by player, later used to hold words found by computer.
    Vector<string> usedWords;

    /// Declares lexicon variable.
    Lexicon lex("lexicon.dat");

    /// To hold a single dice string.
    string dice;

    /// Number of hexagons on the board.
    int numberOfHex = 19;

    /// Holds the user response when prompted to replay the game.
    string replayResponse = "";
    /// Set replay variable to false.
    bool replay = false;

    /// Opens the dice file.
    ifstream fin("dice.txt");

    /// Checks if file is open.
    if(!fin)
    {
        cout << "Error opening dice text file." << endl;
        return 0;
    }

    /// Reads the dice file and holds the strings in a vector.
    for (int i = 0; i < numberOfHex; i++)
    {
            fin >> dice;
            diceVector.add(dice);
    }
    /// Closes the file.
    fin.close();

    /// Draws a blank board.
    DrawBoard();
    UpdateDisplay();
    PrintMessage("Welcome to Hoggle\n");

    /// This the the main game loop.
    while(1)
    {
        /// Shuffles and displays the dices.
        SetBoard(diceVector);
        PrintMessage ("\n --HUMAN TURN-- \n");

        /// Words found by player are held in a vector.
        usedWords = PlayerInput(lex);
        PrintMessage ("\n --COMPUTER TURN-- \n");

        /// The computer player takes the words vector so that there are no repeated words.
        /// Finds all remaining viable words on the board.
        ComputerWordSearch(usedWords, lex);

        /// Asks the user to replay, loops until a valid answer is given.
        while(!replay)
        {
            /// Prompts the user for an answer.
            PrintMessage ("Play again? Y/N: ");
            replayResponse = BoardGetLine();

            /// If n or N is entered, exits the program.
            if (replayResponse == "n" || replayResponse == "N")
            {
                return 0;
            }

            /// If y or Y is entered, exit the loop and continue with the main game loop.
            else if(replayResponse == "y" || replayResponse == "Y")
            {
                replay = true;
            }
        }
        /// Reset the replay variable to false to run the above loop again after the next game has finished.
        replay = false;

        PrintMessage ("--NEW GAME--");

        /// Clears the list of words for a new game.
        usedWords.clear();

        /// Resets the scores.
        DrawBoard();
        UpdateDisplay();
    }
    return 0;
}

/// Takes the list of dices, randomizes their positions on the board and shuffles their faces.
/// Displays them on the board.
void SetBoard(Vector<string> diceList)
{
    /// Holds a random variable used to select which die for a given hexagon.
    int randomDice;

    /// Goes through each hexagon on the board.
    for (int r = 0; r < 5; r++)
    {
        for (int c = 0; c < DiceInRow(r); c++)
        {
            /// Randomly choses a index integer from the dice list.
            randomDice = RandomInt(0, diceList.size() - 1);

            /// Randomly choses a face from the randomly chosen die and displays it on the given hexagon.
            LabelHex(r, c, diceList[randomDice][RandomInt(0, 5)]);

            /// Remove the chosen die from the dice list.
            diceList.removeAt(randomDice);
        }
    }
    /// Displays the completed board.
    UpdateDisplay();
    return;
}

/// The main player input function.
Vector<string> PlayerInput(Lexicon & inputLexicon)
{
    /// Holds the list of words found by the player.
    Vector<string> playerWords;

    /// Initializes the variable that will hold player input.
    string inputWord = "";

    /// Initializes the flag to tell if a inputted word already exists in the list.
    bool existingWord = false;

    /// Loops until user just presses enter for his/her inputted word to continue to
    /// the computer player's turn.
    while (1)
    {
        /// Prompts user for a word.
        PrintMessage ("Enter word: ");
        inputWord = BoardGetLine();

        /// If user just presses enter, return to the main to continue to computer player's function.
        if(inputWord == "")
        {
            return playerWords;
        }

        /// Checks if word is long enough and restarts this loop if it is not.
        if (inputWord.length() < 3)
        {
            PrintMessage ("Words must be three letters or more! \n");
            continue;
        }

        /// Goes through the word list vector to see if inputted word already exists.
        foreach(string word in playerWords)
        {
            if (word == inputWord)
            {
                existingWord = true;
            }
        }

        /// If so, restart this loop.
        if (existingWord)
        {
            PrintMessage (inputWord + " already found! \n");
            existingWord = false;
            continue;
        }

        /// Looks for the word on the board.
        if (PlayerWordSearch(inputWord))
        {
            /// If on the board, check with  the lexicon if it is a real word.
            if (inputLexicon.containsWord(inputWord))
            {
                /// If so, add it to the word list and score it.
                PrintMessage ("Word found! \n");
                RecordNewWord(inputWord, Human);
                playerWords.add(inputWord);
            }
            /// If not, let player know and restart this loop.
            else
            {
                PrintMessage (inputWord + " found on board but is not a real word! \n");
            }
        }
        /// Lets the player know that the word entered is not on the board.
        else
        {
            PrintMessage ("Word not on board! \n");
        }
    }
}

/// Contains a recursive function to find the inputted word on the board.
bool PlayerWordSearch(string input)
{
    /// These vectors will hold the coordinates of the path of a word on the board.
    Vector<int> solutionRow;
    Vector<int> solutionCol;

    /// Nested loops to find the first letter of the word on the board.
    for (int r = 0; r < 5; r++)
    {
        for (int c = 0; c < DiceInRow(r); c++)
        {
            /// If the letter is found, clear the solution paths and run the recursive helper function.
            if (input[0] == LetterAt(r, c))
            {
                solutionRow.clear();
                solutionCol.clear();

                /// If a word had been found, return true and run the high light function, displaying
                /// the path of the letters that form the word.
                if (PlayerWordSearchHelper(input,r , c, solutionRow, solutionCol))
                {
                    HighlightPath(solutionRow,solutionCol);
                    return true;
                }
            }
        }
    }
    /// Else, return false when the word is not found on the board.
    return false;
}

/// This recursive function takes in the position of the current hexagon location looks for the next letter
/// in the word in its adjacent hexagons.  It also takes by reference the vectors that holds the coordinates of
/// the solution path.
bool PlayerWordSearchHelper(string input, int row, int col, Vector<int> & pathRow, Vector<int> & pathCol)
{
    /// This table holds the increments a current position needs to add in order to move into 1 of its 6 adjacent hexagons.
    /// The first index describes which section a current hexagon exists in: Row 0 and 1 is in section 1, Row 2 is in section 2,
    /// Row 3 and 4 is in section 3. It is in three sections because moving a hexagon to its adjacent hexagons differs amongst them.
    /// The second index holds rows and column movement tables.  The third index shows by how much to move to one of a hexagon's 6
    /// neighbors.  The first of these show how much to move to the top left and the rest goes clock-wise from there.
    static int HexTable[3][2][6] = {{{-1, -1, 0, 1, 1, 0},{-1, 0, 1, 1, 0, -1}},{{-1, -1, 0, 1, 1, 0},{-1, 0, 1, 0, -1, -1}},{{-1, -1, 0, 1, 1, 0},{0, 1, 1, 0, -1, -1}}};

    /// Holds the 1 of 3 position a current hexagon exists in.  To be used in the above table as the first index.
    int hexPosition;

    /// Uses the first table if it is row 0 or 1.
    if (row < 2)
    {
        hexPosition = 0;
    }

    /// Uses the second table if it is row 2.
    if (row == 2)
    {
        hexPosition = 1;
    }

    /// Uses the third table if it is row 3 or 4.
    if (row > 2)
    {
        hexPosition = 2;
    }

    /// If the hexagon's letter is not the current letter or has already been accounted for
    /// in this search, return false.
    if (input[0] != LetterAt(row, col) || islower(LetterAt(row, col)))
    {
        return false;
    }

    /// Else, if it is also the last letter of word, return true and add its coordinates to
    /// the solution path.
    if (input.length() == 1)
    {
        pathRow.add(row);
        pathCol.add(col);
        return true;
    }

    /// Else, set the current letter to lowercase to show that this letter is already part of the path.
    LabelHex(row, col, tolower(LetterAt(row, col)));

    /// These next 6 if-else statements checks the 6 adjacent hexagons.  First checking if they are within bounds,
    /// then running this function again.  If it returns true, then the current hexagon is part of the solution, which will
    /// reset the current hexagon letter back to uppercase and add its coordinates to the solution path.  If it returns false,
    /// then it will continue to the next adjacent hexagon.
    if (HexBoundsCheck(row + HexTable[hexPosition][0][0], col + HexTable[hexPosition][1][0]) && PlayerWordSearchHelper(input.substr(1), row + HexTable[hexPosition][0][0], col + HexTable[hexPosition][1][0], pathRow, pathCol))
    {
        LabelHex(row, col, toupper(LetterAt(row, col)));
        pathRow.insertAt(0, row);
        pathCol.insertAt(0, col);
        return true;
    }
    else if (HexBoundsCheck(row + HexTable[hexPosition][0][1], col + HexTable[hexPosition][1][1]) && PlayerWordSearchHelper(input.substr(1), row + HexTable[hexPosition][0][1], col + HexTable[hexPosition][1][1], pathRow, pathCol))
    {
        LabelHex(row, col, toupper(LetterAt(row, col)));
        pathRow.insertAt(0, row);
        pathCol.insertAt(0, col);
        return true;
    }
    else if (HexBoundsCheck(row + HexTable[hexPosition][0][2], col + HexTable[hexPosition][1][2]) && PlayerWordSearchHelper(input.substr(1), row + HexTable[hexPosition][0][2], col + HexTable[hexPosition][1][2], pathRow, pathCol))
    {
        LabelHex(row, col, toupper(LetterAt(row, col)));
        pathRow.insertAt(0, row);
        pathCol.insertAt(0, col);
        return true;
    }
    else if (HexBoundsCheck(row + HexTable[hexPosition][0][3], col + HexTable[hexPosition][1][3]) && PlayerWordSearchHelper(input.substr(1), row + HexTable[hexPosition][0][3], col + HexTable[hexPosition][1][3], pathRow, pathCol))
    {
        LabelHex(row, col, toupper(LetterAt(row, col)));
        pathRow.insertAt(0, row);
        pathCol.insertAt(0, col);
        return true;
    }
    else if (HexBoundsCheck(row + HexTable[hexPosition][0][4], col + HexTable[hexPosition][1][4]) && PlayerWordSearchHelper(input.substr(1), row + HexTable[hexPosition][0][4], col + HexTable[hexPosition][1][4], pathRow, pathCol))
    {
        LabelHex(row, col, toupper(LetterAt(row, col)));
        pathRow.insertAt(0, row);
        pathCol.insertAt(0, col);
        return true;
    }
    else if (HexBoundsCheck(row + HexTable[hexPosition][0][5], col + HexTable[hexPosition][1][5]) && PlayerWordSearchHelper(input.substr(1), row + HexTable[hexPosition][0][5], col + HexTable[hexPosition][1][5], pathRow, pathCol))
    {
        LabelHex(row, col, toupper(LetterAt(row, col)));
        pathRow.insertAt(0, row);
        pathCol.insertAt(0, col);
        return true;
    }
    /// If no adjacent hexagon is part of the solution, then set the current hexagon letter back to
    /// uppercase then return false.
    else
    {
        LabelHex(row, col, toupper(LetterAt(row, col)));
        return false;
    }
}

/// This function will go through each hexagon on the word to find all the legal words
/// not found by the player.
void ComputerWordSearch(Vector<string> & wordList, Lexicon & inputLexicon)
{
    /// Holds the solution path.
    Vector<int> solutionRow, solutionCol;

    /// Goes through each hexagon, and uses that letter as the first letter of its word searches.
    for (int r = 0; r < 5; r++)
    {
        for (int c = 0; c < DiceInRow(r); c++)
        {
            /// Clears the solution vectors for a new word search.
            solutionRow.clear();
            solutionCol.clear();

            /// Calls the recursive helper function to find legal words.
            ComputerWordSearchHelper(wordList, r, c, "", solutionRow, solutionCol, inputLexicon);
        }
    }
    return;
}

/// This recursive function will build a word letter by letter by looking to its adjacent hexagons.
void ComputerWordSearchHelper(Vector<string> & wordList, int row, int col, string word, Vector<int> & pathRow, Vector<int> & pathCol, Lexicon & inputLexicon)
{
    /// Same movement table as in the PlayerWordSearchHelper function.
    static int HexTable[3][2][6] = {{{-1, -1, 0, 1, 1, 0},{-1, 0, 1, 1, 0, -1}},{{-1, -1, 0, 1, 1, 0},{-1, 0, 1, 0, -1, -1}},{{-1, -1, 0, 1, 1, 0},{0, 1, 1, 0, -1, -1}}};
    int hexPosition;

    /// Initializes the flag to check if the created word already exists in the word list.
    bool existingWord = false;

    if (row < 2)
    {
        hexPosition = 0;
    }
    if (row == 2)
    {
        hexPosition = 1;
    }
    if (row > 2)
    {
        hexPosition = 2;
    }

    /// Adds the current letter to the word the function is currently building.
    word += LetterAt(row, col);

    /// Adds the current coordinates as part of the solution path.
    pathRow.add(row);
    pathCol.add(col);

    /// Returns the function if the attempted word is not going to be a word or if
    /// the current hexagon is already part of the word.
    if (!inputLexicon.containsPrefix(word) || islower(LetterAt(row, col)))
    {
        return;
    }

    /// Check if the attempted word is a word and is longer than 2 letters.
    if (inputLexicon.containsWord(word) && word.length() > 2)
    {
        /// Check if the word has already been listed.
        foreach(string n in wordList)
        {
            if(n == word)
            {
                existingWord = true;
            }
        }

        /// If so, end the function.
        if (existingWord)
        {
            return;
        }

        /// If not, highlight the solution path on the board, add the word to the list, and then score it for the
        /// computer player.

        /// The SetCase function turns every letter in the solution path to its original cap case.  These is so that
        /// when the path is highlighted, the letters are not in lower case.  This is purely an aesthetic thing.
        SetCase(pathRow, pathCol, true);

        /// Highlights the solution path.
        HighlightPath(pathRow, pathCol);

        /// Revert the path's letters back to lower case for the rest of the function to work properly.
        SetCase(pathRow, pathCol, false);

        /// Add the word to the list.
        wordList.add(word);

        /// Scores the computer player's word.
        RecordNewWord(word, Computer);
    }

    /// Set the current letter to lowercase to show that this letter is already part of the path.
    LabelHex(row, col, tolower(LetterAt(row, col)));

    /// The next 6 if-statements are similar: Checks to see if the adjacent hexagon is within bounds.  If so,
    /// run this function again, which will only return after if the adjacent hexagon has exhausted its words and
    /// then remove its coordinates from the solution path.  Do the same for the next 5 adjacent hexagons.
    if (HexBoundsCheck(row + HexTable[hexPosition][0][0], col + HexTable[hexPosition][1][0]))
    {
        ComputerWordSearchHelper(wordList, row + HexTable[hexPosition][0][0], col + HexTable[hexPosition][1][0], word, pathRow, pathCol, inputLexicon);
        pathRow.removeAt(pathRow.size() - 1);
        pathCol.removeAt(pathCol.size() - 1);
    }

    if (HexBoundsCheck(row + HexTable[hexPosition][0][1], col + HexTable[hexPosition][1][1]))
    {
        ComputerWordSearchHelper(wordList, row + HexTable[hexPosition][0][1], col + HexTable[hexPosition][1][1], word, pathRow, pathCol, inputLexicon);
        pathRow.removeAt(pathRow.size() - 1);
        pathCol.removeAt(pathCol.size() - 1);
    }

    if (HexBoundsCheck(row + HexTable[hexPosition][0][2], col + HexTable[hexPosition][1][2]))
    {
        ComputerWordSearchHelper(wordList, row + HexTable[hexPosition][0][2], col + HexTable[hexPosition][1][2], word, pathRow, pathCol, inputLexicon);
        pathRow.removeAt(pathRow.size() - 1);
        pathCol.removeAt(pathCol.size() - 1);
    }

    if (HexBoundsCheck(row + HexTable[hexPosition][0][3], col + HexTable[hexPosition][1][3]))
    {
        ComputerWordSearchHelper(wordList, row + HexTable[hexPosition][0][3], col + HexTable[hexPosition][1][3], word, pathRow, pathCol, inputLexicon);
        pathRow.removeAt(pathRow.size() - 1);
        pathCol.removeAt(pathCol.size() - 1);
    }

    if (HexBoundsCheck(row + HexTable[hexPosition][0][4], col + HexTable[hexPosition][1][4]))
    {
        ComputerWordSearchHelper(wordList, row + HexTable[hexPosition][0][4], col + HexTable[hexPosition][1][4], word, pathRow, pathCol, inputLexicon);
        pathRow.removeAt(pathRow.size() - 1);
        pathCol.removeAt(pathCol.size() - 1);
    }

    if (HexBoundsCheck(row + HexTable[hexPosition][0][5], col + HexTable[hexPosition][1][5]))
    {
        ComputerWordSearchHelper(wordList, row + HexTable[hexPosition][0][5], col + HexTable[hexPosition][1][5], word, pathRow, pathCol, inputLexicon);
        pathRow.removeAt(pathRow.size() - 1);
        pathCol.removeAt(pathCol.size() - 1);
    }
    /// Once all of the adjacent hexagons has exhausted their words, then return the current hexagon
    /// to uppercase and return.
    LabelHex(row, col, toupper(LetterAt(row, col)));
    return;
}

/// This function check if the given row and col are within the bounds of the board.
bool HexBoundsCheck(int row, int col)
{
    /// Since the board is not a square, I created a 2-d array that mimics the dimensions of the board by
    /// creating false "holes" to account for the areas not within the bounds of the board.
    static bool HexTable[5][5] = {{true, true, true, false, false},{true, true, true, true, false},{true, true, true, true, true},{true, true, true, true, false},{true, true, true, false, false}};

    /// Anything outside of the 2-d array are also out of bounds.
    if(row < 0 || row > 4 || col < 0 || col > 4)
    {
        return false;
    }

    /// Returns a boolean value from the table.
    return HexTable[row][col];
}

/// Takes two vectors as coordinates then highlight their corresponding hexagons.
void HighlightPath(Vector<int> row, Vector<int> col)
{
    for(int i = 0; i < row.size(); i++)
    {
        /// Highlights the hexagons in the order of the solution vectors' listed coordinates.
        HighlightHex(row[i], col[i], true);
        Pause(0.05);
    }

    for(int j = 0; j < row.size(); j++)
    {
        /// After they are highlighted, turn the highlighting off.
        HighlightHex(row[j], col[j], false);
    }
}

/// Takes two vectors as coordinates then changes the corresponding hexagons' letter's case.
void SetCase(Vector<int> row, Vector<int> col, bool caps)
{
    for(int i = 0; i < row.size(); i++)
    {
        /// Changes the letters to all caps.
        if (caps)
        {
            LabelHex(row[i], col[i], toupper(LetterAt(row[i], col[i])));
        }
        /// Changes the letters to all lower case.
        else
        {
            LabelHex(row[i], col[i], tolower(LetterAt(row[i], col[i])));
        }
    }
}
