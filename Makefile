CC := gcc
CFLAGS := -Wall -Wextra -g

BUILD_DIR := build
SRC_DIR := src

TARGET := $(BUILD_DIR)/multiping

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: clean all run

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(OBJS) -o $@
	sudo setcap cap_net_raw+ep $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	@args="$(filter-out $@,$(MAKECMDGOALS))"; \
	./$(TARGET) $$args
