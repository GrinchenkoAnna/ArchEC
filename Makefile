SHELL := /bin/bash
CC = gcc
CFLAGS = -c -Wall -Werror

DIR_SRC = ./src/
DIR_OBJ = ./build/o_src/
DIR_TEST_SCR = ./test/
DIR_TEST_OBJ = ./build/o_test/
DIR_CTEST = ./thirdparty/
DIR_TEMP = ./src/temp/
DIR_BIN = ./bin/

LIBRARY = $(DIR_OBJ)library.a
LIBRARY_TEST = $(DIR_BIN)library_test

MYTERM = $(DIR_OBJ)myTerm.a
MYTERM_TEST = $(DIR_BIN)myTerm_test

MYBIGCHARS = $(DIR_OBJ)myBigChars.a
MYBIGCHARS_TEST = $(DIR_BIN)myBigChars_test

MYREADKEY = $(DIR_OBJ)myReadkey.a
MYREADKEY_TEST = $(DIR_BIN)myReadkey_test

DISPLAY = $(DIR_BIN)display

HDD = $(DIR_OBJ)hdd.a
HDD_TEST = $(DIR_BIN)hdd_test

CU = $(DIR_BIN)CU
ALU = $(DIR_BIN)ALU

SAT = sat
SBT = sbt

.PHONY: all library myterm mybchars myreadkey display hdd cu alu sat sbt clean test

