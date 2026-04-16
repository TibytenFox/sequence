CXX = g++
CXXFLAGS = -I. -Wall

BUILD_DIR = build

# Исходные файлы
TEST_SRC = test.cpp
MAIN_SRC = main.cpp

# Объектные файлы
TEST_OBJ = $(BUILD_DIR)/test.o
MAIN_OBJ = $(BUILD_DIR)/main.o

# Исполняемые файлы
TEST_TARGET = $(BUILD_DIR)/test
MAIN_TARGET = $(BUILD_DIR)/main

TARGETS = $(TEST_TARGET) $(MAIN_TARGET)

HEADERS = \
	sequence/ArraySequence.hpp sequence/ArraySequence.tpp \
	sequence/DynamicArray.hpp sequence/DynamicArray.tpp \
	sequence/Exceptions.hpp \
	sequence/ICollection.hpp sequence/IEnumerable.hpp sequence/IEnumerator.hpp \
	sequence/LinkedList.hpp sequence/LinkedList.tpp \
	sequence/ListSequence.hpp sequence/ListSequence.tpp \
	sequence/Sequence.hpp sequence/Sequence.tpp sequence/SequenceOutput.hpp

all: $(TARGETS)

# Создание каталога build
$(BUILD_DIR):
	mkdir -p $@

# Компиляция .cpp → .o (build должен существовать, но его время модификации не влияет на пересборку)
$(BUILD_DIR)/%.o: %.cpp $(HEADERS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Линковка исполняемого файла test
$(TEST_TARGET): $(TEST_OBJ)
	$(CXX) $^ -o $@

# Линковка исполняемого файла main
$(MAIN_TARGET): $(MAIN_OBJ)
	$(CXX) $^ -o $@

# Запуск тестов
test_run: $(TEST_TARGET)
	./$(TEST_TARGET)

# Запуск основной программы
run: $(MAIN_TARGET)
	./$(MAIN_TARGET)

# Очистка
clean:
	rm -fr $(BUILD_DIR)

.PHONY: all clean test_run run