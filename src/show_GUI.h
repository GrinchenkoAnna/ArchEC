#ifndef SHOW_GUI_H
#define SHOW_GUI_H

#include "myTerm.c"
#include "myBigChars.c"
#include "library.c"

void print_memory(dye, dye);
void print_accumulator();
void print_instructionCounter();
void print_operation();
void print_flags();
void print_keys();
void print_bigChar(int, dye, dye);

void show_GUI(dye, dye);

#endif

