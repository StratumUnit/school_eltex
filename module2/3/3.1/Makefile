
CFLAGS := -Isrc -Wall

OBJ_DIR := obj
BIN_DIR := bin
SRC_DIR := .

all: $(BIN_DIR)/chmodProgram

$(BIN_DIR)/chmodProgram: $(OBJ_DIR)/main.o $(OBJ_DIR)/chmod.o

	@mkdir -p $(BIN_DIR)
	gcc $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
