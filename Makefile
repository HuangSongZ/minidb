CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O0

# 源文件目录
SRC_DIR = src
TEST_DIR = test

# 目标目录
BUILD_DIR = build
BIN_DIR = bin

# 创建目录
$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR))

# 源文件
DISK_MANAGER_SRC = $(SRC_DIR)/storage/disk_manager.cpp
TEST_DISK_MANAGER_SRC = $(TEST_DIR)/test_disk_manager.cpp

# 目标文件
DISK_MANAGER_OBJ = $(BUILD_DIR)/disk_manager.o
TEST_DISK_MANAGER_OBJ = $(BUILD_DIR)/test_disk_manager.o

# 可执行文件
TEST_DISK_MANAGER = $(BIN_DIR)/test_disk_manager

# 默认目标
all: $(TEST_DISK_MANAGER)

# 编译磁盘管理器
$(DISK_MANAGER_OBJ): $(DISK_MANAGER_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 编译磁盘管理器测试
$(TEST_DISK_MANAGER_OBJ): $(TEST_DISK_MANAGER_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 链接测试程序
$(TEST_DISK_MANAGER): $(DISK_MANAGER_OBJ) $(TEST_DISK_MANAGER_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# 运行测试
test: $(TEST_DISK_MANAGER)
	./$(TEST_DISK_MANAGER)

# 清理
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all test clean
