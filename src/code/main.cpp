#include"Systhesissystem.hpp"
#include "mytype.hpp"
#include <cstdio>
int main() {
    std::string systemname="test"; 
    Systhesissystem system( systemname,true);
    try{
        system.process();
    }
    catch(const TrainSystemError &e){
        std::cerr<<e.what()<<std::endl;
    }
    return 0;
}
/*
[1] add_user -u haha -p 123456 -n 你好 -m sjtu
[2] login -p 12345 -u haha
[3] login -p 123456 -u haha
[4] query_profile -u haha -c haha
[5] modify_profile -c haha -u haha -p 123456 -n 吃屎 -m sjtu
[6] query_profile -u haha -c haha



[6] add_train -i G123 -n 1 -s shanghai -t beijing -d 2024-1-1 -a 10 -p 1000 -x 1000 -o 1000
[7] query_train -i G123
[8] query_ticket -s shanghai -t beijing -d 2024-1-1 -n 1
[9] buy_ticket -u haha -i G123 -n 1 -d 2024-1-1
[10] query_order -u haha
[11] refund_ticket -u haha -n 1
[12] query_order -u haha
[13] logout -u haha
[14] login -p 123456 -u haha
[15] query_order -u haha
[16] clean
[17] exit

*/