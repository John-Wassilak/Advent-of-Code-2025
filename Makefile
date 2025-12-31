# Compiler
CC = gcc

# Compiler flags
CFLAGS = -lcurl -Wall -Wextra -Wpedantic -Werror

# Target executables and their source files
TARGETS = day-01 day-02 day-03
SRCS_day-01 = day-01.c util.c
SRCS_day-02 = day-02.c util.c
SRCS_day-03 = day-03.c util.c

# Default target
all: $(TARGETS)

# Link object files to create executables
$(TARGETS): %: %.o util.o
	$(CC) -o $@ $^ $(CFLAGS)

# Compile source files to object files
%.o: %.c
	$(CC) -c $< -o $@

# Object files for each target
util.o: util.c
	$(CC) -c util.c -o util.o


# Check target for debugging and running with valgrind
check: $(TARGETS)
	@for target in $(TARGETS); do \
		echo -e "\n\nRunning Valgrind on $$target..."; \
		valgrind --leak-check=full ./$$target; \
	done

# Clean up
clean:
	rm -f $(TARGETS) *.o
