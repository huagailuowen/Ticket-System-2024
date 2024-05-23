#ifndef TRAINSYSTEM_HPP
#define TRAINSYSTEM_HPP

// Your code goes here
// #include "easyinclude.hpp"
#include "mytype.hpp"
#include<iostream>
#include "../include/B_plustree.hpp"
#include "../include/external_bpt.hpp"
extern int TIME;
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
    // int travelTime[MAXSTATIONNUM];
    short stopoverTime[MAXSTATIONNUM];
    Mydate arrivetime[MAXSTATIONNUM];
    int saleDate[2];
    char type;
public:
    Train() = default;
    Train(const TrainID_type &trainID, int stationNum, const Stationname_type *station, int seatNum, const int *price, int startTime, const int *travelTime, const int *stopoverTime, const int *saleDate, char type)
        : trainID(trainID), stationNum(stationNum), seatNum(seatNum), startTime(startTime), type(type) {
        for (int i = 0; i < stationNum; i++) {
            this->station[i] = station[i];
            this->price[i] = price[i];
            // this->travelTime[i] = travelTime[i];
            this->stopoverTime[i] = stopoverTime[i];

        }
        this->saleDate[0] = saleDate[0];
        this->saleDate[1] = saleDate[1];
    }
    Train(const Train& other) {
        trainID = other.trainID;
        stationNum = other.stationNum;
        for (int i = 0; i < stationNum; i++) {
            station[i] = other.station[i];
            price[i] = other.price[i];
            // travelTime[i] = other.travelTime[i];
            stopoverTime[i] = other.stopoverTime[i];
            arrivetime[i] = other.arrivetime[i];
        }
        seatNum = other.seatNum;
        startTime = other.startTime;
        saleDate[0] = other.saleDate[0];
        saleDate[1] = other.saleDate[1];
        type = other.type;
    }

    Train& operator=(const Train& other) {
        if (this == &other) {
            return *this;
        }
        trainID = other.trainID;
        stationNum = other.stationNum;
        for (int i = 0; i < stationNum; i++) {
            station[i] = other.station[i];
            price[i] = other.price[i];
            // travelTime[i] = other.travelTime[i];
            stopoverTime[i] = other.stopoverTime[i];
            arrivetime[i] = other.arrivetime[i];
        }
        seatNum = other.seatNum;
        startTime = other.startTime;
        saleDate[0] = other.saleDate[0];
        saleDate[1] = other.saleDate[1];
        type = other.type;
        return *this;
    }
    TrainID_type& getTrainID() { return trainID; }
    TrainID_type getTrainID() const { return trainID; }
    int getStationNum() const { return stationNum; }
    Stationname_type& getStation(int i) { return station[i]; }
    Stationname_type getStation(int i) const { return station[i]; }
    int getSeatNum() const { return seatNum; }
    int getPrice(int i) const { return price[i]; }
    int getStartTime() const { return startTime; }
    // int getTravelTime(int i) const { return travelTime[i]; }
    int getStopoverTime(int i) const { return stopoverTime[i]; }
    int getSaleDate(int i) const { return saleDate[i]; }
    char getType() const { return type; }
    void setTrainID(const TrainID_type &trainID) { this->trainID = trainID; }
    void setStationNum(int stationNum) { this->stationNum = stationNum; }
    void setStation(int i, const Stationname_type &station) { this->station[i] = station; }
    void setSeatNum(int seatNum) { this->seatNum = seatNum; }
    void setPrice(int i, int price) { this->price[i] = price; }
    void setStartTime(int startTime) { this->startTime = startTime; }
    // void setTravelTime(int i, int travelTime) { this->travelTime[i] = travelTime; }
    void setStopoverTime(int i, int stopoverTime) { this->stopoverTime[i] = stopoverTime; }
    void setSaleDate(int i, int saleDate) { this->saleDate[i] = saleDate; }
    void setType(char type) { this->type = type; }
    void setarrivetime(int i, Mydate arrivetime) { this->arrivetime[i] = arrivetime; }
    Mydate getarrivetime(int i) const { if(i>=stationNum)return 0;return this->arrivetime[i]; }
    void updatearrivetime()
    {
        Mydate tmp = Mydate(0,startTime);
        for (int i = 0; i < stationNum; i++)
        {
            Mydate cur=tmp;
            if(i+1<stationNum)tmp = tmp + Mydate(0,arrivetime[i]);
            arrivetime[i] = cur;
            if (i!=0&&i+1< stationNum )
                tmp = tmp + Mydate(0,stopoverTime[i-1]);
        }
    }
    Mydate getleavetime(int i) const
    {
        if(i>=stationNum)throw "error";
        // stationNum-1 is also useless
        Mydate tmp = arrivetime[i];
        if(i!=0)tmp = tmp + Mydate(0,stopoverTime[i-1]);
        return tmp;
    }
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
        splittodate(input, arrivetime, '|');
    }
    void setStopoverTime(const std::string &input)
    {
        splittoshort(input, stopoverTime, '|');
    }
    void setSaleDate(const std::string &input)
    {
        splittoi(input, saleDate, '|', 1);
    }
    friend std::ostream & operator <<  (std::ostream &os, const Train &train);
};
std::ostream &  operator <<  (std::ostream &os, const Train &train)
{
    os<<train.getTrainID()<<" "<<train.getStationNum()<<" ";
    for(int i=0;i<train.getStationNum();i++)
    {
        os<<train.getStation(i)<<" ";
        os<<train.getPrice(i)<<" ";
    }
    os<<train.getSeatNum()<<" ";
    for(int i=0;i<train.getStationNum();i++)
    {
    }
    os<<train.getStartTime()<<" ";
    for(int i=0;i<train.getStationNum();i++)
    {
        os<<train.getarrivetime(i)<<" ";
    }
    for(int i=0;i<train.getStationNum();i++)
    {
        os<<train.getStopoverTime(i)<<" ";
    }
    os<<std::endl;
    os<<train.getSaleDate(0)<<" "<<train.getSaleDate(1)<<" "<<train.getType();
    return os;
}
class Smalltrain{
    public:
    TrainID_type trainID;
    int saleDate[2];
    char type;
    int stationindex;
    int price;
    //the total price when reach this station
    Mydate cur_date;
    Mydate stopoverTime;
    Mydate start_date;
    //leave time of the station
    public:
    Smalltrain()=default;
    Smalltrain(const Train &train,const Stationname_type &station)
    {
        trainID=train.getTrainID();
        saleDate[0]=train.getSaleDate(0);
        saleDate[1]=train.getSaleDate(1);
        type=train.getType();
        start_date=Mydate(0,train.getStartTime());
        price=0;
        for(int i=0;i<train.getStationNum();i++)
        {
            if(train.getStation(i)==station)
            {
                cur_date=train.getleavetime(i);
                stationindex=i;
                if(i+1==train.getStationNum()||!i)
                    stopoverTime=Mydate(0,0);
                else 
                    stopoverTime=Mydate(0,train.getStopoverTime(i-1));
                break;
            }
            price+=train.getPrice(i);
        }
    }
    Smalltrain(const Train &train,int index)
    {
        trainID=train.getTrainID();
        saleDate[0]=train.getSaleDate(0);
        saleDate[1]=train.getSaleDate(1);
        type=train.getType();
        start_date=Mydate(0,train.getStartTime());
        price=0;
        for(int i=0;i<train.getStationNum();i++)
        {
            if(i==index)
            {
                cur_date=train.getleavetime(i);
                stationindex=i;
                if(i+1==train.getStationNum()||!i)
                    stopoverTime=Mydate(0,0);
                else 
                    stopoverTime=Mydate(0,train.getStopoverTime(i-1));
                break;
            }
            price+=train.getPrice(i);
        }
    }
    
   
    Smalltrain(const Smalltrain &other)
    {
        trainID=other.trainID;
        saleDate[0]=other.saleDate[0];
        saleDate[1]=other.saleDate[1];
        start_date=other.start_date;
        stopoverTime=other.stopoverTime;
        type=other.type;
        stationindex=other.stationindex;
        price=other.price;
        cur_date=other.cur_date;
    }
    Smalltrain& operator=(const Smalltrain &other)
    {
        if(this==&other)return *this;
        trainID=other.trainID;
        saleDate[0]=other.saleDate[0];
        saleDate[1]=other.saleDate[1];
        start_date=other.start_date;
        stopoverTime=other.stopoverTime;
        type=other.type;
        stationindex=other.stationindex;
        price=other.price;
        cur_date=other.cur_date;
        return *this;
    }
    TrainID_type getTrainID() const { return trainID; }
    int getSaleDate(int i) const { return saleDate[i]; }
    char getType() const { return type; }
    int getStationindex() const { return stationindex; }
    int getPrice() const { return price; }
    Mydate getleavetime() const { return cur_date; }
    Mydate getStopoverTime() const { return stopoverTime; }
    Mydate getarrivetime() const { return cur_date-stopoverTime; }
    
