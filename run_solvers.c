#include "input_utils.h"
#include "solver.h"
#include "run_solvers.h"

#ifdef _WIN32 // Windows implementation (multi-threaded)

#include <windows.h>

// Try all column/rotation permutations and solve the sequence in 'sequenceParams' using the solvers in 'solvers'. Create and start a solver thread for each solver, using Windows threading routines
void runSolvers(solver solvers[NUMBER_OF_SOLVERS], sequence_params *sequenceParams)
{       
    HANDLE solverThreadHandles[NUMBER_OF_SOLVERS];
    solver_thread_params solverThreadParams[NUMBER_OF_SOLVERS];

    for (int solver = 0; solver < NUMBER_OF_SOLVERS; solver++)
    {        
        solverThreadParams[solver].Solver = &solvers[solver];
        solverThreadParams[solver].SequenceParams = sequenceParams;
        solverThreadHandles[solver] = CreateThread(NULL, 0, runSolver, &solverThreadParams[solver], 0, NULL);

        if (solverThreadHandles[solver] == NULL)
        {
            printf("Could not create solver thread!\nRunning solver %d in main thread...\n\n", solvers[solver].SolverID);
            runSolver(&solverThreadParams[solver]);
        }
    }    

    for (int solverThread = 0; solverThread < NUMBER_OF_SOLVERS; solverThread++) 
        WaitForSingleObject(solverThreadHandles[solverThread], INFINITE);    
}

// Runs the solver in its own thread of execution, using the 'solver' and 'sequence_params' parameters inside 'threadParams' (must point to a solver_thread_params)
DWORD WINAPI runSolver(LPVOID threadParams)
{
    solver_thread_params *solverThreadParams = (solver_thread_params *) threadParams;

    solver *solver = solverThreadParams->Solver;
    sequence_params *sequenceParams = solverThreadParams->SequenceParams;
    int stackHeight;
    int progressDisplayThreshold = 1;

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
        if (solver->CurrentPermutation / PROGRESS_DISPLAY_INTERVAL == progressDisplayThreshold)
        {
            printSolverProgress(solver, startTime);                                 
            progressDisplayThreshold++;
        }            
    }

    printSolverProgress(solver, startTime);
    return 0;
}


#elif linux // Linux implementation (multi-threaded)

#include <pthread.h>

// Try all column/rotation permutations and solve the sequence in 'sequenceParams' using the solvers in 'solvers'. Create and start a solver thread for each solver, using Linux threading routines
void runSolvers(solver solvers[NUMBER_OF_SOLVERS], sequence_params *sequenceParams)
{       
    pthread_t solverThreadHandles[NUMBER_OF_SOLVERS];
    solver_thread_params solverThreadParams[NUMBER_OF_SOLVERS];

    for (int solver = 0; solver < NUMBER_OF_SOLVERS; solver++)
    {        
        solverThreadParams[solver].Solver = &solvers[solver];
        solverThreadParams[solver].SequenceParams = sequenceParams;
        pthread_create(&solverThreadHandles[solver], NULL, runSolver, &solverThreadParams[solver], 0, NULL);

        if (solverThreadHandles[solver] == NULL)
        {
            printf("Could not create solver thread!\nRunning solver %d in main thread...\n\n", solvers[solver].SolverID);
            runSolver(&solverThreadParams[solver]);
        }
    }    

    for (int solverThread = 0; solverThread < NUMBER_OF_SOLVERS; solverThread++) 
        pthread_join(solverThreadHandles[solverThread], NULL);        
}

// Runs the solver in its own thread of execution, using the 'solver' and 'sequence_params' parameters inside 'threadParams' (must point to a solver_thread_params)
void* runSolver(void* threadParams)
{
    solver_thread_params *solverThreadParams = (solver_thread_params *) threadParams;

    solver *solver = solverThreadParams->Solver;
    sequence_params *sequenceParams = solverThreadParams->SequenceParams;
    int stackHeight;
    int progressDisplayThreshold = 1;

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
        if (solver->CurrentPermutation / PROGRESS_DISPLAY_INTERVAL == progressDisplayThreshold)
        {
            printSolverProgress(solver, startTime);                                 
            progressDisplayThreshold++;
        }                                       
    }

    printSolverProgress(solver, startTime);
    return 0;
}


#else // Standard implementation (single-threaded)

// Runs 'solver' to solve the sequence in 'sequenceParams'
void runSolver(solver * solver, sequence_params *sequenceParams)
{
    int stackHeight; 
    int progressDisplayThreshold = 1;

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
        if (solver->CurrentPermutation / PROGRESS_DISPLAY_INTERVAL == progressDisplayThreshold)
        {
            printSolverProgress(solver, startTime);                                 
            progressDisplayThreshold++;
        }                                            
    }

    printSolverProgress(solver, startTime);
}

// Try all column/rotation permutations and solve the sequence in 'sequenceParams' using the solvers in 'solvers'. Run all solvers one by one using the main thread only
void runSolvers(solver solvers[NUMBER_OF_SOLVERS], sequence_params *sequenceParams)
{       
    for (int solver = 0; solver < NUMBER_OF_SOLVERS; solver++)
        runSolver(&solvers[solver], sequenceParams);
}

#endif
