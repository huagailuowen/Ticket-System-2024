#ifndef SYSTHESSISSYSTEM_HPP
#define SYSTHESSISSYSTEM_HPP
#include <cassert>
#include <cstdio>
#include <iostream>
// #include <iterator>
#include <ostream>
#include <string>
#include <algorithm>
#include <utility> // Include the <utility> header file
#include <vector>
#include "Commandsystem.hpp"
#include "Ticketsystem.hpp"
#include "Trainsystem.hpp"
#include "Usersystem.hpp"
#include "mytype.hpp"
#include"../include/map.hpp"
#include"../include/timer.hpp"
int TIME=0;
Timer timer_qt("q_ticket_t");
Timer timer_trans("q_tans_t");
Timer timer_bt("b_ticket_t");

class Systhesissystem{
private:
#ifdef DEBUG
public:
#endif
    Commandsystem commandsystem;
    Ticketsystem ticketsystem;
    Usersystem usersystem;
    // Trainsystem trainsystem;
    sjtu::map<UserName_type, int>Userlist;
public:
    Systhesissystem() = delete;
    Systhesissystem(const Systhesissystem &systhesissystem) = delete;
    Systhesissystem &operator=(const Systhesissystem &systhesissystem) = delete;
    Systhesissystem(std::string name,bool isnew=false):commandsystem(),ticketsystem(name+"ticketsystem",isnew),usersystem(name+"usersystem",isnew){
        Userlist.clear();
    }
    void query_smalltrain(const Stationname_type &start_station,const Stationname_type &end_station,sjtu::vector<sjtu::pair<Smalltrain,Smalltrain>>&trains);
    int add_user(const Command &command,std::ostream &os);
    int login(const Command &command,std::ostream &os);
    int logout(const Command &command,std::ostream &os);
    int query_profile(const Command &command,std::ostream &os);
    int modify_profile(const Command &command,std::ostream &os);
    int add_train(const Command &command,std::ostream &os);
    int delete_train(const Command &command,std::ostream &os);
    int release_train(const Command &command,std::ostream &os);
    int query_train(const Command &command,std::ostream &os);
    int query_ticket(const Command &command,std::ostream &os);
    int query_transfer(const Command &command,std::ostream &os);
    int buy_ticket(const Command &command,std::ostream &os);//return 0 represent queue
    int query_order(const Command &command,std::ostream &os);
    int refund_ticket(const Command &command,std::ostream &os);
    int clean(const Command &command,std::ostream &os);
    int exit(const Command &command,std::ostream &os);
    void process();

};
int Systhesissystem::add_user(const Command &command,std::ostream &os){
    if(command.type!=command_type::add_user)
        throw TrainSystemError("add_user");
    UserName_type cur_user;
    User user;

    for(int i=0;i<command.auguementNum;i++){
        if(command.key[i]=='c'){
            cur_user=command.auguement[i];
            
        }
        else if(command.key[i]=='u'){
            user.setUserName(command.auguement[i]);
        }
        else if(command.key[i]=='p'){
            user.setPassword(command.auguement[i]);
        }
        else if(command.key[i]=='n'){
            user.setRealName(command.auguement[i]);
        }
        else if(command.key[i]=='m'){
            user.setMailAddr(command.auguement[i]);
        }
        else if(command.key[i]=='g'){
            user.setPrivilege(std::stoi(command.auguement[i]));
        }else{
            throw TrainSystemError("add_user");
        }
    }
    if(usersystem.user_num()==0){
        user.setPrivilege(10);
    }
    else{
        //how to ignore the -c -g
        auto it=Userlist.find(cur_user);
        if(it==Userlist.end())
            return os<<"-1"<<std::endl,-1;
        User check_user;
        bool res=usersystem.find_user(user.getUserName(),check_user);
        
        if(res==true)
            return os<<"-1"<<std::endl,-1;
        if(it->second<=user.getPrivilege())
            return os<<"-1"<<std::endl,-1;
    }
    
    usersystem.add_user(user);
    ticketsystem.add_user(user.getUserName());
    return os<<'0'<<std::endl,0;
}
int Systhesissystem::login(const Command &command,std::ostream &os){
    if(command.type!=command_type::login)
        throw TrainSystemError("login");
    UserName_type cur_user;
    Password_type cur_password;
    for(int i=0;i<command.auguementNum;i++){
        if(command.key[i]=='u'){
            cur_user=command.auguement[i];
        }
        else if(command.key[i]=='p'){
            cur_password=command.auguement[i];
        }else{
            throw TrainSystemError("login");
        }
    }
    if(Userlist.find(cur_user)!=Userlist.end())
        return os<<"-1"<<std::endl,-1;
    User user;
    bool res=usersystem.find_user(cur_user,user);
    if(res==false)
        return os<<"-1"<<std::endl,-1;
    if(user.getPassword()!=cur_password)
        return os<<"-1"<<std::endl,-1;
    Userlist[cur_user]=user.getPrivilege();
    return os<<'0'<<std::endl,0;
}
int Systhesissystem::logout(const Command &command,std::ostream &os){
    // std::cerr<<"&";
    if(command.type!=command_type::logout)
        throw TrainSystemError("logout");
    UserName_type cur_user;
    for(int i=0;i<command.auguementNum;i++){
        if(command.key[i]=='u'){
            cur_user=command.auguement[i];
        }else{
            throw TrainSystemError("logout");
        }
    }
    if(Userlist.find(cur_user)==Userlist.end())
        return os<<"-1"<<std::endl,-1;
    Userlist.erase(cur_user);
    return os<<'0'<<std::endl,0;
}
int Systhesissystem::query_profile(const Command &command,std::ostream &os){
    if(command.type!=command_type::query_profile)
        throw TrainSystemError("query_profile");
    UserName_type cur_user,query_user;
    for(int i=0;i<command.auguementNum;i++){
        if(command.key[i]=='c'){
            cur_user=command.auguement[i];
        }
        else if(command.key[i]=='u'){
            query_user=command.auguement[i];
        }else{
            throw TrainSystemError("query_profile");
        }
    }
    auto it=Userlist.find(cur_user);
    if(it==Userlist.end())
        return os<<"-1"<<std::endl,-1;
    User user;
    bool res=usersystem.find_user(query_user,user);
    
    
    if(res==false)
        return os<<"-1"<<std::endl,-1;
    if(it->second<=user.getPrivilege()&&cur_user!=query_user)
        return os<<"-1"<<std::endl,-1;
    os<<user.getUserName()<<" "<<user.getRealName()<<" "<<user.getMailAddr()<<" "<<user.getPrivilege()<<std::endl;
    return 0;
}
int Systhesissystem::modify_profile(const Command &command,std::ostream &os){
    if(command.type!=command_type::modify_profile)
        throw TrainSystemError("modify_profile");
    UserName_type cur_user,query_user;
    User user;
    for(int i=0;i<command.auguementNum;i++){
        if(command.key[i]=='c'){
            cur_user=command.auguement[i];
        }
        else if(command.key[i]=='u'){
            query_user=command.auguement[i];
            user.setUserName(query_user);
        }
        else if(command.key[i]=='n'){
            user.setRealName(command.auguement[i]);
        }
        else if(command.key[i]=='m'){
            user.setMailAddr(command.auguement[i]);
        }
        else if(command.key[i]=='p'){
            user.setPassword(command.auguement[i]);
        }else if(command.key[i]=='g'){
            user.setPrivilege(std::stoi(command.auguement[i]));
        }else {
            throw TrainSystemError("modify_profile");
        }
    }
    auto it=Userlist.find(cur_user);
    if(it==Userlist.end())
        return os<<"-1"<<std::endl,-1;
    User check_user;
    bool res=usersystem.find_user(query_user,check_user);
    if(res==false)
        return os<<"-1"<<std::endl,-1;
    if(it->second<=check_user.getPrivilege()&&cur_user!=query_user)
        return os<<"-1"<<std::endl,-1;
    if(user.getPrivilege()!=-1&&it->second<=user.getPrivilege())
        return os<<"-1"<<std::endl,-1;
    if(user.getPrivilege()==-1)user.setPrivilege(check_user.getPrivilege());
    if(!user.getUserName().size())user.setUserName(user.getUserName());
    if(!user.getRealName().size())user.setRealName(check_user.getRealName());
    if(!user.getMailAddr().size())user.setMailAddr(check_user.getMailAddr());
    if(!user.getPassword().size())user.setPassword(check_user.getPassword());
    usersystem.modify_user(query_user,user);
    os<<user.getUserName()<<" "<<user.getRealName()<<" "<<user.getMailAddr()<<" "<<user.getPrivilege()<<std::endl;
    return 0;
}
int Systhesissystem::add_train(const Command &command,std::ostream &os){
    if(command.type!=command_type::add_train)
        throw TrainSystemError("add_train");
    Train train;
    for(int i=0;i<command.auguementNum;i++){
        // if(command.key[i]=='c'){
        //     cur_user=command.auguement[i];
        // }
        // else 
        if(command.key[i]=='i'){
            train.setTrainID(command.auguement[i]);
        }
        else if(command.key[i]=='n'){
            train.setStationNum(std::stoi(command.auguement[i]));
        }
        else if(command.key[i]=='s'){
            //station
            train.setStation(command.auguement[i]);
        }
        else if(command.key[i]=='p'){
            //price
            train.setPrice(command.auguement[i]);
        }
        else if(command.key[i]=='x'){
            train.setStartTime(time_to_int(command.auguement[i]));
        }
        else if(command.key[i]=='m'){
            train.setSeatNum(std::stoi(command.auguement[i]));
        }
        else if(command.key[i]=='d'){
            //sale date
            train.setSaleDate(command.auguement[i]);
        }
        else if(command.key[i]=='t'){
            train.setTravelTime(command.auguement[i]);   
        }
        else if(command.key[i]=='o'){
            train.setStopoverTime(command.auguement[i]);

        }else if(command.key[i]=='y'){
            #ifdef DEBUG
            if(command.auguement[i].size()!=1)
                throw TrainSystemError("add_train");
            #endif
            train.setType(command.auguement[i][0]);
        }
        else{
            throw TrainSystemError("add_train");
        }
    }
    // auto it=Userlist.find(cur_user);
    // if(it==Userlist.end())
    //     return os<<"-1"<<std::endl,-1;
    // if(it->second<2)
    //     return os<<"-1"<<std::endl,-1;
    train.updatearrivetime();
    
    bool res=ticketsystem.add_train(train);
    return res==true?(os<<'0'<<std::endl,0):(os<<"-1"<<std::endl,-1);
}
int Systhesissystem::delete_train(const Command &command,std::ostream &os){
    if(command.type!=command_type::delete_train)
        throw TrainSystemError("delete_train");
    TrainID_type trainID;
    for(int i=0;i<command.auguementNum;i++){
        if(command.key[i]=='i'){
            trainID=command.auguement[i];
        }else{
            throw TrainSystemError("delete_train");
        }
    }
    // auto it=Userlist.find(cur_user);
    // if(it==Userlist.end())
    //     return os<<"-1"<<std::endl,-1;
    // if(it->second<2)
    //     return os<<"-1"<<std::endl,-1;
    bool res=ticketsystem.delete_train(trainID);
    return res==true?(os<<'0'<<std::endl,0):(os<<"-1"<<std::endl,-1);
}
int Systhesissystem::release_train(const Command &command,std::ostream &os){
    if(command.type!=command_type::release_train)
        throw TrainSystemError("release_train");
    TrainID_type trainID;
    int date;
    for(int i=0;i<command.auguementNum;i++){
        if(command.key[i]=='i'){
            trainID=command.auguement[i];
        }else{
            throw TrainSystemError("release_train");
        }
    }
    bool res=ticketsystem.release_train(trainID);
    return res==true?(os<<'0'<<std::endl,0):(os<<"-1"<<std::endl,-1);
}
int Systhesissystem::query_train(const Command  &command,std::ostream &os){
    if(command.type!=command_type::query_train)
        throw TrainSystemError("query_train");
    TrainID_type trainID;
    int date;
    for(int i=0;i<command.auguementNum;i++){
        if(command.key[i]=='i'){
            trainID=command.auguement[i];
        }
        else if(command.key[i]=='d'){
            date=Date_to_int(command.auguement[i]);
        }else{
            throw TrainSystemError("query_train");
        }
    }
    Train train;
    ReleasedTrain released_train;
    bool res=ticketsystem.query_train(trainID,date,train,released_train);
    
    if(res==false)
        return os<<"-1"<<std::endl,-1;
    os<<train.getTrainID()<<" "<<train.getType()<<std::endl;
    int price=0;
    int begin_day=released_train.getDate();
    Mydate curdate(begin_day,train.getStartTime());
    for(int i=0;i<train.getStationNum();i++){
        os<<train.getStation(i)<<" ";
        if(i==0){
            os<<"xx-xx xx:xx";
        }else{
            os<<curdate;
            
        }
        os<<" -> ";
        if(i>0&&i+1<train.getStationNum()){
            curdate=curdate+Mydate(0,train.getStopoverTime(i-1));
        }
        if(i+1==train.getStationNum()){
            os<<"xx-xx xx:xx";
        }
        else{
            os<<curdate;
        } 
        
        curdate=Mydate(begin_day,train.getarrivetime(i+1));
        
        os<<" "<<price<<" ";
        if(i+1==train.getStationNum()){
            os<<"x";
        }
        else{
            os<<released_train.getSeat(i);
            price+=train.getPrice(i);
        } 
        os<<std::endl;
    }
    return 0;
}
class Train_sort{
private:
#ifdef DEBUG
public:
#endif
    TrainID_type trainID;
    Mydate leaving_time;
    Mydate arriving_time;
    int price;
    int seat;
    Stationname_type start_station;
    Stationname_type end_station;
    friend class cmpTrain_by_time;
    friend class cmpTrain_by_cost;
public:
    Train_sort()=default;
    Train_sort(const TrainID_type &trainID,const Stationname_type &start_station,const Stationname_type &end_station,const Mydate &leaving_time,const Mydate &arriving_time,const int &price,const int &seat):trainID(trainID),start_station(start_station),end_station(end_station),leaving_time(leaving_time),arriving_time(arriving_time),price(price),seat(seat){}

