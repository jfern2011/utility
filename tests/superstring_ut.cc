/**
 *  \file   superstring_ut.cc
 *  \author Jason Fernandez
 *  \date   05/30/2020
 *
 *  Copyright 2020 Jason Fernandez
 *
 *  https://github.com/jfern2011/utility
 */

#include <list>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "superstring/superstring.h"

namespace {

TEST(superstring, ends_with) {
    const auto sstring = jfern::superstring("hello");
    EXPECT_TRUE(sstring.ends_with("hello"));
    EXPECT_TRUE(sstring.ends_with("ello"));
    EXPECT_TRUE(sstring.ends_with("o"));
    EXPECT_FALSE(sstring.ends_with(""));
    EXPECT_FALSE(sstring.ends_with("ell"));

    const auto one_element = jfern::superstring("a");
    EXPECT_TRUE(one_element.ends_with("a"));

    const auto empty = jfern::superstring("");
    EXPECT_FALSE(empty.ends_with(""));
    EXPECT_FALSE(empty.ends_with("blah"));
}

TEST(superstring, starts_with) {
    const auto sstring = jfern::superstring("hello");
    EXPECT_TRUE(sstring.starts_with("hello"));
    EXPECT_FALSE(sstring.starts_with("helloWorld"));
    EXPECT_TRUE(sstring.starts_with("he"));
    EXPECT_TRUE(sstring.starts_with("h"));
    EXPECT_FALSE(sstring.starts_with(""));
    EXPECT_FALSE(sstring.starts_with("ello"));

    const auto one_element = jfern::superstring("a");
    EXPECT_TRUE(one_element.starts_with("a"));

    const auto empty = jfern::superstring("");
    EXPECT_FALSE(empty.starts_with(""));
    EXPECT_FALSE(empty.starts_with("blah"));
}

TEST(superstring, to_lower) {
    const auto all_upper = jfern::superstring("HELLO");
    const auto all_lower = jfern::superstring("hello");
    const auto mixed     = jfern::superstring("hELlO");

    const std::string hello = "hello";

    EXPECT_EQ(all_upper.to_lower().get(), hello);
    EXPECT_EQ(all_lower.to_lower().get(), hello);
    EXPECT_EQ(mixed.to_lower().get(),     hello);
}

TEST(superstring, to_upper) {
    const auto all_upper = jfern::superstring("HELLO");
    const auto all_lower = jfern::superstring("hello");
    const auto mixed     = jfern::superstring("hELlO");

    const std::string hello = "HELLO";

    EXPECT_EQ(all_upper.to_upper().get(), hello);
    EXPECT_EQ(all_lower.to_upper().get(), hello);
    EXPECT_EQ(mixed.to_upper().get(),     hello);
}

TEST(superstring, ltrim) {
    const auto str1 = jfern::superstring("\t\n\v\f\r hello");
    const auto str2 = jfern::superstring("hello");

    const std::string hello = "hello";

    EXPECT_EQ(str1.ltrim().get(), hello);
    EXPECT_EQ(str2.ltrim().get(), hello);
}

TEST(superstring, rtrim) {
    const auto str1 = jfern::superstring("hello\t\n\v\f\r ");
    const auto str2 = jfern::superstring("hello");

    const std::string hello = "hello";

    EXPECT_EQ(str1.rtrim().get(), hello);
    EXPECT_EQ(str2.rtrim().get(), hello);
}

TEST(superstring, trim) {
    const auto str1 = jfern::superstring("\t\n\v\f\r hello\t\n\v\f\r ");
    const auto str2 = jfern::superstring("hello");

    const std::string hello = "hello";

    EXPECT_EQ(str1.trim().get(), hello);
    EXPECT_EQ(str2.trim().get(), hello);
}

TEST(superstring, split) {
    const auto str1 = jfern::superstring("This,is,a,sentence.");

    auto tokens = str1.split(",");
    ASSERT_EQ(tokens.size(), 4u);

    EXPECT_EQ(tokens[0], "This");
    EXPECT_EQ(tokens[1], "is");
    EXPECT_EQ(tokens[2], "a");
    EXPECT_EQ(tokens[3], "sentence.");

    // ------------------------------------------------------------------------

    const auto str2 = jfern::superstring("This is another sentence.");
    tokens = str2.split();
    ASSERT_EQ(tokens.size(), 4u);

    EXPECT_EQ(tokens[0], "This");
    EXPECT_EQ(tokens[1], "is");
    EXPECT_EQ(tokens[2], "another");
    EXPECT_EQ(tokens[3], "sentence.");

    // ------------------------------------------------------------------------

    const auto str3 = jfern::superstring("  Hey there    buddy  ! ");
    tokens = str3.split();
    ASSERT_EQ(tokens.size(), 4u);

    EXPECT_EQ(tokens[0], "Hey");
    EXPECT_EQ(tokens[1], "there");
    EXPECT_EQ(tokens[2], "buddy");
    EXPECT_EQ(tokens[3], "!");

    // ------------------------------------------------------------------------

    const auto str4 = jfern::superstring("");
    EXPECT_EQ(str4.split().size(), 0);

    // ------------------------------------------------------------------------

    const auto str5 = jfern::superstring(".............");
    EXPECT_EQ(str5.split(".").size(), 0);

    // ------------------------------------------------------------------------

    const auto str6 = jfern::superstring(".Hello");
    tokens = str6.split(".");
    ASSERT_EQ(tokens.size(), 1u);

    EXPECT_EQ(tokens[0], "Hello");
    tokens.clear();

    // ------------------------------------------------------------------------

    const auto str7 = jfern::superstring("Hello.");
    tokens = str7.split(".");
    ASSERT_EQ(tokens.size(), 1u);

    EXPECT_EQ(tokens[0], "Hello");
    tokens.clear();

    // ------------------------------------------------------------------------

    const auto str8 = jfern::superstring(".Hello.");
    tokens = str8.split(".");
    ASSERT_EQ(tokens.size(), 1u);

    EXPECT_EQ(tokens[0], "Hello");

    // ------------------------------------------------------------------------

    const auto str9 = jfern::superstring("hello");

    tokens = str9.split(0);
    EXPECT_EQ(tokens.size(), 0u);

    tokens = str9.split(1);
    ASSERT_EQ(tokens.size(), 5u);
    EXPECT_EQ(tokens[0], "h");
    EXPECT_EQ(tokens[1], "e");
    EXPECT_EQ(tokens[2], "l");
    EXPECT_EQ(tokens[3], "l");
    EXPECT_EQ(tokens[4], "o");

    tokens = str9.split(2);
    ASSERT_EQ(tokens.size(), 3u);
    EXPECT_EQ(tokens[0], "he");
    EXPECT_EQ(tokens[1], "ll");
    EXPECT_EQ(tokens[2], "o");

    tokens = str9.split(3);
    ASSERT_EQ(tokens.size(), 2u);
    EXPECT_EQ(tokens[0], "hel");
    EXPECT_EQ(tokens[1], "lo");

    tokens = str9.split(4);
    ASSERT_EQ(tokens.size(), 2u);
    EXPECT_EQ(tokens[0], "hell");
    EXPECT_EQ(tokens[1], "o");

    tokens = str9.split(5);
    ASSERT_EQ(tokens.size(), 1u);
    EXPECT_EQ(tokens[0], "hello");
    tokens.clear();

    tokens = str9.split(6);
    ASSERT_EQ(tokens.size(), 1u);
    EXPECT_EQ(tokens[0], "hello");
    tokens.clear();

    tokens = str9.split(10);
    ASSERT_EQ(tokens.size(), 1u);
    EXPECT_EQ(tokens[0], "hello");
}

TEST(superstring, type_conversion) {
    const std::string input = "  HeY tHeRe BuDdY ";
    const std::string expected = "hey there buddy";

    jfern::superstring greeting(input);

    const std::string result = greeting.rtrim().ltrim().to_lower();
    EXPECT_EQ(result, expected);
}

TEST(superstring, build) {
    const std::string sep = "+";
    const std::list<std::string> strs = { "hello", "world" };

    const std::string expected = "hello+world";
    const std::string actual   =
        jfern::superstring::build(sep, strs.begin(), strs.end());

    EXPECT_EQ(expected, actual);
}

}  // namespace
