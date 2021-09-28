#include "input_utils.h"
#include "solver.h"

typedef struct // Stores the data required by a solver thread
{
    solver *Solver;
    sequence_params *SequenceParams;
} solver_thread_params;