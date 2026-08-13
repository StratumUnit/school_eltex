
CFLAGS := -Isrc -Wall

OBJ_DIR := obj
BIN_DIR := bin
SRC_DIR := src

all: $(BIN_DIR)/producer $(BIN_DIR)/consumer

$(BIN_DIR)/producer: $(OBJ_DIR)/producer.o
	@mkdir -p $(BIN_DIR)
	gcc $^ -o $@

$(BIN_DIR)/consumer: $(OBJ_DIR)/consumer.o
	@mkdir -p $(BIN_DIR)
	gcc $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean