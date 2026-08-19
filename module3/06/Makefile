
CFLAGS := -Isrc -Wall -pthread

OBJ_DIR := obj
BIN_DIR := bin
SRC_DIR := src

all: $(BIN_DIR)/chat


$(BIN_DIR)/chat: $(OBJ_DIR)/chat.o
	@mkdir -p $(BIN_DIR)
	gcc -pthread $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean