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
    char instruction[128];
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
int varCounter = -1;


void load_program_factorial(const char* factorial_filename_sBasic,
                            const char* factorial_filename_sAssembler)
{
    if ((factorial_sBasic = fopen(factorial_filename_sBasic, "r")) == NULL)
    {
        fprintf(stderr, "%s cannot be found or opened. Translation breaked\n", factorial_filename_sBasic);
        exit(EXIT_FAILURE);
    }
    if ((factorial_sAssembler = fopen (factorial_filename_sAssembler, "w")) == NULL)
    {
        fprintf(stderr, "%s cannot be found or opened. Translation breaked\n", factorial_filename_sAssembler);
        exit(EXIT_FAILURE);
    }
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
        variables[varCounter].value = value;
        fprintf(factorial_sAssembler, "%.2d = +%.4d\n",
                variables[varCounter].address, variables[varCounter].value);
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
    for (int i = 0; i < varCounter; i++)
    {
        if (variables[i].name == variable_name) { variable_name++; }
    }
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
    for (int i = 0; i < varCounter; i++)
    {
        if (variables[i].name == variable_name) { variable_name++; }
    }
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

void REM(int i, char* args) { }

void INPUT(int i, char* args) // -> READ - Ввод с терминала в указанную ячейку памяти
{
    if (!(args[0] >= 'A' && args[0] <= 'Z'))
    {
        fprintf(stderr, "line %d: %s cannot be a variable name. Translation breaked\n", i, args);
        exit(EXIT_FAILURE);

        if (strlen(args) != 1)
        {
            fprintf(stderr, "line %d: a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", i,  args);
            exit(EXIT_FAILURE);
        }
    }

    fprintf(factorial_sAssembler, "%.2d READ %d\n", commandCounterSA, getVarAddress(args[0]));

    commandCounterSA++;
}

void PRINT(int i, char* args) // -> WRITE - Вывод на терминал значение указанной ячейки памяти
{
    if (!(args[0] >= 'A' && args[0] <= 'Z'))
    {
        fprintf(stderr, "line %d: %s cannot be a variable name. Translation breaked\n", i, args);
        exit(EXIT_FAILURE);

        if (strlen(args) != 1)
        {
            fprintf(stderr, "line %d: a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", i,  args);
            exit(EXIT_FAILURE);
        }
    }

    fprintf(factorial_sAssembler, "%.2d WRITE %d\n",
            commandCounterSA, getVarAddress(args[0]));

    commandCounterSA++;
}

void GOTO(int i, char option, int number_of_command) //-> JUMP - Переход к указанному адресу памяти
{
    for (int i = 0; i < commandCounterSB; i++)
    {
        if (program[i].number == number_of_command)
        {
            switch(option)
            {
            case '<':
            case '>':
                fprintf(factorial_sAssembler, "%.2d JNEG %d\n",
                        commandCounterSA, program[i].address);
                break;

            case '=':
                fprintf(factorial_sAssembler, "%.2d JZ %d\n",
                        commandCounterSA, program[i].address);
                break;

            case '0':
                fprintf(factorial_sAssembler, "%.2d JUMP %d\n",
                    commandCounterSA, program[i].address);
                break;

            default:
                fprintf(stderr, "line %d: cannot recognize goto condition. Translation breaked\n", i);
                exit(EXIT_FAILURE);
                break; //?
            }

            commandCounterSA++;
            return;
        }
    }
}

void LET(int i, char* args)
{
    char separator[5] = { '=', '>', '<', ' ' };
    int assignment_part_position;

    //подсчет: где начинается часть задания значения переменной
    for (assignment_part_position = 0; assignment_part_position < strlen(args); assignment_part_position++)
    {
        if (args[assignment_part_position] == '=')
        { assignment_part_position++; break; }
    }
    char* assignment_part = args + assignment_part_position;
    if (assignment_part[0] == ' ') { assignment_part++; }

    //выделение переменной, которой будет присвоено значение
    char* variable = strtok(args, separator);
    if (!(variable[0] >= 'A' && variable[0] <= 'Z'))
    {
        fprintf(stderr, "line %d: %s cannot be a variable name. Translation breaked\n", i, variable);
        exit(EXIT_FAILURE);

        if (strlen(variable) != 1)
        {
            fprintf(stderr, "line %d: a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", i, variable);
            exit(EXIT_FAILURE);
        }
    }
    getVarValue(variable[0]);

    //обработка присваемого выражения
    //переменная или выражение
    if (atoi(assignment_part) == 0 && (assignment_part[0] >= 'A' && assignment_part[0] <= 'Z'))
    {
        char operator[4] = { '+', '-', '*', '/'};

        //LET var = выражение([переменная] +,-,* или / [другая переменная или число])
        // ******************* //
        // БЕЗ СКОБОК!!! Добавить
        // ******************* //

        //может быть одна переменная без знака - добавить
        // //операция между операндами
        char sign_of_operator;
        for (int j = 0; j < strlen(assignment_part); j++)
        {
            for (int i = 0; i < 4; i++)
            {
                if (assignment_part[j] == operator[i])
                { sign_of_operator = assignment_part[j]; }
            }
        }

        //первый операнд
        char* op1 = strtok(assignment_part, operator);
        //переменная
        if (atoi(op1) == 0 && (op1[0] >= 'A' && op1[0] <= 'Z'))
        {
            //переменная содержит в названии более 1 символа
            if (strlen(op1) != 1)
            {
                fprintf(stderr, "line %d: a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", i, op1);
                exit(EXIT_FAILURE);
            }
            getVarValue(op1[0]);
        }
        //число
        else if (op1[0] >= '0' && op1[0] <= '9') { op1[0] = getVarName(atoi(op1)); }
        //другое
        else
        {
            fprintf(stderr, "line %d: cannot recognize not a symbol, nor a digit in line \"%s\". Translation breaked\n", i, op1);
            exit(EXIT_FAILURE);
        }

        //второй операнд
        char* op2 = assignment_part + strlen(op1) + 1;
        while (op2[1] != '\n')
        {
            if ((op2[0] <= 'A' && op2[0] >= 'Z')
                || (op2[0] <= '0' && op2[0] >= '9')) { break; }
            op2++;
        }
        op2 = strtok(op2, "\n");
        //переменная
        if (atoi(op2) == 0 && (op2[0] >= 'A' && op2[0] <= 'Z'))
        {
            //переменная содержит в названии более 1 символа
            if (strlen(op2) != 1)
            {
                fprintf(stderr, "line %d: a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", i, op2);
                exit(EXIT_FAILURE);
            }
            getVarValue(op2[0]);
        }
        //число
        else if (op2[0] >= '0' && op2[0] <= '9') { op2[0] = getVarName(atoi(op2)); }
        //другое
        else
        {
            fprintf(stderr, "line %d: cannot recognize not a symbol, nor a digit in line \"%s\". Translation breaked\n", i, op2);
            exit(EXIT_FAILURE);
        }

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
            fprintf(stderr, "line %d: cannot recognize operator sign: %c. Translation breaked\n", i, sign_of_operator);
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
        fprintf(stderr, "line %d: cannot recognize not a symbol, nor a digit in line \"%s\". Translation breaked\n", i, assignment_part);
        exit(EXIT_FAILURE);
    }
}

//if args=(expression then)
//expression: op1 sign_of_comparision op2
void IF(int i, char *args)
{
    char sign[4] = { '>', '=', '<', ' ' };

    char expression[strlen(args) + 1];
    strcpy(expression, args);

    //первое вхождение в expression знака сравнения
    char sign_of_comparision;
    for (int j = 0; j < strlen(args); j++)
    {
        for (int i = 0; i < 3; i++)
        {
            if (args[j] == sign[i])
            { sign_of_comparision = args[j]; }
        }
    }
    printf("sign_of_comparision = %c\n", sign_of_comparision);

    //первый операнд в выражении
    char* op1 = strtok(args, sign);
    //переменная
    if (atoi(op1) == 0 && (op1[0] >= 'A' && op1[0] <= 'Z'))
    {
        //переменная содержит в названии более 1 символа
        if (strlen(op1) != 1)
        {
            fprintf(stderr, "line %d: a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", i, op1);
            exit(EXIT_FAILURE);
        }
        getVarValue(op1[0]);
    }
    //число
    else if (op1[0] >= '0' && op1[0] <= '9') { op1[0] = getVarName(atoi(op1)); }
    //другое
    else
    {
        fprintf(stderr, "line %d: cannot recognize not a symbol, nor a digit in line \"%s\". Translation breaked\n", i, op1);
        exit(EXIT_FAILURE);
    }
    printf("op1 = %s\n", op1);

    //второй операнд в выражении
    char* op2 = expression + strlen(op1);
    printf("op2 = %s\n", op2);
    op2 = strtok(op2, sign);
    printf("op2 = %s\n", op2);
    //переменная
    if (atoi(op2) == 0 && (op2[0] >= 'A' && op2[0] <= 'Z'))
    {
        //переменная содержит в названии более 1 символа
        if (strlen(op2) != 1)
        {
            fprintf(stderr, "line %d: a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", i, op2);
            exit(EXIT_FAILURE);
        }
        getVarValue(op2[0]);
    }
    //число
    else if (op2[0] >= '0' && op2[0] <= '9') { op2[0] = getVarName(atoi(op2)); }
    //другое
    else
    {
        fprintf(stderr, "line %d: cannot recognize not a symbol, nor a digit in line \"%s\". Translation breaked\n", i, op2);
        exit(EXIT_FAILURE);
    }
    printf("op2 = %s\n", op2);

    char* then = expression;
    printf("then = %s\n", then);

    /*strstr ищет первое вхождение подстроки GOTO в строке then.
     * Возвращает указатель на первое вхождение строки GOTO в строку then,
     * или пустой указатель, если строка GOTO не является частью строки then*/
    if (strstr(then, "GOTO") == NULL)
    {
        do { then = strtok(NULL, " "); }
        while (strstr(then, "GOTO"));
        printf("then = %s\n", then);

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
            GOTO(i, '<', atoi(then));
            break;

        case '>':
            fprintf(factorial_sAssembler, "%.2d LOAD %d\n",
                    commandCounterSA, getVarAddress(op2[0]));
            commandCounterSA++;

            fprintf(factorial_sAssembler, "%.2d SUB %d\n",
                    commandCounterSA, getVarAddress(op1[0]));
            commandCounterSA++;
            GOTO(i, '>', atoi(then));
            break;

        case '=':
            fprintf(factorial_sAssembler, "%.2d LOAD %d\n",
                    commandCounterSA, getVarAddress(op1[0]));
            commandCounterSA++;

            fprintf(factorial_sAssembler, "%.2d SUB %d\n",
                    commandCounterSA, getVarAddress(op2[0]));
            commandCounterSA++;
            GOTO(i, '=', atoi(then));
            break;

        default:
            fprintf(stderr, "line %d: cannot recognize comparision sign: %c. Translation breaked\n", i, sign_of_comparision);
            exit(EXIT_FAILURE);
            break; //нужно ли?
        }
    }
    else { LET(i, then); } //иначе идет присвоение значения какой-либо переменной
}

