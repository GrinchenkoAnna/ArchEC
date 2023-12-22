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

DISPLAY = $(DIR_BIN)display

.PHONY: all library myterm display clean clean-test clean-all test

all: library myterm 
library: $(LIBRARY)
myterm: $(MYTERM)
display: $(DISPLAY)
clean: 
	rm -rf $(LIBRARY) $(MYTERM) $(DISPLAY) $(DIR_OBJ)*.o 
clean-test:
	rm -rf $(LIBRARY_TEST) $(MYTERM_TEST) $(DIR_TEST_OBJ)*.o $(DIR_TEMP)
clean-all:
	rm -rf $(LIBRARY) $(MYTERM) $(DISPLAY) $(DIR_OBJ)*.o \
	$(LIBRARY_TEST) $(MYTERM_TEST) $(DIR_TEST_OBJ)*.o $(DIR_TEMP)
test: test-lib test-myterm
test-lib: $(LIBRARY_TEST)
test-myterm: $(MYTERM_TEST)

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

#display
$(DIR_OBJ)display.o: $(DIR_SRC)display.c 
	$(CC) $(CFLAGS) $< -o $@

$(DISPLAY): $(DIR_OBJ)myTerm.a $(DIR_OBJ)library.a $(DIR_OBJ)display.o
	$(CC) $(DIR_OBJ)myTerm.a $(DIR_OBJ)library.a $(DIR_OBJ)display.o \
	-Wall -Werror -o $(DISPLAY)

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
