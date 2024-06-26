#include <stdio.h>
#include <stdlib.h>

#include "../thirdparty/ctest.h"
#include "../src/mySimpleComputer.c"

CTEST(mySimpleComputer, memoryInit) //заполнение массива 0
{
    int result = 0;
    sc_memoryInit();
    for (int i = 0; i < MEMORY_SIZE; i++) if (sc_memory[i] != 0) result = 1;
    const int expected = 0;
    ASSERT_EQUAL(expected, result);
}

CTEST(mySimpleComputer, memorySetOK) //sc_memory[2] == 1
{
    int result = sc_memorySet(2, 1); //0
    const int expected = 0;
    ASSERT_EQUAL(expected, result);
}

CTEST(mySimpleComputer, memorySetFAIL) //sc_memory[101] == 1
{
    int result = sc_memorySet(101, 1); //error
    const int expected = -1;
    ASSERT_EQUAL(expected, result);
}

CTEST(mySimpleComputer, memoryGetOK_1) //sc_memory[2] == 1
{
    int value;
    sc_memoryGet(2, &value); //1
    const int expected = 1;
    ASSERT_EQUAL(expected, value);
}

CTEST(mySimpleComputer, memoryGetOK_2) //sc_memory[1] == 0
{
    int value;
    int result = sc_memoryGet(1, &value); //0
    const int expected = 0;
    ASSERT_EQUAL(expected, result);
}

CTEST(mySimpleComputer, memoryGetFAIL) //sc_memory[101] == 1
{
    int value;
    int result = sc_memoryGet(-2, &value); //error
    const int expected = -1;
    ASSERT_EQUAL(expected, result);
}

CTEST(mySimpleComputer, memorySaveOK)
{
    // 0 и 1 ячейка sc_memory содержат 1
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        if (i == 0 || i == 1) sc_memory[i] = 1;
        else sc_memory[i] = 0;
    }

    int result = sc_memorySave(filename); //1, если файл невозможно открыть
    FILE *sc_memory_file;
    sc_memory_file = fopen(filename, "rb");
    int fake_memory[100]; //сюда считается содержимое файла
    fread(fake_memory, sizeof(int), 100, sc_memory_file);
    fclose(sc_memory_file);

    /*в результат суммируется кол-во ошибок
    (0 - успех) + содержимое ячеек (2 - успех). */
    for (int i = 0; i < 100; i++) { result += fake_memory[i]; }
    const int expected = 2;
    ASSERT_EQUAL(expected, result);
}

CTEST(mySimpleComputer, memoryLoadOK)
{
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        sc_memory[i] = 0;
    }
    // 0 и 1 ячейка sc_memory содержат 1
    int result = sc_memoryLoad(filename); //1, если файл невозможно открыть
    /*в результат суммируется кол-во ошибок
    (0 - успех) + содержимое ячеек (2 - успех). */
    for (int i = 0; i < 100; i++) { result += sc_memory[i]; }
    const int expected = 2;
    ASSERT_EQUAL(expected, result);
}

CTEST(mySimpleComputer, regInit) //registr = 0 (reset all flags)
{
    int result = 0;
    registr = 1;
    sc_regInit();
    if (registr != 0) result = 1;
    const int expected = 0;
    ASSERT_EQUAL(expected, result);
}

CTEST(mySimpleComputer, regSetOK) //setflag
{
    registr = 0;
    int result = sc_regSet(INVALID_COMMAND, 1);
    result += (registr >> INVALID_COMMAND) & 0x1;
    const int expected = 1;
    ASSERT_EQUAL(expected, result);
}

CTEST(mySimpleComputer, regSetFAIL) //setflag
{
    registr = 0;
    int result = sc_regSet(7, 1);
    result += (registr >> 7) & 0x1;
    const int expected = -1;
    ASSERT_EQUAL(expected, result);
}

CTEST(mySimpleComputer, regGetOK_1) //getflag
{
    registr = 0;
    sc_regSet(DIVISION_BY_ZERO, 1);

    int value;
    sc_memoryGet(DIVISION_BY_ZERO, &value); //1

    const int expected = 1;
    ASSERT_EQUAL(expected, value);
}

CTEST(mySimpleComputer, regGetOK_2) //getflag
{
    registr = 1;
    sc_regSet(MEMORY_ERROR, 0);

    int value;
    int result = sc_memoryGet(MEMORY_ERROR, &value); //0

    const int expected = 0;
    ASSERT_EQUAL(expected, result);
}

CTEST(mySimpleComputer, regGetFAIL) //getflag
{
    int value;
    int result = sc_memoryGet(-1, &value); //error

    const int expected = -1;
    ASSERT_EQUAL(expected, result);
}

CTEST(mySimpleComputer, commandEncodeOK)
{
    int value;
    sc_commandEncode(10, 10, &value); //1290

    const int expected = 1290;
    ASSERT_EQUAL(expected, value);
}

CTEST(mySimpleComputer, commandEncodeFAIL_1)
{
    int value = 0;
    int result = sc_commandEncode(1, 10, &value);

    const int expected = -1;
    ASSERT_EQUAL(expected, value + result);
}

CTEST(mySimpleComputer, commandEncodeFAIL_2)
{
    int value = 0;
    int result = sc_commandEncode(10, 200, &value);

    const int expected = -1;
    ASSERT_EQUAL(expected, value + result);
}

CTEST(mySimpleComputer, commandEncodeFAIL_3)
{
    int value = 0;
    int result = sc_commandEncode(-2, 200, &value);

    const int expected = -1;
    ASSERT_EQUAL(expected, value + result);
}

CTEST(mySimpleComputer, commandDecodeOK)
{
    int command, operand;
    int result = sc_commandDecode(1290, &command, &operand);

    const int expected = 0;
    ASSERT_EQUAL(expected, result);
}

CTEST(mySimpleComputer, commandDecodeFAIL)
{
    int command, operand;
    int result = sc_commandDecode(0, &command, &operand);

    const int expected = -1;
    ASSERT_EQUAL(expected, result);
}
