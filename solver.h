#ifndef SOLVER_H
#define SOLVER_H

#include "grid.h"
#include "tetromino.h"
#include "input_utils.h"

#define NUMBER_OF_SOLVERS 10
#define PROGRESS_DISPLAY_INTERVAL 10000000

typedef unsigned int uint32;

typedef struct
{
    // Stores the number of columns each piece can be dropped in, given its current rotation
    int ColumnCounts[MAX_SEQUENCE_SIZE]; 
    // Stores the number of ways each piece can be rotated
    int RotationCounts[MAX_SEQUENCE_SIZE]; 

    // Stores the current column/rotation being tried for each piece
    int ColumnCounters[MAX_SEQUENCE_SIZE];
    int RotationCounters[MAX_SEQUENCE_SIZE];

    // Stores the state of the grid i.e. height of each column
    int ColumnHeights[GRID_WIDTH];
    // Stores the states of the grid after each tetromino is dropped
    int SavedColumnHeights[MAX_SEQUENCE_SIZE-1][GRID_WIDTH];

    // Stores the best column/rotation of each puzzle piece in the best permutation
    int BestPieceColumns[MAX_SEQUENCE_SIZE];
    int BestPieceRotations[MAX_SEQUENCE_SIZE];

    // Stores the index of the earliest piece in the sequence which has a new column or rotation from the last permutation
    int LastChangedPiece; 

    int SolverID;
    int MinStackHeight;     

    uint32 CurrentPermutation;
    uint32 Permutations;    
} solver;

// Calculate and return the total number of permutations the sequence in 'sequenceParams' can be dropped to the grid
uint32 getSequencePermutations(sequence_params *sequenceParams);

// Calculate the number of permutations which an increment in each piece's column counter represents
void getColumnCounterPermutations(sequence_params *sequenceParams);

// Update the counters of 'solver' to the next permutation. Return the index of the earliest piece in the sequence which has a new column or rotation from the last permutation
int getNextPermutation(solver *solver, sequence_params *sequenceParams);

// In 'solver', update the column counter of the tetromino at index 'pieceIndex' to the next permutation. Return the index of the earliest piece in the sequence which has a new column or rotation from the permutation before the increment
int incrementColumnCounter(solver *solver, sequence_params *sequenceParams, int pieceIndex);

// Update the counters of 'solver' to the next 'n'th permutation
void getNextNthPermutation(solver *solver, sequence_params *sequenceParams, uint32 n);

// Set the starting permutation for 'solver' to the permutation in the main solver
void setSolverStartPermutation(int solver);

// Set 'solver' to the first permutation of the sequence in 'sequenceParams'
void setToFirstPermutation(solver *solver, sequence_params *sequenceParams);

// Set each solver's starting permutation and the number of permutations assigned to it for solving the sequence in 'sequenceParams'
void initialiseSolvers(solver solvers[NUMBER_OF_SOLVERS], sequence_params *sequenceParams);

// Return the y coordinate at which tetromino 'tet' will land when dropped into column 'droppedColumn', given height of the grid's columns in 'columnHeights'
int getLandingHeight(tetromino *tet, int droppedColumn, int columnHeights[GRID_WIDTH]);

// Return the height of the tetromino stack on the grid, given the height of the grid's columns in 'columnHeights' 
int getStackHeight(int columnHeights[GRID_WIDTH]);

// Stack the sequence in the current permutation of 'solver' and return the height of the resulting stack. Reuse a saved intermediate grid state if current permutation has an identical beginning to the previous one
int tryPermutation(solver *solver, sequence_params *sequenceParams);

// Drop tetromino 'tet' into column 'droppedColumn', and update the heights of the grid's columns in 'columnHeights'.
void dropTetromino(tetromino *tet, int droppedColumn, int columnHeights[GRID_WIDTH]);

// Print the time elapsed and number/percentage of permutations tried for solver 'solver' 
void printSolverProgress(solver *solver, time_t startTime);

// Try all column/rotation permutations and solve the sequence in 'sequenceParams' using the solvers in 'solvers'
void runSolvers(solver solvers[NUMBER_OF_SOLVERS], sequence_params *sequenceParams);

// Return the solver in 'solvers' which found the solution resulting in the lowest stack height 
solver * getBestSolver(solver solvers[NUMBER_OF_SOLVERS]);

// Print the stack of tetrominos produced when dropped to the best columns and in the best rotations
void printSolution(solver *solver, sequence_params *sequenceParams, time_t startTime);

// Print the internal state and counters of 'solver'
void printSolver(int solver, uint32 remainingPermutations);

// Solve the tetromino sequence in 'sequenceParams' and display the solution
void solveSequence(sequence_params *sequenceParams);

#endif