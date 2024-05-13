#ifndef TICKETSYSTEM_HPP
#define TICKETSYSTEM_HPP
#include"../include/B_plustree.hpp"
#include"easyinclude.hpp"
#include"Trainsystem.hpp"
class Ticket{

};
class Ticketsystem{
    sjtu::BPlusTree<sjtu::pair<TrainID_type,int >, ReleasedTrain,4,4>released_train_info;
    sjtu::BPlusTree<sjtu::pair<TrainID_type,int >, Ticket,4,4>ticket_queue;
    
};
#endif