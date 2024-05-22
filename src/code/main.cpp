// #define DEBUG
// #define FAST
#include "Ticketsystem.hpp"
#include<string>
int status=0;
std::string BUFFER="[0] query_train -d 06-28 -i LeavesofGrass";
#include"Systhesissystem.hpp"
#include "mytype.hpp"
#include <cstdio>
extern int TIME;
int main() {
    // freopen("test.in","r",stdin);
    // freopen("testmy.out","w",stdout);
    // std::cerr<<"3243";
    std::ios::sync_with_stdio(false); // Disable synchronization between C and C++ standard streams
    std::cin.tie(nullptr); // Unbind cin from cout
    // std::cerr<<Date_to_int("06-01")<<"--------------------------\n";
    std::string systemname="test"; 
    Systhesissystem system( systemname,false);
    // std::cerr<<sizeof(sjtu::release_bpt::release_block);
    // return 0;
    try{
        system.process();
    }
    catch(const TrainSystemError &e){
        std::cerr<<e.what()<<std::endl;
        std::cout<<e.what()<<std::endl;
    }
    catch(const char* e){
        std::cerr<<e<<std::endl;
        std::cout<<e<<std::endl;
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