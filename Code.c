#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <Windows.h>
#define SIZE 8

// Initializing board
char board[SIZE][SIZE] = {
    {'X', ' ', 'X', ' ', 'X', ' ', 'X', ' '},
    {' ', 'X', ' ', 'X', ' ', 'X', ' ', 'X'},
    {'X', ' ', 'X', ' ', 'X', ' ', 'X', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'O', ' ', 'O', ' ', 'O', ' ', 'O'},
    {'O', ' ', 'O', ' ', 'O', ' ', 'O', ' '},
    {' ', 'O', ' ', 'O', ' ', 'O', ' ', 'O'}};
// for saving, resume game state
void pauseOrResume(char board[][SIZE], int game)
{
    // 1 means newgame, 3 means pause
    if (game == 1 || game == 3)
    {
        FILE *ptr = fopen("Data.txt", "w");
        for (int i = 0; i < SIZE; ++i)
        {
            fputs(board[i], ptr);
        }
    }
    else if (game == 2)
    {
        FILE *ptr = fopen("Data.txt", "r");
        // if file doesnt exist then dont write anything to array, because array would become empty
        if (ptr != NULL)
        {
            for (int i = 0; i < SIZE; ++i)
            {
                for (int j = 0; j < SIZE; ++j)
                {
                    board[i][j] = fgetc(ptr);
                }
            }
        }
    }
}

// To confirm is a piece is present on the location
int confirm_piece(char board[][SIZE], int current_row, int current_column, char ch)
{
    // c for col
    // r for row
    if (board[current_row][current_column] == ch)
        return 1;
    else
        return 0;
} // end of confirm_piece

// Confirming if there is a space or not (when moving to a location where a piece is already present)
int confirm_space(char board[][SIZE], int final_row, int final_column)
{
    // if there is space or not
    if (board[final_row][final_column] == 'O' || board[final_row][final_column] == 'X')
        return 0;
    else
        return 1;
} // end of confirm_space

// this function is responsible for removing a piece on the board
int capture_piece(char board[][SIZE], int diag_row, int diag_col, int current_row, int current_col, bool isComputer)
{
    char current_piece = board[current_row][current_col];
    char diag_piece;

    // checks which piece is at current location
    if (current_piece == 'X' || current_piece == 'K')
    {
        diag_piece = 'O';
    }
    else if (current_piece == 'O' || current_piece == 'Q')
    {
        diag_piece = 'X';
    }

    // if middle location empty, return 0
    if (board[diag_row][diag_col] == ' ')
    {
        if (isComputer == false)
            printf("INVALID LOCATION ENTERED: YOU CAN ONLY MOVE ONE SPACE DIAGONALLY!\n");
        return 0;
    }
    // if middle location has the players piece present in current location return 0
    else if (board[diag_row][diag_col] == current_piece)
    {
        if (isComputer == false)
            printf("INVALID LOCATION ENTERED: YOUR PIECE IS ALREADY THERE DIAGONALLY\n");
        return 0;
    }
    // replace the middle piece with space
    else
    {
        if (board[diag_row][diag_col] == diag_piece ||
            (board[diag_row][diag_col] == 'K' && current_piece == 'O') ||
            (board[diag_row][diag_col] == 'Q' && current_piece == 'X') ||
            (board[diag_row][diag_col] == 'Q' && current_piece == 'K') ||
            (board[diag_row][diag_col] == 'K' && current_piece == 'Q'))
        {
            board[diag_row][diag_col] = ' ';
            return 1;
        }
    }

} // end of capture_piece

void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void pieceColor(char piece)
{
    if (piece == 'X')
    {
        // 4 is the color red
        setColor(4);
    }
    else if (piece == 'O')
    {
        // 1 is the color blue
        setColor(1);
    }
} // end of pieceColor

