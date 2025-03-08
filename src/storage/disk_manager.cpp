#include "disk_manager.h"
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>

namespace minidb {

DiskManager::DiskManager(const std::string& db_file) 
    : db_file_name_(db_file), next_page_id_(0) {
    
    // 打开数据库文件，如果不存在则创建
    db_io_.open(db_file_name_, std::ios::binary | std::ios::in | std::ios::out);
    
    // 如果文件不存在，创建新文件
    if (!db_io_.is_open()) {
        // 创建新文件
        db_io_.clear();
        db_io_.open(db_file_name_, std::ios::binary | std::ios::trunc | std::ios::out);
        db_io_.close();
        // 重新以读写模式打开
        db_io_.open(db_file_name_, std::ios::binary | std::ios::in | std::ios::out);
        if (!db_io_.is_open()) {
            throw DBException("无法创建数据库文件: " + db_file_name_);
        }
    }
    
    // 获取文件大小并计算页面数
    db_io_.seekg(0, std::ios::end);
    size_t file_size = db_io_.tellg();
    if (file_size > 0) {
        next_page_id_ = file_size / PAGE_SIZE;
    } else {
        next_page_id_ = 0;
    }
}

DiskManager::~DiskManager() {
    if (db_io_.is_open()) {
        db_io_.close();
    }
}

void DiskManager::ReadPage(page_id_t page_id, char* page_data) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (page_id < 0) {
        throw DBException("无效的页面ID: " + std::to_string(page_id));
    }
    
    size_t offset = PageOffset(page_id);
    
    // 检查页面是否超出文件范围
    db_io_.seekg(0, std::ios::end);
    size_t file_size = db_io_.tellg();
    if (offset >= file_size) {
        // 页面不存在，填充零
        std::memset(page_data, 0, PAGE_SIZE);
        return;
    }
    
    // 定位到页面位置
    db_io_.seekg(offset);
    
    // 读取页面数据
    if (!db_io_.read(page_data, PAGE_SIZE)) {
        if (db_io_.eof()) {
            // 读取的数据不足一个页面，填充剩余部分为零
            size_t read_count = db_io_.gcount();
            if (read_count < PAGE_SIZE) {
                std::memset(page_data + read_count, 0, PAGE_SIZE - read_count);
            }
            // 清除EOF标志
            db_io_.clear();
        } else {
            throw DBException("读取页面失败: " + std::to_string(page_id));
        }
    }
}

void DiskManager::WritePage(page_id_t page_id, const char* page_data) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (page_id < 0) {
        throw DBException("无效的页面ID: " + std::to_string(page_id));
    }
    
    size_t offset = PageOffset(page_id);
    
    // 定位到页面位置
    db_io_.seekp(offset);
    
    // 写入页面数据
    if (!db_io_.write(page_data, PAGE_SIZE)) {
        throw DBException("写入页面失败: " + std::to_string(page_id));
    }
    
    // 刷新到磁盘
    db_io_.flush();
    
    // 更新next_page_id_
    if (page_id >= next_page_id_) {
        next_page_id_ = page_id + 1;
    }
}

page_id_t DiskManager::AllocatePage() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // 分配新页面ID并递增
    page_id_t new_page_id = next_page_id_++;
    return new_page_id;
}

void DiskManager::DeallocatePage([[maybe_unused]] page_id_t page_id) {
    // 在实际实现中，我们可以维护一个空闲页面列表
    // 简化版本中，我们只是标记页面为可重用（这里暂时不做任何操作）
    // 在更复杂的实现中，可以将页面添加到空闲列表中
}

size_t DiskManager::GetNumPages() const {
    return next_page_id_;
}

void DiskManager::Close() {
    if (db_io_.is_open()) {
        db_io_.close();
    }
}

void DiskManager::RemoveDBFile() {
    Close();
    // 删除文件
    if (remove(db_file_name_.c_str()) != 0) {
        throw DBException("无法删除数据库文件: " + db_file_name_);
    }
}

} // namespace minidb
