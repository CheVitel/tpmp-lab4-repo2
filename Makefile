CC = gcc
CFLAGS = -Wall -Wextra -Iincludes
LDFLAGS = -lsqlite3

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Исходные файлы и объектные файлы
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Имя исполняемого файла
TARGET = $(BIN_DIR)/music_salon

all: directories $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)
