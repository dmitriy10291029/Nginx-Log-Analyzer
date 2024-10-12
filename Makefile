.PHONY: init build run test_init test_run



BUILD_DIR := build

init:
	@rm -rf $(BUILD_DIR) || true
	@mkdir -p $(BUILD_DIR) || true
	@cd $(BUILD_DIR) && cmake .. 

build:
	@cd $(BUILD_DIR) && cmake --build .

run:
	@./$(BUILD_DIR)/bin/AnalyzeLog $(args)



TEST_FILE_NAME="NASA_access_log_Jul95.gz"

test_init:
	@mkdir -p res || true
	@curl -L 'https://docs.google.com/uc?export=download&id=1jjzMocc0Rn9TqkK_51Oo93Fy78KYnm2i' -o ./res/$(TEST_FILE_NAME)
	@cd res && gunzip -f ${TEST_FILE_NAME}

test_run:
	@cd build/tests && ctest -V

