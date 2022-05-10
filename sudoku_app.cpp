#include <iomanip>
#include <fstream>
#include <iostream>
#include <ostream>
using namespace std;

// Board 
#define AREA 9

// Prototypes
string getText(string prompt);
void readFile(string filename, int board[][AREA], bool spaceData[][AREA]);
void interact(int board[][AREA], bool spaceData[][AREA]);
void displayBoard(int board[][AREA], bool spaceData[][AREA]);
void editSquare(int board[][AREA], bool spaceData[][AREA]);
void computeValues(int board[][AREA], bool spaceData[][AREA], int r, char charCol);
void displayPossible(int board[][AREA]);
bool checkBoard(int board[][AREA]);
void writeFile(string filename, const int board[][AREA]);
void possibleValue(int board[][AREA], int array[], int row, char charCol);
bool solveSuduko(int board[][AREA], int row, int col);
bool isSafe(int board[][AREA], int r, int c, int numberCheck);
void getInput(int values[]);

// Main
int main()
{
    int board[AREA][AREA];
    bool spaceData[AREA][AREA];

    string filename = getText("Where is your board located? ");
    readFile(filename, board, spaceData);

    if (!checkBoard(board))
        return 0;

    interact(board, spaceData);
    string writeFilename = getText("What file would you like to write your board to: ");
    writeFile(writeFilename, board);

    return 0;
}

// Makes sure board is valid
bool checkBoard(int board[][AREA])
{
    int row = 0;
    int col = 0;
    char coords[2];
    for (int loop = 0; loop < AREA; loop++)
    {
        int numCol[10] = { 0 };
        int numRow[10] = { 0 };
        int numBox[10] = { 0 };
        for (int r = 0; r < 9; r++)
        {
            if (board[r][col] != 0)
                numCol[board[r][col]] += 1;
            if (numCol[board[r][col]] > 1)
            {
                coords[1] = r + 48;
                coords[0] = col + 66;
                cout << "ERROR: Duplicate value '" << board[r][col] << "' in inside square represented by '" << coords[0] << coords[1] << "'\n";
                return false;
            }
        }
        for (int c = 0; c < 9; c++)
        {
            if (board[row][c] != 0)
                numRow[board[row][c]] += 1;
            if (numRow[board[row][c]] > 1)
            {
                coords[1] = row + 48;
                coords[0] = c + 66;
                cout << "ERROR: Duplicate value '" << board[row][c] << "' in inside square represented by '" << coords[0] << coords[1] << "'\n";
                return false;
            }
        }
        int mRow = (row / 3) * 3;
        int mCol = (col / 3) * 3;
        for (int r = mRow; r < (mRow + 3); r++)
        {
            for (int c = mCol; c < (mCol + 3); c++)
            {
                if (board[r][c] != 0)
                    numBox[board[r][c]] += 1;
                if (numBox[board[r][c]] > 1)
                {
                    coords[1] = r + 48;
                    coords[0] = c + 66;
                    cout << "ERROR: Duplicate value '" << board[r][c] << "' in inside square represented by '" << coords[0] << coords[1] << "'\n";
                    return false;
                }
            }
        }

        row++;
        col++;
    }
    return true;
}


string getText(string prompt)
{
    // Mega basic get text function for filenames and stuff
    string text;
    cout << prompt;
    cin >> text;

    return text;
}

void readFile(string filename, int board[][AREA], bool spaceData[][AREA])
{
    // Open file
    ifstream fin(filename);

    // Did the file open?
    if (fin.fail())
    {
        cout << "Error reading file \"" << filename << "\"\n";
        return;
    }

    // Fill our 2d array with the numbers from the file
    for (int r = 0; r < AREA; r++)
    {
        for (int c = 0; c < AREA; c++)
        {
            fin >> board[r][c];

            // Does using the ternary operator make me cooler
            spaceData[r][c] = (board[r][c] == 0) ? true : false;
        }
    }

    fin.close();
}

void printInstructions()
{
    // Diplay menu options
    cout << "Options:\n";
    cout << "   ?  Show these instructions\n";
    cout << "   D  Display the board\n";
    cout << "   E  Edit one square\n";
    cout << "   S  Show the possible values for a square\n";
    cout << "   Q  Save and Quit\n";
    cout << "   F  Solve Sudoku\n";
    cout << "\n";
}

void doSudoku(int board[][AREA], bool spaceData[][AREA])
{
    if (solveSuduko(board, 0, 0))
    {
        cout << "Solved:\n";
        displayBoard(board, spaceData);
    }
    else
    {
        cout << "Can't be solved!\n";
    }
}

void interact(int board[][AREA], bool spaceData[][AREA])
{
    bool quit = false;
    char option;

    printInstructions();

    displayBoard(board, spaceData);

    while (!quit)
    {
        cout << "> ";
        cin >> option;

        // To upper so case doesn't matter
        switch (toupper(option))
        {
        case '?':
            cout << "Options:\n";
            cout << "   ?  Show these instructions\n";
            cout << "   D  Display the board\n";
            cout << "   E  Edit one square\n";
            cout << "   S  Show the possible values for a square\n";
            cout << "   Q  Save and Quit\n\n\n";
            cout << "   F  Solve Sudoku\n";

                // I know i cant repeat code but I made a
                // function that did exatly this and it
                // didnt work so this must stay
            break;
        case 'D':
            displayBoard(board, spaceData);
            break;
        case 'E':
            editSquare(board, spaceData);
            break;
        case 'Q':
            quit = true;
            break;
        case 'S':
            displayPossible(board);
            break;
        case 'F':
            doSudoku(board, spaceData);
            break;
        default:
            cout << "ERROR: Invalid command\n\n";
            break;
        }
    }
}

