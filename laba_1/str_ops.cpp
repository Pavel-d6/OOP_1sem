#include <iostream>
#include "str_ops.h"


std::size_t str_len(const char* src){
    std::size_t cnt = 0;
    while (src[cnt] != '\0'){
        cnt++;
    }
    return cnt;
}

void str_delete(char*& src) {
    delete[] src;
    src = nullptr;
}

void str_copy(char* nst, const char* src){
    std::size_t i = 0;
    while ((nst[i] = src[i]) != '\0') {
        ++i;
    } 
}

 char* str_alloc(const char* src){
    std::size_t cnt = str_len(src);
    char* copy = new char[cnt + 1];
    str_copy(copy, src);
    return copy;
 }

void str_print(const char* src) {
    if (src) {
        std::cout<<src<<'\n';
    } else {
        std::cout<<"Ничего не введено\n";
    }
}

std::size_t str_count_words(const char* src){
    std::size_t cnt = 1;
    std::size_t i = 0;
    while (src[i] != '\0')
    {
        if ((src[i] == ' ') && (src[i + 1] != ' ')){
            cnt+=1;
        }
        i++;
    }
    return cnt;
}
