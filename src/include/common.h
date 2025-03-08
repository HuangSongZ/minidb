#ifndef MINIDB_COMMON_H
#define MINIDB_COMMON_H

#include <cstdint>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

namespace minidb {

// 定义页面ID和页面大小
using page_id_t = int32_t;
using frame_id_t = int32_t;

// 无效页面ID
constexpr page_id_t INVALID_PAGE_ID = -1;

// 页面大小（默认4KB）
constexpr size_t PAGE_SIZE = 4096;

// 数据库错误类
class DBException : public std::runtime_error {
public:
    explicit DBException(const std::string& message) : std::runtime_error(message) {}
};

} // namespace minidb

#endif // MINIDB_COMMON_H
