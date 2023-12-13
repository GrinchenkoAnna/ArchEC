#include <stdio.h>
#include <stdlib.h>

#include "../thirdparty/ctest.h"
#include "../src/library.c"

CTEST(LIBRARY, memoryInit)
{
    // Given
    sc_memoryInit();
    int res = 0;
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        if (sc_memory[i] != 0) res = 1;
    }
    
    const int expected = 0;  

    ASSERT_EQUAL(expected, res);
}

CTEST(LIBRARY, memorySet)
{
    // Given
    sc_memorySet(2, 1);
    int res = 0;
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        if (sc_memory[2] == 1) res = 1;
    }
    
    const int expected = 1;  

    ASSERT_EQUAL(expected, res);
}