// Display the board
void display(char board[][SIZE])
{
    // 14 is the color yellow
    setColor(14);
    printf("  ==================\n");
    printf("  ||0|1|2|3|4|5|6|7|\n");
    printf(" _|=================\n");

    for (int i = 0; i < SIZE; i++)
    {
        setColor(14);
        printf("|%c|", i + 65);
        for (int j = 0; j < SIZE; j++)
        {
            setColor(14);
            printf("|");
            pieceColor(board[i][j]);
            if (board[i][j] == 'K')
            {
                setColor(4);
            }
            if (board[i][j] == 'Q')
            {
                setColor(1);
                printf("K");
            }
            else
                printf("%c", board[i][j]);
        }
        setColor(14);
        printf("|");
        if (i == 7)
        {
            printf("\n====================");
        }
        printf("\n");
        // to reset back to white for the rest
        setColor(15);
    }

    printf("Player 1 is: ");
    setColor(4);
    printf("X\n");
    setColor(15);
    printf("Player 2 is: ");
    setColor(1);
    printf("O\n");
    setColor(15);

    return;
} // end of display

// this function moves a piece, and replaces space in a previous location the piece was at
// it also is responsible for creating king piece
void move_piece(char board[][SIZE], int irow, int icol, int frow, int fcol, char a)
{
    // checks if a piece should become king
    // if piece reaches final row
    // player 1
    if (a == 'X' && frow == 7)
    {
        board[irow][icol] = ' ';
        char b = 'K';
        board[frow][fcol] = b;
    }
    // player 2 condition
    else if (a == 'O' && frow == 0)
    {
        board[irow][icol] = ' ';
        board[frow][fcol] = 'Q';
    }
    // normal piece condition
    else
    {
        board[irow][icol] = ' ';
        board[frow][fcol] = a;
    }
    return;
} // end of move_piece

// king function
int king(char board[][SIZE], int current_row, int current_col, int final_row, int final_col, int up_or_down, int left_or_right, int b, int c, bool isComputer)
{
    // b and c are mod of up_or_down and left_or_right
    // if it can move 2 rows and 2 cols (simple words: will it move 2 spaces diagonally)
    if (b == 2 && c == 2)
    {
        // basically checks if king is to move up or downwards
        if (up_or_down > 0)
        {
            switch (left_or_right)
            {
            // when moving rightwards
            case 2:
                // if capture_piece is not 1 then that means incorrect final location was entered
                if (capture_piece(board, final_row + 1, final_col + 1, current_row, current_col, false) != 1)
                {
                    return 1;
                }
                break;
            // when moving leftwards
            case -2:
                if (capture_piece(board, final_row + 1, final_col - 1, current_row, current_col, false) != 1)
                {
                    return 1;
                }
                break;
            default:
                return 0;
            } // end of switch
            return 0;
        } // end of if (up_or_down > 0)
        // same as above but for downwards
        else if (up_or_down < 0)
        {
            switch (left_or_right)
            {
            case -2:
                if (capture_piece(board, final_row - 1, final_col - 1, current_row, current_col, false) != 1)
                {
                    return 1;
                }
                break;
            case 2:
                if (capture_piece(board, final_row - 1, final_col + 1, current_row, current_col, false) != 1)
                {
                    return 1;
                }
                break;
            default:
                return 0;
            } // end of switch
            return 0;
        } // end of if (left_or_right > 0)
    }     // end of if b == 2 && a == 2

    // for king
    else
        // if user wants to move 1 space only
        if (b == 1 && c == 1)
        {
            // if piece already there or not
            if (board[final_row][final_col] != ' ')
            {
                if (isComputer == false)
                    printf("INVALID LOCATION ENTERED: A PIECE IS ALREADY THERE\n");
                return 1;
            }
            else
                return 0;
        }
        else
            return 0;
} // end of king function

