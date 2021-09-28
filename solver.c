#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "bool.h"
#include "solver.h"
#include "input_utils.h"
#include "tetromino.h"

#define SKIPPED_PERMUTATION -1

typedef struct
{
    solver *Solver;
    sequence_params *SequenceParams;
} solver_thread_params;

// Calculate and return the total number of permutations at which the sequence in 'sequenceParams' can be dropped to the grid. If a non-null 'overflow' is passed, and if the permutation counter cannot store all permutations, return TRUE in 'overflow'
uint64_t getSequencePermutations(sequence_params *sequenceParams, int *overflow)
{
    uint64_t newSequencePermutations = sequenceParams->Size == 0 ? 0 : 1; // Stores permutations for entire sequence
    uint64_t oldSequencePermutations = newSequencePermutations; // Used to check overflows
    
    int piecePermutations = 0; // Stores permutations for current piece
    int pieceRotations;

    for (int piece = 0; piece < sequenceParams->Size; piece++)
    {
        pieceRotations = sequenceParams->AllowRotation ? getRotations(sequenceParams->Sequence[piece]) : 1;

        for (int rotation = 0; rotation < pieceRotations; rotation++)
            piecePermutations += GRID_WIDTH + 1 - getTetromino(sequenceParams->Sequence[piece], rotation)->Width;     

        newSequencePermutations *= piecePermutations;

        if (newSequencePermutations / piecePermutations != oldSequencePermutations && overflow != NULL)
        {
            *overflow = TRUE;            
            printf("OVERFLOW DETECTED:\nNumber of permutations do not fit inside 64-bit uint\nTry a shorter sequence, or a sequence containing tetrominos with less permutations e.g. O or I\n\n");
            break;
        }         

        else 
        {
            oldSequencePermutations = newSequencePermutations;        
            piecePermutations = 0;        
        }; 
    }       

    return newSequencePermutations;
}

// Calculate the number of permutations which an increment in each piece's column counter represents
void getColumnCounterPermutations(sequence_params *sequenceParams)
{        
    char previousPiece;
    int previousPieceRotations;

    sequenceParams->ColumnCounterPermutations[sequenceParams->Size - 1] = 1;

    for (int piece = sequenceParams->Size - 2; piece >= 0; piece--)
    {
        sequenceParams->ColumnCounterPermutations[piece] = 0;
        previousPiece = sequenceParams->Sequence[piece + 1];
        previousPieceRotations = sequenceParams->AllowRotation ? getRotations(sequenceParams->Sequence[piece + 1]) : 1;
             
        for (int rotation = 0; rotation < previousPieceRotations; rotation++) 
            sequenceParams->ColumnCounterPermutations[piece] += GRID_WIDTH + 1 - getTetromino(previousPiece, rotation)->Width;

        sequenceParams->ColumnCounterPermutations[piece] *= sequenceParams->ColumnCounterPermutations[piece + 1];
    }
}

// Update the counters of 'solver' to the next permutation. Return the index of the earliest piece in the sequence which has a new column or rotation from the last permutation
int getNextPermutation(solver *solver, sequence_params *sequenceParams)
{
    for (int piece = sequenceParams->Size-1; piece >= 0; piece--)
    {
        solver->ColumnCounters[piece]++;
        
        if (solver->ColumnCounters[piece] == solver->ColumnCounts[piece]) // Tried all columns in piece's current rotation, change to next rotation
        {
            solver->ColumnCounters[piece] = 0;
            solver->RotationCounters[piece]++;
                        
            if (solver->RotationCounters[piece] == solver->RotationCounts[piece]) // Tried all rotations for piece, reset to first permutation of the piece
            {
                solver->RotationCounters[piece] = 0;
                solver->ColumnCounts[piece] = GRID_WIDTH + 1 - getTetromino(sequenceParams->Sequence[piece], 0)->Width;                        
            }            

            else
            {
                solver->ColumnCounts[piece] = GRID_WIDTH + 1 - getTetromino(sequenceParams->Sequence[piece], solver->RotationCounters[piece])->Width;                        
                return piece;
            } 
        }

        // Don't change pieces earlier in the sequence
        else return piece;
    }   

    return FALSE; 
}

