# The binary name
TARGET_BINARY ?= XCASH_DPOPS

# The build directory, where the target binary will be stored
BUILD_DIR ?= ./build

# The source directory, where the 
SRC_DIRS ?= ./src

# List all source files (check for every C++, C or assembly file)
SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)

# Create an object file for every source file
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# Create a dependency file for every object file
DEPS := $(OBJS:.o=.d)

# Dont include any of the dependency files into the main make file, since the main make file will create all object files
-include $(DEPS)

# List all directories that contain source files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)

# Prefix all source directories with -I
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# MongoDB include directories
MongoDB_INC_DIRS := -I/usr/local/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0

# C flags
CFLAGS ?= $(INC_FLAGS) $(MongoDB_INC_DIRS) -MMD -MP

# Linker flags
LDFLAGS ?= -lmongoc-1.0 -lbson-1.0 -lpthread

# Set the compiler flags
COMPILERFLAGS ?= -Wall -Wextra -Wstrict-prototypes -Wcast-qual

# Set the debug and release flags
debug: COMPILERFLAGS += -g -fno-stack-protector
release: COMPILERFLAGS += -O3

# Build all of the assembly objects files
$(BUILD_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# Build all of the C objects files
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(COMPILERFLAGS) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build all of the C++ objects files
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Set the debug and release rules to phony, since they just change the compiler flags variable and dont create any files. Set the clean rule to phony since we want it to run the command everytime we run it
.PHONY: debug release clean

# Set the debug and release rules to do the same thing, since the only difference is the compiler flags
debug: $(BUILD_DIR)/$(TARGET_BINARY)
release: $(BUILD_DIR)/$(TARGET_BINARY)

# Link all of the objects files
$(BUILD_DIR)/$(TARGET_BINARY): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Remove the build directory
clean:
	$(RM) -r $(BUILD_DIR)