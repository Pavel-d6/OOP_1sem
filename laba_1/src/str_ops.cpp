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
    if (str_len(src) == 0) {
        return 0;
    }
    std::size_t cnt = 1;
    std::size_t i = 0;
    while (src[i] != '\0')
    {
         if ((src[i] == ' ') && (src[i + 1] != ' ') && (src[i + 1] != '\0')){
            cnt+=1;
        }
        i++;
    }
    return cnt;
}

void prefix(const char* p, int m, int pi[]){
    pi[0] = 0;
    int k = 0;
    for (int i = 1; i<m; ++i){
        while(k > 0 && p[i] != p[k]){
            k = pi[k-1];
        }
        if (p[i] == p[k]){
            ++k;
        }
        pi[i] = k;
    }
}

int kmp(const char* p, const char* text){
    int n = static_cast<int>(str_len(text));
    int m = static_cast<int>(str_len(p));

    if (m == 0){
        std::cout << "введена пустая строка для поиска \n";
        return -1; 
    }

    int* pi = new int[m];
    prefix(p, m, pi);

    int k = 0;
    int result = -1;
    for (int i = 0; i < n; ++i){
        while (k > 0 && text[i] != p[k]){
            k = pi[k-1];
        }
        if (text[i] == p[k]){
            ++k;
        }
        if (k == m){
            result = i - m + 1;
            break;
        }
    }

    delete[] pi;
    return result;
}

bool str_find_substr(const char* s, const char* sub, int& out_pos){
    int key = kmp(sub, s);
    if (key != -1){
        out_pos = key; 
        return true;
    }
    return false;
}