// this function can take input from 1st person
void playerInput(char board[][SIZE], char piece, bool flag)
{
    system("cls");
    display(board);
    if (piece == 'X' || piece == 'K')
    {
        printf("Turn of ");
        setColor(4);
        printf("Player 1\n");
        setColor(15);
    }
    else
    {
        printf("Turn of ");
        setColor(1);
        printf("Player 2\n");
        setColor(15);
    }
    // to find current location of the piece
    int current_row, current_col;
    char current_letter, final_letter, choice;
    // to get location to move to
    int final_row, final_col;
    int i = 1;
    int a, fail = 0;
    // base condition for recursion
    if (flag == false)
    {
        return;
    }

    // loop ends when correct location is entered by the user
    while (i == 1)
    {
        printf("Enter Row, Column Of The Piece You Want To Move: ");
        scanf(" %c%d", &current_letter, &current_col);
        current_row = toupper(current_letter) - 65;

        // confirm_piece returns 1 if input was valid
        a = confirm_piece(board, current_row, current_col, piece);
        // checks if input was out of bounds
        if ((current_row < 0 || current_row > 7) || (current_col < 0 || current_col > 7))
        {
            printf("INVALID INPUT: OUT OF BOUNDS!\n");
        }
        // a = 1 means input by user was correct
        else
        {
            if (a == 0)
            {
                // if king or queen is there then i = -1, so that loop can end
                if ((board[current_row][current_col] == 'K' && piece == 'X') || (board[current_row][current_col] == 'Q' && piece == 'O'))
                    i = -1;
                else
                    printf("INVALID LOCATION ENTERED: YOU DONT HAVE A PIECE HERE\n");
            }
            else
                i = -1;
        }
    }

    i = 1;
    // loop ends when location to move to is correctly entered by the user
    while (i > 0)
    {
        printf("Enter Row, Column Where You Want To Move: ");
        scanf(" %c%d", &final_letter, &final_col);
        final_row = toupper(final_letter) - 65;

        // returns 1 if the location to move to is empty
        a = confirm_space(board, final_row, final_col);

        int difrow = current_row - final_row;
        int difcol = current_col - final_col;
        int b = abs(difrow); // use as a mod function for row
        int c = abs(difcol); // use as a mod function for col

        if ((final_row < 0 || final_row > 7) || (final_col < 0 || final_col > 7))
        {
            fail++;
            printf("INVALID INPUT: OUT OF BOUNDS!\n");
        }
        // to check if user wants to move to the current location
        else if (b == 0 || c == 0)
        {
            fail++;
            printf("You cannot move to the location you are already at\n");
            // to check if user wants to move more than 2 locations
        }
        else if (b > 2 || c > 2)
        {
            fail++;
            printf("You cannot move to this location\n");
        }
        else if (b != c)
        {
            fail++;
            printf("INVALID LOCATION ENTERED: YOU CAN ONLY MOVE DIAGONALLY\n");
        }
        else if (board[current_row][current_col] == 'K' || board[current_row][current_col] == 'Q')
        {
            // ki stores 0 if king can move, 1 if king cant move
            int ki = king(board, current_row, current_col, final_row, final_col, difrow, difcol, b, c, false);

            if (ki == 0)
            {
                i = -1;
            }
        }
        else if (a == 0)
        {
            printf("INVALID LOCATION ENTERED: A PIECE IS ALREADY THERE\n");
            fail++;
        }
        // this prevents the player 1 from moving upwards and player 2 downwards
        else if ((difrow > 0 && piece == 'X') || (difrow < 0 && piece == 'O'))
        {
            printf("INVALID LOCATION ENTERED!");
            fail++;
        }
        // b = 2 and c = 2 means user is to move two locations
        else if (b == 2 && c == 2)
        {
            // if X then subtract row, if O then add to row for capture_piece function
            if (piece == 'X')
            {
                final_row--;
            }
            else if (piece == 'O')
            {
                final_row++;
            }
            if ((difrow < 0 && piece == 'X') || (difrow > 0 && piece == 'O'))
            {
                switch (difcol)
                {
                case -2:
                    // final_row, final_col-1 gives the middle location
                    if (capture_piece(board, final_row, final_col - 1, current_row, current_col, false) != 1)
                    {
                        fail++;
                        break;
                    }
                    else
                    {
                        i = -1;
                        break;
                    }
                case 2:
                    if (capture_piece(board, final_row, final_col + 1, current_row, current_col, false) != 1)
                    {
                        fail++;
                        break;
                    }
                    else
                    {
                        i = -1;
                        break;
                    }
                default:
                    i = -1;
                } // end of switch
            }     // end of if difcol > 0
            // reset the value of final_row to the way it was before so that it can move the piece correctly
            if (piece == 'X')
            {
                final_row++;
            }
            else if (piece == 'O')
            {
                final_row--;
            }
        } // end of if b == 2 && c == 2

        else if (b == 1 && c == 1)
        {
            i = -1;
            break;
        }
        // if user has failed even once then let him choose another piece
        if (fail > 0)
        {
            printf("Do you want to choose another piece? (Y/N): ");
            scanf(" %c", &choice);
            if (toupper(choice) == 'Y')
            {
                playerInput(board, piece, true);
                return;
            }
        }
    }

    // moves the piece to the final location after middle location has been set to space
    move_piece(board, current_row, current_col, final_row, final_col, board[current_row][current_col]);
    return;
} // end of input of player

