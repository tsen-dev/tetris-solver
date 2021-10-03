# Tetris Solver

## Contents
- Overview
- Working Princinples
- Additional Features
- Add Custom Tetrominos
- License

## Overview
![Overview Demo](overview_demo.gif)
- Given a sequence of tetrominos, this program will compute and display a way of dropping them into a grid such that the height of the resulting tetromino stack is minimised.
- The result displays which columns each tetromino should be dropped in, and the angle at which the tetrominos should be rotated before being dropped. The user can also specify to use the default rotations of tetrominos only.
- Dimensions of the grid are determined by the ```GRID_HEIGHT``` and ```GRID_WIDTH``` macros in ```grid.h```.
- Maximum allowed length for a tetromino sequence is determined by the ```MAX_SEQUENCE_SIZE``` macro in ```tetromino.h```.

## Working Principles
- The best **permutation** (a column/rotation value for each tetromino) is calculated by trying all possible permutations saving the one which produced the shortest stack.
- A tetromino can be dropped into ```GRID_WIDTH + 1 - TETROMINO_WIDTH``` columns, where ```TETROMINO_WIDTH``` is the width of a tetromino in a **specified rotation** (0, 90, 180, or 270 degrees). If the tetromino has ```r``` rotations, (assuming its width is the same in all rotations) the number of permutations for that tetromino becomes ```r * (GRID_WIDTH + 1 - TETROMINO_WIDTH)```. Therefore, the number of permutations for a sequence of length ```n``` becomes ```(r * (GRID_WIDTH + 1 - TETROMINO_WIDTH)) ** n```.
- In order to handle the exponentially growing number of permutations, certain **optimisations** are implemented:
    - The search space of all possible permutations is divided and assigned to ```solver``` units which try the permutations assigned to them. Each solver unit runs on a seperate solver thread for **concurrent** operation. **Multi-threading** is supported for **Windows** and **Linux**, otherwise a single solver unit is used which runs on the main thread. The number of solver units is determined by the ```NUMBER_OF_SOLVERS``` macro in ```solver.h``` (defaults to 16 for Windows/Linux).
    - 

## Additional Features
- **Debug mode**: 
- **Tests**: 
- **Overflow detection**: If the number of permutations for a sequence is greater than 2^64, an overflow in the 64-bit permutation counter is detected and the solving operation is aborted as all permutations cannot be tried.

## Add Custom Tetrominos
If you need other shapes which are specific to your problem domain, you can add tetrominos with custom shapes:
    - In ```tetromino.c``` define a ```tetromino``` struct for each rotation of your custom tetromino. In the ```Pattern``` attribute enter a ```'_'``` for blank cells and a different ```char``` which will represent your tetromino for other cells. **Maximum dimensions for a tetromino are 4x4**.
    - Declare your tetromino struct in ```tetromino.h```.
    - 

## License