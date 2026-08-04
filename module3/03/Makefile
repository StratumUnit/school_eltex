CFLAGS := -Isrc -Wall -pthread

OBJ_DIR := obj
BIN_DIR := bin
SRC_DIR := .

all: $(BIN_DIR)/p2pChatProgram


$(BIN_DIR)/p2pChatProgram: $(OBJ_DIR)/main.o | $(BIN_DIR)
	gcc $^ -o $@ -pthread -lrt


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	gcc $(CFLAGS) -c $< -o $@


$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
