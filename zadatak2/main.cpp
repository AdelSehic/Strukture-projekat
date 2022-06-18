#include <iostream>
#include "hash_mapa.hpp"
#include <bitset>
#include <list>
#include <functional>

int main(int argc, char const *argv[]){
    hash_map<int, char> a;

    for(int i = 65; i <= 180; ++i) a.emplace(i,i);
    
    std::cout << "\n-------- state 1 --------\n";
    a.print();
    for(auto& i : a) std::cout << i << ' ';
    std::cout << "\n-------------------------\n";
    // a.rehash();
    // std::cout << "\n-------- state 2 --------\n";
    // a.print();
    // for(auto& i : a) std::cout << i << ' ';
    // std::cout << "\n-------------------------\n";
    // std::cout << std::endl;

    return 0;
}
