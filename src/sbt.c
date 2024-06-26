#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mySimpleComputer.c"

int pass = 0;
int* goto_from;
int* goto_to;
int goto_index = 0;
int goto_current = 0;

FILE* sBasic = NULL;
FILE* sAssembler = NULL;

char variable_name = 'Z'; //всего 26

int commandCounterSA = 0; // номер команды Simple Assembler
int commandCounterSB = 0;
int varCounter = -1;

int getVarValue (char name);
int getVarAddress(char name);
char getVarName (int value);

struct command //команды Simple Basic
{
    int number;  // Номер команды SB
    char instruction[128];
    int address; //адрес ячейки памяти, содержащей команду
};
struct command* program;

struct var //вводимые переменные
{
    char name;
    int address; //адрес ячейки памяти, содержащей переменную
    int value;
};
struct var variables[99];

typedef struct Node
{
    char data;
    struct Node* next;
} node;

void push(char data, node** head)
{
    node* temp = (node*)malloc(sizeof(node));
    temp->data = data;
    temp->next = *head;
    *head = temp;
}

char pop(node** head)
{
    node* temp;
    char data;

    if (*head == NULL)
    {
        fprintf(stderr, "error in pop function: *head == NULL\n");
        exit(EXIT_FAILURE);
    }

    temp = *head;
    *head = temp->next;
    data = temp->data;
    free(temp);

    return data;
}

char top(node* head) { return head->data; }

int get_priority(char sign)
{
    switch (sign)
    {
    case '+':
    case '-':
        return 1;

    case '*':
    case '/':
        return 2;

    case '(':
    case ')':
        return 3;

    default:
        return 0;
    }
}

char* translate_to_RPN(char* args, char* rpn)
{
    node* head = NULL;
    int rpn_index = 0;
    int i = 0;

    while (args[i] != '\n' && args[i] != '\0')
    {
        char temp = args[i];

        if ((temp >= 'A' && temp <= 'Z') || (temp >= '0' && temp <= '9')) //переменная или число
        {
            rpn[rpn_index] = temp;
            rpn_index++;
        }
        else if (temp == '(') { push(temp, &head); }
        else if (temp == ')')
        {
            while (top(head) != '(')
            {
                char temp1 = pop(&head);
                rpn[rpn_index] = temp1;
                rpn_index++;
                if (head == NULL)
                {
                    fprintf(stderr, "impossible to find paired brackets in the expression %s. Translation breaked\n", args);
                    exit(EXIT_FAILURE);
                }
            }
            pop(&head);
        }
        else if (temp == '+' || temp == '-' || temp == '*' || temp == '/')
        {
            while (head != NULL && get_priority(head->data) >= get_priority(temp) && head->data != '(')
            {
                char temp2 = pop(&head);
                rpn[rpn_index] = temp2;
                rpn_index++;
            }
            push(temp, &head);
        }
        else { i++; continue; }
        i++;
    }

    while (head != NULL)
    {
        char temp = pop(&head);
        rpn[rpn_index] = temp;
        rpn_index++;
    }
    rpn[rpn_index] = '\0';

    return rpn;
}

void calculate_RPN(int j, char* rpn) //запуталась
{
    int i = 0;

    while (rpn[i] != '\0')
    {
        if (rpn[i] >= 'A' && rpn[i] <= 'Z')
        {
            fprintf(sAssembler, "%.2d LOAD %d\n",
                        commandCounterSA, getVarAddress(rpn[i]));
            commandCounterSA++;

            fprintf(sAssembler, "%.2d STORE %d\n",
                        commandCounterSA, 99 - varCounter);
            commandCounterSA++;

            varCounter++;
        }
        else if (rpn[i] >= '0' && rpn[i] <= '9')
        {
            fprintf(sAssembler, "%.2d LOAD %d\n",
                        commandCounterSA, getVarAddress(getVarName(rpn[i])));
            commandCounterSA++;

            fprintf(sAssembler, "%.2d STORE %d\n",
                        commandCounterSA, 99 - varCounter);
            commandCounterSA++;

            varCounter++;
        }
        else
        {
            fprintf(sAssembler, "%.2d LOAD %d\n",
                        commandCounterSA, 99 - varCounter + 1);
            commandCounterSA++;

            switch (rpn[i])
            {
            case '+':
                fprintf(sAssembler, "%.2d ADD %d\n",
                        commandCounterSA, 99 - varCounter + 2);
                commandCounterSA++;
                break;

            case '-':
                fprintf(sAssembler, "%.2d SUB %d\n",
                        commandCounterSA, 99 - varCounter + 2);
                commandCounterSA++;
                break;

            case '*':
                fprintf(sAssembler, "%.2d MUL %d\n",
                        commandCounterSA, 99 - varCounter + 2);
                commandCounterSA++;
                break;

            case '/':
                fprintf(sAssembler, "%.2d DIVIDE %d\n",
                        commandCounterSA, 99 - varCounter + 2);
                commandCounterSA++;
                break;

            default:
                fprintf(stderr, "cannot calculate string %s in %c. Translation breaked\n", rpn, rpn[i]);
                exit(EXIT_FAILURE);
            }

            fprintf(sAssembler, "%.2d STORE %d\n",
                    commandCounterSA, 99 - varCounter + 2);
            commandCounterSA++;
            varCounter--;
        }
        i++;
    }
}

