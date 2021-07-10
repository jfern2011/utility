/**
 *  \file   superstring.cc
 *  \author Jason Fernandez
 *  \date   05/25/2020
 *
 *  Copyright 2020 Jason Fernandez
 *
 *  https://github.com/jfern2011/utility
 */

#include "superstring/superstring.h"

#include <algorithm>
#include <cctype>

namespace jfern {

/**
 * Constructor
 *
 * @param[in] str The std::string on which to operate
 */
superstring::superstring(const std::string& str) : m_internal(str) {
}

/**
 * Check if the wrapped std::string ends with a phrase
 *
 * @param[in] suffix Check if the wrapped std::string ends with this
 *
 * @return True if the wrapped string ends with \a suffix
 */
bool superstring::ends_with(const std::string& suffix) const {
    if (suffix.empty()) return false;

    const std::size_t start = m_internal.size() - suffix.size();

    return suffix.size() <= m_internal.size() &&
            m_internal.compare(start, suffix.size(), suffix) == 0;
}

/**
 * Check if the wrapped std::string starts with a phrase
 *
 * @param[in] prefix Check if the wrapped std::string starts with this
 *
 * @return True if the wrapped string starts with \a suffix
 */
bool superstring::starts_with(const std::string& prefix) const {
    if (prefix.empty()) return false;

    return prefix.size() <= m_internal.size() &&
            m_internal.compare(0, prefix.size(), prefix) == 0;
}

/**
 * Get the wrapped std::string object
 *
 * @return The wrapped string
 */
std::string superstring::get() const noexcept {
    return m_internal;
}

/**
 * Convert the wrapped (internal) std::string to lower case
 *
 * @return A copy of this object in lower case
 */
superstring superstring::to_lower() const noexcept {
    std::string internal = m_internal;
    std::transform(internal.begin(), internal.end(), internal.begin(),
        [] (unsigned char c) { return std::tolower(c); });
    return superstring(internal);
}

/**
 * Convert the wrapped (internal) std::string to upper case
 *
 * @return A copy of this object in upper case
 */
superstring superstring::to_upper() const noexcept {
    std::string internal = m_internal;
    std::transform(internal.begin(), internal.end(), internal.begin(),
        [] (unsigned char c) { return std::toupper(c); });
    return superstring(internal);
}

/**
 * Remove leading whitespace from a string. This includes the character
 * set " \t\n\v\f\r"
 *
 * @return A copy of this object with leading whitespace removed
 */
superstring superstring::ltrim() const noexcept {
    const std::string space = "\t\n\v\f\r ";

    const std::size_t start =
        m_internal.find_first_not_of(space);

    if (start == std::string::npos) return superstring("");

    return superstring(m_internal.substr(start));
}

/**
 * Remove trailing whitespace from a string. This includes the character
 * set " \t\n\v\f\r"
 *
 * @return A copy of this object with trailing whitespace removed
 */
superstring superstring::rtrim() const noexcept {
    const std::string space = "\t\n\v\f\r ";

    const std::size_t stop =
        m_internal.find_last_not_of(space);

    if (stop == std::string::npos) return superstring("");

    return superstring(m_internal.substr(0, stop+1));
}

/**
 * Remove leading and trailing whitespace from a string. This includes the
 * character set " \t\n\v\f\r"
 *
 * @return A copy of this object with all leading and trailing whitespace
 *         removed
 */
superstring superstring::trim() const noexcept {
    if (m_internal.empty()) return superstring(m_internal);

    const std::string space = "\t\n\v\f\r ";

    std::size_t start =
        m_internal.find_first_not_of(space);

    if (start == std::string::npos)
        return superstring("");

    std::size_t stop = m_internal.find_last_not_of(space);

    return superstring(m_internal.substr(start, stop-start+1));
}

/**
 * Split the wrapped string into tokens. Each token is separated by a delimiter
 *
 * @param[in] delimiter The delimiter
 *
 * @return A std::vector of tokens from the split string
 */
std::vector<std::string>
superstring::split(const std::string& delimiter) const {
    std::vector<std::string> tokens;

    if (delimiter.empty()) {
        tokens.push_back(m_internal);
    } else {
        std::size_t ind, start = 0;
        while ((ind = m_internal.find(delimiter, start)) != std::string::npos) {
            if (ind - start > 0)
                tokens.push_back(
                    m_internal.substr(start, ind - start));
            start = ind + delimiter.size();
        }

        /* Final token */

        if (start < m_internal.size())
            tokens.push_back(m_internal.substr(
                start, std::string::npos));
    }

    return tokens;
}

/**
 * Split the wrapped string into evenly sized tokens, each one \a size
 * characters in length. The final token contains characters remaining after
 * dividing up the string 
 *
 * @param[in] size The desired size of each token
 *
  * @return A std::vector of tokens from the split string
 */
std::vector<std::string> superstring::split(std::size_t size) const {
    std::vector<std::string> tokens;

    if (size == 0 || size == std::string::npos) {
        if (size == std::string::npos)
            tokens.push_back(m_internal);
        return tokens;
    }

    std::size_t start = 0;
    do {
        tokens.push_back(m_internal.substr(start, size));
        start += size;
    } while (start < m_internal.size());

    return tokens;
}

/**
 * Type conversion to a std::string. Allows a \ref superstring to be used
 * in contexts that require a std::string
 * 
 * @return The wrapped string
 */
superstring::operator std::string() const {
    return m_internal;
}

}  // namespace jfern
