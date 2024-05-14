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
    Trainsystem trainsystem;
    sjtu::map<UserName_type, int>Userlist;
public:
    Systhesissystem() = delete;
    Systhesissystem(const Systhesissystem &systhesissystem) = delete;
    Systhesissystem &operator=(const Systhesissystem &systhesissystem) = delete;
    Systhesissystem(const std::string &name,bool isnew=false):commandsystem(),ticketsystem(name+"ticketsystem",isnew),usersystem(name+"usersystem",isnew),trainsystem(name+"trainsystem",isnew){
        Userlist.clear();
    }
    int add_user(const Command &command);
    int login(const Command &command);
    int logout(const Command &command);
    int query_profile(const Command &command);
    int modify_profile(const Command &command);
    int add_train(const Command &command);
    int delete_train(const Command &command);
    int release_train(const Command &command);
    int query_train(const Command &command,std::ostream &os);
    int query_ticket(const Command &command,std::ostream &os);
    int query_transfer(const Command &command,std::ostream &os);
    int buy_ticket(const Command &command);//return 0 represent queue
    int query_order(const Command &command,std::ostream &os);
    int refund_ticket(const Command &command);
    int clean();
    int exit();
    bool process();

};
int Systhesissystem::add_user(const Command &command){
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
        }
    }
    if(usersystem.user_num()==0){
        user.setPrivilege(10);
    }
    else{
        //how to ignore the -c -g
        auto it=Userlist.find(cur_user);
        if(it==Userlist.end())
            return -1;
        User check_user;
        bool res=usersystem.find_user(user.getUserName(),check_user);
        if(res==true)
            return -1;
        if(it->second<=user.getPrivilege())
            return -1;
    }
    usersystem.add_user(user);
    return 0;
}
int Systhesissystem::login(const Command &command){
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
        }
    }
    if(Userlist.find(cur_user)!=Userlist.end())
        return -1;
    User user;
    bool res=usersystem.find_user(cur_user,user);
    if(res==false)
        return -1;
    if(user.getPassword()!=cur_password)
        return -1;
    Userlist[cur_user]=user.getPrivilege();
    return 0;
}
int Systhesissystem::logout(const Command &command){
    if(command.type!=command_type::logout)
        throw TrainSystemError("logout");
    UserName_type cur_user;
    for(int i=0;i<command.auguementNum;i++){
        if(command.key[i]=='u'){
            cur_user=command.auguement[i];
        }
    }
    if(Userlist.find(cur_user)==Userlist.end())
        return -1;
    Userlist.erase(cur_user);
    return 0;
}
bool Systhesissystem::process()
{
    Command cmd;
    while (commandsystem.readcommand(cmd),true)
    {
        bool res;
        std::ostream& os(std::cout);
        switch (cmd.type)
        {
        case command_type::add_user:
            res=add_user(cmd);
            break;
        case command_type::login:
            res=login(cmd);
            break;  
        case command_type::query_profile:
            res=query_profile(cmd);
            break;  
        case command_type::modify_profile:
            res=modify_profile(cmd);
            break;
        case command_type::add_train:
            res=add_train(cmd);
            break;  
        case command_type::delete_train:
            res=delete_train(cmd);
            break;  
        case command_type::release_train:
            res=release_train(cmd);
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
            res=buy_ticket(cmd);
            break;  
        case command_type::query_order:
            // std::ostringstream os;
            res=query_order(cmd,os);
            break;
        case command_type::refund_ticket:
            res=refund_ticket(cmd);
            break;  
        case command_type::clean:
            res=clean();
            break;  
        case command_type::exit:
            res=exit();
            if(res==0)
                return false;
            break;  
        case command_type::invalid:
            throw TrainSystemError("invalid command");
            break;
        default:
            break;  

        }

    }
}



#endif