    TrainID_type getTrainID() const{
        return trainID;
    }
    Mydate getLeavingTime() const{
        return leaving_time;
    }
    Mydate getArrivingTime() const{
        return arriving_time;
    }
    int getPrice() const{
        return price;
    }
    int getSeat() const{
        return seat;
    }
    static Train_sort max(){return Train_sort(TrainID_type::setmax(),Stationname_type::setmax(),Stationname_type::setmax(),Mydate(),Mydate::max(),100000,100000);}
    friend std::ostream &operator<<(std::ostream &os,const Train_sort &a);
};
std::ostream& operator<<(std::ostream& os, const Train_sort& a) {
    os << a.trainID << " " << a.start_station << " " << a.leaving_time << " -> " << a.end_station << " " << a.arriving_time << " " << a.price << " " << a.seat;
    return os;
}
class cmpTrain_by_time{
public:
    bool operator()(const Train_sort &a,const Train_sort &b){
        int time1=(int)a.arriving_time-(int)a.leaving_time;
        int time2=(int)b.arriving_time-(int)b.leaving_time;
        if(time1==time2){
            return a.trainID<b.trainID;
            if(a.price==b.price)
                return a.trainID<b.trainID;
            else
                return a.price<b.price;
        }else{
            return time1<time2;
        }
    }
};
class cmpTrain_by_cost{
public:
    bool operator()(const Train_sort &a,const Train_sort &b){
        if(a.price==b.price){
            return a.trainID<b.trainID;
            int time1=(int)a.arriving_time-(int)a.leaving_time;
            int time2=(int)b.arriving_time-(int)b.leaving_time;
            if(time1==time2)
                return a.trainID<b.trainID;
            else
                return time1<time2;
        }else{
            return a.price<b.price;
        }
    }
};

