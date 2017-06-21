#ifndef __UTIL__
#define __UTIL__

#include <sys/stat.h>

#include <string>
#include <vector>

#include "abort.h"
#include "types.h"

namespace Util
{
	typedef std::vector<std::string> str_v;

	/**
	 **********************************************************************
	 *
	 * Count the number of bits set in a word in O(n) time
	 *
	 * @param [in] word An n-bit word
	 *
	 * @return The number of bits set in the word
	 *
	 **********************************************************************
	 */
	template<typename T> static int bitCount(T word)
	{
		int count = 0;

		for (count = 0; word; count++)
			word &= word - 1;

		return count;
	}

	/**
	 **********************************************************************
	 *
	 *  Combine a vector of std::strings into a single string. This is
	 *  nearly the opposite of split()
	 *
	 * @param[in] tokens The strings to combine
	 * @param[in] sep    An optional separator that will get placed in
	 *                   between tokens
	 *
	 * @return The combined string
	 *
	 **********************************************************************
	 */
	static std::string build_string(const str_v& tokens,
					const std::string& sep = "")
	{
		std::string out = "";

		for (size_t i = 0; i < tokens.size(); i++)
		{
			out += tokens[i] + sep;
		}

		if (sep != "")
		{
			return
				out.substr(0,out.size() - sep.size());
		}
		else
			return out;
	}

	/**
	 **********************************************************************
	 *
	 * Clear the specified bit within a word
	 *
	 * @param [in]     bit  The bit to clear
	 * @param [in,out] word An n-bit word
	 *
	 **********************************************************************
	 */
	template<typename T> inline void clearBit(int bit, T& word)
	{
		word ^= (word & ((T)1 << bit));
	}

	/**
	 **********************************************************************
	 *
	 * Clear the specified bits of a word
	 *
	 * @param [in]     mask Specifies the bits within \a word to clear
	 * @param [in,out] word An n-bit word
	 *
	 **********************************************************************
	 */
	template<typename T> inline void clearBits(T mask, T& word)
	{
		word ^= (word & mask);
	}

	/**
     **********************************************************************
     *
     * Determine if a file or directory exists
     *
     * @param [in] dir The full (absolute) path to the file or directory
     *
     * @return True if it exists
     *
     **********************************************************************
     */
    bool dirExists(const std::string& dir)
    {
        struct stat info;

        return(!stat(dir.c_str(), &info));
    }

	/**
	 **********************************************************************
	 *
	 * Check if a string ends with another string
	 *
	 * @param [in] str    String to scan
	 * @param [in] suffix The ending to search for
	 *
	 **********************************************************************
	 */
	bool ends_with(const std::string& str, const std::string& suffix)
	{
		size_t start = str.size() - suffix.size();
		return str.size() >= suffix.size() &&
				str.compare(start, suffix.size(), suffix) == 0;
	}

	/**
     **********************************************************************
     *
     * Get the size of a regular file. See stat(2) man page for details
     *
     * @param [in] dir The full (absolute) path to the file
     *
     * @return The file size in bytes or -1 on error, including if this
     *         is a dirctory
     *
     **********************************************************************
     */
	int64 file_size(const std::string& dir)
	{
		struct stat st;
		AbortIf(stat(dir.c_str(), &st) == -1 || S_ISDIR(st.st_mode), -1);

		return
			st.st_size;
	}

	/**
	 **********************************************************************
	 *
	 * Retrieve a bitmask with only the specified bit set
	 *
	 * @param [in] bit The desired bit, indexed from 0
	 *
	 * @return A power of 2 whose base-2 logarithm = bit. If bit exceeds
	 *         the size of T (in bits), -1 is returned, i.e. the maximum
	 *         value of the unsigned type T
	 *
	 **********************************************************************
	 */
	template<typename T> inline T getBit(uint32 bit)
	{
		AbortIfNot(bit < 8*sizeof(T), (T)~0);
		return ((T)1) << bit;
	}

	/**
	 **********************************************************************
	 *
	 * Get the index of the least significant bit set. This uses an O(n)
	 * algorithm, and should not be used beyond initialization for speed
	 * considerations
	 *
	 * @param [in] word The word to scan
	 *
	 * @return The LSB, or -1 if no bits are set
	 *
	 **********************************************************************
	 */
	template<typename T> static int getLSB(T word)
	{
		int bit = 0;
		T mask = 1;

		while (mask)
		{
			if (mask & word) return bit;
			mask <<= 1; bit += 1;
		}

		return -1;
	}

	/**
	 **********************************************************************
	 *
	 * Get the index of the most significant bit set. This uses an O(n)
	 * algorithm and should not be used beyond initialization for speed
	 * considerations
	 *
	 * @param [in] word The word to scan
	 *
	 * @return The MSB, or -1 if no bits are set
	 *
	 **********************************************************************
	 */
	template<typename T> static int getMSB(T word)
	{
		int bit = (8 * sizeof(T) - 1 );
		T mask = ((T)1) << bit;

		while (mask)
		{
			if (mask & word) return bit;
			mask >>= 1; bit -= 1;
		}

		return -1;
	}

