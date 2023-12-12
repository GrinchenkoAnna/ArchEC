#include "library.h"



int sc_memoryInit()
{
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        sc_memory[i] = 0;
    }
}

int sc_memorySet(int address, int value)
{
    if (address <= 0 || address >= MEMORY_SIZE)
    {
        registr = registr | (1 << MEMORY_ERROR);        
    }
    else sc_memory[address] = value;

    return ((registr >> MEMORY_ERROR) & 0x1);
}

int sc_memoryGet(int address, int* value)
{
    if (address <= 0 && address >= MEMORY_SIZE)
    {
        registr = registr | (1 << MEMORY_ERROR);  
        return ((registr >> MEMORY_ERROR) & 0x1);
    }
    else *value = sc_memory[address];
    
    return 0;    
}

int sc_memorySave(char* filename)
{
    FILE *sc_memory_file;
    sc_memory_file = fopen(filename, "wb");
    fwrite(sc_memory, sizeof(int), MEMORY_SIZE, sc_memory_file);
    fclose(sc_memory_file);

    return 0;
}

int sc_memoryLoad(char* filename)
{
    FILE *sc_memory_file;
    if (sc_memory_file = fopen(filename, "rb") == NULL) return 1;
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
    if (reg < OPERATION_OVERFLOW || reg > INVALID_COMMAND) return 1;
    else 
    {
        if (value) registr = registr | (1 << value);
        else registr = registr & (~ (1 << value));        
    }

    return 0;
}

int sc_regGet(int reg, int* value)
{
    if (reg >= OPERATION_OVERFLOW || reg <= INVALID_COMMAND) 
    {
         *value = (registr >> reg) & 0x1;
         return *value;
    }
    else return 2;
}

int sc_commandEncode(int command, int operand, int* value)
{
    
}