// bool calculate_ticket(const sjtu::pair<SmallTrain,ReleasedTrain> &train,const Stationname_type &start_station,const Stationname_type &end_station,Train_sort &train_sort,bool cantherenotickets=true){
//     int start_station_num=-1;
//     int end_station_num=-1;
//     for(int i=0;i<train.first.getStationNum();i++){
//         if(train.first.getStation(i)==start_station){
//             start_station_num=i;
//         }
//         if(train.first.getStation(i)==end_station){
//             end_station_num=i;
//         }
//     }
//     if(start_station_num==-1||end_station_num==-1)
//         return false;
//     if(start_station_num>=end_station_num)
//         return false;
    
//     Mydate leaving_time=train.first.getleavetime(start_station_num)+Mydate(train.second.getDate(),0);
//     Mydate arriving_time=train.first.getarrivetime(end_station_num)+Mydate(train.second.getDate(),0);
//     int price=0,seat=1000000;
//     for(int i=start_station_num;i<end_station_num;i++){
//         price+=train.first.getPrice(i);
//         seat=std::min(seat,train.second.getSeat(i));
//     }
//     //可以不拷贝
//     //???要不要显示？？？
//     if(seat==0&&!cantherenotickets)
//         return false;
//     train_sort=Train_sort(train.first.getTrainID(),train.first.getStation(start_station_num),train.first.getStation(end_station_num),leaving_time,arriving_time,price,seat);
//     return true;

