#include "mySimpleComputer.h"

int sc_memoryInit()
{
    for (int i = 0; i < MEMORY_SIZE; i++) { sc_memory[i] = 0; }
    return 0;
}

int sc_memorySet(int address, int value)
{
    if ((address < 0 || address >= MEMORY_SIZE)
        || value > 9999 || value < -9999)
    {
        registr = registr | (1 << MEMORY_ERROR);
        return -1;
    }
    else sc_memory[address] = value;
    return 0;
}

int sc_memoryGet(int address, int *value)
{
    if (address < 0 || address >= MEMORY_SIZE)
    {
        registr = registr | (1 << MEMORY_ERROR);
        return -1;
    }
    *value = sc_memory[address];
    return 0;
}

int sc_memorySave(char* filename)
{
    FILE *sc_memory_file;
    if ((sc_memory_file = fopen(filename, "wb")) == NULL) return -1;
    fwrite(sc_memory, sizeof(int), MEMORY_SIZE, sc_memory_file);
    fclose(sc_memory_file);
    return 0;
}

int sc_memoryLoad(char* filename)
{
    FILE *sc_memory_file;
    if ((sc_memory_file = fopen(filename, "rb")) == NULL) return -1;
    else
    {
        fread(sc_memory, sizeof(int), MEMORY_SIZE, sc_memory_file);
        fclose(sc_memory_file);
    }
    return 0;
}

int sc_regInit(void)
{
    registr = 0;
    return 0;
}

int sc_regSet(int reg, int value)
{
    if (reg < OPERATION_OVERFLOW || reg > INVALID_COMMAND) return -1;
    else
    {
        if (value) registr = registr | (1 << reg);
        else registr = registr & (~ (1 << reg));
    }
    return 0;
}

int sc_regGet(int reg, int* value)
{
    if (reg >= OPERATION_OVERFLOW && reg <= INVALID_COMMAND)
    {
         *value = (registr >> reg) & 0x1;
         return 0;
    }
    else return -1;
}

int sc_commandEncode(int command, int operand, int* value)
{
    if ((command != READ
        && command != WRITE
        && command != LOAD
        && command != STORE
        && !(command >= ADD && command <= MUL)
        && !(command >= JUMP && command <= HALT)
        && !(command >= NOT && command <= SUBCC))
        || operand < 0
        || operand > 127) return -1;

    *value = (command << 7) | operand;
    return 0;
}

int sc_commandDecode(int value, int* command, int* operand)
{
    int temp_command = value >> 7;
    int temp_operand = value & 127;

    if ((temp_command != READ
        && temp_command != WRITE
        && temp_command != LOAD
        && temp_command != STORE
        && !(temp_command >= ADD && temp_command <= MUL)
        && !(temp_command >= JUMP && temp_command <= HALT)
        && !(temp_command >= NOT && temp_command <= SUBCC))
        || temp_operand < 0
        || temp_operand > 127) return -1;

    *command = temp_command;
    *operand = temp_operand;

    return 0;
}
