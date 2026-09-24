#pragma once
#include <cstddef> 

char* str_alloc(const char* src);
std::size_t str_len(const char* s);
void str_copy(char* dst, const char* src);
void str_delete(char*& s);
void str_print(const char* s);
std::size_t str_count_words (const char* src);
bool str_find_substr(const char* src, const char* searchString, int& out_pos);
