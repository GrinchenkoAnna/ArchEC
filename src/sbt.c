#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "library.c"

FILE* factorial_sBasic = NULL;
FILE* factorial_sAssembler = NULL;
int goto_instruction = 0;
int goto_cell[99];
char variable_name = 'A'; //всего 26

struct command
{
    int number;  // Номер команды SB
    char* instruction;
    int address; // адрес ячейки памяти, содержащей команду
};
struct command* program;

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
        fprintf(stderr, "%s cannot be found or opened. Translation breaked\n",
                factorial_filename_sBasic);
        exit(EXIT_FAILURE);
    }

    factorial_sAssembler = fopen (factorial_filename_sAssembler, "w");
}

char getVarName (int value) //создать новую переменную, задать значение, адрес и вернуть имя
{
    varCounter++;
    for (int i = 0; i < varCounter - 1; i++)
    {
        if (variables[i].name == variable_name) { variable_name++; }
    }
    if (variable_name <= 'Z')
    {
        variables[varCounter].name = variable_name;
        variables[varCounter].address = 99 - varCounter;
        if (variables[varCounter].address <= commandCounterSA)
        {
            fprintf(stderr, "cell %d cannot hold a variable because it holds a command. Translation breaked\n",
                    variables[varCounter].address);
            exit(EXIT_FAILURE);
        }
        variables[varCounter].value = 0;
    }
    else
    {
        fprintf(stderr, "%c cannot be a variable name. Translation breaked\n", variable_name);
        exit(EXIT_FAILURE);
    }

    return variables[varCounter].name;
}

int getVarValue (char name) //найти значение ячейки памяти, где содержится переменная, либо создать новую и вернуть значение
{
    for (int i = 0; i <= varCounter; i++)
    {
        if (variables[i].name == name) { return variables[i].value; }
    }

    varCounter++;
    variables[varCounter].name = name;
    variables[varCounter].address = 99 - varCounter;
    if (variables[varCounter].address <= commandCounterSA)
    {
        fprintf(stderr, "cell %d cannot hold a variable because it holds a command. Translation breaked\n",
                variables[varCounter].address);
        exit(EXIT_FAILURE);
    }
    variables[varCounter].value = 0;

    return variables[varCounter].value;
}

int getVarAddress(char name) //найти номер ячейки памяти, где содержится переменная, либо создать новую и вернуть адрес
{
    for (int i = 0; i <= varCounter; i++)
    {
        if (variables[i].name == name) { return variables[i].address; }
    }

    varCounter++;
    variables[varCounter].name = name;
    variables[varCounter].address = 99 - varCounter;
    if (variables[varCounter].address <= commandCounterSA)
    {
        fprintf(stderr, "cell %d cannot hold a variable because it holds a command. Translation breaked\n",
                variables[varCounter].address);
        exit(EXIT_FAILURE);
    }
    variables[varCounter].value = 0;

    return variables[varCounter].address;
}

void REM(char* args)
{

}

void INPUT(char* args) // -> READ - Ввод с терминала в указанную ячейку памяти
{
    if (!(args[0] >= 'A' && args[0] <= 'Z'))
    {
        fprintf(stderr, "%s cannot be a variable name. Translation breaked\n", args);
        exit(EXIT_FAILURE);
    }
    else if (args[1] != ' ' || args[1] != '\0')
    {
        fprintf(stderr, "a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", args);
        exit(EXIT_FAILURE);
    }

    fprintf(factorial_sAssembler, "%.2d READ %d\n", commandCounterSA, getVarAddress(args[0]));

    commandCounterSA++;
}

void PRINT(char* args) // -> WRITE - Вывод на терминал значение указанной ячейки памяти
{
    if (!(args[0] >= 'A' && args[0] <= 'Z'))
    {
        fprintf(stderr, "%s cannot be a variable name. Translation breaked\n", args);
        exit(EXIT_FAILURE);
    }
    else if (args[1] != ' ' || args[1] != '\0')
    {
        fprintf(stderr, "a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", args);
        exit(EXIT_FAILURE);
    }

    fprintf(factorial_sAssembler, "%.2d WRITE %d\n",
            commandCounterSA, getVarAddress(args[0]));

    commandCounterSA++;
}

void GOTO(int number_of_command) //-> JUMP - Переход к указанному адресу памяти
{
    for (int i = 0; i < commandCounterSB; i++)
    {
        if (program[i].number == number_of_command)
        {
            fprintf(factorial_sAssembler, "%.2d JUMP %d\n",
                    commandCounterSA, program[i].address);
            commandCounterSA++;
            return;
        }
    }
}

