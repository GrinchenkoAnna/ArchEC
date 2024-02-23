#ifndef CU_H
#define CU_H

//#include "library.c"
//#include "display.c"
//#include "myTerm.c"
//#include "myBigChars.c"
//#include "myReadkey.c"
#include "ALU.c"

int CU(void);

int fREAD(int operand);
int fWRITE(int operand);
int fLOAD(int operand);
int fSTORE(int operand);
int fJUMP(int operand);
int fJNEG(int operand);
int fJZ(int operand);
int fHALT();

#endif
