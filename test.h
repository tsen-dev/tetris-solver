#ifndef TEST_H
#define TEST_H

#include "solver.h"

#define NUMBER_OF_TESTS 10

#define TEST_GRID_WIDTH 6 // The grid width for which the test solutions were generated
#define MAX_TEST_SEQUENCE_LENGTH 8 // Maximum length of the sequences used in the test cases

typedef struct
{
    sequence_params SequenceParams;
    int PieceColumns[MAX_TEST_SEQUENCE_LENGTH]; // Stores which column each piece is dropped into
    int PieceRotations[MAX_TEST_SEQUENCE_LENGTH]; // Stores which rotation each piece is dropped in
} testcase;

// Solve the sequence in 'testSequenceParams', display the solution, and return the solver holding the solution
solver *solveTestCaseSequence(sequence_params *testSequenceParams, solver solvers[NUMBER_OF_SOLVERS]);

// Validate the solving routines of the program by comparing its solutions of the test cases to the known solutions in test.h
void runTests();

#endif