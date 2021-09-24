#ifndef TETROMINO_H
#define TETROMINO_H

#define MAX_SEQUENCE_SIZE 20

#define ROTATION_0 0
#define ROTATION_90_ACW 1
#define ROTATION_180_ACW 2
#define ROTATION_270_ACW 3

typedef struct
{
    char Pattern[4][4];
    int ColumnHeights[4];
    int Width;
    int Height;
} tetromino;

tetromino tet_I[2];
tetromino tet_O[1];
tetromino tet_T[4];
tetromino tet_J[4];
tetromino tet_L[4];
tetromino tet_S[2];
tetromino tet_Z[2];

// Return 'rotation' orientation of the tetromino struct for 'tet'. Return FALSE if 'tet' is not a valid tetromino
tetromino *getTetromino(char tet, int rotation);

// Return the number ways the tetromino for 'tet' can be rotated. Return FALSE if 'tet' is not a valid tetromino
int getRotations(char tet);

#endif