void computer(char board[][SIZE], char piece)
{
    system("cls");
    display(board);
    printf("Computer Is Now Going To Move A Piece");
    Sleep(2000);
    int current_row, current_col, final_row, final_col, a, i;
    while (1)
    {
        // random value 0 to 7 for row and col
        current_row = rand() % 8;
        current_col = rand() % 8;
        // a stores 1 if piece is found there
        a = confirm_piece(board, current_row, current_col, piece);
        if (a == 0)
        {
            // if piece not found then see if at that location there is Q piece
            if (board[current_row][current_col] == 'Q')
                break; // break out of loop of Q is there
        }
        else
            break; // break out of loop if a is not zero
    }
    i = 1;
    int count = 0;
    while (i == 1)
    {
        // counts the number of times loop repeats
        count++;
        // if count exceeds then recursive call so a different piece can be picked
        // this is done so that after 100 tries of moving the current piece, the computer finds a different "O" piece instead
        if (count == 100)
        {
            computer(board, piece);
            return;
        }
        // finding final row and making sure its between 0 and 7 inclusive
        do
        {
            if (board[current_row][current_col] == 'Q')
            {
                if (rand() % 2 == 0)
                    final_row = current_row - rand() % 3;
                else
                    final_row = current_row + rand() % 3;
            }
            else
                final_row = current_row - rand() % 3;
        } while (final_row < 0 || final_row > 7);
        // finding final col and making sure its between 0 and 7 inclusive
        do
        {
            if (rand() % 2 == 0)
                final_col = current_col - rand() % 3;
            else
                final_col = current_col + rand() % 3;
        } while (final_col < 0 || final_col > 7);
        // a stores 1 if there is space where we are moving to
        a = confirm_space(board, final_row, final_col);

        int difrow = current_row - final_row;
        int difcol = current_col - final_col;
        int b = abs(difrow);
        int c = abs(difcol);

        if (b == 0 || c == 0)
            continue; // repeat the loop if b or c are zero
        if (b > 2 || c > 2)
            continue; // repeat the loop if b or c are more than 2
        else if (b != c)
            continue; // repeat the loop if b is not equals to c
        else if (board[current_row][current_col] == 'Q')
        {
            // if its queen the call king function
            int ki = king(board, current_row, current_col, final_row, final_col, difrow, difcol, b, c, true);

            if (ki == 0)
                i = -1; // if ki is zero that means king moved then get out of loop
        }
        else if (a == 0)
            continue; // repeat the loop if no space
        else if (difrow < 0)
            continue; // repeat the loop if trying to move downwards
        else if (b == 2 && c == 2)
        {
            switch (difcol)
            {
            case -2:
                if (capture_piece(board, final_row + 1, final_col - 1, current_row, current_col, true) != 1)
                    continue; // if couldnt capture the piece then repeat the loop
                else
                {
                    i = -1;
                    break;
                }
            case 2:
                if (capture_piece(board, final_row + 1, final_col + 1, current_row, current_col, true) != 1)
                    continue; // if if couldnt capture the piece then repeat the loop
                else
                {
                    i = -1;
                    break;
                }
            }
        }
        else if (b == 1 && c == 1)
        {
            i = -1;
        }
    }
    move_piece(board, current_row, current_col, final_row, final_col, board[current_row][current_col]);
    return;
} // end of computer

// this functon checks for base condition to stop game
int check(char board[][SIZE], char searchChar, char kingOrQueen)
{
    bool flag = true;
    // search for player piece

    for (int i = 0; i < SIZE; i++)
    {
        // strchr checks every column of a row without the need of another for loop
        if (strchr(board[i], searchChar) != NULL || strchr(board[i], kingOrQueen) != NULL)
        {
            // if a piece is found then set to false and break
            flag = false;
            break;
        }
    }

    if (flag == true)
    {
        return 0;
    }
    return 1;
} // end of check

