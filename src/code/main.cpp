#include <iostream>
#include "../include/map.hpp"
#include "../include/vector.hpp"
#include "../include/B_plustree.hpp"


int main() {
    std::cout << "Hello, World!" << std::endl;
    sjtu::map<int,int>v;
    v.insert(sjtu::make_pair(1, 1));
    // sjtu::BPlusTree<int,int,4,4>tree("it","itt",true);
    return 0;
}