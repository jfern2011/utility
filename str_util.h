/**
 *  \file   str_util.h
 *  \author Jason Fernandez
 *  \date   3/11/2018
 *
 *  https://github.com/jfern2011/util
 */

#ifndef __STR_UTIL_H__
#define __STR_UTIL_H__

#include <cfloat>
#include <string>
#include <vector>
#include <cstdlib>

namespace Util
{
	/*
	 * Forward declarations
	 */
	std::string trim(const std::string& str);
	std::string to_lower(const std::string& str);

	/**
	 ******************************************************************
	 *
	 *  Combine a vector of std::strings into a single string. This is
	 *  essentially the opposite of split()
	 *
	 * @param[in] tokens The strings to combine
	 * @param[in] sep    An optional separator that will get placed in
	 *                   between tokens
	 *
	 * @return The combined string
	 *
	 ******************************************************************
	 */
	inline std::string build_string(const std::vector<std::string>& tokens,
									const std::string& sep = "")
	{
		std::string out = "";

		for (size_t i = 0; i < tokens.size(); i++)
		{
			out += tokens[i] + sep;
		}

		if (sep != "" && out.size() > sep.size())
		{
			return
				out.substr(0,out.size() - sep.size());
		}
		else
			return out;
	}

	/**
	 ******************************************************************
	 *
	 * Check if a string ends with another string
	 *
	 * @param [in] str    String to scan
	 * @param [in] suffix The ending to search for
	 *
	 * @return  True if \a str ends with \a suffix
	 *
	 ******************************************************************
	 */
	inline bool ends_with(
		const std::string& str, const std::string& suffix)
	{
		size_t start = str.size() - suffix.size();

		return suffix.size() <= str.size() &&
				str.compare(start, suffix.size(), suffix) == 0;
	}

	/**
	 ******************************************************************
	 *
	 * Convert from a std::string back to a C++ basic data type
	 *
	 * @tparam The C++ data type
	 *
	 * @param [in]  str  The string to convert
	 * @param [out] val  The value represented by string \a str
	 *
	 * @return True on success
	 *
	 ******************************************************************
	 */
	template <typename T>
	bool from_string(const std::string& str, T& val)
	{
		return false;
	}

	/**
	 ******************************************************************
	 *
	 * Boolean specialization of \ref from_string(). If the input is
	 * "1" or "true", the output value is true; if the input is "0"
	 * or "false", the output is false. Otherwise, this call returns
	 * false
	 *
	 ******************************************************************
	 */
	template<>
	inline bool from_string(const std::string& str, bool& val)
	{
		std::string temp = trim(to_lower(str));

		if (temp == "false" || temp == "0")
			val = false;
		else if (temp == "true" || temp == "1")
			val = true;
		else
			return false;
		return true;
	}

	/**
	 ******************************************************************
	 *
	 * C++ char specialization of \ref from_string()
	 *
	 ******************************************************************
	 */
	template<>
	inline bool from_string(const std::string& str, char& val)
	{
		return std::sscanf(str.c_str(), "%c", &val) == 1;
	}

	/**
	 ******************************************************************
	 *
	 * C++ short specialization of \ref from_string()
	 *
	 ******************************************************************
	 */
	template<>
	inline bool from_string(const std::string& str, short& val)
	{
		return std::sscanf(str.c_str(), "%hd", &val) == 1;
	}

	/**
	 ******************************************************************
	 *
	 * C++ int specialization of \ref from_string()
	 *
	 ******************************************************************
	 */
	template<>
	inline bool from_string(const std::string& str, int& val)
	{
		return std::sscanf(str.c_str(), "%d", &val ) == 1;
	}

	/**
	 ******************************************************************
	 *
	 * C++ long long specialization of \ref from_string()
	 *
	 ******************************************************************
	 */
	template<> inline
	bool from_string(const std::string& str, long long& val)
	{
		return std::sscanf(str.c_str(), "%lld", &val) == 1;
	}

	/**
	 ******************************************************************
	 *
	 * C++ unsigned char specialization of \ref from_string()
	 *
	 ******************************************************************
	 */
	template<> inline
	bool from_string(const std::string& str, unsigned char& val)
	{
		return std::sscanf(str.c_str(), "%hhu", &val) == 1;
	}

	/**
	 ******************************************************************
	 *
	 * C++ unsigned short specialization of \ref from_string()
	 *
	 ******************************************************************
	 */
	template<> inline
	bool from_string(const std::string& str, unsigned short& val)
	{
		return std::sscanf(str.c_str(), "%hu", &val) == 1;
	}

	/**
	 ******************************************************************
	 *
	 * C++ unsigned int specialization of \ref from_string()
	 *
	 ******************************************************************
	 */
	template<> inline
	bool from_string(const std::string& str, unsigned int& val)
	{
		return std::sscanf( str.c_str(), "%u", &val ) == 1;
	}