// In 'solver', update the column counter of the tetromino at index 'pieceIndex' to the next permutation. Return the index of the earliest piece in the sequence which has a new column or rotation from the permutation before the increment
int incrementColumnCounter(solver *solver, sequence_params *sequenceParams, int pieceIndex)
{
     for (int piece = pieceIndex; piece >= 0; piece--)
    {
        solver->ColumnCounters[piece]++;

        // Tried all columns using piece's current rotation
        if (solver->ColumnCounters[piece] == solver->ColumnCounts[piece])
        {
            solver->ColumnCounters[piece] = 0;
            solver->RotationCounters[piece]++;
            
            // Tried all rotations for piece
            if (solver->RotationCounters[piece] == solver->RotationCounts[piece]) 
            {
                solver->RotationCounters[piece] = 0;
                solver->ColumnCounts[piece] = GRID_WIDTH + 1 - getTetromino(sequenceParams->Sequence[piece], 0)->Width;                        
            }            

            // Try next rotation for piece
            else
            {
                solver->ColumnCounts[piece] = GRID_WIDTH + 1 - getTetromino(sequenceParams->Sequence[piece], solver->RotationCounters[piece])->Width;                        
                return piece;
            } 
        }

        // Don't change other pieces
        else return piece;
    }    

    return FALSE;
}

// Update the counters of 'solver' to the next 'n'th permutation. Return the index of the earliest piece in the sequence which has a new column or rotation from the permutation before calling this function
int getNextNthPermutation(solver *solver, sequence_params *sequenceParams, uint64_t n)
{
    uint64_t permutationsUpdated = 0;
    int lastChangedPiece;

    for (int piece = 0; piece < sequenceParams->Size; piece++)
    {
        while (permutationsUpdated + sequenceParams->ColumnCounterPermutations[piece] <= n)
        {
            lastChangedPiece = incrementColumnCounter(solver, sequenceParams, piece);                
            permutationsUpdated += sequenceParams->ColumnCounterPermutations[piece];                    
        }
    }

    return lastChangedPiece;
}

// Set 'solver' to the first permutation of the sequence in 'sequenceParams'
void setToFirstPermutation(solver *solver, sequence_params *sequenceParams)
{
    for (int piece = 0; piece < sequenceParams->Size; piece++)
    {
        solver->ColumnCounts[piece] = GRID_WIDTH - getTetromino(sequenceParams->Sequence[piece], ROTATION_0)->Width + 1;

        if (sequenceParams->AllowRotation) solver->RotationCounts[piece] = getRotations(sequenceParams->Sequence[piece]);
        else solver->RotationCounts[piece] = 1;
    }
}

