CC = gcc
CFLAGS = -c -Wall -Werror

DIR_SRC = ./src/
DIR_OBJ = ./build/o_src/
DIR_TEST_SCR = ./test/
DIR_TEST_OBJ = ./build/o_test/
DIR_CTEST = ./thirdparty/
DIR_TEMP = ./src/temp/
DIR_BIN = ./bin/

TARGET = $(DIR_OBJ)library.a
TEST = $(DIR_BIN)unit_tests

OBJ = $(DIR_OBJ)library.o
TEST_OBJ = $(DIR_TEST_OBJ)library_test.o $(DIR_TEST_OBJ)main.o 

.PHONY: all clean

all: $(TARGET)
clean: 
	rm -rf $(TARGET) $(TEST) $(DIR_OBJ)*.o $(DIR_TEST_OBJ)*.o
test: $(TEST)

#создание статической библиотеки
$(DIR_OBJ)library.o: $(DIR_SRC)library.c 
	$(CC) $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	ar rcs $@ $^

#тест
$(DIR_TEST_OBJ)main.o: $(DIR_TEST_SCR)main.c 
	$(CC) $(CFLAGS) $< -o $@

$(DIR_TEST_OBJ)library_test.o: $(DIR_TEST_SCR)library_test.c 
	$(CC) -I src $(CFLAGS) $< -o $@

$(TEST): $(DIR_OBJ)library.a $(TEST_OBJ) 
	$(CC) $(DIR_OBJ)library.a $(TEST_OBJ) -Wall -Werror -o $(TEST)