    Mydate getStartTime() const { return start_date; }
    
    void setTrainID(const TrainID_type &trainID) { this->trainID = trainID; }
    void setSaleDate(int i, int saleDate) { this->saleDate[i] = saleDate; }
    void setType(char type) { this->type = type; }
    void setStationindex(int stationNum) { this->stationindex = stationindex; }
    void setPrice(int price) { this->price = price; }
    void setcur_date(Mydate cur_date) { this->cur_date = cur_date; }
    void setStopoverTime(Mydate stopoverTime) { this->stopoverTime = stopoverTime; }
    void setStartTime(Mydate start_date) { this->start_date = start_date; }
    friend std::ostream & operator <<  (std::ostream &os, const Smalltrain &train);
};
std::ostream &  operator <<  (std::ostream &os, const Smalltrain &train)
{
    os<<train.getTrainID()<<" "<<int_to_Date(train.getSaleDate(0))<<" "<<int_to_Date(train.getSaleDate(1))<<" "<<train.getType()<<" "<<train.getStationindex()<<" "<<train.getPrice();
    return os;
}
class ReleasedTrain {
#ifdef DEBUG
public:
#endif
    TrainID_type trainID;
    int Date;
    int stationNum;
    int seat[MAXSTATIONNUM];
public:
    ReleasedTrain() = default;
    ReleasedTrain(const TrainID_type &trainID, int Date, int stationNum, const int *seat)
        : trainID(trainID), Date(Date), stationNum(stationNum) {
        for (int i = 0; i < stationNum; i++) {
            this->seat[i] = seat[i];
        }
    }
    ReleasedTrain(const ReleasedTrain &releasedTrain) {
        trainID = releasedTrain.trainID;
        Date = releasedTrain.Date;
        stationNum = releasedTrain.stationNum;
        for (int i = 0; i < stationNum; i++) {
            seat[i] = releasedTrain.seat[i];
        }
    }
    ReleasedTrain& operator=(const ReleasedTrain &releasedTrain) {
        if (this == &releasedTrain) {
            return *this;
        }
        trainID = releasedTrain.trainID;
        Date = releasedTrain.Date;
        stationNum = releasedTrain.stationNum;
        for (int i = 0; i < stationNum; i++) {
            seat[i] = releasedTrain.seat[i];
        }
        return *this;
    }
    TrainID_type getTrainID() const { return trainID; }
    int getDate() const { return Date; }
    int getSeat(int i) const { return seat[i]; }
    int getStationNum() const { return stationNum; }
    TrainID_type& getTrainID() { return trainID; }
    int& getDate() { return Date; }
    int& getSeat(int i) { return seat[i]; }
    int& getStationNum() { return stationNum; }
    void setTrainID(const TrainID_type &trainID) { this->trainID = trainID; }
    void setDate(int Date) { this->Date = Date; }
    void setSeat(int i, int seat) { this->seat[i] = seat; }
    void setStationNum(int stationNum) { this->stationNum = stationNum; }
    friend std::ostream & operator <<  (std::ostream &os, const ReleasedTrain &releasedTrain);
};
std::ostream &  operator <<  (std::ostream &os, const ReleasedTrain &releasedTrain)
{
    os<<releasedTrain.getTrainID()<<" "<<int_to_Date(releasedTrain.getDate())<<" "<<releasedTrain.getStationNum()<<"\n";
    for(int i=0;i<releasedTrain.getStationNum();i++)
    {
        os<<releasedTrain.getSeat(i)<<" ";
    }
    return os;
}
class Trainsystem {
    sjtu::external_bpt<TrainID_type, Train,true>released_train,unreleased_train;
    // sjtu::external_bpt<TrainID_type,Train,true>released_train,unreleased_train;
    sjtu::BPlusTree<sjtu::pair<Stationname_type, TrainID_type>, Smalltrain, 80,80>  station_train;

public:
    Trainsystem ()=delete;
    Trainsystem(const Trainsystem &trainsystem)=delete;
    Trainsystem &operator=(const Trainsystem &trainsystem)=delete;
    Trainsystem(std::string name,bool isnew=false):released_train(name+"released_train",isnew),unreleased_train(name+"unreleased_train",isnew),station_train(name+"station_train",isnew){}
    int train_num() const { return released_train.size()+unreleased_train.size(); }
    bool find_train(const TrainID_type &trainID,Train &train,const int &isreleased=0) { 
        //1 means released, 2 means unreleased, 0 means both
        bool res=false;
        if(isreleased!=2)res=released_train.search(trainID,train);
        //we ensure that the same trainID will only appear in one of the two trees
        if(isreleased==1)return res;
        if(res==true)return res;
        if(isreleased!=1) res=unreleased_train.search(trainID,train);
        return res; 
    }
    bool add_train(const Train &train) { 
        Train tmp;
        if(released_train.search(train.getTrainID(),tmp)||unreleased_train.search(train.getTrainID(),tmp)) return false;
        unreleased_train.insert(train.getTrainID(),train);
        
        return true;
    }
    bool delete_train(const TrainID_type &trainID) { 
        Train train;
        if(released_train.search(trainID,train)) return false;
        bool res=unreleased_train.remove(trainID);
        if(res==false) return false;
        return true;
    }
    bool release_train(const TrainID_type &trainID) { 
        Train train;
        if(!unreleased_train.search(trainID,train)) return false;
        for(int i=0;i<train.getStationNum();i++){
            station_train.insert(sjtu::make_pair(train.getStation(i),train.getTrainID()),Smalltrain(train,i));
        }
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
    void getallsmalltrain_bystation(const Stationname_type &station,sjtu::vector<Smalltrain> &trains){
        trains.clear();
        station_train.searchall(sjtu::make_pair(station,TrainID_type::setmin()),sjtu::make_pair(station,TrainID_type::setmax()),trains);
    }
    void gettrainbysmalltrain(const Smalltrain &smalltrain,Train &train){
        released_train.search(smalltrain.getTrainID(),train);
    }
    void getalltrainbysmalltrain(const sjtu::vector<Smalltrain> &smalltrains,sjtu::vector<Train> &trains){
        trains.resize(smalltrains.size());
        for(int i=0;i<smalltrains.size();i++){
            released_train.search(smalltrains[i].getTrainID(),trains[i]);
        }
    }
};

#endif // TRAINSYSTEM_HPP