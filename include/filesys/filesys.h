/**
 *  \file   filesys.h
 *  \author Jason Fernandez
 *  \date   5/31/2020
 *
 *  \brief A simple filesystem API where C++17, which introduces
 *         std::filesystem, is not available
 *
 *  https://github.com/jfern2011/util
 */

#ifndef FILE_SYS_H_
#define FILE_SYS_H_

#include <cstddef>
#include <string>
#include <vector>

namespace jfern {
namespace filesys {

/** Sentinel to represent an out-of-range value */
constexpr std::size_t npos = std::string::npos;

bool        exists (const std::string& path);
std::size_t fsize  (const std::string& filename);
bool        is_dir (const std::string& path);
bool        is_file(const std::string& path);

bool readlines(const std::string& filename,
               std::vector<std::string>& lines);

}  // namespace filesys
}  // namespace jfern

#endif  // FILE_SYS_H_