void load_program(const char* filename_sBasic, const char* filename_sAssembler)
{
    if ((sBasic = fopen(filename_sBasic, "r")) == NULL)
    {
        fprintf(stderr, "%s cannot be found or opened. Translation breaked\n", filename_sBasic);
        exit(EXIT_FAILURE);
    }
    if ((sAssembler = fopen (filename_sAssembler, "w")) == NULL)
    {
        fprintf(stderr, "%s cannot be found or opened. Translation breaked\n", filename_sAssembler);
        exit(EXIT_FAILURE);
    }
}

char getVarName (int value) //создать новую переменную, задать значение, адрес и вернуть имя
{
    varCounter++;
    for (int i = 0; i < varCounter - 1; i++)
    {
        if (variables[i].name == variable_name) { variable_name--; i = 0; }
    }
    if (variable_name >= 'A')
    {
        variables[varCounter].name = variable_name;
        variable_name--;
        variables[varCounter].address = 99 - varCounter;
        if (variables[varCounter].address <= commandCounterSA)
        {
            fprintf(stderr, "cell %d cannot hold a variable because it holds a command. Translation breaked\n",
                    variables[varCounter].address);
            exit(EXIT_FAILURE);
        }
        variables[varCounter].value = value;
        fprintf(sAssembler, "%.2d = +%.4d\n",
                variables[varCounter].address, variables[varCounter].value);
    }
    else
    {
        fprintf(stderr, "getVarName: %c cannot be a variable name. Translation breaked\n", variable_name);
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
        if (variables[i].name == variable_name) { variable_name--; i = 0; }
    }
    variables[varCounter].address = 99 - varCounter;
    if (variables[varCounter].address <= commandCounterSA)
    {
        fprintf(stderr, "getVarValue: cell %d cannot hold a variable because it holds a command. Translation breaked\n",
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
        if (variables[i].name == variable_name) { variable_name--; i = 0; }
    }
    variables[varCounter].address = 99 - varCounter;
    if (variables[varCounter].address <= commandCounterSA)
    {
        fprintf(stderr, "getVarAddress: cell %d cannot hold a variable because it holds a command. Translation breaked\n",
                variables[varCounter].address);
        exit(EXIT_FAILURE);
    }

    return variables[varCounter].address;
}

void REM(int i, char* args) { }

void INPUT(int i, char* args) // -> READ - Ввод с терминала в указанную ячейку памяти
{
    if (!(args[0] >= 'A' && args[0] <= 'Z'))
    {
        fprintf(stderr, "INPUT: line %d: %s cannot be a variable name. Translation breaked\n", i, args);

        if (strlen(args) != 1)
        {
            fprintf(stderr, "INPUT: line %d: a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", i,  args);
        }
        exit(EXIT_FAILURE);
    }

    fprintf(sAssembler, "%.2d READ %d\n",
                commandCounterSA, getVarAddress(args[0]));
    commandCounterSA++;
}

void PRINT(int i, char* args) // -> WRITE - Вывод на терминал значение указанной ячейки памяти
{
    if (!(args[0] >= 'A' && args[0] <= 'Z'))
    {
        fprintf(stderr, "PRINT: line %d: %s cannot be a variable name. Translation breaked\n", i, args);

        if (strlen(args) != 1)
        {
            fprintf(stderr, "PRINT: line %d: a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", i,  args);
        }
        exit(EXIT_FAILURE);
    }

    fprintf(sAssembler, "%.2d WRITE %d\n",
            commandCounterSA, getVarAddress(args[0]));
    commandCounterSA++;
}

void GOTO(int i, char option, int number_of_command) //-> JUMP - Переход к указанному адресу памяти
{
    if (!pass)
    {
        goto_to = (int*)realloc(goto_to, sizeof(int));
        goto_from = (int*)realloc(goto_from, sizeof(int));
        goto_to[goto_index] = number_of_command;
        goto_from[goto_index] = commandCounterSA;
        goto_index++;
    }
    else
    {
        switch(option)
        {
        case '<':
        case '>':
            fprintf(sAssembler, "%.2d JNEG %d\n",
                    commandCounterSA, goto_to[goto_current]);
            break;

        case '=':
            fprintf(sAssembler, "%.2d JZ %d\n",
                    commandCounterSA, goto_to[goto_current]);
            break;

        case '0':
            fprintf(sAssembler, "%.2d JUMP %d\n",
                    commandCounterSA, goto_to[goto_current]);
            break;

        default:
            fprintf(stderr, "GOTO: line %d: cannot recognize goto condition. Translation breaked\n", i);
            exit(EXIT_FAILURE);
            break; //?
        }
        goto_current++;
    }
    commandCounterSA++;

    return;
}

void LET(int i, char* args)
{
    char separator[5] = { '=', '>', '<', ' ' };
    char operator[4] = { '+', '-', '*', '/'};
    int assignment_part_position;
    int complicated = 0;

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
        fprintf(stderr, "LET: line %d: %s cannot be a variable name. Translation breaked\n", i, variable);

        if (strlen(variable) != 1)
        {
            fprintf(stderr, "LET: line %d: a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", i, variable);
        }
        exit(EXIT_FAILURE);
    }
    getVarValue(variable[0]);

    //будет ли сложное выражение
    //
    for (int j = 0; j < strlen(assignment_part); j++)
    {
        for (int i = 0; i < 4; i++)
        {
            if (assignment_part[j] == operator[i]
                || assignment_part[j] == '(' || assignment_part[j] == ')')
            { complicated++; }
        }
    }

    //обработка присваемого выражения
    //переменная или простое выражение
    if (atoi(assignment_part) == 0 && (assignment_part[0] >= 'A' && assignment_part[0] <= 'Z') && complicated < 2)
    {
        //операция между операндами
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
            if (strlen(op1) != 1 && op1[1] != '\n')
            {
                fprintf(stderr, "LET: line %d: a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", i, op1);
                exit(EXIT_FAILURE);
            }
            getVarValue(op1[0]);
        }
        //число
        else if (op1[0] >= '0' && op1[0] <= '9') { op1[0] = getVarName(atoi(op1)); }
        //другое
        else
        {
            fprintf(stderr, "LET: line %d: cannot recognize not a symbol, nor a digit in line \"%s\". Translation breaked\n", i, op1);
            exit(EXIT_FAILURE);
        }
        if (op1[1] == '\n')
        {
            fprintf(sAssembler, "%.2d LOAD %d\n",
                    commandCounterSA, getVarAddress(op1[0]));
            commandCounterSA++;
            fprintf(sAssembler, "%.2d STORE %d\n",
                    commandCounterSA, getVarAddress(variable[0]));
            commandCounterSA++;
            return;
        }
        else
        {
            //второй операнд
            char* op2 = assignment_part + strlen(op1) + 1;
            while ((op2[0] == operator[0]) || (op2[0] == operator[1])
                   || (op2[0] == operator[2]) || (op2[0] == operator[3])
                   || (op2[0] == ' ')) { op2++; }
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
                    fprintf(stderr, "LET: line %d: a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", i, op2);
                    exit(EXIT_FAILURE);
                }
                getVarValue(op2[0]);
            }
            //число
            else if (op2[0] >= '0' && op2[0] <= '9') { op2[0] = getVarName(atoi(op2)); }
            //другое
            else
            {
                fprintf(stderr, "LET: line %d: cannot recognize not a symbol, nor a digit in line \"%s\". Translation breaked\n", i, op2);
                exit(EXIT_FAILURE);
            }

            //обработка выражения
            if (pass)
            {
                fprintf(sAssembler, "%.2d LOAD %d\n",
                        commandCounterSA, getVarAddress(op1[0]));
            }
            commandCounterSA++;

            switch (sign_of_operator)
            {
            case '+':
                fprintf(sAssembler, "%.2d ADD %d\n",
                        commandCounterSA, getVarAddress(op2[0]));
                break;

            case '-':
                fprintf(sAssembler, "%.2d SUB %d\n",
                            commandCounterSA, getVarAddress(op2[0]));
                break;

            case '*':
                fprintf(sAssembler, "%.2d MUL %d\n",
                            commandCounterSA, getVarAddress(op2[0]));
                break;

            case '/':
                fprintf(sAssembler, "%.2d DIVIDE %d\n",
                            commandCounterSA, getVarAddress(op2[0]));
                break;

            default:
                fprintf(stderr, "LET: line %d: cannot recognize operator sign: %c. Translation breaked\n", i, sign_of_operator);
                exit(EXIT_FAILURE);
                break; //нужно ли?
            }
            commandCounterSA++;

            fprintf(sAssembler, "%.2d STORE %d\n",
                    commandCounterSA, getVarAddress(variable[0]));
            commandCounterSA++;
        }
    }
    //число
    else if (atoi(assignment_part) != 0 && (assignment_part[0] >= '0' && assignment_part[0] <= '9') && complicated < 2)
    {
        fprintf(sAssembler, "%.2d = +%.4d\n",
                getVarAddress(variable[0]), atoi(assignment_part));
    }
    //выражение со скобками
    else if (complicated >=2)
    {
        char rpn[strlen(assignment_part) + 1];
        translate_to_RPN(assignment_part, rpn);
        calculate_RPN(i,  rpn);
    }
    //другое
    else
    {
        fprintf(stderr, "LET: line %d: cannot recognize not a symbol, nor a digit in line \"%s\". Translation breaked\n", i, assignment_part);
        exit(EXIT_FAILURE);
    }

    complicated = 0;
}

