#include <iostream>
#include <limits>
#include "str_ops.h"

int menu(){
    std::cout<<"1. Ввести строку\n";
    std::cout<<"2. Напечатать\n";
    std::cout<<"3. Длина \n";
    std::cout<<"4. Скопировать в буфер и напечатать\n";
    std::cout<<"5. Алгоритм варианта\n";
    std::cout<<"0. Выход\n";
    return 0;
}

char* input_learn(char* src){
    std::size_t capacity = 128;
    std::size_t len = 0;
    char* buf = new char[capacity];
    char sim;

    std::cin >> std::ws;

    while (std::cin.get(sim) && sim != '\n'){
        if (len + 1 >= capacity){
            capacity *= 2;
            char* new_buf = new char[capacity];
            str_copy(new_buf, buf);
            delete[] buf;
            buf = new_buf;
        }
        buf[len] = sim;
        len++;
        buf[len] = '\0';
    }
    if (!std::cin){ 
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Некорректное значение\n";
        delete[] buf;
        return src; 
    }

    if (src != nullptr){
        str_delete(src);
    }
    src = str_alloc(buf); 
    delete[] buf;          
    return src;
}


int main(){
    int input_param, out_pos;
    char* src = nullptr;
    char* searchString = nullptr;

    std::cout<<"Вариант 10: C-строки через указатели char*\n";

    while (true){
        menu();
        
        if (!(std::cin>>input_param)){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr<<"Введите число от 0 до 5\n";
            continue;
        }
        if ((input_param < 0) || (input_param > 5)){
            std::cout<<"Введите число от 0 до 5\n";
            continue;
        }

        switch (input_param){
            case 1:
                src = input_learn(src);
                break;
            case 2:
                if (src != nullptr){
                    str_print(src);
                    break;
                }
                std::cout<<"Введите строку в пункте 1\n";
                break;
            case 3:
                if (src != nullptr){
                     std::cout<<str_len(src)<<'\n';
                     break;
                }
                std::cout<<"Введите строку в пункте 1\n";
                break;
            case 4:
                if (src != nullptr){
                    char* copy = str_alloc(src);
                    std::cout << copy << "\n";
                    str_delete(copy);
                    break;
                }
                std::cout<<"Введите строку в пункте 1\n";
                break;
            case 5:
                if (src != nullptr){
                    out_pos = -1;
                    std::cout<<str_count_words(src)<<"слова; строка >> "<<searchString<<" - "<<str_find_substr(src, searchString, out_pos)<<'\n';
                    break;
                }
                std::cout<<"Введите строку в пункте 1\n";
                break;
            case 0:
                str_delete(src);
                return 0;
            
        }
    }
}
