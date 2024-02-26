#include <stdio.h>
#include <stdlib.h>

#include "library.c"

const char commands[][7] = { "REM", "INPUT", "OUTPUT", "GOTO", "IF", "LET", "END" };

FILE* factorial_sBasic = NULL;
FILE* factorial_sAssembler = NULL;

struct command
{
    int number;
    char* com;
    int address;
};

struct command* factorial;


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

    factorial = (struct command*)malloc(sizeof(struct command)*instructionCounter);
    for (int i = 0; i < instructionCounter; i++)
    {
        factorial[i].com = (char*)malloc(sizeof(char)*256); //выделение памяти под команду
        if (!fgets(factorial[i].com, 255, factorial_sBasic)) //чтение в структуру
        { fprintf(stderr, "line %d of programm cannot be read\n", i++); }

        for (int i = 0; i < instructionCounter; i++)
        {

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
