CFLAGS := -Isrc -Wall -Wextra -O2

TEST_CFLAGS := -Isrc -Wall -Wextra -O0 -g

OBJ_DIR := obj
BIN_DIR := bin
SRC_DIR := src
TESTS_DIR := tests
UNITY_DIR := unity
LIB_DIR := lib

STATIC_LIB := $(LIB_DIR)/libcontacts.a

all: $(BIN_DIR)/phonebook $(BIN_DIR)/unit_tests

$(BIN_DIR)/phonebook: $(OBJ_DIR)/main.o $(OBJ_DIR)/menu.o $(STATIC_LIB)
	@mkdir -p $(BIN_DIR)
	gcc $(OBJ_DIR)/main.o $(OBJ_DIR)/menu.o -L$(LIB_DIR) -lcontacts -o $@

$(BIN_DIR)/unit_tests: $(OBJ_DIR)/test.o $(OBJ_DIR)/unity.o $(STATIC_LIB)
	@mkdir -p $(BIN_DIR)
	gcc $(OBJ_DIR)/test.o $(OBJ_DIR)/unity.o -L$(LIB_DIR) -lcontacts -o $@

$(STATIC_LIB): $(OBJ_DIR)/contacts.o
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(CFLAGS) -c $< -o $@


$(OBJ_DIR)/%.o: $(TESTS_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(TEST_CFLAGS) -c $< -o $@


$(OBJ_DIR)/%.o: $(UNITY_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(TEST_CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)


.PHONY: all clean
