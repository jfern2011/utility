/**
 *  \file   bitops.h
 *  \author Jason Fernandez
 *  \date   5/30/2020
 *
 *  https://github.com/jfern2011/util
 */

#ifndef BITOPS_H_
#define BITOPS_H_

#include <cstddef>
#include <cstdint>
#include <vector>

namespace jfern {
namespace bitops {
/**
 * Count the number of bits set in a word in O(n) time
 *
 * @param [in] word An n-bit word
 *
 * @return The number of bits set in the word
 */
template< typename T > constexpr std::uint8_t count(T word) noexcept {
    std::uint8_t count = 0;

    for (; word; count++) word &= word - 1;

    return count;
}

/**
 * Clear the specified bit within a word
 *
 * @param [in]     bit  The bit to clear
 * @param [in,out] word An n-bit word
 */
template<typename T> constexpr void clear(int bit, T* word) noexcept {
    *word ^= (*word & (T(1) << bit));
}

/**
 * Recursive base case of \ref create_mask()
 *
 * @tparam T The integral type of this mask
 *
 * @return 0
 */
template <typename T> constexpr T create_mask() noexcept {
    return T(0);
}

/**
 * Create a bitmask with the specified bits set
 *
 * @tparam T  The integral type of this bitmask
 * @tparam I  Set the bit at this index to 1
 * @tparam Is Additional indexes of bits to set to 1
 *
 * @return The desired bitmask
 */
template <typename T, std::size_t I, std::size_t... Is>
constexpr T create_mask() noexcept {
    return (T(1) << I) | create_mask<T,Is...>();
}

/**
 * Retrieve a bitmask with only the specified bit set
 *
 * @param [in] bit The desired bit, indexed from 0
 *
 * @return A power of 2 whose base-2 logarithm = \a bit. If \a bit exceeds the
 *         size of T (in bits), the result is undefined
 */
template<typename T> constexpr T get_bit(int bit) noexcept {
    return (T(1)) << bit;
}


/**
 * Get the index of the least significant bit set in O(n) time
 *
 * @param [in] word The word to scan
 *
 * @return The LSB, or -1 if no bits are set
 */
template<typename T> constexpr std::int8_t lsb(T word) noexcept {
    std::int8_t bit = 0; T mask = 1;

    if (word != 0) {
        while (!(mask & word)) {
            mask <<= 1; bit += 1;
        }
        return bit;
    }

    return -1;
}

/**
 * Get the index of the most significant bit set in O(n) time
 *
 * @param [in] word The word to scan
 *
 * @return The MSB, or -1 if no bits are set
 */
template<typename T> constexpr std::int8_t msb(T word) noexcept {
    std::int8_t bit = (8 * sizeof(T) - 1 );
    T mask = (T(1)) << bit;

    if (word != 0) {
        while (!(mask & word)) {
            mask >>= 1; bit -= 1;
        }
        return bit;
    }

    return -1;
}

/**
 * Clear the specified bits of a word
 *
 * @param [in]     mask Specifies the bits within \a word to clear
 * @param [in,out] word An n-bit word
 */
template<typename T> constexpr void multi_clear(T mask, T* word) noexcept {
    *word ^= (*word & mask);
}

/**
 *  Returns the indexes of all bits set in a word
 *
 * @param [in]  word    The word to parse
 * @param [out] indexes A list of which bits are set
 *
 * @return The number of bits set
 */
template<typename T> constexpr
std::size_t get_1bits(T word, int* indexes) noexcept {
    std::size_t count = 0;

    while (word) {
        const int lsb_index = lsb(word);
        indexes[count++] = lsb_index;

        clear(lsb_index, &word);
    }

    return count;
}

/**
 *  Returns the indexes of all bits set in a word
 *
 * @param [in]  word    The word to parse
 * @param [out] indexes A list of which bits are set
 */
template<typename T> inline
void get_1bits(T word, std::vector<int>* indexes) {
    indexes->clear();

    while (word) {
        const int lsb_index = lsb(word);
        indexes->push_back(lsb_index);

        clear(lsb_index, &word);
    }
}

/**
 * Set the specified bit in a word
 *
 * @param [in]     bit  The desired bit to set, indexed from 0
 * @param [in,out] word The word to modify
 */
template<typename T>
constexpr void set(int bit, T* word) noexcept {
    *word |= T(1) << bit;
}

}  // namespace bitops
}  // namespace jfern

#endif  // BITOPS_H_
