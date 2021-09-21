#include <stdio.h>
#include <string.h>

#include "test.h"
#include "grid.h"
#include "tetromino.h"
#include "solver.h"
#include "bool.h"

testcase testCases[NUMBER_OF_TESTS] =
{
    {
        {
            "ZOZS",
            TEST_SEQUENCE_LENGTH,
            FALSE
        },
            {0, 0, 2, 2},
            {0, 0, 0, 0}
    },

    {
        {
            "ZISS",
            TEST_SEQUENCE_LENGTH,
            TRUE
        },
            {4, 0, 2, 0},
            {1, 1, 0, 0}
    },

    {
        {
            "SISS",
            TEST_SEQUENCE_LENGTH,
            FALSE
        },
            {0, 5, 2, 0},
            {0, 0, 0, 0}
    },

    {
        {
            "SZLS",
            TEST_SEQUENCE_LENGTH,
            TRUE
        },
            {0, 0, 3, 4},
            {0, 0, 0, 1}                
    },
    
    {
        {
            "SSJS",
            TEST_SEQUENCE_LENGTH,
            FALSE
        },
            {0, 0, 4, 3},
            {0, 0, 0, 0}        
    }
};

// Solve the sequence in 'testSequenceParams', display the solution, and return the solver holding the solution
solver *solveTestCaseSequence(sequence_params *testSequenceParams, solver solvers[NUMBER_OF_SOLVERS])
{
    solver *bestSolver;

    time_t startTime;
    time(&startTime);

    initialiseSolvers(solvers, testSequenceParams);
    runSolvers(solvers, testSequenceParams);                

    bestSolver = getBestSolver(solvers);
    printSolution(bestSolver, testSequenceParams, startTime);

    return bestSolver;
}

// Validate the solving routines of the program by comparing its solutions for the test cases to the known solutions in test.h
void runTests()
{
    if (GRID_WIDTH != TEST_GRID_WIDTH) 
        printf("GRID_WIDTH is different from TEST_GRID_WIDTH i.e. the width of the grid used when computing the test cases. Can't run tests!\n");

    else if (MAX_SEQUENCE_SIZE < TEST_SEQUENCE_LENGTH)
        printf("MAX_SEQUENCE_SIZE is less than TEST_SEQUENCE_SIZE, can't run tests!\n");

    else 
    {
        solver solvers[NUMBER_OF_SOLVERS];
        solver *bestSolver;

        int passedTests = 0;
        int failedTests = 0;

        printf("Running tests...\n");

        for (int test = 0; test < NUMBER_OF_TESTS; test++)
        {        
            printf("Test: %d\nPuzzle: %.*s\nRotation: %s\nSolving...\n\n", \
            test, TEST_SEQUENCE_LENGTH, testCases[test].SequenceParams.Sequence, testCases[test].SequenceParams.AllowRotation == TRUE ? "Y" : "N");

            bestSolver = solveTestCaseSequence(&testCases[test].SequenceParams, solvers);

            if (memcmp(testCases[test].PieceColumns, bestSolver->BestPieceColumns, sizeof(testCases[test].PieceColumns)) == 0 && \
                memcmp(testCases[test].PieceRotations, bestSolver->BestPieceRotations, sizeof(testCases[test].PieceRotations)) == 0)
            {
                printf("Test %d: PASSED\n\n", test);
                passedTests++;
            }                
            else 
            {
                printf("Expected permutation: ");
                for (int piece = 0; piece < TEST_SEQUENCE_LENGTH; piece++)
                    printf("%c:%d(%d) ", testCases[test].SequenceParams.Sequence[piece], testCases[test].PieceColumns[piece], testCases[test].PieceRotations[piece]*90);
                printf("\n\n");              
                
                printf("Test %d: FAILED\n\n", test); 
                failedTests++;
            }
        }

        printf("PASSED %d test(s), FAILED %d test(s)\n\n", passedTests, failedTests);   
    }
}
