#include <stdio.h>
#include <string.h>

#include "bool.h"
#include "input_utils.h"
#include "grid.h"
#include "tetromino.h"

// Runs debug mode, allowing the user to drop tetrominos onto a grid
void debugMode()
{
    char grid[GRID_HEIGHT][GRID_WIDTH];    
    int gridColumnHeights[GRID_WIDTH];

    char tet;
    int droppedColumn;
    int rotation;
    char input;

    memset(grid, '_', sizeof(grid));
    memset(gridColumnHeights, 0, sizeof(gridColumnHeights));

    printGrid(grid);

    while (TRUE)
    {
        tet = getTetrominoToDrop();
        rotation = getRotation(tet);
        droppedColumn = getColumnToDropIn(getTetromino(tet, rotation)->Width);

        if (dropTetrominoToGrid(getTetromino(tet, rotation), droppedColumn, grid, gridColumnHeights) == FALSE)
            printf("Not enough space! Clear the grid or try another tetromino/column\n\n");            
        
        else printGrid(grid);        
        
        input = getDebugMenuInput();
        switch(input)
        {
            case '1':
                break;
            case '2':
                memset(grid, '_', sizeof(grid));
                memset(gridColumnHeights, 0, sizeof(gridColumnHeights));
                printGrid(grid);
                break;
            case '3':
                return;
        }
    }
}