all: library myterm mybchars myreadkey display hdd cu alu sat
library: $(LIBRARY)
myterm: $(MYTERM)
mybchars: $(MYBIGCHARS)
myreadkey: $(MYREADKEY)
display: $(DISPLAY)
hdd: $(HDD)
cu: $(CU)
clean:
	rm -rf $(LIBRARY) $(MYTERM) $(DISPLAY) $(MYBIGCHARS) $(MYREADKEY) \
	$(HDD) $(DIR_OBJ)/* \
	$(LIBRARY_TEST) $(MYTERM_TEST) $(MYBIGCHARS_TEST) $(MYREADKEY_TEST) \
	$(HDD_TEST) $(DIR_TEST_OBJ)*.o \
	$(DIR_TEMP) $(DIR_BIN)* \
	$(DIR_TEST_SCR)testfile.txt $(DIR_TEST_SCR)testfile_empty.txt \
	$(DIR_TEST_SRC)*.o sat sbt
test: test-lib test-myterm test-mybchars test-myreadkey test-hdd
test-lib: $(LIBRARY_TEST)
test-myterm: $(MYTERM_TEST)
test-mybchars: $(MYBIGCHARS_TEST)
test-myreadkey: $(MYREADKEY_TEST)
test-hdd: $(HDD_TEST)

#---создание статических библиотек---
#library
$(DIR_OBJ)library.o: $(DIR_SRC)library.c 
	$(CC) $(CFLAGS) $< -o $@

$(LIBRARY): $(DIR_OBJ)library.o
	ar rcs $@ $^

#myTerm
$(DIR_OBJ)myTerm.o: $(DIR_SRC)myTerm.c 
	$(CC) $(CFLAGS) $< -o $@

$(MYTERM): $(DIR_OBJ)myTerm.o
	ar rcs $@ $^

#myBigChars
$(DIR_OBJ)myBigChars.o: $(DIR_SRC)myBigChars.c 
	$(CC) $(CFLAGS) $< -o $@

$(MYBIGCHARS): $(DIR_OBJ)myBigChars.o
	ar rcs $@ $^

#myReadkey
$(DIR_OBJ)myReadkey.o: $(DIR_SRC)myReadkey.c
	$(CC) $(CFLAGS) $< -o $@

$(MYREADKEY): $(DIR_OBJ)myReadkey.o
	ar rcs $@ $^

#hdd
$(DIR_OBJ)hdd.o: $(DIR_SRC)hdd.c
	$(CC) $(CFLAGS) $< -o $@

$(HDD): $(DIR_OBJ)hdd.o
	ar rcs $@ $^

#---прочее---
$(SAT): $(DIR_SRC)sat.c
	$(CC) $(DIR_SRC)sat.c -Wall -Werror -o $(SAT)

$(SBT): $(DIR_SRC)sbt.c
	$(CC) $(DIR_SRC)sbt.c -Wall -Werror -o $(SBT)

$(CU): $(DIR_SRC)CU.c
	$(CC) $(CFLAGS) -lm $< -o $@

$(ALU): $(DIR_SRC)ALU.c
	$(CC) $(CFLAGS) $< -o $@

$(DIR_OBJ)Signal.o: $(DIR_SRC)Signal.c
	$(CC) $(DIR_SRC)Signal.c -Wall -Werror -o $(DIR_OBJ)Signal.o

$(DIR_OBJ)show_GUI.o: $(DIR_SRC)show_GUI.c
	$(CC) $(DIR_SRC)show_GUI.c -lm -Wall -Werror -o $(DIR_OBJ)show_GUI.o

#---вывод на экран---
#display
$(DIR_OBJ)display.o: $(DIR_SRC)display.c
	$(CC) $(CFLAGS) -lm $< -o $@

$(DISPLAY): $(DIR_OBJ)myTerm.a $(DIR_OBJ)library.a $(DIR_OBJ)myBigChars.a \
$(DIR_OBJ)display.o
	$(CC) $(DIR_OBJ)myTerm.a $(DIR_OBJ)library.a $(DIR_OBJ)myBigChars.a \
	$(DIR_OBJ)display.o -Wall -Werror -lm -o $(DISPLAY)

#---тесты---
$(DIR_TEST_OBJ)main.o: $(DIR_TEST_SCR)main.c 
	$(CC) $(CFLAGS) $< -o $@

#library
$(DIR_TEST_OBJ)library_test.o: $(DIR_TEST_SCR)library_test.c 
	$(CC) -I src $(CFLAGS) $< -o $@

$(LIBRARY_TEST): $(DIR_OBJ)library.a $(DIR_TEST_OBJ)library_test.o \
$(DIR_TEST_OBJ)main.o 
	$(CC) $(DIR_OBJ)library.a $(DIR_TEST_OBJ)library_test.o \
	$(DIR_TEST_OBJ)main.o -Wall -Werror -o $(LIBRARY_TEST)

#myTerm
$(DIR_TEST_OBJ)main_myTerm.o: $(DIR_TEST_SCR)main_myTerm.c 
	$(CC) $(CFLAGS) $< -o $@

$(MYTERM_TEST): $(DIR_OBJ)myTerm.a $(DIR_TEST_OBJ)main_myTerm.o
	$(CC) $(DIR_OBJ)myTerm.a $(DIR_TEST_OBJ)main_myTerm.o \
	-Wall -Werror -o $(MYTERM_TEST)

#myBigChars
$(DIR_TEST_OBJ)main_myBigChars.o: $(DIR_TEST_SCR)main_myBigChars.c 
	$(CC) $(CFLAGS) $< -o $@

$(MYBIGCHARS_TEST): $(DIR_OBJ)myBigChars.a $(DIR_TEST_OBJ)main_myBigChars.o
	$(CC) $(DIR_OBJ)myBigChars.a $(DIR_TEST_OBJ)main_myBigChars.o \
	-Wall -Werror -o $(MYBIGCHARS_TEST)

#myReadkey
$(DIR_TEST_OBJ)main_myReadkey.o: $(DIR_TEST_SCR)main_myReadkey.c
	$(CC) $(CFLAGS) $< -o $@

$(MYREADKEY_TEST): $(DIR_OBJ)myReadkey.a $(DIR_TEST_OBJ)main_myReadkey.o
	$(CC) $(DIR_OBJ)myReadkey.a $(DIR_TEST_OBJ)main_myReadkey.o \
	-Wall -Werror -o $(MYREADKEY_TEST)

#hdd
$(DIR_TEST_OBJ)main_hdd.o: $(DIR_TEST_SCR)main_hdd.c
	$(CC) $(CFLAGS) $< -o $@

$(HDD_TEST): $(DIR_OBJ)hdd.a $(DIR_TEST_OBJ)main_hdd.o
	$(CC) $(DIR_OBJ)hdd.a $(DIR_TEST_OBJ)main_hdd.o \
	-Wall -Werror -o $(HDD_TEST)