	/**
	 ******************************************************************
	 *
	 * C++ unsigned long long specialization of \ref from_string()
	 *
	 ******************************************************************
	 */
	template<> inline
	bool from_string(const std::string& str, unsigned long long& val)
	{
		return std::sscanf(str.c_str(), "%llu", &val) == 1;
	}

	/**
	 ******************************************************************
	 *
	 * C++ float specialization of \ref from_string()
	 *
	 ******************************************************************
	 */
	template<>
	inline bool from_string(const std::string& str, float& val)
	{
		errno = 0;
		val = std::strtof(str.c_str(), nullptr);
		
		return (errno == 0);
	}

	/**
	 ******************************************************************
	 *
	 * C++ double specialization of \ref from_string()
	 *
	 ******************************************************************
	 */
	template<>
	inline bool from_string(const std::string& str, double& val)
	{
		errno = 0;
		val = std::strtod(str.c_str(), nullptr);
		
		return (errno == 0);
	}

	/**
	 ******************************************************************
	 *
	 * std::string specialization of \ref from_string()
	 *
	 ******************************************************************
	 */
	template<> inline
	bool from_string(const std::string& str, std::string& val)
	{
		val = str; return true;
	}

	/**
	 ******************************************************************
	 *
	 * Split a string into tokens. A delimiter is specified to indicate
	 * how to slice the string
	 *
	 * @param [in]  str    The string to split
	 * @param [out] tokens A vector of the elements of the split string
	 * @param [in]  delim  The delimiter
	 *
	 ******************************************************************
	 */
	inline void split(const std::string& str,
					  std::vector<std::string>& tokens,
					  const std::string& delim=" ")
	{
		size_t ind, start = 0;
		const std::string line = str;

		tokens.clear();

		if (delim.empty()){ return; }

		while (true)
		{
			if ((ind = line.find(delim, start)) != std::string::npos)
			{
				if (ind - start > 0)
					tokens.push_back(line.substr(start, ind - start));
				start = ind + delim.size();
			}
			else
			{
				if (start < line.size())
					tokens.push_back(line.substr(start,
						std::string::npos));
				break;
			}
		}
	}

	/**
	 ******************************************************************
	 *
	 * Split a string into evenly sized tokens, each one of size \a
	 * size characters. The final token contains characters remaining
	 * after dividing up the string 
	 *
	 * @param [in]  str     The string to split
	 * @param [out] tokens  A vector of the parts of the split string
	 * @param [in]  size    The size of each token
	 *
	 ******************************************************************
	 */
	inline void split(const std::string& str,
					  std::vector<std::string>& tokens,
					  size_t size)
	{

		size_t start = 0;

		if (size == 0) return;
		else
			tokens.clear();

		do
		{
			tokens.push_back(str.substr(start, size) );
			start += size;

		} while (start < str.size());
	}

	/**
	 ******************************************************************
	 *
	 * @brief
	 * Convert a character to lower case. Only when using the default C
	 * locale is this equivalent to std::tolower()
	 *
	 * @details
	 * Converts the given character in the range A-Z to lower case. If
	 * it is already in lower case or if there is no lower case 
	 * equivalent (e.g. a digit), the original character is returned
	 *
	 * @param [in] c The character to convert to lower case
	 *
	 * @return The lower case equivalent (if it exists) of the character
	 *
	 ******************************************************************
	 */
	inline char to_lower(char c)
	{
		if (c >= 65 && c <= 90)
			c += 32;

		return c;
	}

	/**
	 ******************************************************************
	 *
	 * Converts a string to lower case. See \ref to_lower() for details
	 *
	 * @param[in] str The string to convert
	 *
	 * @return The lower case equivalent of the string
	 *
	 ******************************************************************
	 */
	inline std::string to_lower(const std::string& str)
	{
		std::string res = str;
		for (size_t i = 0; i < str.size(); i++)
			res[i] = to_lower(res[i]);

		return res;
	}

	/**
	 ******************************************************************
	 *
	 * Convert a C++ fundamental data type to a std::string
	 *
	 * @tparam T The type of \a val

	 * @param [in]  val The value to convert
	 * @param [out] str The string representation of \a val
	 *
	 * @return True on success
	 *
	 ******************************************************************
	 */
	template <typename T>
	bool to_string(const T& val, std::string& str)
	{
		return false;
	}

	/**
	 ******************************************************************
	 *
	 * C++ boolean specialization of \ref to_string()
	 *
	 ******************************************************************
	 */
	template <>
	inline bool to_string(const bool& val, std::string& str)
	{
		str = val ? "true" : "false";
		return true;
	}

	/**
	 ******************************************************************
	 *
	 * C++ char specialization of \ref to_string()
	 *
	 ******************************************************************
	 */
	template <>
	inline bool to_string(const char& val, std::string& str)
	{
		char buf[64];

		if (std::snprintf(buf, 64, "%c", val) < 0)
			return false;

		str = std::string(buf);
		return true;
	}