//if args=(expression then)
//expression: op1 sign_of_comparision op2
void IF(int i, char *args)
{
    int spaces = 2; //считает пробелы для обработки строк

    char sign[4] = { '>', '=', '<', ' ' };

    char expression[strlen(args) + 1];
    strcpy(expression, args);
    char then[strlen(args) + 1];
    strcpy(then, args);

    //первое вхождение в expression знака сравнения
    char sign_of_comparision;
    for (int j = 0; j < strlen(args); j++)
    {
        for (int i = 0; i < 3; i++)
        {
            if (args[j] == sign[i])
            {
                sign_of_comparision = args[j];
                if (args[j - 1] == ' ') { spaces++; }
                if (args[j + 1] == ' ') { spaces++; }
                break;
            }
        }
    }

    //первый операнд в выражении
    char* op1 = strtok(args, " ");
    op1 = strtok(op1, sign);
    //переменная
    if (atoi(op1) == 0 && (op1[0] >= 'A' && op1[0] <= 'Z'))
    {
        //переменная содержит в названии более 1 символа
        if (strlen(op1) != 1)
        {
            fprintf(stderr, "IF: line %d: a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", i, op1);
            exit(EXIT_FAILURE);
        }
        getVarValue(op1[0]);
    }
    //число
    else if (op1[0] >= '0' && op1[0] <= '9') { op1[0] = getVarName(atoi(op1)); }
    //другое
    else
    {
        fprintf(stderr, "IF: line %d: cannot recognize not a symbol, nor a digit in line \"%s\". Translation breaked\n", i, op1);
        exit(EXIT_FAILURE);
    }

    //второй операнд в выражении
    char* op2 = expression + strlen(op1) + 1;
    while ((op2[0] == sign[0]) || (op2[0] == sign[1])
           || (op2[0] == sign[2]) || (op2[0] == sign[3])) { op2++; }
    op2 = strtok(op2, " ");
    //переменная
    if (atoi(op2) == 0 && (op2[0] >= 'A' && op2[0] <= 'Z'))
    {
        //переменная содержит в названии более 1 символа
        if (strlen(op2) != 1)
        {
            fprintf(stderr, "IF: line %d: a string variable name %s cannot contains more than 1 symbol. Translation breaked\n", i, op2);
            exit(EXIT_FAILURE);
        }
        getVarValue(op2[0]);
    }
    //число
    else if (op2[0] >= '0' && op2[0] <= '9') { op2[0] = getVarName(atoi(op2)); }
    //другое
    else
    {
        fprintf(stderr, "IF: line %d: cannot recognize not a symbol, nor a digit in line \"%s\". Translation breaked\n", i, op2);
        exit(EXIT_FAILURE);
    }

    char* result = then + strlen(op1) + strlen(op2) + spaces;

    if (strstr(result, "GOTO") != NULL)
    {
        while (result[0] == ' ' || result[0] == 'G' || result[0] == 'O' || result[0] == 'T')
        { result++; }

        //обработка условия
        switch (sign_of_comparision)
        {
        case '<':
            fprintf(sAssembler, "%.2d LOAD %d\n",
                    commandCounterSA, getVarAddress(op1[0]));
            commandCounterSA++;
            fprintf(sAssembler, "%.2d SUB %d\n",
                    commandCounterSA, getVarAddress(op2[0]));
            commandCounterSA++;
            GOTO(i, '<', atoi(result));
            break;

        case '>':
            fprintf(sAssembler, "%.2d LOAD %d\n",
                    commandCounterSA, getVarAddress(op2[0]));
            commandCounterSA++;
            fprintf(sAssembler, "%.2d SUB %d\n",
                    commandCounterSA, getVarAddress(op1[0]));
            commandCounterSA++;
            GOTO(i, '>', atoi(result));
            break;

        case '=':
            fprintf(sAssembler, "%.2d LOAD %d\n",
                    commandCounterSA, getVarAddress(op1[0]));
            commandCounterSA++;
            fprintf(sAssembler, "%.2d SUB %d\n",
                    commandCounterSA, getVarAddress(op2[0]));
            commandCounterSA++;
            GOTO(i, '=', atoi(result));
            break;

        default:
            fprintf(stderr, "IF: line %d: cannot recognize comparision sign: %c. Translation breaked\n", i, sign_of_comparision);
            exit(EXIT_FAILURE);
            break; //нужно ли?
        }
    }
    //иначе идет присвоение значения какой-либо переменной
    else { LET(i, result); }
}