void END() { fprintf(factorial_sAssembler, "%.2i HALT 00\n", commandCounterSA); }

void translate_basic_to_assembler()
{
    instructionCounter = 0;
    char line[128];
    while (1) //подсчет команд = кол-ва строк в файле
    {
        if (feof(factorial_sBasic)) { break; }
        fgets(line, 127, factorial_sBasic);
        instructionCounter++;
    }
    if (instructionCounter > 99)
    {
        fprintf(stderr, "program contains too many lines: %d. Translation breaked\n",
                instructionCounter);
        exit(EXIT_FAILURE);
    }
    commandCounterSB = instructionCounter;

    fseek(factorial_sBasic, 0, SEEK_SET);

    program = (struct command*)malloc(sizeof(struct command)*instructionCounter);
    for (int i = 0; i < instructionCounter; i++)
    {
        if (fgets(program[i].instruction, 127, factorial_sBasic) == NULL) //чтение в структуру
        {
            if (feof(factorial_sBasic)) { break; }
            fprintf(stderr, "line %d of programm cannot be read. Translation breaked\n", i++);
            exit(EXIT_FAILURE);
        }
        program[i].address = i;
        sprintf(line, "%s", program[i].instruction);
        program[i].number = atoi(strtok(line, " "));
    }

    for (int i = 0; i < instructionCounter; i++)
    {
        sprintf(line, "%s", program[i].instruction);

        /* program[i].number = atoi(strtok(line_to_split, " ")); */
        /* printf("program[i].number = %d\n", program[i].number); */
        strtok(line, " ");
        char* function = strtok(NULL, " ");
        if (function[0] == 'E' && function[1] == 'N' && function[2] == 'D')
        {
            END();
            return;
        }

        char* args = strtok(NULL, "");

        if (strcmp(function, "REM") == 0) { REM(i+1, args); }
        else if (strcmp(function, "INPUT") == 0) { INPUT(i+1, args); }
        else if (strcmp(function, "PRINT") == 0) { PRINT(i+1, args); }
        else if (strcmp(function, "GOTO") == 0) { GOTO(i+1, '0', atoi(args)); }
        else if (strcmp(function, "IF") == 0) { IF(i+1, args); }
        else if (strcmp(function, "LET") == 0) { LET(i+1, args); }
        else
        {
            fprintf(stderr, "line %d: unexpected command: \"%s\". Translation breaked\n",
                    i+1, function);
            exit(EXIT_FAILURE);
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
