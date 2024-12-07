/*
    Date: 01/12/2023

    Name: Marwan Haitham Mahmoud
    ID: 23P0286
    Course Name: Introduction to Computer Programming
    Course Code: CSE141

    Description:
    - This program implements the 2048 game in C++ with customizable board sizes,
      dynamic scoring, leaderboard functionality, and robust move validation.
    - High scores are saved to and read from a file for persistence.
*/
/* Reference
Color Codes: (used for background and font color)
0 = Black
8 = Gray
1 = Blue
9 = Light Blue
2 = Green
A = Light Green
3 = Aqua
B = Light Aqua
4 = Red
C = Light Red
5 = Purple
D = Light Purple
6 = Yellow
E = Light Yellow
7 = White
F = BrightWhite
*/
/* Reference
ASCII Value of special Keys with _getch function
ESC >> 27
Right Arrow >> 77
Left Arrow >> 75
Up Arrow >> 72
Down Arrow >> 80
N >> 78
n >> 110
*/
// _getsch Takes character Pressed without having to press enter and converts into its ASCII Code and returns the ASCII Code Value
#include <iostream>
#include <iomanip>
#include <fstream>
#include <conio.h> // Library for _getch function
using namespace std;
struct Game {
    int Board[8][8], BoardSize, Score; // The 2D Array Holding All Values (Default 8x8 but the functions only work and print the size user chooses)
    int ScoreDigits, HighScoreDigits; // Score Variables // digit Counters started at 1 digit (0)
    int LegalMoves, RightMoves, LeftMoves, DownMoves, UpMoves, EmptySquares; // Variables to check if moving at a directions is allowed and fixes bugs
};
struct player {
    int Highscore{};
    string name;
};
int PlayerID = -1;
void StartGame();
void Leaderboard();
int CountDigits(int);
void CalculateScore(int);
void ShowBoard();
void TakeInput();
void MoveUp();
void MoveDown();
void MoveRight();
void MoveLeft();
void GameOver();
void CheckLevelUp();
void CheckLegalMoves();
void GenerateNumber();
void WriteScoresToFile();
void ReadScoresFromFile();
void ClearFile();
Game CurrentGame;
player Players[10];
bool Loaded = false;
int main() {
    if (!Loaded) {
        ReadScoresFromFile();
        Loaded = true;
    }

    system("cls");
    system("color 0f");
    cout << "-------------------------------------------------" << endl;
    if (CurrentGame.Score != 0) {
        cout << "|" << setw(30) << "Resume Game (R)" << setw(18) << "|" << endl;
        cout << "-------------------------------------------------" << endl;
    }
    cout << "|" << setw(30) << "New Game (N)" << setw(18) << "|" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "|" << setw(30) << "Leaderboard (L)" << setw(18) << "|" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "|" << setw(30) << "Exit (ESC)" << setw(18) << "|" << endl;
    cout << "-------------------------------------------------" << endl;
    int exitloop = 1;
    while (exitloop == 1) {
        int x = _getch();
        switch (x) {
        case 78:        case 110:
            StartGame(); // Calls the function to start the Game
            while (true) { // Infinite Loop to keep game going until No Legal Moves or User Presses ESC or User restarts game
                CalculateScore(0); // Fixes misalligned scores
                ShowBoard();// Prints the Board and Score
                CheckLevelUp();
                CheckLegalMoves(); // Checks If you can move
                TakeInput(); // Takes input from user then moves based on input
                GenerateNumber(); // Generates a new number in an empty square after moving tiles
            }
        case 27:
            WriteScoresToFile();
            exit(0);
        case 76:        case 108:
            Leaderboard();
            break;
        case 82: case 114:
            while (true) { // Infinite Loop to keep game going until No Legal Moves or User Presses ESC or User restarts game
                CalculateScore(0); // Fixes misalligned scores
                ShowBoard();// Prints the Board and Score
                CheckLevelUp();
                CheckLegalMoves(); // Checks If you can move
                TakeInput(); // Takes input from user then moves based on input
                GenerateNumber(); // Generates a new number in an empty square after moving tiles
            }
        default:
            exitloop++;
        }
    }
}
void StartGame() {
    PlayerID++;
    system("CLS");// Clears the console
    system("color af"); // Makes console light green background(a) and white text(f)
    cout << "Username: ";
    cin >> Players[PlayerID].name;
    for (int i = 0; i < PlayerID; i++) {
        if (Players[PlayerID].name == Players[i].name) {
            PlayerID = i;
        }
    }
    system("CLS");
    system("color af");
    cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << setw(33) << "Choose Board Size" << endl << setw(37) << "Pick a number from 4 to 8" << endl; // endlines to center the message
    int key = 0;
    while ((key > 56) || (key < 52)) { // Forces the user to press a key from 4 to 8 by looping untill the ascii value is between 52(4) and 56(8)
        key = _getch();
    }
    CurrentGame.BoardSize = key - 48; // lowest case 4 pressed ascii code = 52 so board size = 52-48 = 4 and highest case 8 pressed ascii = 56, 56-48 = 8
    system("color f0"); // Makes the console White Background (f) and black text (0) (hexadecimal colors)
    srand(time(nullptr)); // New random seed
    CurrentGame.Score = 0; // Resets Score
    for (int i = 0; i < CurrentGame.BoardSize; i++) { // Passes through each row
        for (int j = 0; j < CurrentGame.BoardSize; j++) { // Passes through each collumn
            CurrentGame.Board[i][j] = 0; // Clears the grid (New Game)
        }
    }
    for (int i = 1; i <= (CurrentGame.BoardSize / 2); i++) { // Generates a number of tiles at start based on board size, 4 = 2 tiles, 6 = 3 tiles, 8 = 4 tiles.
        int x = rand() % CurrentGame.BoardSize; // generates random x to be checked
        int y = rand() % CurrentGame.BoardSize; // generates random y to be checked
        if (CurrentGame.Board[x][y] != 0) { // If the Square is not empty
            while (CurrentGame.Board[x][y] != 0) { // Looks for an empty square
                x = rand() % CurrentGame.BoardSize; // generates random x to be checked
                y = rand() % CurrentGame.BoardSize; // generates random y to be checked
            }
        }
        CurrentGame.Board[x][y] = (2 * ((rand() % 2) + 1)); // Generates 2 or 4 on that tile after being checked for empty
    }
}
int CountDigits(int Number) { // Function to count the number of digits in a number for formatting
    int digitscount = 0;
    if (Number == 0)
        digitscount = 1;
    while (Number != 0) {
        Number = Number / 10;
        digitscount++;
    }
    return digitscount;
}
void CalculateScore(int ScoreToAdd) {
    CurrentGame.Score += ScoreToAdd; // Adds Recieved Score To the total Score
    if (CurrentGame.Score > Players[PlayerID].Highscore) // If the current score is higher than the highscore then it is the new highscore
        Players[PlayerID].Highscore = CurrentGame.Score;
    CurrentGame.ScoreDigits = CountDigits(CurrentGame.Score);
    CurrentGame.HighScoreDigits = CountDigits(Players[PlayerID].Highscore);
}
void Leaderboard() {
    system("cls");
    system("color 0f");
    cout << setw(16) << "Username" << setw(8) << "Scores" << endl;
    for (int j = 0; j < PlayerID; j++) {
        for (int i = 0; i < PlayerID; i++)
            if (Players[i].Highscore < Players[i + 1].Highscore) {
                int temp1 = Players[i].Highscore;
                string temp2 = Players[i].name;
                Players[i].Highscore = Players[i + 1].Highscore;
                Players[i].name = Players[i + 1].name;
                Players[i + 1].Highscore = temp1;
                Players[i + 1].name = temp2;
            }
    }
    for (int i = 0; i <= PlayerID; i++)
        cout << setw(16) << Players[i].name << setw(8) << Players[i].Highscore << endl;
     // Variable to store ASCII value of key pressed
    cout << endl << endl << "ESC To Exit" << endl << "N For New Game" << endl << "C for Clear"; // Prints End Message and Score
    while (true) { // Force the User to Press ESC or N or n
        int ExitKey = _getch(); // _getch Takes character Pressed and converts into its ASCII Code and returns the ASCII Code Value
        if ((ExitKey == 78) || (ExitKey == 110)|| (ExitKey == 27)) // Small n or capital N pressed or escape
            main(); // Goes to main Menu
        if (ExitKey == 67 || ExitKey == 99) {
            ClearFile();
            PlayerID = -1;
            Leaderboard();
        }
    }
}
void ShowBoard() {
    system("color f0"); // Makes the console White Background (f) and black text (0) (hexadecimal colors)
    int spaces = (24 - (CurrentGame.BoardSize * 3)); // Each Board Width is 6n+1 ex 4x4 is 25 wide 5x5 31 wide so leaves spaces based on size to center
    system("CLS"); // Clears The console screen
    for (int i = 0; i < 8 - CurrentGame.BoardSize; i++)
        cout << endl;
    cout << setw(11 + spaces) << "ESC to Exit" << setw(15 + (6 * (CurrentGame.BoardSize - 4)))/*increases spaces between the messages based on board size*/ << "N to New Game\n";
    cout << setw(6 + spaces) << "Score:" << setw(CurrentGame.ScoreDigits) << CurrentGame.Score << setw(7 + (6 - CurrentGame.HighScoreDigits) + (6 - CurrentGame.ScoreDigits) + (6 * (CurrentGame.BoardSize - 4))) /*increases spaces between the scoreboards based on board size*/ << "Highscore:" << setw(CurrentGame.HighScoreDigits) << Players[PlayerID].Highscore << endl; // Prints Score everytime Board updates
    for (int i = 0; i < CurrentGame.BoardSize; i++) { // Goes to all Rows
        cout << setw(6 + spaces); // Leaves Spaces
        for (int k = 0; k < CurrentGame.BoardSize; k++)
            cout << "------"; // ---- is Line Between Each Row
        cout << "-" << endl;// last dash to complete the board
        cout << setw(1 + spaces); // Leaves Spaces to left right to left before writing left to right
        for (int j = 0; j < CurrentGame.BoardSize; j++) { // Goes to all collumns
            if (CurrentGame.Board[i][j] != 0) { // Checks if the Square is not Empty (Value 0)
                if (CountDigits(CurrentGame.Board[i][j]) >= 3) { // Checks if the number is 3 digits or more to shift it one character right for symmetry
                    if (CountDigits(CurrentGame.Board[i][j]) == 5)// Checks if number is 5 digits
                        cout << "|" << setw(5) << CurrentGame.Board[i][j] << setw(1); // leaves 5 empty spaces then prints number then leaves another 1 spaces
                    else // Other case number is 3 or 4 digits
                        cout << "|" << setw(4) << CurrentGame.Board[i][j] << setw(2); // leaves 4 empty spaces then prints number then leaves another 2 spaces
                }
                else { // Other case the number is 2 digits or less
                    cout << "|" << setw(3) << CurrentGame.Board[i][j] << setw(3); // leaves 3 empty spaces then prints number then leaves another 3 spaces
                }
            }
            else // Other case the number is Empty (0) to not display it
            {
                cout << "|"; // | is Line Between Collumns
                cout << setw(6); // leaves 6 empty spaces if number not printed
            }
        }
        cout << "|" << endl; // adds a 5th | to complete grid
    }
    for (int x = 0; x < spaces; x++) // Leaves empty spaces based on CurrentGame.BoardSize to center board
        cout << " ";
    for (int k = 0; k < CurrentGame.BoardSize; k++)
        cout << "------"; // ---- is Line Between Each Row << endl; // 5th horizontal Line under Table
    cout << "-" << endl;
    cout << setw(25 + 12) << "Press Arrow Keys to move.";  // setwidth (Constant equal to number of characters to print) + Spaces based on board size
}
void CheckLevelUp() {
    if ((CurrentGame.Score >= 2500) && (CurrentGame.BoardSize == 4)) // if the score is 2500 or more and the board is already 4x4 it becomes 5x5
        CurrentGame.BoardSize++;
    if ((CurrentGame.Score >= 7500) && (CurrentGame.BoardSize == 5)) // if the score is 7500 or more and the board is already 5x5 it becomes 5x5
        CurrentGame.BoardSize++;
    if ((CurrentGame.Score >= 17500) && (CurrentGame.BoardSize == 6))// if the score is 17500 or more and the board is already 6x6 it becomes 7x7
        CurrentGame.BoardSize++;
    if ((CurrentGame.Score >= 37500) && (CurrentGame.BoardSize == 7))// if the score is 37500 or more and the board is already 7x7 it becomes 8x8
        CurrentGame.BoardSize++;
}
void CheckLegalMoves() {
    CurrentGame.LegalMoves = 0, CurrentGame.LeftMoves = 0, CurrentGame.RightMoves = 0, CurrentGame.DownMoves = 0, CurrentGame.UpMoves = 0, CurrentGame.EmptySquares = 0; // Resets Previous Count of legal moves to check again
    for (int i = 0; i < CurrentGame.BoardSize; i++) { // Check whole board if There is an Empty Square (Value 0)
        for (int j = 0; j < CurrentGame.BoardSize; j++) {
            if (CurrentGame.Board[i][j] == 0) {
                CurrentGame.LegalMoves++; // Counts every legal move to make sure they are not zero (game end)
                CurrentGame.EmptySquares++; // Counts every empty square to make sure they are not zero (fix for infinite loop in generatenumbers)
            }
        }
    } //      Checks for possible down moves
    for (int i = 0; i < (CurrentGame.BoardSize - 1); i++) { // Checks Rows 1 to n-1 (nth Row tiles can't move down out of array)
        for (int j = 0; j < CurrentGame.BoardSize; j++) {// Checks Each Collumn
            if (CurrentGame.Board[i][j] != 0) { // Only Checks Squares that are not empty (note: using double If's = AND of conditions)
                if (CurrentGame.Board[i + 1][j] == CurrentGame.Board[i][j]) { // Checks if tile under it is equal to it
                    CurrentGame.LegalMoves++;
                    CurrentGame.DownMoves++;
                }
                if (CurrentGame.Board[i + 1][j] == 0) // If there is a empty space under any tile and that tile is not zero then there is a down move
                    CurrentGame.DownMoves++;
            }
        }
    } //      Checks for possible up moves
    for (int i = CurrentGame.BoardSize - 1; i > 0; i--) { // Checks Rows n-1 to 2 (1st row tiles cant move up)
        for (int j = 0; j < CurrentGame.BoardSize; j++) {// Checks Each Collumn
            if (CurrentGame.Board[i][j] != 0) { // Only Checks Squares that are not empty
                if (CurrentGame.Board[i - 1][j] == CurrentGame.Board[i][j]) { // Checks if a tile has a Tile above it equal to it
                    CurrentGame.LegalMoves++;
                    CurrentGame.UpMoves++;
                }
                if (CurrentGame.Board[i - 1][j] == 0) // Checks if a tile has an empty tile above it
                    CurrentGame.UpMoves++;
            }
        }
    } //      Checks for possible left moves
    for (int i = 0; i < CurrentGame.BoardSize; i++) { // Checks each row if you can move Left (Checks if there The Square Left to each square in array is same value)
        for (int j = (CurrentGame.BoardSize - 1); j > 0; j--) { // Checks Collumns 4 to 2 (1st collumn tiles are on edge so cant move left)
            if (CurrentGame.Board[i][j] != 0) { // Checks if a tile is not empty
                if (CurrentGame.Board[i][j - 1] == CurrentGame.Board[i][j]) { // Checks if a tile has a tile to left of it that is equal to it
                    CurrentGame.LegalMoves++; CurrentGame.LeftMoves++;
                }
                if (CurrentGame.Board[i][j - 1] == 0) // Checks if a tile has an empty tile to the left of it
                    CurrentGame.LeftMoves++;
            }
        }
    } //      Checks for possible right moves
    for (int i = 0; i < CurrentGame.BoardSize; i++) { // Checks each row if you can move Right (Checks if there The Square Right to each square in array is same value)
        for (int j = 0; j < (CurrentGame.BoardSize - 1); j++) { // Checks collumns 1 to (CurrentGame.BoardSize -1) (4th collumn tiles are on edge so cant move right)
            if (CurrentGame.Board[i][j] != 0) { // Checks if a tile is not empty
                if (CurrentGame.Board[i][j + 1] == CurrentGame.Board[i][j]) { // Checks if a tile has a tile to right of it that is equal to it
                    CurrentGame.LegalMoves++; CurrentGame.RightMoves++;
                }
                if (CurrentGame.Board[i][j + 1] == 0) // Checks if a tile has an empty tile to the right of it
                    CurrentGame.RightMoves++;
            }
        }
    }
    if (CurrentGame.LegalMoves == 0) // If there is no legal moves game ends
        GameOver(); // Calls the gameover function
}
void TakeInput() {
    int exitloop = 1; // Variable to allow the loop to be executed once unless unmatched key
    while (exitloop == 1) { // Forces user to enter a valid input (up,down,right,left,esc,n, N only)
        exitloop = 0;
        int x = _getch(); // _getch Takes character pressed without having to press enter and returns the ASCII Code Value
        switch (x) {
        case 77: // ASCII Value of Right Arrow
            if (CurrentGame.RightMoves != 0)
                MoveRight();
            else
                exitloop++;
            break;
        case 75: // ASCII Value of Left Arrow
            if (CurrentGame.LeftMoves != 0)
                MoveLeft();
            else
                exitloop++;
            break;
        case 72: // ASCII Value of Up Arrow
            if (CurrentGame.UpMoves != 0)
                MoveUp();
            else
                exitloop++;
            break;
        case 80: // ASCII Value of Down Arrow
            if (CurrentGame.DownMoves != 0)
                MoveDown();
            else
                exitloop++;
            break;
        case 78: case 110:// ASCII Capital N for new game// ASCII small n for new game too
            main();
        case 27: // ASCII Value of ESC Key
            main();
        default: // None of the above
            exitloop++; // If the key pressed does not match any value the loop works again
        }
    }
}
void MoveUp() {
    for (int samelinefix = 0; samelinefix < (CurrentGame.BoardSize - 1); samelinefix++) { // Repeats this code n-1 n(Size of board) times to fix adjacent numbers not moving
        for (int i = 1; i < CurrentGame.BoardSize; i++) {// Checks Rows from bottom to top from row (CurrentGame.BoardSize-1 = bottom row index) to 2nd (1st row has no square above it)
            for (int j = 0; j < CurrentGame.BoardSize; j++) { // Checks All Collumns
                if (CurrentGame.Board[i - 1][j] == 0) { // Checks if square Above is empty (Value 0)
                    CurrentGame.Board[i - 1][j] = CurrentGame.Board[i][j]; // Puts the value of the square being checked in the square above it
                    CurrentGame.Board[i][j] = 0; // Removes the number from old square
                }
            }
        }
    }
    for (int i = 1; i < CurrentGame.BoardSize; i++) {// Checks Rows from bottom to top from row (CurrentGame.BoardSize-1 = bottom row index) to 2nd (1st row has no square above it)
        for (int j = 0; j < CurrentGame.BoardSize; j++) { // Checks All Collumns
            if (CurrentGame.Board[i - 1][j] == CurrentGame.Board[i][j]) { // Checks if the square above is the same number
                CalculateScore(2 * CurrentGame.Board[i][j]); // Sends the number of the new square to be added to total score
                CurrentGame.Board[i - 1][j] += CurrentGame.Board[i][j]; // Puts the sum of the 2 square in the square above
                CurrentGame.Board[i][j] = 0; // Removes the number from old square
            }
        }
    }
    for (int samelinefix = 0; samelinefix < (CurrentGame.BoardSize - 1); samelinefix++) { // Repeats this code n-1 n(Size of board) times to fix adjacent numbers not moving
        for (int i = 1; i < CurrentGame.BoardSize; i++) {// Checks Rows from bottom to top from row (CurrentGame.BoardSize-1 = bottom row index) to 2nd (1st row has no square above it)
            for (int j = 0; j < CurrentGame.BoardSize; j++) { // Checks All Collumns
                if (CurrentGame.Board[i - 1][j] == 0) { // Checks if square Above is empty (Value 0)
                    CurrentGame.Board[i - 1][j] = CurrentGame.Board[i][j]; // Puts the value of the square being checked in the square above it
                    CurrentGame.Board[i][j] = 0; // Removes the number from old square
                }
            }
        }
    }
    // If no square above empty or equal to any square function does nothing
}
void MoveDown() {
    for (int samelinefix = 0; samelinefix < (CurrentGame.BoardSize - 1); samelinefix++) { // Repeats this code (CurrentGame.BoardSize -1) times to fix adjacent numbers not moving
        for (int i = (CurrentGame.BoardSize - 2); i >= 0; i--) { // Checks Rows from top to bottom from row 1 to (CurrentGame.BoardSize -1) (4th row has no square under it)
            for (int j = 0; j < CurrentGame.BoardSize; j++) { // Checks All Collumns
                if (CurrentGame.Board[i + 1][j] == 0) { // Checks if square under is empty (Value 0)
                    CurrentGame.Board[i + 1][j] = CurrentGame.Board[i][j]; // Puts the number in old square in the empty square under it
                    CurrentGame.Board[i][j] = 0; // Removes the number from old square
                }

            }
        }
    }
    for (int i = (CurrentGame.BoardSize - 2); i >= 0; i--) { // Checks Rows from top to bottom from row 1 to (CurrentGame.BoardSize -1) (4th row has no square under it)
        for (int j = 0; j < CurrentGame.BoardSize; j++) { // Checks All Collumns
            if (CurrentGame.Board[i + 1][j] == CurrentGame.Board[i][j]) { // Checks if the square under is the same number
                CalculateScore(2 * CurrentGame.Board[i][j]); // Increases score by the value of the new square
                CurrentGame.Board[i + 1][j] += CurrentGame.Board[i][j]; // Puts the sum of the 2 square in the square under
                CurrentGame.Board[i][j] = 0; // Removes the number from old square
            }
        }
    }
    for (int samelinefix = 0; samelinefix < (CurrentGame.BoardSize - 1); samelinefix++) { // Repeats this code (CurrentGame.BoardSize -1) times to fix adjacent numbers not moving
        for (int i = (CurrentGame.BoardSize - 2); i >= 0; i--) { // Checks Rows from top to bottom from row 1 to (CurrentGame.BoardSize -1) (4th row has no square under it)
            for (int j = 0; j < CurrentGame.BoardSize; j++) { // Checks All Collumns
                if (CurrentGame.Board[i + 1][j] == 0) { // Checks if square under is empty (Value 0)
                    CurrentGame.Board[i + 1][j] = CurrentGame.Board[i][j]; // Puts the number in old square in the empty square under it
                    CurrentGame.Board[i][j] = 0; // Removes the number from old square
                }

            }
        }
    }
}
void MoveRight() {
    for (int samelinefix = 0; samelinefix < (CurrentGame.BoardSize - 1); samelinefix++) { // Repeats this code (CurrentGame.BoardSize -1) times to fix adjacent numbers not moving
        for (int i = 0; i < CurrentGame.BoardSize; i++) { // Checks All Rows
            for (int j = (CurrentGame.BoardSize - 2); j >= 0; j--) { // Checks Collumns left to right Collumns 1 to (CurrentGame.BoardSize -1) (4th one does not have a square to the right of it)
                if (CurrentGame.Board[i][j + 1] == 0) { // Checks if square to Right is empty (Value 0)
                    CurrentGame.Board[i][j + 1] = CurrentGame.Board[i][j]; // Puts the Value of square in square to the right of it (Moving Square)
                    CurrentGame.Board[i][j] = 0; // Removes the number from the old square
                }

            }
        }
    }
    for (int i = 0; i < CurrentGame.BoardSize; i++) { // Checks All Rows
        for (int j = (CurrentGame.BoardSize - 2); j >= 0; j--) { // Checks Collumns left to right Collumns 1 to (CurrentGame.BoardSize -1) (4th one does not have a square to the right of it)
            if (CurrentGame.Board[i][j + 1] == CurrentGame.Board[i][j]) { // Checks if square to Right is the same value
                CalculateScore(2 * CurrentGame.Board[i][j]); // Sends the value of the new square to be added
                CurrentGame.Board[i][j + 1] += CurrentGame.Board[i][j]; // Puts the sum of the 2 square on the square on the right
                CurrentGame.Board[i][j] = 0; // Removes the number from old square
            }
        }
    }
    for (int samelinefix = 0; samelinefix < (CurrentGame.BoardSize - 1); samelinefix++) { // Repeats this code (CurrentGame.BoardSize -1) times to fix adjacent numbers not moving
        for (int i = 0; i < CurrentGame.BoardSize; i++) { // Checks All Rows
            for (int j = (CurrentGame.BoardSize - 2); j >= 0; j--) { // Checks Collumns left to right Collumns 1 to (CurrentGame.BoardSize -1) (4th one does not have a square to the right of it)
                if (CurrentGame.Board[i][j + 1] == 0) { // Checks if square to Right is empty (Value 0)
                    CurrentGame.Board[i][j + 1] = CurrentGame.Board[i][j]; // Puts the Value of square in square to the right of it (Moving Square)
                    CurrentGame.Board[i][j] = 0; // Removes the number from the old square
                }

            }
        }
    }
}
void MoveLeft() {
    for (int samelinefix = 0; samelinefix < (CurrentGame.BoardSize - 1); samelinefix++) { // Repeats this code (CurrentGame.BoardSize -1) times to fix adjacent numbers not moving
        for (int i = 0; i < CurrentGame.BoardSize; i++) {// Checks All Rows
            for (int j = 1; j < CurrentGame.BoardSize; j++) { // Checks Collumns right to left Collumns 4 to 2 (1st one does not have a square to the Left of it)
                if (CurrentGame.Board[i][j - 1] == 0) { // Checks if square to Left is empty (Value 0)
                    CurrentGame.Board[i][j - 1] = CurrentGame.Board[i][j]; // Puts the Value of square in the square to the left of it (Moving Square)
                    CurrentGame.Board[i][j] = 0; // Removes the number from the old square
                }
            }
        }
    }
    for (int i = 0; i < CurrentGame.BoardSize; i++) {// Checks All Rows
        for (int j = 1; j < CurrentGame.BoardSize; j++) { // Checks Collumns right to left Collumns 4 to 2 (1st one does not have a square to the Left of it)
            if (CurrentGame.Board[i][j - 1] == CurrentGame.Board[i][j]) { // Checks if square to left is the same value
                CalculateScore(2 * CurrentGame.Board[i][j]); // Increases score by the value of the new square
                CurrentGame.Board[i][j - 1] += CurrentGame.Board[i][j]; // Puts the sum of the 2 square on the square on the right
                CurrentGame.Board[i][j] = 0; // Removes the number from old square
            }
        }
    }
    for (int samelinefix = 0; samelinefix < (CurrentGame.BoardSize - 1); samelinefix++) { // Repeats this code (CurrentGame.BoardSize -1) times to fix adjacent numbers not moving
        for (int i = 0; i < CurrentGame.BoardSize; i++) {// Checks All Rows
            for (int j = 1; j < CurrentGame.BoardSize; j++) { // Checks Collumns right to left Collumns 4 to 2 (1st one does not have a square to the Left of it)
                if (CurrentGame.Board[i][j - 1] == 0) { // Checks if square to Left is empty (Value 0)
                    CurrentGame.Board[i][j - 1] = CurrentGame.Board[i][j]; // Puts the Value of square in the square to the left of it (Moving Square)
                    CurrentGame.Board[i][j] = 0; // Removes the number from the old square
                }
            }
        }
    }
}
void GameOver() {
    system("CLS"); // Clears the screen
    system("color 4f"); // light red background color (4) with white font color (f)
    cout << setw(32) << "GAME OVER!" << endl << "Username: " << Players[PlayerID].name << endl << "Final Score: " << CurrentGame.Score << endl << "Highest Score: " << Players[PlayerID].Highscore << endl << endl << "ESC To Exit" << endl << "N For New Game" << endl << "L for Leaderboard" << endl; // Prints End Message and Score
    CurrentGame.Score = 0;
    while (true) { // Force the User to Press ESC or N or n
        int ExitKey = _getch(); // _getch Takes character Pressed and converts into its ASCII Code and returns the ASCII Code Value
        if (ExitKey == 78 || ExitKey == 110 || ExitKey == 27) // Small n or capital N pressed or Escape
            main(); // Starts new game
        if ((ExitKey == 76) || (ExitKey == 108))
            Leaderboard();
    }
}
void GenerateNumber() {
    if (CurrentGame.EmptySquares != 0) { // Checks if there is an empty square before generating another to escape infinite loop of searching for new square
        int x = rand() % CurrentGame.BoardSize; // Random x from 0 to (CurrentGame.BoardSize -1)
        int y = rand() % CurrentGame.BoardSize; // Random y from 0 to (CurrentGame.BoardSize -1)
        if (CurrentGame.Board[x][y] != 0) { // Checks if the random position in array[x][y] is empty or not
            while (CurrentGame.Board[x][y] != 0) { // Loops untill Program finds an empty Space
                x = rand() % CurrentGame.BoardSize; // Generates new x
                y = rand() % CurrentGame.BoardSize; // Generates new y
            }
        }
        CurrentGame.Board[x][y] = 2 * (rand() % 2 + 1); // Puts 2 or 4 in the Empty Place it found
    }
}
void WriteScoresToFile() {
    ofstream outFile("../Scores.txt");
    if (outFile.is_open()) {
        for (int i = 0; i <= PlayerID; i++) {
            if (Players[i].Highscore != 0)
                outFile << Players[i].name << " " << Players[i].Highscore << endl;
        }
        outFile.close();
    } else {
        cout << "Unable to open file for writing." << endl;
    }
}
void ReadScoresFromFile() {
    ifstream inFile("../Scores.txt");
    if (inFile.is_open()) {
        PlayerID = -1;
        string name;
        int highscore;
        while (inFile >> name >> highscore) {
            PlayerID++;
            Players[PlayerID].name = name;
            Players[PlayerID].Highscore = highscore;
        }
        inFile.close();
    } else {
        cout << "Unable to open file for reading." << endl;
    }
}
void ClearFile() {
    ofstream outFile("../Scores.txt");
    if (outFile.is_open()) {
        outFile.clear();
        outFile.close();
    } else {
        cout << "Unable to open file for Clearing." << endl;
    }
}