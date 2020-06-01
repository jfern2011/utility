/**
 *  \file   superstring.h
 *  \author Jason Fernandez
 *  \date   05/25/2020
 *
 *  Copyright 2020 Jason Fernandez
 *
 *  https://github.com/jfern2011/utility
 */

#ifndef UTILITY_INCLUDE_SUPERSTRING_SUPERSTRING_H_
#define UTILITY_INCLUDE_SUPERSTRING_SUPERSTRING_H_

#include <cstddef>
#include <string>
#include <vector>

namespace jfern {

/**
 * A superstring wraps a std::string object and allows you to do what you wish
 * you could do with a normal C++ string
 */
class superstring final {
 public:
    explicit superstring(const std::string& str);

    superstring(const superstring& sstring)            = default;
    superstring(superstring&& sstring)                 noexcept = default;
    superstring& operator=(const superstring& sstring) = default;
    superstring& operator=(superstring&& sstring)      noexcept = default;
    ~superstring()                                     = default;

    bool        ends_with(const std::string& suffix)   const;
    bool        starts_with(const std::string& prefix) const;

    std::string get()      const noexcept;
    std::string to_lower() const noexcept;
    std::string to_upper() const noexcept;

    std::string ltrim()    const noexcept;
    std::string rtrim()    const noexcept;
    std::string trim()     const noexcept;

    std::vector<std::string> split(const std::string& delimiter = " ") const;
    std::vector<std::string> split(std::size_t size)                   const;

    template <class InputIterator>
    static std::string build(
        const std::string& separator, InputIterator first, InputIterator last);

 private:
    /** The internally managed string */
    std::string m_internal;
};

/**
 * Build a string by combining substrings into a larger one
 *
 * @param[in] separator What to put in between the substrings
 * @param[in] first     Input iterator to the first position in a range
 * @param[in] last      Input iterator to the final position in a range
 *
 * @return The combined string
 */
template <class InputIterator>
std::string superstring::build(
    const std::string& separator, InputIterator first, InputIterator last) {
    std::string out = "";

    for (auto iter = first; iter != last; ++iter) {
        if (iter != first) out += separator;
        out += (*iter);
    }

    return out;
}

}  // namespace jfern

#endif  // UTILITY_INCLUDE_SUPERSTRING_SUPERSTRING_H_
