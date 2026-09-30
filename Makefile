BUILD_DIR := build
SRC_DIR := src

TARGET := $(BUILD_DIR)/multiping

.PHONY: clean all

all: $(TARGET)

$(TARGET): $(SRC_DIR)/*.c 
	mkdir -p $(BUILD_DIR)
	gcc $< -o $@
	sudo setcap cap_net_raw+ep $@

clean: 
	@rm -rf $(BUILD_DIR)

run: 
	@args="$(filter-out $@,$(MAKECMDGOALS))"; \
	./$(TARGET) $$args

