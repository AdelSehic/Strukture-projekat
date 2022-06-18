#include "heap.hpp"
#include <iostream>

template<typename T>
heap<T> funk(){
    heap<int> temp;
    temp.insert(10).insert(8).insert(7).insert(6);
    return temp;
}

int main(int argc, char const *argv[])
{
    heap<int> hip;
    heap<int> toBe{funk<int>()};
    //       square
    hip.insert(10).insert(9).insert(5).insert(2).insert(20).insert(4).insert(1).insert(8).insert(7).insert(6);
    while(toBe.size()) std::cout << toBe.removeMin() << ' ';
    std::cout << std::endl;
    toBe = hip;

    hip.print();
    std::cout << std::endl;
    std::cout << toBe;
    std::cout << std::endl;

    while(hip.size()) std::cout << hip.removeMax() << ' ';
    std::cout << std::endl;

    while(toBe.size()) std::cout << toBe.removeMin() << ' ';
    std::cout << std::endl;

    return 0;
}