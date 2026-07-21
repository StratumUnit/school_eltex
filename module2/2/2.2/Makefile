
CFLAGS := -Isrc -Wall

OBJ_DIR := obj
BIN_DIR := bin
SRC_DIR := src

all: $(BIN_DIR)/calculator

$(BIN_DIR)/calculator: $(OBJ_DIR)/main.o $(OBJ_DIR)/calc.o
	@mkdir -p $(BIN_DIR)
	gcc $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