//if args=(expression then)
//expression: op1 sign_of_comparision op2
void IF(char *args) //добавить обработку пробелов
{
    char sign[3] = { '>', '=', '<' };

    char* expression = strtok(args, "");

    char* op1 = strtok(expression, sign);
    //переменная
    if (atoi(op1) == 0 && (op1[0] >= 'A' && op1[0] <= 'Z'))
    {
        //переменная содержит в названии более 1 символа
        if (op1[1] != ' ' || op1[1] != '\0')
        {
            fprintf(stderr, "a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", op1);
            exit(EXIT_FAILURE);
        }
        getVarValue(op1[0]);
    }
    //число
    else if (atoi(op1) != 0 && (op1[0] >= '0' && op1[0] <= '9'))
        { op1[0] = getVarName(atoi(op1)); }
    //другое
    else
    {
        fprintf(stderr, "cannot recognize not a symbol, nor a digit in line \"%s\". Translation breaked\n", op1);
        exit(EXIT_FAILURE);
    }

    //первое вхождение в expression знака сравнения
    char sign_of_comparision;
    for (int i = 0; i < 3; i++)
    {
        if (strchr(expression, sign[i]) != NULL)
            { sign_of_comparision = *strchr(expression, sign[i]); }
    }

    char* op2 = expression + strlen(op1) + 1;
    //переменная
    if (atoi(op2) == 0 && (op2[0] >= 'A' && op2[0] <= 'Z'))
    {
        //переменная содержит в названии более 1 символа или ее название состоит не из буквы верхнего регистра
        if (op2[1] != ' ' || op2[1] != '\0')
        {
            fprintf(stderr, "a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", op2);
            exit(EXIT_FAILURE);
        }
        getVarValue(op2[0]);
    }
    //число
    else if (atoi(op2) != 0 && (op1[0] >= '0' && op1[0] <= '9'))
        { op2[0] = getVarName(atoi(op2)); }
    //другое
    else
    {
        fprintf(stderr, "cannot recognize not a symbol, nor a digit in line \"%s\". Translation breaked\n", op2);
        exit(EXIT_FAILURE);
    }

    char* then = strtok(args, " "); //пока обработка только GOTO
    if (strstr(then, "GOTO") != NULL)
    {
        goto_cell[goto_instruction] = atoi(strtok(then, " "));
        goto_instruction++;
    }
    else { }

    //обработка условия
    switch (sign_of_comparision)
    {
    case '<':
        fprintf(factorial_sAssembler, "%.2d LOAD %d\n",
                commandCounterSA, getVarAddress(op1[0]));
        commandCounterSA++;

        fprintf(factorial_sAssembler, "%.2d SUB %d\n",
                commandCounterSA, getVarAddress(op2[0]));
        commandCounterSA++;

        fprintf(factorial_sAssembler, "%.2d JNEG %d\n",
                commandCounterSA, goto_cell[goto_instruction]);
        commandCounterSA++;
        break;

    case '>':
        fprintf(factorial_sAssembler, "%.2d LOAD %d\n",
                commandCounterSA, getVarAddress(op2[0]));
        commandCounterSA++;

        fprintf(factorial_sAssembler, "%.2d SUB %d\n",
                commandCounterSA, getVarAddress(op1[0]));
        commandCounterSA++;

        fprintf(factorial_sAssembler, "%.2d JNEG %d\n",
                commandCounterSA, goto_cell[goto_instruction]);
        commandCounterSA++;
        break;

    case '=':
        fprintf(factorial_sAssembler, "%.2d LOAD %d\n",
                commandCounterSA, getVarAddress(op1[0]));
        commandCounterSA++;

        fprintf(factorial_sAssembler, "%.2d SUB %d\n",
                commandCounterSA, getVarAddress(op2[0]));
        commandCounterSA++;

        fprintf(factorial_sAssembler, "%.2d JZ %d\n",
                commandCounterSA, goto_cell[goto_instruction]);
        commandCounterSA++;
        break;

    default:
        fprintf(stderr, "cannot recognize comparision sign: %c. Translation breaked\n", sign_of_comparision);
        exit(EXIT_FAILURE);
        break; //нужно ли?
    }
}