// play game in the function
void playGame(char board[][SIZE], int i, int playerOrComputer)
{
    char choice;
    if (check(board, 'X', 'K') == 0)
    {
        setColor(1);
        printf("Player 2 Wins\n");
        return;
    }
    if (check(board, 'O', 'Q') == 0)
    {
        setColor(4);
        printf("Player 1 Wins\n");
        return;
    }

    if (i == 3000)
    {
        int newResume;
        printf("Do You Want To Start A New Game or Resume A Previous One:\n1- New Game\n2- Resume\nInput: ");
        scanf("%d", &newResume);
        pauseOrResume(board, newResume);
    }

    // if i is even then player 1 plays
    if (i % 2 == 0)
    {
        // Call the function for player 1 input
        playerInput(board, 'X', true);
    }
    // else player 2 plays
    else if (playerOrComputer == 1)
    {
        computer(board, 'O');
    }
    else
    {
        playerInput(board, 'O', true);
    }

    system("cls");
    display(board);

    if (check(board, 'X', 'K') == 0)
    {
        setColor(1);
        printf("Player 2 Wins\n");
        return;
    }
    if (check(board, 'O', 'Q') == 0)
    {
        setColor(4);
        printf("Player 1 Wins\n");
        return;
    }

    if (i % 2 != 0)
    {
        // cls and display again so that player 2 updated array is shown to user
        system("cls");
        display(board);
        printf("Do You Want To Pause?(Y/N): ");
        scanf(" %c", &choice);
        if (toupper(choice) == 'Y')
        {
            pauseOrResume(board, 3);
            return;
        }
    }

    playGame(board, i - 1, playerOrComputer);

    return;
} // end of playGame

int main(int argc, char const *argv[])
{
    char agree;

    setColor(8);
    printf("Objective: \n");
    setColor(15);
    printf("The objective of checkers is to capture all of the opponent's pieces or block them in such a way that they cannot make a move.\n");
    setColor(8);
    printf("Board Setup:\n");
    setColor(15);
    printf("1- The game is played on an 8x8 square board.\n2- Each player starts with 12 pieces, X marked as red and O marked as blue\n");
    setColor(8);
    printf("Movement:\n");
    setColor(15);
    printf("Players take turns moving their pieces diagonally.\n1- Normal pieces (not kings) can only move forward.\n2- Kings can move diagonally in any direction.\n");
    setColor(8);
    printf("Capture:\n");
    setColor(15);
    printf("If a player's piece is in a position to capture an opposing piece, it is optional for the opponent to make the capture. The opponent is not forced to capture if they have the option to do so.\n");
    printf("1- Capturing is done by jumping over an opponent's piece, and the captured piece is removed from the board.\n2- If a piece reaches the last row on the opponent's side, it becomes a king.\n");
    setColor(8);
    printf("Kings:\n");
    setColor(15);
    printf("Kings have the ability to move diagonally in any direction.\n1- When capturing, kings can move both forward and backward over the opponent's pieces.\n");
    setColor(8);
    printf("Winning the Game:\n");
    setColor(15);
    printf("The game can be won by a player if the opponent loses all their pieces. A player wins when the opponent has no more pieces remaining on the board.\n");
    setColor(8);
    printf("Maximum of Two Jumps:\n");
    setColor(15);
    printf("A player is allowed to perform a maximum of two jumps with the a piece to capture another piece during their turn. Even if more than one opposing piece can be captured in a sequence, the player is limited to a maximum of two jumps per turn.\n");

    printf("\nDo You Agree With The Rules? (Y/N): ");
    scanf(" %c", &agree);

    if (toupper(agree) == 'N')
        return 0;

    system("cls");
    display(board);

    int choice;
    printf("1: Player Vs Computer\n2: Player 1 Vs Player 2\nInput: ");
    scanf("%d", &choice);

    system("cls");
    display(board);

    playGame(board, 3000, choice);
    Sleep(5000);

    return 0;
}