	/**
	 ******************************************************************
	 *
	 * C++ short specialization of \ref to_string()
	 *
	 ******************************************************************
	 */
	template <>
	inline bool to_string(const short& val, std::string& str)
	{
		char buf[64];

		if (std::snprintf(buf, 64, "%hd", val) < 0)
			return false;

		str = std::string(buf);
		return true;
	}

	/**
	 ******************************************************************
	 *
	 * C++ int specialization of \ref to_string()
	 *
	 ******************************************************************
	 */
	template <>
	inline bool to_string(const int& val, std::string& str)
	{
		char buf[64];

		if (std::snprintf(buf, 64, "%d", val) < 0)
			return false;

		str = std::string(buf);
		return true;
	}

	/**
	 ******************************************************************
	 *
	 * C++ long long specialization of \ref to_string()
	 *
	 ******************************************************************
	 */
	template <> inline
	bool to_string(const long long& val, std::string& str)
	{
		char buf[64];

		if (std::snprintf(buf, 64, "%lld", val) < 0)
			return false;

		str = std::string(buf);
		return true;
	}

	/**
	 ******************************************************************
	 *
	 * C++ unsigned char specialization of \ref to_string()
	 *
	 ******************************************************************
	 */
	template <>
	inline bool to_string(const unsigned char& val, std::string& str)
	{
		char buf[64];

		if (std::snprintf(buf, 64, "%hhu", val) < 0)
			return false;

		str = std::string(buf);
		return true;
	}

	/**
	 ******************************************************************
	 *
	 * C++ unsigned short specialization of \ref to_string()
	 *
	 ******************************************************************
	 */
	template <>
	inline bool to_string(const unsigned short& val, std::string& str)
	{
		char buf[64];

		if (std::snprintf(buf, 64, "%hu", val) < 0)
			return false;

		str = std::string(buf);
		return true;
	}

	/**
	 ******************************************************************
	 *
	 * C++ unsigned int specialization of \ref to_string()
	 *
	 ******************************************************************
	 */
	template <>
	inline bool to_string(const unsigned int& val, std::string& str)
	{
		char buf[64];

		if (std::snprintf(buf, 64, "%u", val) < 0)
			return false;

		str = std::string(buf);
		return true;
	}

	/**
	 ******************************************************************
	 *
	 * C++ unsigned long long specialization of \ref to_string()
	 *
	 ******************************************************************
	 */
	template <> inline
	bool to_string(const unsigned long long& val, std::string& str)
	{
		char buf[64];

		if (std::snprintf(buf, 64, "%llu", val) < 0)
			return false;

		str = std::string(buf);
		return true;
	}

	/**
	 ******************************************************************
	 *
	 * C++ float specialization of \ref to_string()
	 *
	 ******************************************************************
	 */
	template <>
	inline bool to_string(const float& val , std::string& str)
	{
		char buf[64];

		if (std::snprintf(buf, 64, "%0.*f", FLT_DIG, val) < 0)
			return false;

		str = std::string(buf);
		return true;
	}

	/**
	 ******************************************************************
	 *
	 * C++ double specialization of \ref to_string()
	 *
	 ******************************************************************
	 */
	template <>
	inline bool to_string(const double& val, std::string& str)
	{
		char buf[64];

		if (std::snprintf(buf, 64, "%0.*f", DBL_DIG, val) < 0)
			return false;

		str = std::string(buf);
		return true;
	}

	/**
	 ******************************************************************
	 *
	 * std::string specialization of \ref to_string()
	 *
	 ******************************************************************
	 */
	template <>
	inline bool to_string(const std::string& val, std::string& str)
	{
		str = val; return true;
	}

	/**
	 ******************************************************************
	 *
	 * @brief
	 * Convert a character to upper case. Only when using the default C
	 * locale is this equivalent to std::toupper()
	 *
	 * @details
	 * Converts the given character in the range A-Z to upper case. If
	 * it is already in upper case or if there is no upper case 
	 * equivalent (e.g. a digit), the original character is returned
	 *
	 * @param [in] c The character to convert to upper case
	 *
	 * @return The upper case equivalent (if it exists) of the character
	 *
	 ******************************************************************
	 */
	inline char to_upper(char c)
	{
		if (c >= 97 && c <= 122)
			c -= 32;

		return c;
	}

	/**
	 ******************************************************************
	 *
	 * Remove leading and trailing whitespace from a string. This
	 * includes the character set " \t\n\v\f\r"
	 *
	 * @param[in] str Input string
	 *
	 * @return     The input string with all leading and trailing
	 *             whitespace removed
	 *
	 ******************************************************************
	 */
	inline std::string trim(const std::string& str)
	{
		if (str.empty()) return str;

		const std::string space = "\t\n\v\f\r ";

		size_t start =
			str.find_first_not_of(space);

		if (start == std::string::npos)
			return "";

		size_t stop = str.find_last_not_of(space);

		return
			str.substr(start,stop-start+1);
	}
}

#endif
