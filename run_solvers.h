#include "input_utils.h"
#include "solver.h"

typedef struct // Stores the data required by a solver thread
{
    solver *Solver;
    sequence_params *SequenceParams;
} solver_thread_params;


#ifdef _WIN32 // Windows implementation (multi-threaded)

#include <windows.h>

// Try all column/rotation permutations and solve the sequence in 'sequenceParams' using the solvers in 'solvers'. Create and start a solver thread for each solver, using Windows threading routines
void runSolvers(solver solvers[NUMBER_OF_SOLVERS], sequence_params *sequenceParams);

// Runs the solver in its own thread of execution, using the 'solver' and 'sequence_params' parameters inside 'threadParams' (must point to a solver_thread_params)
DWORD WINAPI runSolver(LPVOID threadParams);


#elif linux // Linux implementation (multi-threaded)

// Try all column/rotation permutations and solve the sequence in 'sequenceParams' using the solvers in 'solvers'. Create and start a solver thread for each solver, using Linux threading routines
void runSolvers(solver solvers[NUMBER_OF_SOLVERS], sequence_params *sequenceParams);

// Runs the solver in its own thread of execution, using the 'solver' and 'sequence_params' parameters inside 'threadParams' (must point to a solver_thread_params)
void* runSolver(void *threadParams);


#else // Standard implementation (single-threaded)

// Try all column/rotation permutations and solve the sequence in 'sequenceParams' using the solvers in 'solvers'. Run all solvers one by one using the main thread
void runSolvers(solver solvers[NUMBER_OF_SOLVERS], sequence_params *sequenceParams);

// Runs 'solver' to solve the sequence in 'sequenceParams'
void runSolver(solver * solver, sequence_params *sequenceParams);

#endif
