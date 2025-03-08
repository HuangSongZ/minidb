#ifndef MINIDB_DISK_MANAGER_H
#define MINIDB_DISK_MANAGER_H

#include <string>
#include <fstream>
#include <mutex>
#include "../include/common.h"

namespace minidb {

/**
 * DiskManager 负责数据库文件的读写操作
 * 它提供了页面级别的读写接口，管理数据库文件的创建和访问
 */
class DiskManager {
public:
    /**
     * 构造函数
     * @param db_file 数据库文件路径
     */
    explicit DiskManager(const std::string& db_file);

    /**
     * 析构函数
     */
    ~DiskManager();

    /**
     * 从磁盘读取一个页面
     * @param page_id 页面ID
     * @param page_data 页面数据的目标缓冲区
     */
    void ReadPage(page_id_t page_id, char* page_data);

    /**
     * 将一个页面写入磁盘
     * @param page_id 页面ID
     * @param page_data 要写入的页面数据
     */
    void WritePage(page_id_t page_id, const char* page_data);

    /**
     * 分配一个新的页面ID
     * @return 新分配的页面ID
     */
    page_id_t AllocatePage();

    /**
     * 释放一个页面（标记为可重用）
     * @param page_id 要释放的页面ID
     */
    void DeallocatePage(page_id_t page_id);

    /**
     * 获取数据库文件的大小（以页面数量计）
     * @return 页面数量
     */
    size_t GetNumPages() const;

    /**
     * 关闭数据库文件
     */
    void Close();

    /**
     * 删除数据库文件
     */
    void RemoveDBFile();

private:
    // 数据库文件名
    std::string db_file_name_;
    // 文件流
    std::fstream db_io_;
    // 下一个可用的页面ID
    page_id_t next_page_id_;
    // 互斥锁，用于并发控制
    std::mutex mutex_;

    /**
     * 计算页面在文件中的偏移量
     * @param page_id 页面ID
     * @return 页面偏移量（字节）
     */
    inline size_t PageOffset(page_id_t page_id) const {
        return static_cast<size_t>(page_id) * PAGE_SIZE;
    }
};

} // namespace minidb

#endif // MINIDB_DISK_MANAGER_H
