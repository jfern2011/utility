/**
 *  \file   filesys.cc
 *  \author Jason Fernandez
 *  \date   5/31/2020
 *
 *  https://github.com/jfern2011/util
 */

#include "filesys/filesys.h"

#include <fstream>
#include <ios>
#include <limits>

#ifdef _WIN32
#include <stdexcept>
#else
#include <sys/stat.h>
#endif

namespace jfern {
namespace filesys {

/**
 * Get the size of a file in bytes
 *
 * @param[in] filename The file whose size to get
 *
 * @return  The file size, in bytes, or \ref filesys::npos if the
 *          file does not exist
 */
std::size_t fsize(const std::string& filename) {
    std::ifstream ifs(filename.c_str());

    if (!ifs.good()) return npos;

    ifs.ignore(std::numeric_limits<std::streamsize>::max());
    return ifs.gcount();
}

/**
 * Check for the existence of a directory
 *
 * @note This is a non-standard C++ function which currently only
 *       works on POSIX-compliant systems
 *
 * @param[in] path The path to check
 *
 * @return True if it exists, false otherwise
 *
 * @throws std::runtime_error on Windows
 */
bool is_dir(const std::string& path) {
#ifdef _WIN32
    throw std::runtime_error(__FUNCTION__ " unusable on WIN32/64");
#endif
    struct stat st;
    if (::stat(path.c_str(), &st) < 0)
        return false;

    return S_ISDIR(st.st_mode);
}

/**
 * Check for the existence of a file
 *
 * @param[in] path The file to check
 *
 * @return True if it exists, false otherwise
 */
bool is_file(const std::string& path) {
    return std::ifstream(path.c_str()).good();
}

/**
 * Check if a file or directory exists
 *
 * @param[in] path The path to check
 *
 * @return True if it exists, false otherwise
 */
bool exists(const std::string& path) {
    return is_file(path) || is_dir(path);
}

/**
 * Read lines from a text file
 *
 * @param[in]  filename The file to read
 * @param[out] lines    All lines within the file. If the file could
 *                      not be opened, this will be empty
 *
 * @return True on success, or false if the file could not be opened
 */
bool readlines(const std::string& filename,
               std::vector<std::string>& lines) {
    lines.clear();

    std::ifstream infile(filename);
    if (infile.is_open()) {
        std::string line;
        while (std::getline(infile, line)) {
            lines.push_back(line);
        }
        return true;
    } // else cannot be opened

    return false;
}

}  // namespace filesys
}  // namespace jfern