// }
bool calculate_ticket(const sjtu::pair<Train,ReleasedTrain> &train,const int &start_station_num,const int &end_station_num,Train_sort &train_sort,sjtu::pair<int,sjtu::pair<int,int>>hint=sjtu::make_pair(-1,sjtu::make_pair(-1,-1)),bool can_no_seat=true){
//hint 1.type 0 means from begin had calculated,tyep 1 means from end had calculated -1
//hint 2.price 3.seat
    if(start_station_num==-1||end_station_num==-1)
        return false;
    if(start_station_num>=end_station_num)
        return false;
    if(hint.first< -1||hint.first>1)throw TrainSystemError("calculate_ticket");
    Mydate leaving_time=train.first.getleavetime(start_station_num)+Mydate(train.second.getDate(),0);
    Mydate arriving_time=train.first.getarrivetime(end_station_num)+Mydate(train.second.getDate(),0);

    int price=0,seat=1000000;
    // type 0 means from begin had calculated,tyep 1 means from end had calculated -1
    if(hint.first==0){
        price=hint.second.first+train.first.getPrice(end_station_num-1);
        seat=std::min(hint.second.second,train.second.getSeat(end_station_num-1));
    }else if(hint.first==1){
        price=hint.second.first+train.first.getPrice(start_station_num);
        seat=std::min(hint.second.second,train.second.getSeat(start_station_num));
    }
    else{
        for(int i=start_station_num;i< end_station_num;i++){
            price+=train.first.getPrice(i);
            seat=std::min(seat,train.second.getSeat(i));
        }
    }
    //可以不拷贝
    //???要不要显示？？？
    if(seat==0&&!can_no_seat)
        return false;
    train_sort=Train_sort(train.first.getTrainID(),train.first.getStation(start_station_num),train.first.getStation(end_station_num),leaving_time,arriving_time,price,seat);
    return true;

}
bool calculate_ticket(const sjtu::pair<sjtu::pair<Smalltrain,Smalltrain>,ReleasedTrain> &train,const Stationname_type &start_station,const Stationname_type &end_station,Train_sort &train_sort,bool can_no_seat=true){
    //there is no day in arrivetime
    // if(start_station_num==-1||end_station_num==-1)
    //     return false;
    // if(start_station_num>=end_station_num)
    //     return false;
    // if(hint.first< -1||hint.first>1)throw TrainSystemError("calculate_ticket");

    Mydate leaving_time=train.first.first.getleavetime()+Mydate(train.second.getDate(),0);
    Mydate arriving_time=train.first.second.getarrivetime()+Mydate(train.second.getDate(),0);
    int start_station_num=train.first.first.getStationindex();
    int end_station_num=train.first.second.getStationindex();
    if(start_station_num>=end_station_num)
        return false;
    int seat=1000000,price=train.first.second.getPrice()-train.first.first.getPrice();
    for(int i=start_station_num;i< end_station_num;i++){
        seat=std::min(seat,train.second.getSeat(i));
    }
    //可以不拷贝
    //???要不要显示？？？
    if(seat==0&&!can_no_seat)
        return false;
    train_sort=Train_sort(train.first.first.getTrainID(),start_station,end_station,leaving_time,arriving_time,price,seat);
    return true;

}
void Systhesissystem::query_smalltrain(const Stationname_type &start_station,const Stationname_type &end_station,sjtu::vector<sjtu::pair<Smalltrain,Smalltrain>>&trains)
{
    trains.clear();
    sjtu::vector<Smalltrain>bg,ed;
    
    ticketsystem.getallsmalltrain_bystation(start_station,bg);
    ticketsystem.getallsmalltrain_bystation(end_station,ed);
    int it_bg=0,it_ed=0;
    while(it_bg<bg.size()&&it_ed<ed.size()){
        if(bg[it_bg].getTrainID()==ed[it_ed].getTrainID()){
            if(bg[it_bg].getStationindex()<ed[it_ed].getStationindex()){
                trains.push_back(sjtu::make_pair(bg[it_bg],ed[it_ed]));
            }
            it_bg++;
            it_ed++;
        }else if(bg[it_bg].getTrainID()<ed[it_ed].getTrainID()){
            it_bg++;
        }else{
            it_ed++;
        }
    }

}
int Systhesissystem::query_ticket(const Command &command,std::ostream &os){
    if(command.type!=command_type::query_ticket)
        throw TrainSystemError("query_ticket");
    Stationname_type start_station,end_station;
    int date_day;
    int is_priority_time=-1;

    for(int i=0;i<command.auguementNum;i++){
        if(command.key[i]=='s'){
            start_station=command.auguement[i];
        }
        else if(command.key[i]=='t'){
            end_station=command.auguement[i];
        }
        else if(command.key[i]=='d'){
            date_day=Date_to_int( command.auguement[i]);
        }
        else if(command.key[i]=='p'){
            if(command.auguement[i]=="time"){
                is_priority_time=1;
            }else if(command.auguement[i]=="cost"){
                is_priority_time=0;
            }else{
                throw TrainSystemError("query_ticket");
            }
        }else{
            throw TrainSystemError("query_ticket");
        }
    }
    if(is_priority_time==-1)
        throw TrainSystemError("query_ticket");
        // return os<<"-1"<<std::endl,-1;

    // sjtu::vector<sjtu::pair<Train,ReleasedTrain>>possible_train;
    sjtu::vector<sjtu::pair<Smalltrain, Smalltrain>>possible_train;
    
    sjtu::vector<Train_sort>possible_train_sort;
    //根据寻址连续性，这种方式实际上是比较快的，因为选中的trainid也是升序排列的？
    //但是还有更快的方法？，就是在station中直接放releasedtrain，这样就不用再次查询了
    //这两种方法哪个更快？？
    //我先写第一种吧
    Train_sort train_sort;
    // ticketsystem.getalltrain_bystation_time(start_station,Mydate(date_day,0),possible_train,true);
    query_smalltrain(start_station,end_station,possible_train);
    
    for(auto &i:possible_train){
        
        ReleasedTrain released_train;
        bool res=ticketsystem.getreleasedtrain_bysmalltrain(i.first,start_station,Mydate(date_day,0),released_train);
        if(!res)continue;
        if(TIME==7197){
            std::cerr<<i.first.getTrainID()<<std::endl;
        }
        if(calculate_ticket(sjtu::make_pair(i,released_train),start_station,end_station,train_sort)){
            possible_train_sort.push_back(train_sort);
        }
    }
    
    if(is_priority_time==1)
        quickSort<Train_sort,cmpTrain_by_time>(possible_train_sort,0,possible_train_sort.size()-1);
    else if(is_priority_time==0)
        quickSort<Train_sort,cmpTrain_by_cost>(possible_train_sort,0,possible_train_sort.size()-1);
    // if(is_priority_time==1)
    //     std::sort(possible_train_sort.begin(),possible_train_sort.end(),cmpTrain_by_time());
    // else if(is_priority_time==0)
    //     std::sort(possible_train_sort.begin(),possible_train_sort.end(),cmpTrain_by_cost());
    // os<<'0'<<std::endl;
    os<<possible_train_sort.size()<<std::endl;
    // for(int i=0;i+1<possible_train_sort.size();i++){
    //     if(is_priority_time==1){
    //         assert(cmpTrain_by_time()(possible_train_sort[i+1],possible_train_sort[i])==false);
    //     }else{
    //         assert(cmpTrain_by_cost()(possible_train_sort[i+1],possible_train_sort[i])==false);
    //     }
    // }
    for(auto &i:possible_train_sort){
        os<<i<<std::endl;
    }
    return 0;
}
int Systhesissystem::query_transfer(const Command &command,std::ostream &os){
    if(command.type!=command_type::query_transfer)
        throw TrainSystemError("query_transfer");
    Stationname_type start_station,end_station;
    int date_day;
    int is_priority_time=-1;
    for(int i=0;i<command.auguementNum;i++){
        if(command.key[i]=='s'){
            start_station=command.auguement[i];
        }
        else if(command.key[i]=='t'){
            end_station=command.auguement[i];
        }
        else if(command.key[i]=='d'){
            date_day=Date_to_int( command.auguement[i]);
        }
        else if(command.key[i]=='p'){
            if(command.auguement[i]=="time"){
                is_priority_time=1;
            }else if(command.auguement[i]=="cost"){
                is_priority_time=0;
            }else{
                throw TrainSystemError("query_transfer");
            }
        }else{
            throw TrainSystemError("query_transfer");
        }
    }
    if(is_priority_time==-1)
        throw TrainSystemError("query_transfer");
    sjtu::vector<sjtu::pair<Train,ReleasedTrain>>possible_train;
    sjtu::vector<sjtu::pair<Smalltrain, Smalltrain>>possible_train2;
    ticketsystem.getallTrain_bystation_time(start_station,Mydate(date_day,0),possible_train);

    // sjtu::map<Stationname_type,Train_sort>best_train[2];
    //0 is the first best,1 is the second best
    // 这里不会有环形路线吧
// 感觉现在算法有一些问题
// 在以时间作为优先级上;
    sjtu::pair<Train_sort,Train_sort>res={Train_sort(),Train_sort()};
    auto checkmin=[this,&res,is_priority_time](const Train_sort&cur_first,const Train_sort&cur_second)->void
    {
        if(res.first.getTrainID()==TrainID_type()){
            res.first=cur_first;
            res.second=cur_second;
            return ;
        }
        int timeres=res.second.getArrivingTime()-res.first.getLeavingTime();
        int timecur=cur_second.getArrivingTime()-cur_first.getLeavingTime();
        // int timecur=cur.second.getArrivingTime()-cur.first.getLeavingTime();
        int priceres=res.first.getPrice()+res.second.getPrice();
        // int pricecur=cur.first.getPrice()+cur.second.getPrice();
        int pricecur=cur_first.getPrice()+cur_second.getPrice();
        if(is_priority_time){
            // if(TIME==379506){
            //     if(cur_first.getTrainID()==TrainID_type("INCABINDSHIPSAT")&&cur_second.getTrainID()==TrainID_type("aparadoxappearsth")){
            //         Train train;
            //         ReleasedTrain released_train;
            //         ticketsystem.query_train(cur_first.getTrainID(),cur_first.getLeavingTime().day,train,released_train);
            //         std::cerr<<train<<std::endl;
            //     }
            //     std::cerr<<"----------------------------------\n";
            //     std::cerr<<timeres<<" "<<timecur<<" "<<priceres<<" "<<pricecur<<std::endl;
            // }
            if(timeres!=timecur){
                if(timeres>timecur){
                    res.first=cur_first;
                    res.second=cur_second;
                }
                return;
            }
            if(priceres!=pricecur){
                if(priceres>pricecur){
                    res.first=cur_first;
                    res.second=cur_second;
                }
                return;
            }
            if(res.first.getTrainID()!=cur_first.getTrainID()){
                if(res.first.getTrainID()>cur_first.getTrainID()){
                    res.first=cur_first;
                    res.second=cur_second;
                }
                return ;
            }
            if(res.second.getTrainID()!=cur_second.getTrainID()){
                if(res.second.getTrainID()>cur_second.getTrainID()){
                    res.first=cur_first;
                    res.second=cur_second;
                }
                return ;
            }
        }else{
            if(priceres!=pricecur){
                if(priceres>pricecur){
                    res.first=cur_first;
                    res.second=cur_second;
                }
                return;
            }
            if(timeres!=timecur){
                if(timeres>timecur){
                    res.first=cur_first;
                    res.second=cur_second;
                }
                return;
            }
            if(res.first.getTrainID()!=cur_first.getTrainID()){
                if(res.first.getTrainID()>cur_first.getTrainID()){
                    res.first=cur_first;
                    res.second=cur_second;
                }
                return ;
            }
            if(res.second.getTrainID()!=cur_second.getTrainID()){
                if(res.second.getTrainID()>cur_second.getTrainID()){
                    res.first=cur_first;
                    res.second=cur_second;
                }
                return ;
            }
        }
    };
    for(auto &i:possible_train){
        for(int j=0;j<i.first.getStationNum();j++){
            if(i.first.getStation(j)==start_station){
                sjtu::pair<int, sjtu::pair<int,int>>hint(0,sjtu::make_pair(0,1000000));
                for(int k=j+1;k<i.first.getStationNum();k++){
                    if(i.first.getStation(k)==end_station){
                        hint.second.first+=i.first.getPrice(k-1);
                        hint.second.second=std::min(hint.second.second,i.second.getSeat(k-1));
                        continue;
                    }
                    Train_sort train_sort;
                    Stationname_type transfer_station=i.first.getStation(k);
                    bool res=calculate_ticket(i,j,k,train_sort,hint);
                    hint.second.first+=i.first.getPrice(k-1);
                    hint.second.second=std::min(hint.second.second,i.second.getSeat(k-1));
                    if(!res)
                        continue;
                        // if(best_train[0].find(i.first.getStation(k))==best_train[0].end()){
                        //     best_train[0][i.first.getStation(k)]=train_sort;
                        // }else{
                        //     //这里可以加速
                        //     if(best_train[0][i.first.getStation(k)].getArrivingTime()>train_sort.getArrivingTime()){
                        //         best_train[1][i.first.getStation(k)]=best_train[0][i.first.getStation(k)];
                        //         best_train[0][i.first.getStation(k)]=train_sort;
                        //     }else if(best_train[1].find(i.first.getStation(k))==best_train[1].end()||best_train[1][i.first.getStation(k)].getArrivingTime()>train_sort.getArrivingTime()){
                        //         best_train[1][i.first.getStation(k)]=train_sort;
                        //     }
                        // }
                    // std::vector<sjtu::pair<Train,ReleasedTrain>>possible_train2;
                    Mydate transfer_time=train_sort.getArrivingTime();
                    
                    query_smalltrain(transfer_station,end_station,possible_train2);
                    
                    Train_sort train_sort2;
                   
                    for(auto &ii:possible_train2){
                        
                        // cnt++;
                            // if(is_debug){
                            //     std::cerr<<ii.first.getTrainID()<<std::endl;
                            //     // if(ii.first.getTrainID()==TrainID_type("aparadoxappearsth"))
                            //     //     std::cerr<<"AHAHHAHAHAHAHAHAHAHAHAHHA\n"<<ii.first.getTrainID()<<std::endl;
                            // }
                        ReleasedTrain released_train;
                        if(ii.first.getTrainID()==i.first.getTrainID())
                            continue;

                        res=ticketsystem.getreleasedtrain_bysmalltrain(ii.first,transfer_station,transfer_time,released_train,false);
                        if(!res)
                            continue;
                        if(!calculate_ticket(sjtu::make_pair(ii,released_train),transfer_station,end_station,train_sort2,true))
                            continue;
                        
                        
                        checkmin(train_sort,train_sort2);
                        // break;   fuck  FUUUUUCKKKKKKK
                    }
                    // if(is_debug){
                    //     std::cerr<<cnt<<"}}}}}}}}}}}}}"<<std::endl;
                    // }
                }
                break;
            }
        }
    }
    //下面这种做法是错的
    // ticketsystem.getalltrain_bystation(end_station,possible_train2);
    // for(auto &i:possible_train2){
    //     for(int j=i.getStationNum()-1;j>=0;j--){
    //         if(i.getStation(j)==end_station){
    //             Train_sort train_sort;  
    //             ReleasedTrain released_train;
    //             sjtu::pair<int, sjtu::pair<int,int>>hint(1,sjtu::make_pair(0,1000000));
    //             for(int k=j-1;k>=0;k--){
    //                 Stationname_type transfer_station=i.getStation(k);
    //                 auto it=best_train[0].find(transfer_station);
    //                 if(it==best_train[0].end())continue;
    //                 if(it->second.getTrainID()==i.getTrainID()){
    //                     it=best_train[1].find(transfer_station);
    //                     if(it==best_train[1].end())
    //                         continue;
    //                 }
    //                 Mydate transfer_date=it->second.getArrivingTime();
    //                 ticketsystem.getreleasedtrain_bytrain(i,transfer_station,transfer_date,released_train);
    //                 calculate_ticket(sjtu::make_pair(i,released_train),k,j,train_sort);
    //                 checkmin(sjtu::make_pair(it->second,train_sort)) ;                   
    //             }
    //             break;
    //         }
    //     }
    // }
    if(res.first.getTrainID()==TrainID_type())
        return os<<"0"<<std::endl,-1;//not -1
    os<<res.first<<std::endl;
    os<<res.second<<std::endl;
    return 0;
}
int Systhesissystem::buy_ticket(const Command &command,std::ostream &os){
    if(command.type!=command_type::buy_ticket)
        throw TrainSystemError("buy_ticket");
    UserName_type cur_user;
    TrainID_type trainID;
    int date_day;
    //这个是始发还是到哪站的时间？？？
    int ticket_num;
    Stationname_type start_station,end_station;
    bool is_queue=false;
    //有默认值吗？？
    for(int i=0;i<command.auguementNum;i++){
        if(command.key[i]=='u'){
            cur_user=command.auguement[i];
        }
        else if(command.key[i]=='i'){
            trainID=command.auguement[i];
        }
        else if(command.key[i]=='d'){
            date_day=Date_to_int( command.auguement[i]);
        }
        else if(command.key[i]=='n'){
            ticket_num=std::stoi(command.auguement[i]);
        }
        else if(command.key[i]=='f'){
            start_station=command.auguement[i];
        }
        else if(command.key[i]=='t'){
            end_station=command.auguement[i];
        }
        else if(command.key[i]=='q'){
            if(command.auguement[i]=="true"){
                is_queue=true;
            }else if(command.auguement[i]=="false"){
                is_queue=false;
            }else{
                throw TrainSystemError("buy_ticket");
            }
        }else{
            throw TrainSystemError("buy_ticket");
        }
    }
    auto it=Userlist.find(cur_user);
    if(it==Userlist.end())
        return os<<"-1"<<std::endl,-1;
    bool res=false;
    // User user;
    // bool res=usersystem.find_user(cur_user,user);
    // if(res==false)
    //     return os<<"-1"<<std::endl,-1;
    Train train;
    ReleasedTrain released_train;
    res=ticketsystem.query_train(trainID,train);
    if(res==false)
        return os<<"-1"<<std::endl,-1;
    int start_station_num=-1,end_station_num=-1;
    for(int i=0;i<train.getStationNum();i++){
        if(train.getStation(i)==start_station){
            start_station_num=i;
        }
        if(train.getStation(i)==end_station){
            end_station_num=i;
        }
    }
    if(start_station_num==-1||end_station_num==-1)
        return os<<"-1"<<std::endl,-1;
    res=ticketsystem.getreleasedtrain_bysmalltrain(Smalltrain(train,start_station_num),start_station,Mydate(date_day,0),released_train,true);
    // if(TIME==100427){
    //     std::cerr<<res<<std::endl;
    //     std::cerr<<released_train<<std::endl;
    //     std::cerr<<"------------------------------------\n";
    // }
    if(res==false)
        return os<<"-1"<<std::endl,-1;
    Train_sort train_sort;
    // std::cerr<<TIME<<std::endl<<released_train<<std::endl;
    sjtu::pair<Smalltrain,Smalltrain>tmp=   sjtu::make_pair(Smalltrain(train,start_station),Smalltrain(train,end_station));
    res=calculate_ticket(sjtu::make_pair(tmp,released_train),start_station,end_station,train_sort,true);
    if(res==false)
        return os<<"-1"<<std::endl,-1;
    // if(TIME==21589){
    //     std::cerr<<train<<std::endl;
    //     std::cerr<<released_train<<std::endl;
    //     std::cerr<<train_sort<<std::endl;
    //     std::cerr<<ticket_num<<std::endl;
    // }
    if(ticket_num>train.getSeatNum()){
        return os<<"-1"<<std::endl,-1;
    }
    if(train_sort.getSeat()<ticket_num){
        if(is_queue){
            os<<"queue"<<std::endl;
            ticketsystem.add_queue(trainID,start_station,end_station,train,released_train,cur_user,train_sort.getPrice(),ticket_num,command.timestamp);
            return 0;
        }else{
            return os<<"-1"<<std::endl,-1;
        }
    
    }else{
        os<<train_sort.getPrice()*ticket_num<<std::endl;
        {
            assert(train_sort.getLeavingTime().day()==date_day);
            int id=-1;
            for(int i=0;i<train.getStationNum();i++){
                if(train.getStation(i)==start_station){
                    id=i;
                    break;
                }
            }
            assert(id!=-1);
            assert(train_sort.getLeavingTime()==Mydate(released_train.getDate(),0)+train.getleavetime(id));
        }
        ticketsystem.buy_ticket(trainID,start_station,end_station,train,released_train,cur_user,train_sort.getPrice(),ticket_num,command.timestamp);
        return 0;
    }
}

