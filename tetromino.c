#include <stdio.h>

#include "bool.h"
#include "tetromino.h"

tetromino tet_I[2] =
{
  {{{'I', '_', '_', '_'},
    {'I', '_', '_', '_'},
	  {'I', '_', '_', '_'},
	  {'I', '_', '_', '_'}}, {4, 0, 0, 0}, 1, 4},

	{{{'_', '_', '_', '_'},
	  {'_', '_', '_', '_'},
	  {'_', '_', '_', '_'},
	  {'I', 'I', 'I', 'I'}}, {1, 1, 1, 1}, 4, 1}
};

tetromino tet_O[1] =
{
	{{{'_', '_', '_', '_'},
	  {'_', '_', '_', '_'},
	  {'O', 'O', '_', '_'},
	  {'O', 'O', '_', '_'}}, {2, 2, 0, 0}, 2, 2}
};

tetromino tet_T[4] =
{
	{{{'_', '_', '_', '_'},
	  {'_', '_', '_', '_'},
	  {'T', 'T', 'T', '_'},
	  {'_', 'T', '_', '_'}}, {2, 2, 2, 0}, 3, 2},

	{{{'_', '_', '_', '_'},
	  {'T', '_', '_', '_'},
	  {'T', 'T', '_', '_'},
	  {'T', '_', '_', '_'}}, {3, 2, 0, 0}, 2, 3},

	{{{'_', '_', '_', '_'},
	  {'_', '_', '_', '_'},
	  {'_', 'T', '_', '_'},
	  {'T', 'T', 'T', '_'}}, {1, 2, 1, 0}, 3, 2},

	{{{'_', '_', '_', '_'},
	  {'_', 'T', '_', '_'},
	  {'T', 'T', '_', '_'},
	  {'_', 'T', '_', '_'}}, {2, 3, 0, 0}, 2, 3}
};

tetromino tet_J[4] =
{
	{{{'_', '_', '_', '_'},
	  {'_', 'J', '_', '_'},
	  {'_', 'J', '_', '_'},
	  {'J', 'J', '_', '_'}}, {1, 3, 0, 0}, 2, 3},

  {{{'_', '_', '_', '_'},
    {'_', '_', '_', '_'},
    {'J', 'J', 'J', '_'},
    {'_', '_', 'J', '_'}}, {2, 2, 2, 0}, 3, 2},

  {{{'_', '_', '_', '_'},
    {'J', 'J', '_', '_'},
    {'J', '_', '_', '_'},
    {'J', '_', '_', '_'}}, {3, 3, 0, 0}, 2, 3},

  {{{'_', '_', '_', '_'},
    {'_', '_', '_', '_'},
    {'J', '_', '_', '_'},
    {'J', 'J', 'J', '_'}}, {2, 1, 1, 0}, 3, 2}
};

tetromino tet_L[4] =
{
  {{{'_', '_', '_', '_'},
    {'L', '_', '_', '_'},
    {'L', '_', '_', '_'},
    {'L', 'L', '_', '_'}}, {3, 1, 0, 0}, 2, 3},

  {{{'_', '_', '_', '_'},
    {'_', '_', '_', '_'},
    {'_', '_', 'L', '_'},
    {'L', 'L', 'L', '_'}}, {1, 1, 2, 0}, 3, 2},

  {{{'_', '_', '_', '_'},
    {'L', 'L', '_', '_'},
    {'_', 'L', '_', '_'},
    {'_', 'L', '_', '_'}}, {3, 3, 0, 0}, 2, 3},

  {{{'_', '_', '_', '_'},
    {'_', '_', '_', '_'},
    {'L', 'L', 'L', '_'},
    {'L', '_', '_', '_'}}, {2, 2, 2, 0}, 3, 2}
};

tetromino tet_S[2] =
{
  {{{'_', '_', '_', '_'},
    {'_', '_', '_', '_'},
    {'_', 'S', 'S', '_'},
    {'S', 'S', '_', '_'}}, {1, 2, 2, 0}, 3, 2},

  {{{'_', '_', '_', '_'},
    {'S', '_', '_', '_'},
    {'S', 'S', '_', '_'},
    {'_', 'S', '_', '_'}}, {3, 2, 0, 0}, 2, 3}
};

tetromino tet_Z[2] =
{
  {{{'_', '_', '_', '_'},
    {'_', '_', '_', '_'},
    {'Z', 'Z', '_', '_'},
    {'_', 'Z', 'Z', '_'}}, {2, 2, 1, 0}, 3, 2},

  {{{'_', '_', '_', '_'},
    {'_', 'Z', '_', '_'},
    {'Z', 'Z', '_', '_'},
    {'Z', '_', '_', '_'}}, {2, 3, 0, 0}, 2, 3}
};

// Return 'rotation' orientation of the tetromino struct for 'tet'. Return FALSE if 'tet' is not a valid tetromino
tetromino *getTetromino(char tet, int rotation)
{
    switch (tet)
    {
        case 'I':
            return &tet_I[rotation];
        case 'O':
            return &tet_O[rotation];
        case 'T':
            return &tet_T[rotation];
        case 'J':
            return &tet_J[rotation];
        case 'L':
            return &tet_L[rotation];
        case 'S':
            return &tet_S[rotation];
        case 'Z':
            return &tet_Z[rotation];
    }
    return FALSE;
}

// Return the number ways the tetromino for 'tet' can be rotated. Return FALSE if 'tet' is not a valid tetromino
int getRotations(char tet)
{
    switch (tet)
    {
        case 'I':
            return 2;    
        case 'O':
            return 1;
        case 'T':
            return 4;
        case 'J':
            return 4;
        case 'L':
            return 4;
        case 'S':
            return 2;
        case 'Z':
            return 2;
    }
    return FALSE;
}
