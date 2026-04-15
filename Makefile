CC = gcc
# Добавляем флаги для отчетов о покрытии (coverage), которые нужны для ЛР4
CFLAGS = -Wall -Wextra -Iincludes -fprofile-arcs -ftest-coverage
LDFLAGS = -lsqlite3 -lgcov

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
TEST_DIR = tests

# Исполняемые файлы
TARGET = $(BIN_DIR)/music_salon
TEST_TARGET = $(BIN_DIR)/test_suite

# Исходники (разделяем main и логику)
LOGIC_SRCS = $(SRC_DIR)/db_logic.c
MAIN_SRC = $(SRC_DIR)/main.c
TEST_SRC = $(TEST_DIR)/test_runner.c

# Объектные файлы
LOGIC_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(LOGIC_SRCS))
MAIN_OBJ = $(BUILD_DIR)/main.o

# По умолчанию собираем приложение
all: directories $(TARGET)

# Сборка основного приложения
$(TARGET): $(LOGIC_OBJS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# --- СБОРКА И ЗАПУСК ТЕСТОВ ---
# Мы берем объектные файлы логики и компилируем их вместе с файлом тестов
test: directories
	$(CC) $(CFLAGS) $(TEST_SRC) $(LOGIC_SRCS) -o $(TEST_TARGET) $(LDFLAGS)
	./$(TEST_TARGET)

# Компиляция объектных файлов
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Создание необходимых папок
directories:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Очистка (теперь удаляет и файлы покрытия .gcda, .gcno)
clean:
	rm -rf $(BUILD_DIR)/*.o $(BIN_DIR)/* *.gcda *.gcno coverage.info out/
