#ifndef TRAINSYSTEM_HPP
#define TRAINSYSTEM_HPP

// Your code goes here
#include "easyinclude.hpp"
#include "mytype.hpp"
#include "../include/B_plustree.hpp"
const int MAXSTATIONNUM = 100;
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
    sjtu::BPlusTree<TrainID_type, Train,4,4>released_train,unreleased_train;
};

#endif // TRAINSYSTEM_HPP