#ifndef SYSTHESSISSYSTEM_HPP
#define SYSTHESSISSYSTEM_HPP
#include <iterator>
#include <ostream>
#include <string>
#include <utility> // Include the <utility> header file

#include "Commandsystem.hpp"
#include "Ticketsystem.hpp"
#include "Usersystem.hpp"
#include "Trainsystem.hpp"
#include "mytype.hpp"
#include"../include/map.hpp"
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
    Systhesissystem(const std::string &name,bool isnew=false):commandsystem(),ticketsystem(name+"ticketsystem",isnew),usersystem(name+"usersystem",isnew){
        Userlist.clear();
    }
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
    return os<<'0',0;
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
    return os<<'0',0;
}
int Systhesissystem::logout(const Command &command,std::ostream &os){
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
        return -1;
    Userlist.erase(cur_user);
    return os<<'0',0;
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
    os<<'0'<<std::endl;
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
    os<<'0'<<std::endl;
    User check_user;
    bool res=usersystem.find_user(query_user,check_user);
    if(res==false)
        return os<<"-1"<<std::endl,-1;
    if(it->second<=check_user.getPrivilege()&&cur_user!=query_user)
        return os<<"-1"<<std::endl,-1;
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
            train.setSeatNum(std::stoi(command.auguement[i]));
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
            // train.setStopoverTime(command.auguement[i]);

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
    os<<'0'<<std::endl;
    os<<train.getTrainID()<<" "<<train.getType()<<std::endl;
    int price=0;
    Mydate curdate(released_train.getDate(),train.getStartTime());
    for(int i=0;i<train.getStationNum();i++){
        os<<train.getStation(i)<<" ";
        if(i==0){
            os<<"xx-xx xx:xx";
        }else{
            os<<curdate;
            if(i+1<train.getStationNum()){
                curdate=curdate+Mydate(0,train.getTravelTime(i-1));
            }
        }
        os<<" -> ";
        if(i+1==train.getStationNum()){
            os<<"xx-xx xx:xx";
        }
        else{
            os<<curdate;
        } 
        if(i!=0&&i+2<train.getStationNum()){
            curdate=curdate+Mydate(0,train.getStopoverTime(i-1));
        }
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
    Train_sort(TrainID_type trainID,Mydate leaving_time,Mydate arriving_time,int price,int seat):trainID(trainID),leaving_time(leaving_time),arriving_time(arriving_time),price(price),seat(seat){}
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
    static Train_sort max(){return Train_sort(TrainID_type::setmax(),Mydate(),Mydate::max(),100000,100000);}
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

bool calculate_ticket(const sjtu::pair<Train,ReleasedTrain> &train,const Stationname_type &start_station,const Stationname_type &end_station,Train_sort &train_sort,bool cantherenotickets=false){
    int start_station_num=-1;
    int end_station_num=-1;
    for(int i=0;i<train.first.getStationNum();i++){
        if(train.first.getStation(i)==start_station){
            start_station_num=i;
        }
        if(train.first.getStation(i)==end_station){
            end_station_num=i;
        }
    }
    if(start_station_num==-1||end_station_num==-1)
        return false;
    if(start_station_num>=end_station_num)
        return false;
    Mydate leaving_time=train.first.getleavetime(start_station_num);
    Mydate arriving_time=train.first.getarrivetime(end_station_num);
    int price=0,seat=1000000;
    for(int i=start_station_num;i<end_station_num;i++){
        price+=train.first.getPrice(i);
        seat=std::min(seat,train.second.getSeat(i));
    }
    //可以不拷贝
    //???要不要显示？？？
    if(seat==0&&!cantherenotickets)
        return false;
    train_sort=Train_sort(train.first.getTrainID(),leaving_time,arriving_time,price,seat);
    return true;

}
bool calculate_ticket(const sjtu::pair<Train,ReleasedTrain> &train,const int &start_station_num,const int &end_station_num,Train_sort &train_sort,sjtu::pair<int,sjtu::pair<int,int>>hint=sjtu::make_pair(-1,sjtu::make_pair(-1,-1))){
//hint 1.type 0 means from begin had calculated,tyep 1 means from end had calculated -1
//hint 2.price 3.seat
    if(start_station_num==-1||end_station_num==-1)
        return false;
    if(start_station_num>=end_station_num)
        return false;
    if(hint.first< -1||hint.first>1)throw TrainSystemError("calculate_ticket");
    Mydate leaving_time=train.first.getleavetime(start_station_num);
    Mydate arriving_time=train.first.getarrivetime(end_station_num);

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
    if(seat==0)
        return false;
    train_sort=Train_sort(train.first.getTrainID(),leaving_time,arriving_time,price,seat);
    return true;

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
    sjtu::vector<sjtu::pair<Train,ReleasedTrain>>possible_train;
    sjtu::vector<Train_sort>possible_train_sort;
    //根据寻址连续性，这种方式实际上是比较快的，因为选中的trainid也是升序排列的？
    //但是还有更快的方法？，就是在station中直接放releasedtrain，这样就不用再次查询了
    //这两种方法哪个更快？？
    //我先写第一种吧
    Train_sort train_sort;
    ticketsystem.getalltrain_bystation_time(start_station,date_day,possible_train);
    for(auto &i:possible_train){
        if(calculate_ticket(i,start_station,end_station,train_sort)){
            possible_train_sort.push_back(train_sort);
        }
    }
    if(is_priority_time==1)
        quickSort<Train_sort,cmpTrain_by_time>(possible_train_sort,0,possible_train_sort.size()-1);
    else if(is_priority_time==0)
        quickSort<Train_sort,cmpTrain_by_cost>(possible_train_sort,0,possible_train_sort.size()-1);
    // os<<'0'<<std::endl;
    os<<possible_train_sort.size()<<std::endl;
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
    sjtu::vector<Train>
    possible_train2;
    ticketsystem.getalltrain_bystation_time(start_station,date_day,possible_train);

    sjtu::map<Stationname_type,Train_sort>best_train[2];
    //0 is the first best,1 is the second best
    // 这里不会有环形路线吧

    for(auto &i:possible_train){
        for(int j=0;j<i.first.getStationNum();j++){
            if(i.first.getStation(j)==start_station){
                sjtu::pair<int, sjtu::pair<int,int>>hint(0,sjtu::make_pair(0,1000000));
                for(int k=j+1;k<i.first.getStationNum();k++){
                    if(i.first.getStation(k)==end_station)
                        continue;
                    Train_sort train_sort;
                    if(calculate_ticket(i,j,k,train_sort,hint)){
                        if(best_train[0].find(i.first.getStation(k))==best_train[0].end()){
                            best_train[0][i.first.getStation(k)]=train_sort;
                        }else{
                            //这里可以加速
                            if(best_train[0][i.first.getStation(k)].getArrivingTime()>train_sort.getArrivingTime()){
                                best_train[1][i.first.getStation(k)]=best_train[0][i.first.getStation(k)];
                                best_train[0][i.first.getStation(k)]=train_sort;
                            }else if(best_train[1].find(i.first.getStation(k))==best_train[1].end()||best_train[1][i.first.getStation(k)].getArrivingTime()>train_sort.getArrivingTime()){
                                best_train[1][i.first.getStation(k)]=train_sort;
                            }
                        }
                    }
                }
                break;
            }
        }
    }
    sjtu::pair<Train_sort,Train_sort>res={Train_sort(),Train_sort()};
    auto checkmin=[&res,is_priority_time](sjtu::pair<Train_sort,Train_sort>cur)->void
    {
        if(res.first.getTrainID()==TrainID_type()){
            res=cur;
            return ;
        }
        int timeres=res.second.getArrivingTime()-res.first.getLeavingTime();
        int timecur=cur.second.getArrivingTime()-cur.first.getLeavingTime();
        int priceres=res.first.getPrice()+res.second.getPrice();
        int pricecur=cur.first.getPrice()+cur.second.getPrice();
        if(is_priority_time){
            if(priceres!=pricecur){
                if(priceres>pricecur){
                    res=cur;
                }
                return;
            }
            if(timeres!=timecur){
                if(timeres>timecur){
                    res=cur;
                }
                return;
            }
            if(res.first.getTrainID()!=cur.first.getTrainID()){
                if(res.first.getTrainID()>cur.first.getTrainID()){
                    res=cur;
                }
                return ;
            }
            if(res.second.getTrainID()!=cur.second.getTrainID()){
                if(res.second.getTrainID()>cur.second.getTrainID()){
                    res=cur;
                }
                return ;
            }
        }else{
            if(timeres!=timecur){
                if(timeres>timecur){
                    res=cur;
                }
                return;
            }
            if(priceres!=pricecur){
                if(priceres>pricecur){
                    res=cur;
                }
                return;
            }
            if(res.first.getTrainID()!=cur.first.getTrainID()){
                if(res.first.getTrainID()>cur.first.getTrainID()){
                    res=cur;
                }
                return ;
            }
            if(res.second.getTrainID()!=cur.second.getTrainID()){
                if(res.second.getTrainID()>cur.second.getTrainID()){
                    res=cur;
                }
                return ;
            }
        }
    };
    ticketsystem.getalltrain_bystation(end_station,possible_train2);
    for(auto &i:possible_train2){
        for(int j=i.getStationNum()-1;j>=0;j--){
            if(i.getStation(j)==end_station){
                Train_sort train_sort;  
                ReleasedTrain released_train;
                sjtu::pair<int, sjtu::pair<int,int>>hint(1,sjtu::make_pair(0,1000000));
                for(int k=j-1;k>=0;k--){
                    Stationname_type transfer_station=i.getStation(k);
                    auto it=best_train[0].find(transfer_station);
                    if(it==best_train[0].end())continue;
                    if(it->second.getTrainID()==i.getTrainID()){
                        it=best_train[1].find(transfer_station);
                        if(it==best_train[1].end())
                            continue;
                    }
                    Mydate transfer_date=it->second.getArrivingTime();
                    ticketsystem.getreleasedtrain_bytrain(i,transfer_station,transfer_date,released_train);
                    calculate_ticket(sjtu::make_pair(i,released_train),k,j,train_sort);
                    checkmin(sjtu::make_pair(it->second,train_sort)) ;                   
                }
                break;
            }
        }
    }
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
    User user;
    bool res=usersystem.find_user(cur_user,user);
    if(res==false)
        return os<<"-1"<<std::endl,-1;
    Train train;
    ReleasedTrain released_train;
    res=ticketsystem.query_train(trainID,train);
    if(res==false)
        return os<<"-1"<<std::endl,-1;
    res=ticketsystem.getreleasedtrain_bytrain(train,start_station,Mydate(date_day,0),released_train);
    if(res==false)
        return os<<"-1"<<std::endl,-1;
    Train_sort train_sort;
    res=calculate_ticket(sjtu::make_pair(train,released_train),start_station,end_station,train_sort,true);
    if(res==false)
        return os<<"-1"<<std::endl,-1;
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
        os<<ticket[i].getTrainID()<<" "<<ticket[i].getStartStation()<<" "<<ticket[i].getEndStation()<<" "<<ticket[i].getLeaveTime()<<" "<<ticket[i].getArriveTime()<<" "<<ticket[i].getPrice()<<" "<<ticket[i].getSeatNum()<<std::endl;
    }
    return 0;
}
int Systhesissystem::refund_ticket(const Command &command,std::ostream &os){
    if(command.type!=command_type::refund_ticket)
        throw TrainSystemError("refund_ticket");
    UserName_type cur_username;
    int index;
    for(int i=0;i<command.auguementNum;i++){
        if(command.key[i]=='u'){
            cur_username=command.auguement[i];
        }else if(command.key[i]=='n'){
            index=std::stoi(command.auguement[i]);
        }else{
            throw TrainSystemError("refund_ticket");
        }
    }
    auto it=Userlist.find(cur_username);
    if(it==Userlist.end())
        return os<<"-1"<<std::endl,-1;
    if(index<1||index>ticketsystem.get_order_num(cur_username))
        return os<<"-1"<<std::endl,-1;
    bool res=ticketsystem.refund_ticket(cur_username,index);
    if(res==false)
        throw TrainSystemError("refund_ticket");
        // return os<<"-1"<<std::endl,-1;
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
    return os<<'0'<<std::endl,0;
}
void Systhesissystem::process()
{
    Command cmd;
    while (commandsystem.readcommand(cmd),true)
    {
        bool res;
        
        std::ostream& os(std::cout);
        os<<'['<<cmd.timestamp<<']'<<' ';
        switch (cmd.type)
        {
        case command_type::add_user:
            res=add_user(cmd,os);
            break;
        case command_type::login:
            res=login(cmd,os);
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
            res=query_ticket(cmd,os);
            break;
        case command_type::query_transfer:
            // std::ostringstream os;
            res=query_transfer(cmd,os);
            break;  
        case command_type::buy_ticket:
            res=buy_ticket(cmd,     os);
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