#Compiler and Flags
CC = clang++
CFLAGS = -Wall -std=c++17 -Iinclude $(shell pkg-config --cflags glfw3)

#Linker Flags
LDFLAGS = $(shell pkg-config --libs glfw3) -framework OpenGL

 # Source Files
SOURCES = $(shell find src -name '*.cpp' -o -name '*.c')

# Object Files
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)

#Output Executable
OUT = triangle

# Build Target
all: $(OUT)

# Link Object Files to Create Executable
$(OUT): $(OBJECTS)
	$(CC) $(OBJECTS) -v -o $(OUT) $(LDFLAGS)

# Compile .cpp Files to .o
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Compile .c Files to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
#Build
build: $(OBJECTS)
		@echo "Build done"

# Clean Up
clean:
	rm -f $(OBJECTS) $(OUT)

# Run the Application
run: all
	./$(OUT)
