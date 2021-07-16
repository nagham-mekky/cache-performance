# Cache Performance

## Files
1. cache.cpp: the source code of the cache simulator.
2. Report.pdf: A PDF file that contains all the data analysis, design and functions extracted from the output of the cache simulator.

## How to build
Before running the code, the user has to specify a few things. In the code, there is a macro defined named CACHE_LINE_SIZE, and that is where the user should specify the line size they would like to test, from 16 bytes to 128 bytes. Next, there is another macro defined in the code named NUM_WAYS, which the user should use to specify the number of N-Ways. Finally, in the main function, the user should change the generator that they want to test out.

### Test case
To run the test case, the user should change the DRAM size to (64 * 1024), change the cache size to 64, the cache line size to 16, and the number of ways to 2. In the main function, the user should change the number of iterations to 15, and use the function called memGenTestCase() as the generator. All of these can be found within the comments in the code.
