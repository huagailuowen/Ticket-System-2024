#ifndef TRAINSYSTEM_HPP
#define TRAINSYSTEM_HPP

// Your code goes here
#include "easyinclude.hpp"
#include "mytype.hpp"
#include "../include/B_plustree.hpp"
const int MAXSTATIONNUM = 100;
static const int BPlusTreeM=4, BPlusTreeL=4;
class Train {
protected:
#ifdef DEBUG
public:
#endif
    TrainID_type trainID;
    int stationNum;
    Stationname_type station[MAXSTATIONNUM];
    int seatNum;
    int price[MAXSTATIONNUM];
    int startTime;
    int travelTime[MAXSTATIONNUM];
    int stopoverTime[MAXSTATIONNUM];
    int saleDate[2];
    char type;
public:
    Train() = default;
    Train(const TrainID_type &trainID, int stationNum, const Stationname_type *station, int seatNum, const int *price, int startTime, const int *travelTime, const int *stopoverTime, const int *saleDate, char type)
        : trainID(trainID), stationNum(stationNum), seatNum(seatNum), startTime(startTime), type(type) {
        for (int i = 0; i < stationNum; i++) {
            this->station[i] = station[i];
            this->price[i] = price[i];
            this->travelTime[i] = travelTime[i];
            this->stopoverTime[i] = stopoverTime[i];
        }
        this->saleDate[0] = saleDate[0];
        this->saleDate[1] = saleDate[1];
    }
    TrainID_type getTrainID() const { return trainID; }
    int getStationNum() const { return stationNum; }
    Stationname_type getStation(int i) const { return station[i]; }
    int getSeatNum() const { return seatNum; }
    int getPrice(int i) const { return price[i]; }
    int getStartTime() const { return startTime; }
    int getTravelTime(int i) const { return travelTime[i]; }
    int getStopoverTime(int i) const { return stopoverTime[i]; }
    int getSaleDate(int i) const { return saleDate[i]; }
    char getType() const { return type; }
    void setTrainID(const TrainID_type &trainID) { this->trainID = trainID; }
    void setStationNum(int stationNum) { this->stationNum = stationNum; }
    void setStation(int i, const Stationname_type &station) { this->station[i] = station; }
    void setSeatNum(int seatNum) { this->seatNum = seatNum; }
    void setPrice(int i, int price) { this->price[i] = price; }
    void setStartTime(int startTime) { this->startTime = startTime; }
    void setTravelTime(int i, int travelTime) { this->travelTime[i] = travelTime; }
    void setStopoverTime(int i, int stopoverTime) { this->stopoverTime[i] = stopoverTime; }
    void setSaleDate(int i, int saleDate) { this->saleDate[i] = saleDate; }
    void setType(char type) { this->type = type; }
    void setStation(const std::string &input)
    {
        splittos(input, station, '|');
    }
    void setPrice(const std::string &input)
    {
        splittoi(input, price, '|');
    }
    void setTravelTime(const std::string &input)
    {
        splittoi(input, travelTime, '|');
    }
    void setStopoverTime(const std::string &input)
    {
        splittoi(input, stopoverTime, '|');
    }
    void setSaleDate(const std::string &input)
    {
        splittoi(input, saleDate, '|', true);
    }
};
class ReleasedTrain {
#ifdef DEBUG
public:
#endif
    TrainID_type trainID;
    int Date;
    int seat[MAXSTATIONNUM];
public:
    ReleasedTrain() = default;
    ReleasedTrain(const TrainID_type &trainID, int Date, const int *seat)
        : trainID(trainID), Date(Date) {
        for (int i = 0; i < MAXSTATIONNUM; i++) {
            this->seat[i] = seat[i];
        }
    }
    TrainID_type getTrainID() const { return trainID; }
    int getDate() const { return Date; }
    int getSeat(int i) const { return seat[i]; }
    void setTrainID(const TrainID_type &trainID) { this->trainID = trainID; }
    void setDate(int Date) { this->Date = Date; }
    void setSeat(int i, int seat) { this->seat[i] = seat; }
};
class Trainsystem {
    sjtu::BPlusTree<TrainID_type, Train,  BPlusTreeM,BPlusTreeL>released_train,unreleased_train;
    sjtu::BPlusTree<sjtu::pair<Stationname_type, TrainID_type>, TrainID_type, BPlusTreeM,BPlusTreeL>  station_train;
public:
    Trainsystem ()=delete;
    Trainsystem(const Trainsystem &trainsystem)=delete;
    Trainsystem &operator=(const Trainsystem &trainsystem)=delete;
    Trainsystem(std::string name,bool isnew=false):released_train(name+"released_train",isnew),unreleased_train(name+"unreleased_train",isnew),station_train(name+"station_train",isnew){}
    int train_num() const { return released_train.size()+unreleased_train.size(); }
    bool find_train(const TrainID_type &trainID,Train &train,const int &isreleased=0) const { 
        //1 means released, 2 means unreleased, 0 means both
        bool res;
        if(isreleased!=2)res=released_train.search(trainID,train);
        //we ensure that the same trainID will only appear in one of the two trees
        if(isreleased==1)return res;
        if(isreleased!=1) res=unreleased_train.search(trainID,train);
        return res; 
    }
    bool add_train(const Train &train) { 
        Train tmp;
        if(released_train.search(train.getTrainID(),tmp)||unreleased_train.search(train.getTrainID(),tmp)) return false;
        unreleased_train.insert(train.getTrainID(),train);
        for(int i=0;i<train.getStationNum();i++){
            station_train.insert(sjtu::make_pair(train.getStation(i),train.getTrainID()),train.getTrainID());
        }
        return true;
    }
    bool delete_train(const TrainID_type &trainID) { 
        Train train;
        if(released_train.search(trainID,train)) return false;
        bool res=unreleased_train.remove(trainID);
        if(res==false) return false;
        for(int i=0;i<train.getStationNum();i++){
            station_train.remove(sjtu::make_pair(train.getStation(i),train.getTrainID()));
        }
        return true;
    }
    bool release_train(const TrainID_type &trainID) { 
        Train train;
        if(!unreleased_train.search(trainID,train)) return false;
        unreleased_train.remove(trainID);
        released_train.insert(trainID,train);
        return true;
    }
    void clear()
    {
        released_train.clear();
        unreleased_train.clear();
        station_train.clear();
    }
};

#endif // TRAINSYSTEM_HPP