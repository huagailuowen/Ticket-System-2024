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
        Mydate curdate;
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
    bool query_train(const TrainID_type &trainID,Train& train) const{
        return trainsystem.find_train(trainID,train);
    }
    void clear()
    {
        trainsystem.clear();
        released_train_info.clear();
        UserTicket.clear();
        ticket_queue.clear();
    }
    void getalltrain_bystation(const Stationname_type &station,sjtu::vector<Train> &trains) const{
        trainsystem.getalltrain_bystation(station,trains);
    }
    //根据寻址连续性，这种方式实际上是比较快的，因为选中的trainid也是升序排列的？
    //但是还有更快的方法？，就是在station中直接放releasedtrain，这样就不用再次查询了
    //这两种方法哪个更快？？
    //我先写第一种吧
    void getalltrain_bystation_time(const Stationname_type &station,const int &date,sjtu::vector<sjtu::pair<Train,ReleasedTrain>> &trains) const{
        sjtu::vector<Train> possible_train;
        trains.clear();
        getalltrain_bystation(station,possible_train);
        for(auto &i:possible_train){
            //&加速
            //可以不用get函数调用来加速
            ReleasedTrain released_train;
            int station_index=0;
            for(int j=0;j<i.getStationNum();j++){
                if(station==i.getStation(j)){
                    station_index=j;
                    break;
                }
            }
            if(station_index+1==i.getStationNum()) continue;
            //不走终点站
            int takedays=(i.getleavetime(station_index)+Mydate(0,i.getStartTime())).day;
            if(released_train_info.search(sjtu::make_pair(i.getTrainID(),date-takedays),released_train)){
                trains.push_back(sjtu::make_pair(i,released_train));
            }
        }
    }

};
#endif