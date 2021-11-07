# Color print variables
COLOR_PRINT_RED ?= "\033[1;31m"
COLOR_PRINT_GREEN ?= "\033[1;32m"
END_COLOR_PRINT ?= "\033[0m"

# List the current file count and how many files total it is building
ifndef PRINT_CURRENT_FILE
TOTAL_FILES := $(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory -nrRf $(firstword $(MAKEFILE_LIST)) PRINT_CURRENT_FILE="COUNTTHIS" | grep -c "COUNTTHIS")
N := x
CURRENT_FILE = $(words $N)$(eval N := x $N)
PRINT_CURRENT_FILE = echo -ne $(COLOR_PRINT_GREEN)"\r Currently Building File" $(CURRENT_FILE) "Out Of" $(TOTAL_FILES)":"$(END_COLOR_PRINT)
endif

# The binary name
TARGET_BINARY ?= xcash-dpops

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
LDFLAGS ?= -lmongoc-1.0 -lbson-1.0 -lresolv -lpthread -lcrypto

# Set the compiler flags
COMPILERFLAGS ?= -Wall -Wextra -Wstrict-prototypes -Wcast-qual -Wfloat-equal -Wundef -Wshadow -Wcast-align -Wstrict-overflow -Wdouble-promotion -fexceptions -pie -fPIE -Wl,dynamicbase -Wl,nxcompat

# Set the compiler and linker flags for the different options
debug: COMPILERFLAGS += -g -fno-stack-protector
release: COMPILERFLAGS += -O3
optimized: COMPILERFLAGS += -march=native -O3
analyze: COMPILERFLAGS += -g -Og -fsanitize=address -fno-omit-frame-pointer -fsanitize=leak -fsanitize=undefined -fsanitize=shift -fsanitize=bounds -fsanitize=shift-base -fsanitize=integer-divide-by-zero -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=null -fsanitize=return -fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=bounds-strict -fsanitize=alignment -fsanitize=object-size -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fsanitize=nonnull-attribute -fsanitize=returns-nonnull-attribute -fsanitize=bool -fsanitize=enum -fsanitize=vptr
analyze: LDFLAGS += -fsanitize=address -fno-omit-frame-pointer -fsanitize=leak -fsanitize=undefined -fsanitize=shift -fsanitize=bounds -fsanitize=shift-base -fsanitize=integer-divide-by-zero -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=null -fsanitize=return -fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=bounds-strict -fsanitize=alignment -fsanitize=object-size -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fsanitize=nonnull-attribute -fsanitize=returns-nonnull-attribute -fsanitize=bool -fsanitize=enum -fsanitize=vptr
analyzethreads: COMPILERFLAGS += -g -fsanitize=thread -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=shift -fsanitize=bounds -fsanitize=shift-base -fsanitize=integer-divide-by-zero -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=null -fsanitize=return -fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=bounds-strict -fsanitize=alignment -fsanitize=object-size -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fsanitize=nonnull-attribute -fsanitize=returns-nonnull-attribute -fsanitize=bool -fsanitize=enum -fsanitize=vptr
analyzethreads: LDFLAGS += -fsanitize=thread -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=shift -fsanitize=bounds -fsanitize=shift-base -fsanitize=integer-divide-by-zero -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=null -fsanitize=return -fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=bounds-strict -fsanitize=alignment -fsanitize=object-size -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fsanitize=nonnull-attribute -fsanitize=returns-nonnull-attribute -fsanitize=bool -fsanitize=enum -fsanitize=vptr

# Build all of the assembly objects files
$(BUILD_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	@$(PRINT_CURRENT_FILE) $@
	$(AS) $(ASFLAGS) -c $< -o $@

# Build all of the C objects files
$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@$(PRINT_CURRENT_FILE) $@
	@$(CC) $(COMPILERFLAGS) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build all of the C++ objects files
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	@$(PRINT_CURRENT_FILE) $@
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Set the debug and release rules to phony, since they just change the compiler flags variable and dont create any files. Set the clean rule to phony since we want it to run the command everytime we run it
.PHONY: debug release clean

# Set the options to do the same thing, since the only difference is the compiler flags
debug: $(BUILD_DIR)/$(TARGET_BINARY)
release: $(BUILD_DIR)/$(TARGET_BINARY)
optimized: $(BUILD_DIR)/$(TARGET_BINARY)
analyze: $(BUILD_DIR)/$(TARGET_BINARY)
analyzethreads: $(BUILD_DIR)/$(TARGET_BINARY)

# Link all of the objects files
$(BUILD_DIR)/$(TARGET_BINARY): $(OBJS)
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)
	@echo "\n" $(COLOR_PRINT_GREEN)$(TARGET_BINARY) "Has Been Built Successfully"$(END_COLOR_PRINT)

# Remove the build directory
clean:
	@$(RM) -r $(BUILD_DIR)
	@echo $(COLOR_PRINT_RED) "Removed" $(BUILD_DIR)$(END_COLOR_PRINT)
