# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# List of source files
SRC = main.c file_create.c google_test.c read_binary_file.c file_sockets_server_struct.c file_sockets_client_struct.c file_sockets_server.c file_sockets_client.c

# List of object files
OBJ = $(SRC:.c=.o)

# Name of the target executable
TARGET = lecture3

# Name of the test target executable
TEST_TARGET = test_project

# Default target to build
all: $(TARGET)

# Rule to link object files into the final executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Rule to compile .c files into .o object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up generated files
clean:
	rm -f $(OBJ) $(TARGET) $(TEST_TARGET)

# Rule to build and run tests
test: $(OBJ)
	$(CC) $(OBJ) -o $(TEST_TARGET) -lgtest -lgtest_main -pthread
	./$(TEST_TARGET)

# Declare phony targets
.PHONY: all clean test