void END() { fprintf(sAssembler, "%.2i HALT 00\n", commandCounterSA); }

void translate_basic_to_assembler()
{
    char line[128];
    while (1) //подсчет команд = кол-ва строк в файле
    {
        if (feof(sBasic)) { break; }
        fgets(line, 127, sBasic);
        commandCounterSB++;
    }

    fseek(sBasic, 0, SEEK_SET);

    program = (struct command*)malloc(sizeof(struct command)*commandCounterSB);
    for (int i = 0; i < commandCounterSB; i++)
    {
        if (commandCounterSA > 99)
        {
            fprintf(stderr, "program contains too many lines: %d. Translation breaked\n",
                    instructionCounter);
            exit(EXIT_FAILURE);
        }

        if (fgets(program[i].instruction, 127, sBasic) == NULL) //чтение в структуру
        {
            if (feof(sBasic)) { break; }
            fprintf(stderr, "line %d of programm cannot be read. Translation breaked\n", i++);
            exit(EXIT_FAILURE);
        }
        program[i].address = commandCounterSA;
        sprintf(line, "%s", program[i].instruction);

        program[i].number = atoi(strtok(line, " "));
        sprintf(line, "%s", program[i].instruction);

        strtok(line, " ");
        char* function = strtok(NULL, " ");
        if (function[0] == 'E' && function[1] == 'N' && function[2] == 'D')
        {
            END();
            continue;
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

    for (int i = 0; i < commandCounterSB; i++)
    {
        for (int j = 0; j < goto_index; j++)
        {
            if (program[i].number == goto_to[j])
            { goto_to[j] = program[i].address; }
        }
    }

    pass++;
    goto_current = 0;
    variable_name = 'Z';
    varCounter = -1;
    commandCounterSA = 0;

    fseek(sBasic, 0, SEEK_SET);
    fseek(sAssembler, 0, SEEK_SET);

    program = (struct command*)malloc(sizeof(struct command)*commandCounterSB);
    for (int i = 0; i < commandCounterSB; i++)
    {

        if (fgets(program[i].instruction, 127, sBasic) == NULL) //чтение в структуру
        {
            if (feof(sBasic)) { break; }
        }

        program[i].address = commandCounterSA;
        sprintf(line, "%s", program[i].instruction);

        program[i].number = atoi(strtok(line, " "));
        sprintf(line, "%s", program[i].instruction);

        strtok(line, " ");
        char* function = strtok(NULL, " ");
        if (function[0] == 'E' && function[1] == 'N' && function[2] == 'D')
        {
            END();
            continue;
        }

        char* args = strtok(NULL, "");

        if (strcmp(function, "REM") == 0) { REM(i+1, args); }
        else if (strcmp(function, "INPUT") == 0) { INPUT(i+1, args); }
        else if (strcmp(function, "PRINT") == 0) { PRINT(i+1, args); }
        else if (strcmp(function, "GOTO") == 0) { GOTO(i+1, '0', atoi(args)); }
        else if (strcmp(function, "IF") == 0) { IF(i+1, args); }
        else if (strcmp(function, "LET") == 0) { LET(i+1, args); }
    }
}

int main (int argc, const char** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "incorrect %s launch\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    load_program(argv[1], argv[2]); // file.sb, file.sa
    translate_basic_to_assembler();

    return 0;
}
