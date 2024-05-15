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
            return os<<"-1",-1;
        User check_user;
        bool res=usersystem.find_user(user.getUserName(),check_user);
        if(res==true)
            return os<<"-1",-1;
        if(it->second<=user.getPrivilege())
            return os<<"-1",-1;
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
        return os<<"-1",-1;
    User user;
    bool res=usersystem.find_user(cur_user,user);
    if(res==false)
        return os<<"-1",-1;
    if(user.getPassword()!=cur_password)
        return os<<"-1",-1;
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
        return os<<"-1",-1;
    User user;
    bool res=usersystem.find_user(query_user,user);
    if(res==false)
        return os<<"-1",-1;
    if(it->second<=user.getPrivilege()&&cur_user!=query_user)
        return os<<"-1",-1;
    os<<user.getUserName()<<" "<<user.getRealName()<<" "<<user.getMailAddr()<<" "<<user.getPrivilege()<<std::endl;
    return os<<'0',0;
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
        return os<<"-1",-1;
    User check_user;
    bool res=usersystem.find_user(query_user,check_user);
    if(res==false)
        return os<<"-1",-1;
    if(it->second<=check_user.getPrivilege()&&cur_user!=query_user)
        return os<<"-1",-1;
    usersystem.modify_user(query_user,user);
    os<<user.getUserName()<<" "<<user.getRealName()<<" "<<user.getMailAddr()<<" "<<user.getPrivilege()<<std::endl;
    return os<<'0',0;
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
    //     return os<<"-1",-1;
    // if(it->second<2)
    //     return os<<"-1",-1;
    bool res=ticketsystem.add_train(train);
    return res==true?(os<<'0',0):(os<<"-1",-1);
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
    //     return os<<"-1",-1;
    // if(it->second<2)
    //     return os<<"-1",-1;
    bool res=ticketsystem.delete_train(trainID);
    return res==true?(os<<'0',0):(os<<"-1",-1);
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
    return res==true?(os<<'0',0):(os<<"-1",-1);
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
        return os<<"-1",-1;
    /*
    查询成功：输出共 (<stationNum> + 1) 行。

第一行为 <trainID> <type>。

接下来 <stationNum> 行，第 i 行为 <stations[i]> <ARRIVING_TIME> -> <LEAVING_TIME> <PRICE> <SEAT>，其中 <ARRIVING_TIME> 和 <LEAVING_TIME> 为列车到达本站和离开本站的绝对时间，格式为 mm-dd hr:mi。<PRICE> 为从始发站乘坐至该站的累计票价，<SEAT> 为从该站到下一站的剩余票数。对于始发站的到达时间和终点站的出发时间，所有数字均用 x 代替；终点站的剩余票数用 x 代替。如果车辆还未 release 则认为所有票都没有被卖出去。
    */
    os<<train.getTrainID()<<" "<<train.getType()<<std::endl;
    int price=0;
    Mydate curdate(released_train.getDate(),train.getStartTime());
    for(int i=0;i<train.getStationNum();i++){
        os<<train.getStation(i)<<" ";
        if(i==0){
            os<<"xx-xx xx:xx";
        }else{
            os<<curdate;
            if(i+1!=train.getStationNum()){
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
        if(i!=0&&i+1!=train.getStationNum()){
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
    return os<<'0',0;
}

int Systhesissystem::clean(const Command &command,std::ostream &os){
    if(command.type!=command_type::clean)
        throw TrainSystemError("clean");
    ticketsystem.clear();
    usersystem.clear();
    Userlist.clear();
    return os<<'0',0;
}
int Systhesissystem::exit(const Command &command,std::ostream &os){
    if(command.type!=command_type::exit)
        throw TrainSystemError("exit");
    Userlist.clear();
    return os<<'0',0;
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