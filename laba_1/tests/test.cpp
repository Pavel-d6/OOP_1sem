#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include "str_ops.h"

TEST(StrLen, EmptyString) {
    EXPECT_EQ(str_len(""), 0u);
}

TEST(StrLen, StringWithSpaces) {
    EXPECT_EQ(str_len("hello world"), 11u);
}

TEST(StrCopy, CopiesIntoBuffer) {
    char buf[16];
    str_copy(buf, "hello");
    EXPECT_STREQ(buf, "hello");
}

TEST(StrCopy, ShorterStringOverwritesLonger) {
    char buf[16] = "longer string";
    str_copy(buf, "hi");
    EXPECT_STREQ(buf, "hi");   
}

TEST(StrAlloc, CopiesContent) {
    char* p = str_alloc("test");
    ASSERT_NE(p, nullptr);
    EXPECT_STREQ(p, "test");
    str_delete(p);
}

TEST(StrAlloc, EmptyString) {
    char* p = str_alloc("");
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p[0], '\0');
    EXPECT_EQ(str_len(p), 0u);
    str_delete(p);
}

TEST(StrAlloc, CopyIsIndependentFromOriginal) {
    char original[] = "test";
    char* p = str_alloc(original);
    ASSERT_NE(p, nullptr);
    EXPECT_NE(p, original);
    original[0] = 'X';
    EXPECT_STREQ(p, "test");
    str_delete(p);
}

TEST(StrDelete, NullsPointer) {
    char* p = str_alloc("abc");
    str_delete(p);
    EXPECT_EQ(p, nullptr);
}

TEST(StrDelete, SafeOnNullptr) {
    char* p = nullptr;
    str_delete(p);
    EXPECT_EQ(p, nullptr);
}

TEST(StrPrint, PrintsString) {
    std::ostringstream out;
    std::streambuf* old = std::cout.rdbuf(out.rdbuf());
    str_print("hello");
    std::cout.rdbuf(old);
    EXPECT_EQ(out.str(), "hello\n");
}

TEST(StrPrint, NullptrPrintsMessage) {
    std::ostringstream out;
    std::streambuf* old = std::cout.rdbuf(out.rdbuf());
    str_print(nullptr);
    std::cout.rdbuf(old);
    EXPECT_FALSE(out.str().empty());   // не падает, что-то печатает
}

TEST(StrCountWords, EmptyString) {
    EXPECT_EQ(str_count_words(""), 0u);
}

TEST(StrCountWords, ThreeWords) {
    EXPECT_EQ(str_count_words("hello world test"), 3u);
}

TEST(StrCountWords, TrailingSpace) {
    EXPECT_EQ(str_count_words("test "), 1u);
}

TEST(StrCountWords, MultipleSpacesBetweenWords) {
    EXPECT_EQ(str_count_words("hello   world"), 2u);
}


TEST(StrFindSubstr, FoundAtStart) {
    int pos = -1;
    EXPECT_TRUE(str_find_substr("hello world test", "hello", pos));
    EXPECT_EQ(pos, 0);
}

TEST(StrFindSubstr, FoundInMiddle) {
    int pos = -1;
    EXPECT_TRUE(str_find_substr("hello world test", "world", pos));
    EXPECT_EQ(pos, 6);
}

TEST(StrFindSubstr, NotFound) {
    int pos = -1;
    EXPECT_FALSE(str_find_substr("hello world test", "xyz", pos));
}

TEST(StrFindSubstr, PatternLongerThanText) {
    int pos = -1;
    EXPECT_FALSE(str_find_substr("hi", "hello", pos));
}

TEST(StrFindSubstr, OverlappingPrefixPattern) {
    int pos = -1;
    EXPECT_TRUE(str_find_substr("aaaaaab", "aaab", pos));
    EXPECT_EQ(pos, 3);
}

TEST(StrFindSubstr, ReturnsLeftmostOccurrence) {
    int pos = -1;
    EXPECT_TRUE(str_find_substr("xababab", "ab", pos));
    EXPECT_EQ(pos, 1);
}

TEST(StrFindSubstr, EmptyPatternIsNotFound) {
    std::ostringstream out;                       
    std::streambuf* old = std::cout.rdbuf(out.rdbuf());
    int pos = -1;
    bool res = str_find_substr("hello", "", pos);
    std::cout.rdbuf(old);
    EXPECT_FALSE(res);
}