	/**
	 **********************************************************************
	 *
	 * Returns the indexes of all bits set in a word
	 *
	 * @param [in]  word    The word to parse
	 * @param [out] indexes A list of bit indexes set
	 *
	 * @return True on success
	 *
	 **********************************************************************
	 */
	template<typename T> static bool getSetBits(T word, uint32_v& indexes)
	{
		AbortIfNot(indexes.empty(), false);

		while (word)
		{
			int lsb = getLSB(word);
			indexes.push_back(lsb);

			clearBit(lsb, word);
		}

		return true;
	}

	/**
	 **********************************************************************
	 *
	 * Set the specified bit in a word
	 *
	 * @param [in]     bit  The desired bit, indexed from 0
	 * @param [in,out] word The word to modify
	 *
	 **********************************************************************
	 */
	template<typename T> inline void setBit(uint32 bit, T& word)
	{
		word |= ((T)1) << bit;
	}
	
	/**
	 **********************************************************************
	 *
	 * Split a string into tokens. A delimiter is specified to indicate
	 * how to slice the string
	 *
	 * @param [in] str     The string to split
	 * @param [out] tokens A vector of the elements of the split string
	 * @param [in] delim   A delimiter (char)
	 *
	 **********************************************************************
	 */
	static void split(const std::string& str, str_v& tokens, char delim=' ')
	{
		std::string line = str;
		size_t ind, start = 0;

		while (true)
		{
			if ((ind = line.find(delim, start)) != std::string::npos)
			{
				if (ind - start > 0)
					tokens.push_back(line.substr(start,ind - start));
				start = ind+1;
			}
			else
			{
				if (start < line.size())
					tokens.push_back(line.substr(start,std::string::npos));
				break;
			}
		}
	}

	/**
	 **********************************************************************
	 *
	 * Split a string into tokens. A delimiter is specified to indicate
	 * how to slice the string
	 *
	 * @param [in] str     The string to split
	 * @param [out] tokens A vector of the elements of the split string
	 * @param [in] delim   A delimiter (string)
	 *
	 **********************************************************************
	 */
	static void split(
		const std::string& str, str_v& tokens, const std::string& delim)
	{
		std::string line = str;
		size_t ind, start = 0;

		while (true)
		{
			if ((ind = line.find(delim, start)) != std::string::npos)
			{
				if (ind - start > 0)
					tokens.push_back(line.substr(start,ind - start));
				start = ind + delim.size();
			}
			else
			{
				if (start < line.size())
					tokens.push_back(line.substr(start,std::string::npos));
				break;
			}
		}
	}

	/**
	 **********************************************************************
	 *
	 * Split a string into evenly sized tokens, each one of size \a size
	 * characters. The final token contains characters remaining after
	 * dividing up the string 
	 *
	 * @param [in] str     The string to split
	 * @param [out] tokens A vector of the elements of the split string
	 * @param [in] size    The size of each token
	 *
	 **********************************************************************
	 */
	static void split(const std::string& str,
		              str_v& tokens, size_t size)
	{

		size_t start = 0;

		if (size == 0)
			return;
		else
			tokens.clear();

		do
		{
			tokens.push_back(str.substr(start,size));
			start += size;

		} while (start < str.size());
	}

	/**
	 **********************************************************************
	 *
	 * Get the integer representation of a string
	 *
	 * @param [in] str  The string to convert
	 * @param [in] base Radix (see strtol())
	 *
	 * @return The signed integer representing the given string. On error,
	 *         returns 0
	 *
	 **********************************************************************
	 */
	static int32 str_to_int32(const std::string& str, int base)
	{
		long int i = strtol(str.c_str(), NULL, base);
		AbortIf(errno == ERANGE, 0);
		AbortIf( i < MIN_INT32 || i > MAX_INT32, 0 );
		return (int32)i;
	}

	/**
	 **********************************************************************
	 *
	 * @brief
	 * Convert a character to lower case. Only when using the default C
	 * locale is this equivalent to ::tolower()
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
	 **********************************************************************
	 */
	static char to_lower(char c)
	{
		if (c >= 65 && c <= 90)
			c += 32;

		return c;
	}

	/**
	 **********************************************************************
	 *
	 * Converts a string to lower case. See to_lower(char) for details
	 *
	 * @param[in] str The string to convert
	 *
	 * @return The lower case equivalent of the string
	 *
	 **********************************************************************
	 */
	static std::string to_lower(const std::string& str)
	{
		std::string res = str;
		for (size_t i = 0; i < str.size(); i++)
			res[i] = to_lower(res[i]);

		return res;
	}

	/**
	 **********************************************************************
	 *
	 * @brief
	 * Convert a character to upper case. Only when using the default C
	 * locale is this equivalent to ::toupper()
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
	 **********************************************************************
	 */
	static char to_upper(char c)
	{
		if (c >= 97 && c <= 122)
			c -= 32;

		return c;
	}

	/**
	 **********************************************************************
	 *
	 * Remove leading and trailing whitespace from a string. This includes
	 * the character set " \t\n\v\f\r"
	 *
	 * @param[in] str Input string
	 *
	 * @return   The input string with all leading and trailing whitespace
	 *           removed
	 *
	 **********************************************************************
	 */
	static std::string trim(const std::string& str)
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
