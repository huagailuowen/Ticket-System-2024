#ifndef TICKETSYSTEM_HPP
#define TICKETSYSTEM_HPP
#include"../include/B_plustree.hpp"
// #include"easyinclude.hpp"
#include"Trainsystem.hpp"
#include "mytype.hpp"
class Ticket{
    UserName_type user_name;
    TrainID_type train_id;
    int date;
    //just the rank of station
    int startstation;
    int endstation;


};
class Ticketsystem{
private:
#ifdef DEBUG
public:
#endif
    sjtu::BPlusTree<sjtu::pair<TrainID_type,int >, ReleasedTrain,4,4>released_train_info;
    sjtu::BPlusTree<sjtu::pair<UserName_type,int>,Ticket,BPlusTreeM,BPlusTreeL> UserTicket;
    sjtu::BPlusTree<sjtu::pair<TrainID_type,int >, Ticket,4,4>ticket_queue;
public:
    Ticketsystem()=delete;
    Ticketsystem(const Ticketsystem &ticketsystem)=delete;
    Ticketsystem &operator=(const Ticketsystem &ticketsystem)=delete;
    Ticketsystem(std::string name,bool isnew=false):released_train_info(name+"released_train_info",isnew),UserTicket(name+"UserTicket",isnew),ticket_queue(name+"ticket_queue",isnew){
        
    }
};
#endif