NAME := dis_utest
TARGET := $(NAME).out
SOURCE_DIR := ./src
BUILD_DIR := ./build

MKDIR := mkdir -p
RM := -rm
CC := gcc
CFLAGS := -g -Wall

SOURCES := $(shell find $(SOURCE_DIR) -name *.c)
OBJECTS := $(SOURCES:%=$(BUILD_DIR)/%.o)

INCLUDES := $(shell find $(SOURCE_DIR) -type d)
INCLUDE_FLAGS := $(addprefix -I, $(INCLUDES))

CPPFLAGS := $(INCLUDE_FLAGS) -MMD -MP
DEPENDENCIES := $(OBJECTS:.o=.d)
-include $(DEPENDENCIES)

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: run clean

run:
	cd $(BUILD_DIR) && ./$(TARGET)

clean:
	$(RM) -r $(BUILD_DIR)
