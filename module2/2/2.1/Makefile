
CFLAGS := -Isrc -Wall

OBJ_DIR := obj
BIN_DIR := bin
SRC_DIR := src

all: $(BIN_DIR)/phonebook $(BIN_DIR)/run_tests

$(BIN_DIR)/phonebook: $(OBJ_DIR)/main.o $(OBJ_DIR)/contacts.o $(OBJ_DIR)/menu.o
	@mkdir -p $(BIN_DIR)
	gcc $^ -o $@

$(BIN_DIR)/run_tests: $(OBJ_DIR)/test.o $(OBJ_DIR)/contacts.o $(OBJ_DIR)/unity.o
	@mkdir -p $(BIN_DIR)
	gcc $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
