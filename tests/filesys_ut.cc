/**
 *  \file   filesys_ut.cc
 *  \author Jason Fernandez
 *  \date   05/31/2020
 *
 *  Copyright 2020 Jason Fernandez
 *
 *  https://github.com/jfern2011/utility
 */

#include <cstdio>
#include <fstream>
#include <memory>

#include "gtest/gtest.h"
#include "filesys/filesys.h"

namespace {

class FilesysTest : public ::testing::Test {
 protected:
    static const char testfile[];
    static const char contents[];
    static const char gibberish[];

    void SetUp() override {
        m_ofs.open(testfile);
        ASSERT_TRUE(m_ofs.good());

        m_ofs << contents;

        m_ofs.close();
    }

    void TearDown() override {
        std::remove(testfile);
    }

    std::ofstream m_ofs;
};

const char FilesysTest::testfile[]  = "filesys_test";
const char FilesysTest::contents[]  = "hello\nworld";
const char FilesysTest::gibberish[] = "@4*!~%#&";

TEST_F(FilesysTest, exists) {
    EXPECT_TRUE(jfern::filesys::exists("."));
    EXPECT_FALSE(jfern::filesys::exists(gibberish));
    EXPECT_TRUE(jfern::filesys::exists(testfile));
}

TEST_F(FilesysTest, fsize) {
    EXPECT_EQ(jfern::filesys::fsize(testfile), std::string(contents).size());
    EXPECT_EQ(jfern::filesys::fsize(gibberish), jfern::filesys::npos);
}

TEST_F(FilesysTest, is_dir) {
    EXPECT_TRUE(jfern::filesys::is_dir("."));
    EXPECT_FALSE(jfern::filesys::is_dir(gibberish));
    EXPECT_FALSE(jfern::filesys::is_dir(testfile));
}

TEST_F(FilesysTest, is_file) {
    EXPECT_FALSE(jfern::filesys::is_file(gibberish));
    EXPECT_TRUE(jfern::filesys::is_file(testfile));
}

TEST_F(FilesysTest, readlines) {
    std::vector<std::string> lines;
    ASSERT_TRUE(jfern::filesys::readlines(testfile, &lines));
    ASSERT_EQ(lines.size(), 2u);

    EXPECT_EQ(lines[0], "hello");
    EXPECT_EQ(lines[1], "world");
}

}  // namespace
