#include <stdio.h>

#include "bool.h"
#include "input_utils.h"
#include "solver.h"

int main()
{
    char input;
    sequence_params sequenceParams;

    printf("\n");

    while (TRUE)
    {        
        input = getChar("1. Solve a puzzle\n2. Debug mode\n3. Run tests\n4. Exit\nEnter 1, 2, 3, or 4\n\n\0");

        switch (input)
        {
            case '1':
                getSequenceParams(&sequenceParams);                   
                solveSequence(&sequenceParams);                
                break;
            case '2':
                debugMode();
                break;
            case '3':
                runTests();
                break;
            case '4':
                return 0;
            default:
                break;
        }       
    }
}