int Systhesissystem::query_order(const Command &command,std::ostream &os){
    if(command.type!=command_type::query_order)
        throw TrainSystemError("query_order");
    UserName_type cur_username;
    for(int i=0;i<command.auguementNum;i++){
        if(command.key[i]=='u'){
            cur_username=command.auguement[i];
        }else{
            throw TrainSystemError("query_order");
        }
    }
    auto it=Userlist.find(cur_username);
    if(it==Userlist.end())
        return os<<"-1"<<std::endl,-1;
    sjtu::vector<Ticket>ticket;
    ticketsystem.query_order(cur_username,ticket);
    os<<ticket.size()<<std::endl;
    //倒序输出
    for(int i=ticket.size()-1;i>=0;i--){
        if(ticket[i].getTicketType()==Ticket::TicketType::success){
            os<<"[success] ";
        }else if(ticket[i].getTicketType()==Ticket::TicketType::pending){
            os<<"[pending] ";
        }else if(ticket[i].getTicketType()==Ticket::TicketType::refunded){
            os<<"[refunded] ";
        }else{
            throw TrainSystemError("query_order");
        }
        //example : [success] INSCRIPTIONS 陕西省渭南市 07-22 11:00 -> 福建省永安市 07-22 14:39 2574 10051
        os<<ticket[i].getTrainID()<<" "<<ticket[i].getStartStation()<<" "<<ticket[i].getLeaveTime()<<" -> "<<ticket[i].getEndStation()<<" "<<ticket[i].getArriveTime()<<" "<<ticket[i].getPrice()<<" "<<ticket[i].getSeatNum()<<std::endl;
    }
    return 0;
}
int Systhesissystem::refund_ticket(const Command &command,std::ostream &os){
    if(command.type!=command_type::refund_ticket)
        throw TrainSystemError("callrefund_ticket");
    UserName_type cur_username;
    int index;
    for(int i=0;i<command.auguementNum;i++){
        if(command.key[i]=='u'){
            cur_username=command.auguement[i];
        }else if(command.key[i]=='n'){
            index=std::stoi(command.auguement[i]);
        }else{
            throw TrainSystemError("callrefund_ticket");
        }
    }
    auto it=Userlist.find(cur_username);
    if(it==Userlist.end())
        return os<<"-1"<<std::endl,-1;
    
    if(index<1||index>ticketsystem.get_order_num(cur_username))
        return os<<"-1"<<std::endl,-1;
    bool res=ticketsystem.refund_ticket(cur_username,index);
    if(res==false)
        // throw TrainSystemError("haharefund_ticket");
        return os<<"-1"<<std::endl,-1;
    return os<<'0'<<std::endl,0;
}

