#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "library.c"

const char commands[][7] = { "REM", "INPUT", "PRINT", "GOTO", "IF", "LET", "END" };

FILE* factorial_sBasic = NULL;
FILE* factorial_sAssembler = NULL;

struct command
{
    int number;  // Номер команды SB
    char* com;
    int address; // адрес ячейки памяти, содержащей команду ??
};
struct command* factorial;

struct var
{
    char name;
    int address; // адрес ячейки памяти, содержащей переменную
    int value;
};
struct var variables[99];

int commandCounterSA = 0; // номер команды Simple Assembler
int commandCounterSB = 0;
int varCounter = 0;


void load_program_factorial(const char* factorial_filename_sBasic,
                            const char* factorial_filename_sAssembler)
{
    if ((factorial_sBasic = fopen(factorial_filename_sBasic, "r")) == NULL)
    {
        fprintf(stderr, "%s cannot be found or opened\n", factorial_filename_sBasic);
        exit(EXIT_FAILURE);
    }

    factorial_sAssembler = fopen (factorial_filename_sAssembler, "w");
}

int getVarAddress(char name) // номер ячейки памяти, где содержится переменная
{
    for (int i = 0; i <= varCounter; i++)
    {
        if (variables[i].name == name) { return variables[i].address; }
    }

    variables[varCounter].name = name;
    variables[varCounter].address = 99 - varCounter;
    varCounter++;

    return variables[varCounter].address;
}

void REM() // args - ??
{

}

void INPUT(char* args) // -> READ - Ввод с терминала в указанную ячейку памяти
{
    if (!(args[0] >= 'A' && args[0] <= 'Z'))
    {
        fprintf(stderr, "%s cannot be a variable name", args);
        exit(EXIT_FAILURE);
    }

    fprintf(factorial_sAssembler, "%.2d READ %d\n", commandCounterSA, getVarAddress(args[0]));

    commandCounterSA++;
}

void PRINT(char* args) // -> WRITE - Вывод на терминал значение указанной ячейки памяти
{
    if (!(args[0] >= 'A' && args[0] <= 'Z'))
    {
        fprintf(stderr, "%s cannot be a variable name", args);
        exit(EXIT_FAILURE);
    }

    fprintf(factorial_sAssembler, "%.2d WRITE %d\n", commandCounterSA, getVarAddress(args[0]));

    commandCounterSA++;
}

void GOTO(int number_of_command) //-> JUMP - Переход к указанному адресу памяти
{
    for (int i = 0; i < commandCounterSB; i++)
    {
        if (factorial[i].number == number_of_command)
        {
            fprintf(factorial_sAssembler, "%.2d JUMP %d\n", commandCounterSA, factorial[i].address);
            commandCounterSA++;
            return;
        }
    }
}

void IF(char *args)
{

}

void LET(char* args)
{

}

void END()
{
    fprintf(factorial_sAssembler, "%.2i HALT 00\n", commandCounterSA);
    commandCounterSA++;
}

void translate_basic_to_assembler()
{
    instructionCounter = 0;
    while (1) //подсчет команд = кол-ва строк в файле
    {
        char line[256];
        fgets(line, 255, factorial_sBasic);
        if (feof(factorial_sBasic)) { break; }
        instructionCounter++;
    }
    commandCounterSB = instructionCounter;

    factorial = (struct command*)malloc(sizeof(struct command)*instructionCounter);
    for (int i = 0; i < instructionCounter; i++)
    {
        factorial[i].com = (char*)malloc(sizeof(char)*256); //выделение памяти под команду
        if (!fgets(factorial[i].com, 255, factorial_sBasic)) //чтение в структуру
        { fprintf(stderr, "line %d of programm cannot be read\n", i++); }

        for (int i = 0; i < instructionCounter; i++)
        {
            char *line_to_split = (char*)malloc(sizeof(char)*256);
            sprintf(line_to_split, "%s", factorial[i].com);

            char* part = strtok(line_to_split, " ");
            int number = atoi(part);
            // проверка?
            factorial[i].number = number;

            part = strtok(line_to_split, " ");
            char* _command = part;

            part = strtok(line_to_split, " ");
            char* args = part;

            // определение команды, переход на функции, которые надо реализовать
        }
    }
}




// argc - количество аргументов командной строки, которые переданы приложению
// sbt файл.sb файл.sa - 3 аргумента, argc = 3
// argv - указатель на массив строк, который представляет переданный набор аргументов
//
// если не передано ни одного аргумента, argc = 1, argv[0] - имя исполняемого файла

int main (int argc, const char** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "incorrect %s launch\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    load_program_factorial(argv[1], argv[2]); // factorial.sb, factorial.sa
    translate_basic_to_assembler();

    return 0;
}
