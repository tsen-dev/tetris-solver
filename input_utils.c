#include <stdio.h>

#include "bool.h"
#include "input_utils.h"
#include "tetromino.h"
#include "grid.h"

// Display 'prompt' (must be null-terminated) and return the char input by the user. If input empty or longer than one char, display 'prompt' again until a valid input
char getChar(char * prompt)
{
    char input;

    while (TRUE)
    {
        printf(prompt);
        input = getchar();

        if (input == '\n') continue; // Input zero characters, ask again     
        else if (getchar() != '\n') // Input longer than one character, ask again
        {
            while (getchar() != '\n'); // Consume the characters remaining in the input buffer
            continue;
        }

        return input;   
    }
}

// Display 'prompt' (must be null-terminated) and return the int input by the user. If input is invalid, display 'prompt' again until a valid input
int getInt(char * prompt)
{
    int input;

    while (TRUE)
    {
        printf(prompt);
        
        if (scanf("%d", &input) == FALSE) // Input not an int 
        {
            while (getchar() != '\n'); // Consume the characters remaining in the input buffer 
            continue;        
        }
        
        else if (getchar() != '\n') // Input longer than an int 
        {
            while (getchar() != '\n'); // Consume the characters remaining in the input buffer 
            continue;            
        }

        else return input;
    }
}

// Receive the sequence to be solved and its length into 'sequenceParams'. Return TRUE if sequence is valid, FALSE otherwise.
int getSequence(sequence_params *sequenceParams)
{
    char input;
    sequenceParams->Size = 0;

    printf("Enter your sequence (max length %d):\n", MAX_SEQUENCE_SIZE);

    while ((input = getchar()) != '\n' && sequenceParams->Size < MAX_SEQUENCE_SIZE) 
    {
        switch (input)
        {
            case 'I':
            case 'O':
            case 'T':
            case 'J':
            case 'L':
            case 'S':
            case 'Z':
                sequenceParams->Sequence[sequenceParams->Size] = input;
                sequenceParams->Size += 1;
                break;        

            default:
                printf("'%c' is not a valid piece!\n", input);
                while (getchar() != '\n');
                return FALSE;
        }
    }

    if (input != '\n') 
    {   
        printf("Sequence's size is longer than MAX_SEQUENCE_SIZE (%d)!\n", MAX_SEQUENCE_SIZE);     
        while (getchar() != '\n');        
        return FALSE;
    }

    return TRUE;
}

// Receive whether rotation is allowed and return TRUE if it is, FALSE otherwise. Repeat until input is valid.
int getAllowRotation()
{
    char input;    

    while (TRUE)
    {
        input = getChar("Allow rotations? Enter 'Y' or 'N'\n\0");
        
        switch (input)
        {
            case 'Y':   
                return TRUE;
            case 'N':
                return FALSE;
            default:
                break;
        }
    }
}

// Receive the input parameters for a new sequence into 'sequenceParams'
void getSequenceParams(sequence_params *sequenceParams)
{
    while (getSequence(sequenceParams) == FALSE);
    sequenceParams->AllowRotation = getAllowRotation();
}

// Receive and return the tetromino which the user wants to drop in debug mode. Repeat until input is a valid tetromino
char getTetrominoToDrop()
{
    char tet;

    while (TRUE)
    {
        tet = getChar("Which tetromino would you like to drop to the grid?\n\0");
        if (getTetromino(tet, ROTATION_0) != FALSE) return tet;
    }
}

// Receive the rotation the user would like to apply to 'tet'. Convert the rotation angle into an index pointing to the struct of 'tet' rotated by the input rotation, and return it. Repeat until input is a valid rotation
int getRotation(char tet)
{
    int rotation;

    while (TRUE)
    {
        rotation = getInt("Enter the angle of anti-clockwise rotation (must be a multiple of 90):\n\0");

        if (rotation % 90 == 0)
        {
            rotation = (rotation / 90) % getRotations(tet); // Wrap rotations around to the number of ways 'tet' can be rotated, and convert to an index
            if (rotation < 0) rotation = getRotations(tet) + rotation; // If input is -ve, convert index to an equivalent clockwise rotation index
            return rotation;   
        }
    }
}

// Receive and return the column which the user wants to drop a tetromino with width 'tetrominoWidth' into in debug mode
int getColumnToDropIn(int tetrominoWidth)
{
    int column;

        while (TRUE)
        {
            column = getInt("Which column would you like to drop it to?\n\0");
            if (column >= 0 && column <= GRID_WIDTH - tetrominoWidth) return column; // Return 'column' the tetromino lands within the bounds of the grid
        }
}

// Receive and return user's debug menu selection. Repeat until input is a valid menu item
char getDebugMenuInput()
{
    char input;

    while (TRUE)
    {
        input = getChar("1. Continue\n2. Clear grid\n3. Main menu\n\n\0");

        switch (input)
        {
            case '1':
            case '2':
            case '3':
                return input;
            default:
                break;
        }
    }   
}