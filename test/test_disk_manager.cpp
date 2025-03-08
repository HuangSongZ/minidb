#include "../src/storage/disk_manager.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cassert>

using namespace minidb;

// 测试函数
void TestDiskManager() {
    // 测试数据库文件名
    std::string db_file = "test_db.db";
    
    // 创建磁盘管理器
    DiskManager disk_manager(db_file);
    
    // 测试写入和读取页面
    const char* test_data = "Hello, MiniDB! This is a test page.";
    char buffer[PAGE_SIZE];
    char read_buffer[PAGE_SIZE];
    
    // 初始化测试数据
    std::memset(buffer, 0, PAGE_SIZE);
    std::memcpy(buffer, test_data, strlen(test_data));
    
    // 写入页面0
    disk_manager.WritePage(0, buffer);
    
    // 修改测试数据并写入页面1
    const char* test_data2 = "This is page 1 data, different from page 0.";
    std::memset(buffer, 0, PAGE_SIZE);
    std::memcpy(buffer, test_data2, strlen(test_data2));
    disk_manager.WritePage(1, buffer);
    
    // 读取页面0并验证
    std::memset(read_buffer, 0, PAGE_SIZE);
    disk_manager.ReadPage(0, read_buffer);
    assert(std::memcmp(read_buffer, test_data, strlen(test_data)) == 0);
    std::cout << "页面0读取成功: " << read_buffer << std::endl;
    
    // 读取页面1并验证
    std::memset(read_buffer, 0, PAGE_SIZE);
    disk_manager.ReadPage(1, read_buffer);
    assert(std::memcmp(read_buffer, test_data2, strlen(test_data2)) == 0);
    std::cout << "页面1读取成功: " << read_buffer << std::endl;
    
    // 测试分配新页面
    page_id_t new_page_id = disk_manager.AllocatePage();
    assert(new_page_id == 2);  // 应该是2，因为我们已经使用了0和1
    std::cout << "分配的新页面ID: " << new_page_id << std::endl;
    
    // 测试获取页面数量
    size_t num_pages = disk_manager.GetNumPages();
    assert(num_pages == 3);  // 应该是3，包括0、1和新分配的2
    std::cout << "总页面数: " << num_pages << std::endl;
    
    // 测试读取不存在的页面（应该返回全零页面）
    std::memset(read_buffer, 1, PAGE_SIZE);  // 填充为1，以便检测是否被清零
    disk_manager.ReadPage(100, read_buffer);
    bool all_zeros = true;
    for (size_t i = 0; i < PAGE_SIZE; i++) {
        if (read_buffer[i] != 0) {
            all_zeros = false;
            break;
        }
    }
    assert(all_zeros);
    std::cout << "不存在的页面读取测试通过" << std::endl;
    
    // 清理
    disk_manager.RemoveDBFile();
    std::cout << "测试完成，数据库文件已删除" << std::endl;
}

int main() {
    try {
        TestDiskManager();
        std::cout << "所有测试通过！" << std::endl;
    } catch (const DBException& e) {
        std::cerr << "测试失败: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "未知错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