void displayBoard(int board[][AREA], bool spaceData[][AREA])
{
    // Header
    cout << "   A B C D E F G H I\n";
    int count = 1;

    // Loop thorugh the board
    for (int r = 0; r < AREA; r++)
    {
        cout << count << "  ";
        for (int c = 0; c < AREA; c++)
        {
            // 0 signifies an empty spot
            if (board[r][c] == 0)
                cout << " ";
            else
            {
                // Tis the space readith only or writith
                if (spaceData[r][c])
                    cout << board[r][c];
                else
                    cout << board[r][c];

            }

            // Math for adding column spaces
            if (c == 2 || c == 5)
                cout << '|';
            else if (c == AREA - 1)
                cout << endl;
            else
                cout << " ";
        }

        if (r == 2 || r == 5)
        {
            cout << "   -----+-----+-----\n";
        }
        count++;
    }
    cout << endl;
}

void editSquare(int board[][AREA], bool spaceData[][AREA])
{
    // [0] = Row, [1] = Column, [2] = Character Column
    int values[3];
    getInput(values);

    int row = values[0];
    int col = values[1];
    char charCol = (col + 65);

    computeValues(board, spaceData, row, charCol);
}

// [0] = Row, [1] = Column
void getInput(int values[])
{
    char inOne, inTwo;
    bool validInput = false;

    while (!validInput)
    {
        cout << "What are the coordinates of the square: ";
        cin >> inOne >> inTwo;

        if (isdigit(inOne))
        {
            values[0] = inOne - '0';
            values[1] = (static_cast<int>(toupper(inTwo)) - 65);
            validInput = true;
        }
        else if (isdigit(inTwo))
        {
            values[0] = inTwo - '0';
            values[1] = (static_cast<int>(toupper(inOne)) - 65);
            validInput = true;
        }
        else
        {
            cout << "ERROR: Square '" << inOne << inTwo << "' is invalid\n";
            cin.clear();
        }
    }
}

void displayPossible(int board[][AREA])
{
    // [0] = Row, [1] = Column, [2] = Character Column
    int values[3];
    getInput(values);

    int row = values[0];
    int col = values[1];
    char charCol = (values[1] + 65);

    // Initalize to zero for possible values
    // Elements that are 0 are skipped when printing
    int array[AREA] = { 0 };
    possibleValue(board, array, row, charCol);

    cout << "The possible values for \'" << charCol << row << "\' are: ";

    // I really should learn how to use pointers
    int lastIndex = 0;
    for (int i = 0; i <= AREA; i++)
    {
        if (array[i] != 0)
            lastIndex = i;
        else
            break;
    }
    for (int i = 0; i <= lastIndex; i++)
    {
        if (i == lastIndex)
            cout << array[i] << endl;
        else
            cout << array[i] << ", ";
    }

    cout << endl;
}

void computeValues(int board[][AREA], bool spaceData[][AREA], int r, char charCol)
{
    int value;
    int col = (static_cast<int>(charCol) - 65);

    if (spaceData[r - 1][col] == false)
    {
        cout << "ERROR: Square \'" << charCol << r << "\' is read-only\n";
    }
    else
    {
        cout << "What is the value at \'" << charCol << r << "\': ";
        cin >> value;

        // Use the converted char and take one from the row because arrays
        if (isSafe(board, r - 1, col, value) && (value <= 9 && value >= 1))
            board[r - 1][col] = value;
        else
        {
            cout << "ERROR: Value \'" << value << "\' in square \'" << charCol << r;
            cout << "\' is invalid\n";
        }
    }

    cout << endl;
}

void possibleValue(int board[][AREA], int array[], int row, char charCol)
{
    int col = (static_cast<int>(charCol) - 65);
    int count = 0;

    // Loop and test all numbers
    for (int i = 1; i <= AREA; i++)
    {
        if (isSafe(board, row - 1, col, i))
        {
            // If the value works
            array[count] = i;
            count++;
        }
    }
}

void writeFile(string filename, const int board[][AREA])
{
    ofstream fout(filename);

    if (fout.fail())
    {
        cout << "Error writing to the file \"" << filename << "\"\n";
        return;
    }

    // Loop board for both sides of array
    //  Put values into file
    for (int r = 0; r < AREA; r++)
        for (int c = 0; c < AREA; c++)
            fout << board[r][c] << " ";

    fout.close();

    cout << "Board written successfully\n";
}

bool isSafe(int board[][AREA], int r, int c, int numberCheck)
{
    for (int x = 0; x <= 8; x++)
        if (board[r][x] == numberCheck)
            return false;

    for (int x = 0; x <= 8; x++)
        if (board[x][c] == numberCheck)
            return false;

    int startRow = r - r % 3,
        startCol = c - c % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i + startRow][j + startCol] == numberCheck)
                return false;

    return true;
}

bool solveSuduko(int board[][AREA], int row, int col)
{
    if (row == AREA - 1 && col == AREA)
        return true;

    if (col == AREA) {
        row++;
        col = 0;
    }

    if (board[row][col] > 0)
        return solveSuduko(board, row, col + 1);

    for (int num = 1; num <= AREA; num++)
    {
        if (isSafe(board, row, col, num))
        {
            board[row][col] = num;

            if (solveSuduko(board, row, col + 1))
                return true;
        }

        board[row][col] = 0;
    }
    return false;
}