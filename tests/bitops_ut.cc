/**
 *  \file   bitops_ut.cc
 *  \author Jason Fernandez
 *  \date   05/30/2020
 *
 *  Copyright 2020 Jason Fernandez
 *
 *  https://github.com/jfern2011/utility
 */

#include <array>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <random>

#include "gtest/gtest.h"
#include "bitops/bitops.h"

namespace {

/* Find and return the LSB from a std::bitset */
template <std::size_t N>
int find_lsb(const std::bitset<N>& set) {
    for (std::size_t i = 0; i < set.size(); i++) {
        if (set[i]) return static_cast<int>(i);
    }

    return -1;  // no bits set
}

/* Find and return the MSB from a std::bitset */
template <std::size_t N>
int find_msb(const std::bitset<N>& set) {
    for (int i = static_cast<int>(set.size())-1; i >= 0; i--) {
        if (set[i]) return i;
    }

    return -1;  // no bits set
}

TEST(bitops, count) {
    EXPECT_EQ(jfern::bitops::count(0), 0);
    EXPECT_EQ(jfern::bitops::count(1), 1u);
    EXPECT_EQ(jfern::bitops::count(std::uint64_t(1) << 40), 1u);

    EXPECT_EQ(jfern::bitops::count((std::uint64_t(1) << 30) |
                                   (std::uint64_t(1) << 40)), 2u);

    EXPECT_EQ(jfern::bitops::count((std::uint64_t(1) << 20) |
                                   (std::uint64_t(1) << 30) |
                                   (std::uint64_t(1) << 40)), 3u);

    EXPECT_EQ(jfern::bitops::count(std::uint16_t(65535)), 16u);

    std::default_random_engine generator;
    std::uniform_int_distribution<std::uint64_t> distribution(0);
    for (int i = 0; i < 1000; i++) {
        const std::uint64_t number = distribution(generator);
        const std::size_t expected = std::bitset<64>(number).count();
        const std::size_t actual   = jfern::bitops::count(number);
        EXPECT_EQ(expected, actual)
            << "Number = 0x" << std::hex << number
            << "\nGot "      << std::dec << actual
            << ", expected " << expected << std::endl;
    }
}

TEST(bitops, clear) {
    std::array<int, 3> indexes = { 0, 20, 63 };

    std::uint64_t word = 0;
    for (int index : indexes) word |= std::uint64_t(1) << index;

    std::bitset<64> set(word);

    for (int index : indexes) {
        jfern::bitops::clear(index, &word);
        set.reset(index);
        ASSERT_EQ(set.to_ullong(), word);
    }
}

TEST(bitops, create_mask) {
    EXPECT_EQ(jfern::bitops::create_mask<int>(), 0);

    constexpr std::array<std::size_t, 3> indexes = { 0, 20, 63 };

    constexpr auto mask = jfern::bitops::create_mask<std::uint64_t,
                                                     indexes.at(0),
                                                     indexes.at(1),
                                                     indexes.at(2)>();

    auto set = std::bitset<64>();
    for (int index : indexes) set.set(index);

    EXPECT_EQ(mask, set.to_ullong());
}

TEST(bitops, get_bit) {
    EXPECT_EQ(jfern::bitops::get_bit<int>(0), 1);
    EXPECT_EQ(jfern::bitops::get_bit<std::int64_t>(32), 0x100000000ll);
    EXPECT_EQ(jfern::bitops::get_bit<std::int16_t>(15), (std::int16_t)0x8000);
}

TEST(bitops, lsb) {
    EXPECT_EQ(jfern::bitops::lsb(0), -1);
    EXPECT_EQ(jfern::bitops::lsb(1),  0);
    EXPECT_EQ(jfern::bitops::lsb(std::uint64_t(1) << 63), 63);
    EXPECT_EQ(jfern::bitops::lsb(std::uint32_t(~0)), 0);

    std::default_random_engine generator;
    std::uniform_int_distribution<std::uint64_t> distribution(0);

    const std::uint64_t random64 = distribution(generator) | 1;
    EXPECT_EQ(jfern::bitops::lsb(random64), 0);

    for (int i = 0; i < 1000; i++) {
        const std::uint64_t rand = distribution(generator);
        const auto set = std::bitset<64>(rand);
        ASSERT_EQ(find_lsb<64>(set), jfern::bitops::lsb(rand))
            << "Failed on value " << std::hex
            << rand << std::endl;
    }
}

TEST(bitops, msb) {
    EXPECT_EQ(jfern::bitops::msb(0), -1);
    EXPECT_EQ(jfern::bitops::msb(1),  0);
    EXPECT_EQ(jfern::bitops::msb(std::uint64_t(1) << 63), 63);
    EXPECT_EQ(jfern::bitops::msb(std::uint32_t(~0)), 31);

    std::default_random_engine generator;
    std::uniform_int_distribution<std::uint64_t> distribution(0);

    const std::uint64_t random64 = distribution(generator)
                                    | 0x8000000000000000ull;

    EXPECT_EQ(jfern::bitops::msb(random64), 63);

    for (int i = 0; i < 1000; i++) {
        const std::uint64_t rand = distribution(generator);
        const auto set = std::bitset<64>(rand);
        ASSERT_EQ(find_msb<64>(set), jfern::bitops::msb(rand))
            << "Failed on value " << std::hex
            << rand << std::endl;
    }
}

TEST(bitops, multi_clear) {
    constexpr std::array<std::size_t, 3> indexes = { 0, 20, 63 };

    auto build_word = [&]() {
        std::uint64_t word = 0;
        for (std::size_t index : indexes) word |= std::uint64_t(1) << index;
        return word;
    };

    ASSERT_NE(build_word(), 0);  // sanity check

    std::uint64_t word = build_word();
    jfern::bitops::multi_clear(std::uint64_t(~0), &word);
    EXPECT_EQ(word, 0);

    word = build_word();
    jfern::bitops::multi_clear(word, &word);
    EXPECT_EQ(word, 0);

    word = build_word();
    jfern::bitops::multi_clear(std::uint64_t(1) << indexes[0], &word);
    EXPECT_EQ(word, std::uint64_t(1) << indexes[1] |
                    std::uint64_t(1) << indexes[2]);

    word = build_word();
    jfern::bitops::multi_clear(std::uint64_t(1) << indexes[1], &word);
    EXPECT_EQ(word, std::uint64_t(1) << indexes[0] |
                    std::uint64_t(1) << indexes[2]);

    word = build_word();
    jfern::bitops::multi_clear(std::uint64_t(1) << indexes[2], &word);
    EXPECT_EQ(word, std::uint64_t(1) << indexes[0] |
                    std::uint64_t(1) << indexes[1]);


    word = build_word();
    jfern::bitops::multi_clear(std::uint64_t(1) << indexes[0] |
                               std::uint64_t(1) << indexes[1], &word);
    EXPECT_EQ(word, std::uint64_t(1) << indexes[2]);

    word = build_word();
    jfern::bitops::multi_clear(std::uint64_t(1) << indexes[0] |
                               std::uint64_t(1) << indexes[2], &word);
    EXPECT_EQ(word, std::uint64_t(1) << indexes[1]);

    word = build_word();
    jfern::bitops::multi_clear(std::uint64_t(1) << indexes[1] |
                               std::uint64_t(1) << indexes[2], &word);
    EXPECT_EQ(word, std::uint64_t(1) << indexes[0]);
}

TEST(bitops, get_1bits) {
    std::default_random_engine generator;
    std::uniform_int_distribution<std::uint64_t> distribution(0);

    std::array<int, 64> indexes;
    std::vector<int>    indexes_v;

    for (int i = 0; i < 1000; i++) {
        const std::uint64_t random64 = distribution(generator);
        const auto set = std::bitset<64>(random64);

        const std::size_t num_set =
            jfern::bitops::get_1bits(random64, indexes.data());
        ASSERT_EQ(num_set, set.count());

        for (std::size_t i = 0; i < num_set; i++) {
            ASSERT_TRUE(set[indexes[i]]);
        }

        // Exercise the overload

        jfern::bitops::get_1bits(random64, &indexes_v);
        ASSERT_EQ(indexes_v.size(), set.count());

        for (int index : indexes_v) {
            ASSERT_TRUE(set[index]);
        }
    }
}

TEST(bitops, set) {
    const std::array<int, 3> indexes = { 0, 20, 63 };

    std::uint64_t word = 0;

    jfern::bitops::set(indexes[0], &word);
    EXPECT_EQ(word, (std::uint64_t(1) << indexes[0]));

    jfern::bitops::set(indexes[1], &word);
    EXPECT_EQ(word, (std::uint64_t(1) << indexes[0]) |
                    (std::uint64_t(1) << indexes[1]));

    jfern::bitops::set(indexes[2], &word);
    EXPECT_EQ(word, (std::uint64_t(1) << indexes[0]) |
                    (std::uint64_t(1) << indexes[1]) |
                    (std::uint64_t(1) << indexes[2]));
}

}  // namespace
