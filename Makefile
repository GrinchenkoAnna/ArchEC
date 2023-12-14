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

.PHONY: all clean

all: $(LIBRARY)
clean: 
	rm -rf $(LIBRARY) $(DIR_OBJ)*.o 
clean-test:
	rm -rf $(LIBRARY_TEST) $(DIR_TEST_OBJ)*.o
clean-all:
	rm -rf $(LIBRARY) $(DIR_OBJ)*.o $(LIBRARY_TEST) $(DIR_TEST_OBJ)*.o
test: $(LIBRARY_TEST)

#создание статической библиотеки
$(DIR_OBJ)library.o: $(DIR_SRC)library.c 
	$(CC) $(CFLAGS) $< -o $@

$(LIBRARY): $(DIR_OBJ)library.o
	ar rcs $@ $^

#тест
$(DIR_TEST_OBJ)main.o: $(DIR_TEST_SCR)main.c 
	$(CC) $(CFLAGS) $< -o $@

$(DIR_TEST_OBJ)library_test.o: $(DIR_TEST_SCR)library_test.c 
	$(CC) -I src $(CFLAGS) $< -o $@

$(LIBRARY_TEST): $(DIR_OBJ)library.a $(DIR_TEST_OBJ)library_test.o \
$(DIR_TEST_OBJ)main.o 
	$(CC) $(DIR_OBJ)library.a $(DIR_TEST_OBJ)library_test.o \
	$(DIR_TEST_OBJ)main.o -Wall -Werror -o $(LIBRARY_TEST)
