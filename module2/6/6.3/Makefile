CFLAGS   = -Wall -g
SRC_DIR  = src
LIB_DIR  = libs
BIN_DIR  = bin

PLUGIN_SRCS := $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))
LIBS := $(patsubst $(SRC_DIR)/%.c, $(LIB_DIR)/lib%.so, $(PLUGIN_SRCS))

TARGET := $(BIN_DIR)/res

.PHONY: all clean run libs

all: $(LIBS) $(TARGET)

$(LIB_DIR)/lib%.so: $(SRC_DIR)/%.c
	@mkdir -p $(LIB_DIR)
	gcc $(CFLAGS) -fPIC -shared -o $@ $< -lm


$(TARGET): $(SRC_DIR)/main.c
	@mkdir -p $(BIN_DIR)
	gcc $(CFLAGS) $< -o $@ -ldl -lm

libs: $(LIBS)


clean:
	rm -rf $(LIB_DIR) $(BIN_DIR)

run: all
	./$(TARGET)