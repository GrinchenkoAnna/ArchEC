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

.PHONY: all library myterm mybchars myreadkey display clean test

all: library myterm mybchars myreadkey display
library: $(LIBRARY)
myterm: $(MYTERM)
mybchars: $(MYBIGCHARS)
myreadkey: $(MYREADKEY)
display: $(DISPLAY)
clean:
	rm -rf $(LIBRARY) $(MYTERM) $(DISPLAY) $(MYBIGCHARS) $(MYREADKEY) \
	$(DIR_OBJ)*.o \
	$(LIBRARY_TEST) $(MYTERM_TEST) $(MYBIGCHARS_TEST) $(MYREADKEY_TEST) \
	$(DIR_TEST_OBJ)*.o \
	$(DIR_TEMP) \
	$(DIR_TEST_SCR)testfile.txt $(DIR_TEST_SCR)testfile_empty.txt \
	$(DIR_TEST_SRC)*.o
test: test-lib test-myterm test-mybchars test-myreadkey
test-lib: $(LIBRARY_TEST)
test-myterm: $(MYTERM_TEST)
test-mybchars: $(MYBIGCHARS_TEST)
test-myreadkey: $(MYREADKEY_TEST)

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
