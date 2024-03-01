#ifndef CU_H
#define CU_H

//#include "ALU.c"
#include "show_GUI.c"

int CU();

int fREAD(int operand);
int fWRITE(int operand);
int fLOAD(int operand);
int fSTORE(int operand);
int fJUMP(int operand);
int fJNEG(int operand);
int fJZ(int operand);
int fHALT();

#endif
