#ifndef LIBRARY_H_
#define LIBRARY_H_

#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 100

//flags
#define OPERATION_OVERFLOW 0
#define DIVISION_BY_ZERO 1
#define MEMORY_ERROR 2
#define IGNORING_CLOCK_PULSES 3
#define INVALID_COMMAND 4

/*commands*/
//ввод/вывод
#define READ 10
#define WRITE 11
//загрузка/выгрузка в аккумулятор
#define LOAD 20
#define STORE 21
//ариметические операции
#define ADD 30
#define SUB 31
#define DIVIDE 32
#define MUL 33
//операции передачи управления
#define JUMP 40
#define JNEG 41
#define JZ 42
#define HALT 43
//пользовательские функции
#define NOT 51
#define AND 52
#define OR 53
#define XOR 54
#define JNS 55
#define JC 56
#define JNC 57
#define JNP 59
#define CHL 60
#define SHR 61
#define RCL 63
#define RCR 63
#define NEG 64
#define ADDC 65
#define SUBC 66
#define LOGLC 67
#define LOGRC 68
#define RCCL 69
#define RCCR 70
#define MOVA 71
#define MOVR 72
#define MOVCA 73
#define MOVCR 74
#define ADDCC 75
#define SUBCC 76

short registr = 0;
int sc_memory[MEMORY_SIZE];



#endif