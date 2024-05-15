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
    Trainsystem trainsystem;
public:
    Ticketsystem()=delete;
    Ticketsystem(const Ticketsystem &ticketsystem)=delete;
    Ticketsystem &operator=(const Ticketsystem &ticketsystem)=delete;
    Ticketsystem(std::string name,bool isnew=false):released_train_info(name+"released_train_info",isnew),UserTicket(name+"UserTicket",isnew),ticket_queue(name+"ticket_queue",isnew),trainsystem(name+"trainsystem",isnew){}
    bool add_train(const Train &train){
        return trainsystem.add_train(train);
    }
    bool delete_train(const TrainID_type &trainID){
        return trainsystem.delete_train(trainID);
    }
    bool release_train(const TrainID_type &trainID){
        bool res=trainsystem.release_train(trainID);
        if(res==false) return false;
        Train train;
        trainsystem.find_train(trainID,train,1);
        ReleasedTrain releasedTrain_tmp;
        releasedTrain_tmp.setTrainID(trainID);
        for(int i=0;i<train.getStationNum()-1;i++){
            //there must minus one because the last station is the terminal station
            releasedTrain_tmp.setSeat(i,train.getSeatNum());
        }
        for(int i=train.getSaleDate(0);i<=train.getSaleDate(1);i++){
            //端点？？？？
            releasedTrain_tmp.setDate(i);
            released_train_info.insert(sjtu::make_pair(trainID,i),releasedTrain_tmp);
        }
        //i do not detect if ir
        return true;
    }
    bool query_train(const TrainID_type &trainID,const int &date,Train& train,ReleasedTrain  &released_train) const{
        bool res=trainsystem.find_train(trainID,train,1);
        if(res==false) return false;
        res=released_train_info.search(sjtu::make_pair(trainID,date),released_train);
        return res;
    }
    void clear()
    {
        trainsystem.clear();
        released_train_info.clear();
        UserTicket.clear();
        ticket_queue.clear();
    }

};
#endif