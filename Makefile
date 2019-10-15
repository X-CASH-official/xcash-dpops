# The name of the compiler
CPP=gcc

# The binary name
TARGET_BINARY ?= XCASH_DPOPS

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

# List all source files
SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

#List all directories that contain source files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# MongoDB include directories
MongoDB_INC_DIRS := -I/usr/local/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0 -MMD -MP

CFLAGS ?= $(INC_FLAGS) $(MongoDB_INC_DIRS) -MMD -MP

LDFLAGS ?= -lmongoc-1.0 -lbson-1.0 -lpthread

# Set the debug and release flags
debug: COMPILERFLAGS=-g -fno-stack-protector -Wall -Wextra -Wstrict-prototypes -Wcast-qual
release: COMPILERFLAGS=-O3 -Wall -Wextra -Wstrict-prototypes -Wcast-qual

debug: $(BUILD_DIR)/$(TARGET_BINARY)
release: $(BUILD_DIR)/$(TARGET_BINARY)

$(BUILD_DIR)/$(TARGET_BINARY): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(COMPILERFLAGS) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)


-include $(DEPS)

MKDIR_P ?= mkdir -p
