CFLAGS := -Isrc -Wall -Wextra

OBJ_DIR := obj
BIN_DIR := bin
SRC_DIR := src
TESTS_DIR := tests
UNITY_DIR := unity
LIB_DIR := lib

SHARED_LIB := $(LIB_DIR)/libcontacts.so

all: $(BIN_DIR)/phonebook $(BIN_DIR)/unit_tests

$(BIN_DIR)/phonebook: $(OBJ_DIR)/main.o $(OBJ_DIR)/menu.o $(SHARED_LIB)
	@mkdir -p $(BIN_DIR)
	gcc $(OBJ_DIR)/main.o $(OBJ_DIR)/menu.o -L$(LIB_DIR) -lcontacts -o $@

$(BIN_DIR)/unit_tests: $(OBJ_DIR)/test.o $(OBJ_DIR)/unity.o $(SHARED_LIB)
	@mkdir -p $(BIN_DIR)
	gcc $(OBJ_DIR)/test.o $(OBJ_DIR)/unity.o -L$(LIB_DIR) -lcontacts -o $@

$(SHARED_LIB): $(OBJ_DIR)/contacts.o
	@mkdir -p $(LIB_DIR)
	gcc -shared $^ -o $@

$(OBJ_DIR)/contacts.o: $(SRC_DIR)/contacts.c
	@mkdir -p $(OBJ_DIR)
	gcc $(CFLAGS) -fPIC -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(CFLAGS) -c $< -o $@


$(OBJ_DIR)/%.o: $(TESTS_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(CFLAGS) -c $< -o $@


$(OBJ_DIR)/%.o: $(UNITY_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(CFLAGS) -c $< -o $@

run: $(BIN_DIR)/phonebook
	LD_LIBRARY_PATH=$(PWD)/$(LIB_DIR):$$LD_LIBRARY_PATH ./$(BIN_DIR)/phonebook

run_tests: $(BIN_DIR)/unit_tests
	LD_LIBRARY_PATH=$(PWD)/$(LIB_DIR):$$LD_LIBRARY_PATH ./$(BIN_DIR)/unit_tests


clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)


.PHONY: all clean
