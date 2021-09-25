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
            4,
            FALSE
        },
            {0, 0, 2, 2},
            {0, 0, 0, 0}
    },

    {
        {
            "ZISS",
            4,
            TRUE
        },
            {4, 0, 2, 0},
            {1, 1, 0, 0}
    },

    {
        {
            "SISS",
            4,
            FALSE
        },
            {0, 5, 2, 0},
            {0, 0, 0, 0}
    },

    {
        {
            "SZLS",
            4,
            TRUE
        },
            {0, 0, 3, 4},
            {0, 0, 0, 1}                
    },
    
    {
        {
            "SSJS",
            4,
            FALSE
        },
            {0, 0, 4, 3},
            {0, 0, 0, 0}        
    },

    {
        {
            "JSLITTLL",
            8,
            FALSE
        },
            {3, 1, 0, 5, 0, 0, 3, 4},
            {0, 0, 0, 0, 0, 0, 0, 0}        
    },

    {
        {
            "IOTSLJZI",
            8,
            TRUE
        },
            {0, 1, 0, 3, 3, 4, 3, 0},
            {0, 0, 0, 0, 3, 0, 1, 1}        
    },

    {
        {
            "TTTTTTTT",
            8,
            TRUE
        },
            {0, 0, 0, 3, 3, 2, 4, 3},
            {0, 2, 1, 2, 1, 1, 3, 0}        
    },

    {
        {
            "IOTIOTIO",
            8,
            TRUE
        },
            {0, 1, 0, 5, 3, 3, 0, 4},
            {0, 0, 0, 0, 0, 1, 1, 0}        
    },

    {
        {
            "LLLLLLLL",
            8,
            TRUE
        },
	        {0, 0, 2, 3, 4, 2, 4, 1},
 	        {0, 0, 0, 0, 2, 3, 2, 3}  
    }
};

// Solve the sequence in 'testSequenceParams', display the solution, and return the solver holding the solution
solver *solveTestCaseSequence(sequence_params *testSequenceParams, solver solvers[NUMBER_OF_SOLVERS])
{
    solver *bestSolver;

    time_t startTime;
    time(&startTime);

    if (initialiseSolvers(solvers, testSequenceParams) == OVERFLOW_DETECTED) return NULL;
    runSolvers(solvers, testSequenceParams);                

    bestSolver = getBestSolver(solvers);
    printSolution(bestSolver, testSequenceParams, startTime);

    return bestSolver;
}

// Validate the solving routines of the program by comparing its solutions for the test cases to the known solutions in test.h
void runTests()
{
    if (GRID_WIDTH != TEST_GRID_WIDTH) 
        printf("GRID_WIDTH is different from TEST_GRID_WIDTH i.e. the width of the grid used when computing the test cases. Can't run tests!\n\n");

    else if (MAX_SEQUENCE_SIZE < MAX_TEST_SEQUENCE_LENGTH)
        printf("MAX_SEQUENCE_SIZE is less than some test sequences (MAX_TEST_SEQUENCE_SIZE), can't run tests!\n\n");

    else 
    {
        solver solvers[NUMBER_OF_SOLVERS];
        solver *bestSolver;

        int passedTests = 0;
        int failedTests = 0;

        printf("Running tests...\n");

        for (int test = 0; test < NUMBER_OF_TESTS; test++)
        {        
            printf("Test: %d\nSequence: %.*s\nRotation: %s\nSolving...\n\n", \
            test, testCases[test].SequenceParams.Size, testCases[test].SequenceParams.Sequence, testCases[test].SequenceParams.AllowRotation == TRUE ? "Y" : "N");

            bestSolver = solveTestCaseSequence(&testCases[test].SequenceParams, solvers);

            // Test failed due to overflow
            if (bestSolver == NULL) failedTests++; 

            // Test passed
            else if (memcmp(testCases[test].PieceColumns, bestSolver->BestPieceColumns, sizeof(testCases[test].PieceColumns)) == 0 && \
                memcmp(testCases[test].PieceRotations, bestSolver->BestPieceRotations, sizeof(testCases[test].PieceRotations)) == 0)
            {
                printf("Test %d: PASSED\n\n", test);
                passedTests++;
            }                

            // Test failed due to incorrect solution
            else 
            {
                printf("Expected permutation: ");
                for (int piece = 0; piece < testCases[test].SequenceParams.Size; piece++)
                    printf("%c:%d(%d) ", testCases[test].SequenceParams.Sequence[piece], testCases[test].PieceColumns[piece], testCases[test].PieceRotations[piece]*90);
                printf("\n\n");              
                
                printf("Test %d: FAILED\n\n", test); 
                failedTests++;
            }
        }

        printf("PASSED %d test(s), FAILED %d test(s)\n\n", passedTests, failedTests);   
    }
}
