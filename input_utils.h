#ifndef INPUT_UTILS_H
#define INPUT_UTILS_H

#include "tetromino.h"

typedef struct // Stores the input parameters for a sequence
{
    char Sequence[MAX_SEQUENCE_SIZE];
    int Size;
    int AllowRotation;
    int ColumnCounterPermutations[MAX_SEQUENCE_SIZE]; // Stores the number of permutations which an increment in each piece's column counter represents
} sequence_params;

// Display 'prompt' (must be null-terminated) and return the char input by the user. If input empty or longer than one char, display 'prompt' again until a valid input
char getChar(char * prompt);

// Display 'prompt' (must be null-terminated) and return the int input by the user. If input is invalid, display 'prompt' again until a valid input
int getInt(char * prompt);

// Receive the sequence to be solved and its length into 'sequenceParams'. Return TRUE if sequence is valid, FALSE otherwise.
int getSequence(sequence_params *sequenceParams);

// Receive whether rotation is allowed and return TRUE if it is, FALSE otherwise. Repeat until input is valid.
int getAllowRotation();

// Receive the input parameters for a new sequence into 'sequenceParams'
void getSequenceParams(sequence_params *sequenceParams);

// Receive and return the tetromino which the user wants to drop in debug mode. Repeat until input is a valid tetromino
char getTetrominoToDrop();

// Receive the rotation the user would like to apply to 'tet' when dropping in debug mode. Convert the rotation angle into an index pointing to the struct of 'tet' rotated by the input rotation, and return it. Repeat until input is a valid rotation
int getRotation(char tet);

// Receive and return the column which the user wants to drop a tetromino with width 'tetrominoWidth' into in debug mode
int getColumnToDropIn(int tetrominoWidth);

// Receive and return user's debug menu selection. Repeat until input is a valid menu item
char getDebugMenuInput();

#endif