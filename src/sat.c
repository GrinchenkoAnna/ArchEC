#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "library.c"

FILE* factorial_sAssembler = NULL;

void load_program_factorial(const char* factorial_filename_sAssembler)
{
    if ((factorial_sAssembler = fopen(factorial_filename_sAssembler, "r")) == NULL)
    {
        fprintf(stderr, "%s cannot be found or opened\n", factorial_filename_sAssembler);
        exit(EXIT_FAILURE);
    }
}

void translate_assembler_to_binary_format(const char* factorial_filename_binary)
{
    for (int i = 0; !feof(factorial_sAssembler); i++)
    {
        char line_to_split[256];
        if (!fgets(line_to_split, 256, factorial_sAssembler))
        {
            if (feof(factorial_sAssembler)) { break; }
            else { fprintf(stderr, "line %d of programm cannot be read\n", i + 1); }
        }

     // 1) string      2) string converted to a number
        char* address; int address_numeric = 0;
        char* command; int command_numeric = 0;
        char* operand; int operand_numeric = 0;
        int value; //for sc_commandEncode

        address = strtok(line_to_split, " ");
        address_numeric = atoi(address);

        command = strtok(NULL, " ");
        for (int i = 0; i < strlen(command); i++)
        {
            if (command[i] == 32)
            {
                command[i] = '\0';
                for (int j = i; j < strlen(command) - i; j++)
                { command[i] = '\0'; }
            }
        }

        //printf("line: %d command: %s\n", i + 1, command);

        if (strcmp(command, "READ") == 0) { command_numeric = READ; }
        else if (strcmp(command, "WRITE") == 0) { command_numeric = WRITE; }
        else if (strcmp(command, "LOAD") == 0) { command_numeric = LOAD; }
        else if (strcmp(command, "STORE") == 0) { command_numeric = STORE; }
        else if (strcmp(command, "ADD") == 0) { command_numeric = ADD; }
        else if (strcmp(command, "SUB") == 0) { command_numeric = SUB; }
        else if (strcmp(command, "DIVIDE") == 0) { command_numeric = DIVIDE; }
        else if (strcmp(command, "MUL") == 0) { command_numeric = MUL; }
        else if (strcmp(command, "JUMP") == 0) { command_numeric = JUMP; }
        else if (strcmp(command, "JNEG") == 0) { command_numeric = JNEG; }
        else if (strcmp(command, "JZ") == 0) { command_numeric = JZ; }
        else if (strcmp(command, "HALT") == 0) { command_numeric = HALT; }
        else if (strcmp(command, "=") == 0)
        {
            operand = strtok(NULL, " +");
            //printf("operand = %s\n", operand);
            operand_numeric = atoi(operand);
            sc_memorySet(address_numeric, operand_numeric);
            //printf("line %d memory set\n", i + 1);
            continue;
        }
        else
        {
            fprintf(stderr, "%d: unexpected command. Translation breaked\n", i + 1);
            break;
        }

        operand = strtok(NULL, " ");
        //printf("operand = %s\n", operand);
        if (operand == NULL && operand[0] != ';')
        {
            fprintf(stderr, "%d: unexpected operand. translation breaked\n", i + 1);
            break;
        }
        operand_numeric = atoi(operand);

        if (sc_commandEncode(command_numeric, operand_numeric, &value) == -1)
        {
            fprintf(stderr, "%d: encoding error. translation breaked\n", i + 1);
            break;
        }
        sc_memorySet(address_numeric, value);
        //printf("line %d memory set\n", i + 1);
    }
    //printf("file processed\n");
    sc_memorySave(strdup(factorial_filename_binary)); // const char* -> char*
    //printf("file saved\n");
    printf("file saving result is %d",                  sc_memorySave(strdup(factorial_filename_binary)));
}


// argc - количество аргументов командной строки, которые переданы приложению
// sat файл.sa файл.o - 3 аргумента, argc = 3
// argv - указатель на массив строк, который представляет переданный набор аргументов
//
// если не передано ни одного аргумента, argc = 1, argv[0] - имя исполняемого файла
int main(int argc, const char** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "incorrect %s launch\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sc_memoryInit();
    load_program_factorial(argv[1]);
    translate_assembler_to_binary_format(argv[2]);

    return 0;
}