// Set each solver's starting permutation and the number of permutations assigned to it for solving the sequence in 'sequenceParams'. Return OVERFLOW_DETECTED if the permutation counter cannot store all permutations
int initialiseSolvers(solver solvers[NUMBER_OF_SOLVERS], sequence_params *sequenceParams)
{
    solver mainSolver = {{ 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, 0, 0, GRID_HEIGHT, 0, 0};    
    uint64_t solverPermutations;
    uint64_t remainingPermutations;
    int overflow = FALSE;

    remainingPermutations = getSequencePermutations(sequenceParams, &overflow);
    if (overflow == TRUE) return OVERFLOW_DETECTED;
    
    getColumnCounterPermutations(sequenceParams);
    setToFirstPermutation(&mainSolver, sequenceParams);

    for (int solver = 0; solver < NUMBER_OF_SOLVERS; solver++)
    {        
        solverPermutations = remainingPermutations / (NUMBER_OF_SOLVERS - solver); 

        if (solverPermutations == 0) solvers[solver].Permutations = 0;            
        else 
        {            
            memcpy(&solvers[solver], &mainSolver, sizeof(mainSolver)); // Set starting permutation of solver
            getNextNthPermutation(&mainSolver, sequenceParams, solverPermutations);              
            solvers[solver].Permutations = solverPermutations;
            remainingPermutations -= solvers[solver].Permutations;                                    
        }
        
        solvers[solver].SolverID = solver;
    }

    return TRUE;    
}

// Return the y coordinate at which tetromino 'tet' will land when dropped into column 'droppedColumn', given height of the grid's columns in 'columnHeights'
int getLandingHeight(tetromino *tet, int droppedColumn, int columnHeights[GRID_WIDTH])
{
    int landingHeight = 0; // The y coordinate at which the bottom row of the tetromino will land

    for (int tetRow = 3; tetRow > 3-tet->Height; tetRow--) // Start scanning from the bottom row of the tetromino
    {
        for (int tetCol = 0; tetCol < tet->Width; tetCol++)
        {
            // If the cell at which the tetromino would land into is occupied, move the tetromino above that cell
            if (tet->Pattern[tetRow][tetCol] != '_' && (columnHeights[droppedColumn + tetCol] + (tetRow - 3)) > landingHeight)
                landingHeight = columnHeights[droppedColumn + tetCol] + (tetRow - 3);
        }
    }

    return landingHeight;
}

// Drop tetromino 'tet' into column 'droppedColumn', and update the heights of the grid's columns in 'columnHeights'.
void dropTetromino(tetromino *tet, int droppedColumn, int columnHeights[GRID_WIDTH])
{
    int landingHeight = getLandingHeight(tet, droppedColumn, columnHeights);    

    for (int tetCol = 0; tetCol < tet->Width; tetCol++)
        columnHeights[droppedColumn + tetCol] = landingHeight + tet->ColumnHeights[tetCol];
}

// Return the height of the tetromino stack on the grid, given the height of the grid's columns in 'columnHeights' 
int getStackHeight(int columnHeights[GRID_WIDTH])
{    
    int stackHeight = 0;

    for (int col = 0; col < GRID_WIDTH; col++)
    {
        if (columnHeights[col] > stackHeight)
            stackHeight = columnHeights[col];
    }

    return stackHeight;
}

// Skip the current permutation in 'solver' and all future permutations which are identical up to piece 'lastDeterminedPiece', as they were determined to be no better than the current best
void getNextUndeterminedPermutation(solver *solver, sequence_params *sequenceParams, int lastDeterminedPiece)
{
    uint64_t skippedPermutations;
    
    // Increment the last determined piece
    skippedPermutations = sequenceParams->ColumnCounterPermutations[lastDeterminedPiece];

    // Reset the columns and rotations of the following pieces
    for (int piece = lastDeterminedPiece + 1; piece < sequenceParams->Size; piece++)
    {
        skippedPermutations -= sequenceParams->ColumnCounterPermutations[piece] * solver->ColumnCounters[piece];

        for (int rotation = 0; rotation < solver->RotationCounters[piece]; rotation++)
            skippedPermutations -= sequenceParams->ColumnCounterPermutations[piece] * (GRID_WIDTH + 1 - getTetromino(sequenceParams->Sequence[piece], rotation)->Width);                    
    }
    
    solver->LastChangedPiece = getNextNthPermutation(solver, sequenceParams, skippedPermutations);
    solver->CurrentPermutation += skippedPermutations;
}

// Stack the sequence in the current permutation of 'solver' and return the height of the resulting stack.
int tryPermutation(solver *solver, sequence_params *sequenceParams)
{
    tetromino *tet;
    int stackHeight = 0;    

    // Reload an intermediate grid state if the current permutation has an identical beginning with the previous one
    if (solver->LastChangedPiece > 0) memcpy(solver->ColumnHeights, solver->SavedColumnHeights[solver->LastChangedPiece-1], sizeof(int)*GRID_WIDTH);                
    // Clear grid state
    else memset(solver->ColumnHeights, 0, sizeof(int)*GRID_WIDTH);        
    
    for (int piece = solver->LastChangedPiece; piece < sequenceParams->Size-1; piece++)
    {
        tet = getTetromino(sequenceParams->Sequence[piece], solver->RotationCounters[piece]);            
        dropTetromino(tet, solver->ColumnCounters[piece], solver->ColumnHeights);                    

        if (getStackHeight(solver->ColumnHeights) < solver->MinStackHeight) // Save intermediate grid state as it can be used again
            memcpy(solver->SavedColumnHeights[piece], solver->ColumnHeights, sizeof(int)*GRID_WIDTH);

        else // Skip current permutation (and all future permutations with an identical beginning) if it is determined to be no better than the current best permutation
        {
            getNextUndeterminedPermutation(solver, sequenceParams, piece);
            return SKIPPED_PERMUTATION;
        }        
    }
    tet = getTetromino(sequenceParams->Sequence[sequenceParams->Size-1], solver->RotationCounters[sequenceParams->Size-1]);            
    dropTetromino(tet, solver->ColumnCounters[sequenceParams->Size-1], solver->ColumnHeights);            

    return getStackHeight(solver->ColumnHeights);
}

// Print the time elapsed and number/percentage of permutations tried for solver 'solver' 
void printSolverProgress(solver *solver, time_t startTime)
{
    time_t currentTime;
    time(&currentTime);

    printf("Solver: %d Permutations tried: %llu / %llu (%.2f%%) %ds\n", \
        solver->SolverID, solver->CurrentPermutation, solver->Permutations, \
        100.0 * solver->CurrentPermutation / solver->Permutations, \
        (long) (currentTime - startTime));  
}

DWORD WINAPI runSolver(LPVOID threadParams)
{
    solver_thread_params *solverThreadParams = (solver_thread_params *) threadParams;

    solver *solver = solverThreadParams->Solver;
    sequence_params *sequenceParams = solverThreadParams->SequenceParams;
    int stackHeight;

    time_t startTime;
    time(&startTime);        
    
    while (solver->CurrentPermutation < solver->Permutations)
    {        
        stackHeight = tryPermutation(solver, sequenceParams);
        
        // If the permutation wasn't skipped 
        if (stackHeight != SKIPPED_PERMUTATION) 
        {
            // Save permutation if it is better than previous ones
            if (stackHeight < solver->MinStackHeight) 
            {
                solver->MinStackHeight = stackHeight;
                memcpy(solver->BestPieceColumns, solver->ColumnCounters, sizeof(solver->BestPieceColumns));
                memcpy(solver->BestPieceRotations, solver->RotationCounters, sizeof(solver->BestPieceRotations)); 
                solver->LastChangedPiece = 0; // Invalidate intermediate grid states since the minStackHeight has changed 
                getNextPermutation(solver, sequenceParams);                         
            }
            
            else solver->LastChangedPiece = getNextPermutation(solver, sequenceParams); 

            solver->CurrentPermutation++;
        }
        
        // Print solver progress
        if (solver->CurrentPermutation % PROGRESS_DISPLAY_INTERVAL == 0)
            printSolverProgress(solver, startTime);                                 
    }

    return 0;
}

// Try all column/rotation permutations and solve the sequence in 'sequenceParams' using the solvers in 'solvers'
void runSolvers(solver solvers[NUMBER_OF_SOLVERS], sequence_params *sequenceParams)
{       
    HANDLE solverThreadHandles[NUMBER_OF_SOLVERS];
    solver_thread_params solverThreadParams[NUMBER_OF_SOLVERS];

    for (int solver = 0; solver < NUMBER_OF_SOLVERS; solver++)
    {        
        solverThreadParams[solver].Solver = &solvers[solver];
        solverThreadParams[solver].SequenceParams = sequenceParams;
        solverThreadHandles[solver] = CreateThread(NULL, 0, runSolver, &solverThreadParams[solver], 0, NULL);
    }    

    WaitForMultipleObjects(NUMBER_OF_SOLVERS, solverThreadHandles, TRUE, INFINITE);
}

// Return the solver in 'solvers' which found the solution resulting in the lowest stack height 
solver * getBestSolver(solver solvers[NUMBER_OF_SOLVERS])
{
    int overallMinStackHeight = GRID_HEIGHT;
    solver *bestSolver = &solvers[0];

    for (int solver = 0; solver < NUMBER_OF_SOLVERS; solver++)
    {
        if (solvers[solver].MinStackHeight < overallMinStackHeight)
        {   
            overallMinStackHeight = solvers[solver].MinStackHeight;         
            bestSolver = &solvers[solver];            
        }
    }

    return bestSolver;
}

// Print the stack of tetrominos produced when dropped to the best columns and in the best rotations
void printSolution(solver *solver, sequence_params *sequenceParams, time_t startTime)
{   
    char grid[GRID_HEIGHT][GRID_WIDTH];
    int gridColumnHeights[GRID_WIDTH];
    time_t endTime;

    time(&endTime);    
    memset(grid, '_', sizeof(grid));
    memset(gridColumnHeights, 0, sizeof(gridColumnHeights));    

    // Drop the sequence into the grid, using the best permutation
    for (int piece = 0; piece < sequenceParams->Size; piece++)
        dropTetrominoToGrid(getTetromino(sequenceParams->Sequence[piece], solver->BestPieceRotations[piece]), \
                            solver->BestPieceColumns[piece], grid, gridColumnHeights);
    printGrid(grid);    

    printf("Best permutation: ");
    for (int piece = 0; piece < sequenceParams->Size; piece++)
        printf("%c:%d(%d) ", sequenceParams->Sequence[piece], solver->BestPieceColumns[piece], solver->BestPieceRotations[piece]*90);
    printf("\n\n");

    printf("Sequence: %.*s\nTried all %llu permutations!\nMinimum stack height: %d\nElapsed time: %lds\n\n", \
        sequenceParams->Size, sequenceParams->Sequence, getSequencePermutations(sequenceParams, NULL), solver->MinStackHeight, (long)(endTime-startTime));
}

// Solve the tetromino sequence in 'sequenceParams' and display the solution
void solveSequence(sequence_params *sequenceParams)
{    
    solver solvers[NUMBER_OF_SOLVERS];
    solver *bestSolver;

    time_t startTime;
    time(&startTime);

    printf("Sequence: %.*s\nSolving...\n\n", sequenceParams->Size, sequenceParams->Sequence);

    if (initialiseSolvers(solvers, sequenceParams) == OVERFLOW_DETECTED) return;
    runSolvers(solvers, sequenceParams);

    bestSolver = getBestSolver(solvers);
    printSolution(bestSolver, sequenceParams, startTime);
}