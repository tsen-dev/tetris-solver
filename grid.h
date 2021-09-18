#ifndef GRID_H
#define GRID_H

#include "tetromino.h"

#define GRID_HEIGHT (MAX_SEQUENCE_SIZE * 4)
#define GRID_WIDTH 6

// Print the state of 'grid', and the height of each column from 'columnHeight'
void printGrid(char grid[GRID_HEIGHT][GRID_WIDTH]);

// Drop tetromino 'tet' into column 'droppedColumn' and update the state of the grid through 'grid' and 'gridColumnHeights'. If the tetromino lands above the top of the grid, return FALSE and abort the dropping. Return TRUE otherwise
int dropTetrominoToGrid(tetromino *tet, int droppedColumn, char grid[GRID_HEIGHT][GRID_WIDTH], int gridColumnHeights[GRID_WIDTH]);

#endif