//使用暴力来完成refundticket的找第i个订单
int Systhesissystem::clean(const Command &command,std::ostream &os){
    if(command.type!=command_type::clean)
        throw TrainSystemError("clean");
    ticketsystem.clear();
    usersystem.clear();
    Userlist.clear();
    return os<<'0'<<std::endl,0;
}
int Systhesissystem::exit(const Command &command,std::ostream &os){
    if(command.type!=command_type::exit)
        throw TrainSystemError("exit");
    Userlist.clear();
    return os<<"bye"<<std::endl,0;
}
void Systhesissystem::process()
{
    Command cmd;
        // getchar();
    while (commandsystem.readcommand(cmd),true)
    {
        if(cmd.type==command_type::inanity){
            continue;
        }
        
        #ifdef DEBUG
        std::cerr<<cmd<<std::endl;
        #endif
        bool res;
        std::ostream& os(std::cout);
        TIME=cmd.timestamp;
        os<<'['<<cmd.timestamp<<']'<<' ';
        // std::cerr<<"m";
        // if(TIME==750566){
        //     ReleasedTrain released_train;
        //     Train train;
        //     bool res;
        //     res=ticketsystem.query_train(TrainID_type("AndIwillmakeaso"),train);
        //     if(!res)std::cerr<<"&&&&&&&&&&\n";
            
        //     res=ticketsystem.getreleasedtrain_bytrain(train,Stationname_type("北京市"),Mydate(Date_to_int("06-29"),0),released_train,true);
        //     std::cerr<<'['<<cmd.timestamp<<']'<<std::endl;
        //     std::cerr<<train<<std::endl;
        //     std::cerr<<released_train<<std::endl;
        //     // K:;
        // }
        
        switch (cmd.type)
        {
        case command_type::add_user:
            res=add_user(cmd,os);
            break;
        case command_type::login:
            res=login(cmd,os);
            break;  
        case command_type::logout:
            res=logout(cmd,os);
            break;
        case command_type::query_profile:
            res=query_profile(cmd,os);
            break;  
        case command_type::modify_profile:
            res=modify_profile(cmd,os);
            break;
        case command_type::add_train:
            res=add_train(cmd,os);
            break;  
        case command_type::delete_train:
            res=delete_train(cmd,os);
            break;  
        case command_type::release_train:
            res=release_train(cmd,os);
            break;  
        case command_type::query_train:
            res=query_train(cmd,os);
            break;
        case command_type::query_ticket:
            // std::stringstream os;
            timer_qt.start();
            res=query_ticket(cmd,os);
            timer_qt.stop();
            break;
        case command_type::query_transfer:
            // std::ostringstream os;
            timer_trans.start();
        
            res=query_transfer(cmd,os);
            timer_trans.stop();
            break;  
        case command_type::buy_ticket:
            timer_bt.start();
            res=buy_ticket(cmd,     os);
            timer_bt.stop();
            break;  
        case command_type::query_order:
            // std::ostringstream os;
            res=query_order(cmd,os);
            break;
        case command_type::refund_ticket:
            res=refund_ticket(cmd   ,os);
            break;  
        case command_type::clean:
            res=clean(cmd,os);
            break;  
        case command_type::exit:
            res=exit(cmd,os);
            if(res==0)
                return;
            break;  
        case command_type::invalid:
            throw TrainSystemError("invalid command");
            break;
        default:
            break;  
        }
    }
    return ;
}



#endif