void LET(char* args)
{
    char separator[4] = { ' ', '=', '>', '<' };

    char* variable = strtok(args, separator);
    if (!(variable[0] >= 'A' && variable[0] <= 'Z'))
    {
        fprintf(stderr, "%s cannot be a variable name. Translation breaked\n", variable);
        exit(EXIT_FAILURE);
    }
    else if ((variable[0] >= 'A' && variable[0] <= 'Z')
        || (variable[1] != ' ' || variable[1] != '\0'))
    {
        fprintf(stderr, "a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", variable);
        exit(EXIT_FAILURE);
    }
    getVarValue(variable[0]);

    char* assignment_part = strtok(NULL, separator);
    //переменная
    if (atoi(assignment_part) == 0 && (assignment_part[0] >= 'A' && assignment_part[0] <= 'Z'))
    {
        if (assignment_part[1] != ' ' || assignment_part[1] != '\0')
        {
            fprintf(stderr, "a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", assignment_part);
        exit(EXIT_FAILURE);
        }
        getVarValue(assignment_part[0]);
        char operator[4] = { '+', '-', '*', '/' };
        //LET var = выражение([переменная] +,-,* или / [другая переменная или число])
        // ******************* //
        // БЕЗ СКОБОК!!! Добавить
        // ******************* //
        char* op1 = strtok(assignment_part, operator);

        char sign_of_operator;
        for (int i = 0; i < 4; i++)
        {
            if (strchr(assignment_part, operator[i]) != NULL)
                { sign_of_operator = *strchr(assignment_part, operator[i]); }
        }

        char* op2 = assignment_part + strlen(op1) + 1;

        //обработка выражения
        fprintf(factorial_sAssembler, "%.2d LOAD %d\n",
                    commandCounterSA, getVarAddress(op1[0]));
        commandCounterSA++;

        switch (sign_of_operator)
        {
        case '+':
            fprintf(factorial_sAssembler, "%.2d ADD %d\n",
                    commandCounterSA, getVarAddress(op2[0]));
            break;

        case '-':
            fprintf(factorial_sAssembler, "%.2d SUB %d\n",
                    commandCounterSA, getVarAddress(op2[0]));
            break;

        case '*':
            fprintf(factorial_sAssembler, "%.2d MUL %d\n",
                    commandCounterSA, getVarAddress(op2[0]));
            break;

        case '/':
            fprintf(factorial_sAssembler, "%.2d DIVIDE %d\n",
                    commandCounterSA, getVarAddress(op2[0]));
            break;

        default:
            fprintf(stderr, "cannot recognize operator sign: %c. Translation breaked\n", sign_of_operator);
            exit(EXIT_FAILURE);
            break; //нужно ли?
        }
        commandCounterSA++;

        fprintf(factorial_sAssembler, "%.2d STORE %d\n",
                    commandCounterSA, getVarAddress(variable[0]));
        commandCounterSA++;
    }
    //число
    else if (atoi(assignment_part) != 0 && (assignment_part[0] >= '0' && assignment_part[0] <= '9'))
    {
        //LET variable = число
        fprintf(factorial_sAssembler, "%.2d = +%.4d\n",
                getVarAddress(variable[0]), atoi(assignment_part));
    }
    //другое
    else
    {
        fprintf(stderr, "cannot recognize not a symbol, nor a digit in line \"%s\". Translation breaked\n", assignment_part);
        exit(EXIT_FAILURE);
    }
}

void END() { fprintf(factorial_sAssembler, "%.2i HALT 00\n", commandCounterSA); }

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
    if (instructionCounter > 99)
    {
        fprintf(stderr, "program contains too many lines: %d. Translation breaked\n",
                instructionCounter);
        exit(EXIT_FAILURE);
    }
    commandCounterSB = instructionCounter;


    program = (struct command*)malloc(sizeof(struct command)*instructionCounter);
    for (int i = 0; i < instructionCounter; i++)
    {
        program[i].instruction = (char*)malloc(sizeof(char)*128); //выделение памяти под команду
        if (!fgets(program[i].instruction, 127, factorial_sBasic)) //чтение в структуру
        { fprintf(stderr, "line %d of programm cannot be read. Translation breaked\n", i++); }

        for (int i = 0; i < instructionCounter; i++)
        {
            char *line_to_split = (char*)malloc(sizeof(char)*128);
            sprintf(line_to_split, "%s", program[i].instruction);

            program[i].number = atoi(strtok(line_to_split, " "));
            char* function = strtok(NULL, " ");
            char* args = strtok(NULL, " ");

            if (strcmp(function, "REM") == 0) { REM(args); }
            else if (strcmp(function, "UNPUT") == 0) { INPUT(args); }
            else if (strcmp(function, "PRINT") == 0) { PRINT(args); }
            else if (strcmp(function, "GOTO") == 0) { GOTO(atoi(args)); }
            else if (strcmp(function, "IF") == 0) { IF(args); }
            else if (strcmp(function, "LET") == 0) { }
            else if (strcmp(function, "END") == 0) { END(); }
            else
            {
                fprintf(stderr, "unexpected command: %d %s %s. Translation breaked\n",
                        program[i].number, function, args);
                exit(EXIT_FAILURE);
            }

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
