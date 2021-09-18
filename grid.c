#include <stdio.h>

#include "bool.h"
#include "grid.h"
#include "tetromino.h"
#include "solver.h"

// Print the state of 'grid'.
void printGrid(char grid[GRID_HEIGHT][GRID_WIDTH])
{
    printf("\n");
    
    for (int row = 0; row < GRID_HEIGHT; row++)
    {
        for (int col = 0; col < GRID_WIDTH; col++) 
            printf("%c", grid[row][col]);            
        printf("\n");
    }    
    
    printf("\n");
}

// Drop tetromino 'tet' into column 'droppedColumn' and update the state of the grid through 'grid' and 'gridColumnHeights'. If the tetromino lands above the top of the grid, return FALSE and abort the dropping. Return TRUE otherwise
int dropTetrominoToGrid(tetromino *tet, int droppedColumn, char grid[GRID_HEIGHT][GRID_WIDTH], int gridColumnHeights[GRID_WIDTH])
{
    int landingHeight = getLandingHeight(tet, droppedColumn, gridColumnHeights);

    if ((landingHeight - 1) + tet->Height >= GRID_HEIGHT) return FALSE;
    
    int landingRow = (GRID_HEIGHT - 1) - landingHeight;
    int drawingRow; // The row of 'grid' that is being written to

    for (int tetRow = 3; tetRow > 3 - tet->Height; tetRow--)
    {
        for (int tetCol = 0; tetCol < tet->Width; tetCol++)
        {
            if (tet->Pattern[tetRow][tetCol] != '_')
            {
                drawingRow = landingRow + (tetRow - 3);
                grid[drawingRow][droppedColumn + tetCol] = tet->Pattern[tetRow][tetCol];
                gridColumnHeights[droppedColumn + tetCol] = GRID_HEIGHT - drawingRow;
            }
        }
    }

    return TRUE;
}