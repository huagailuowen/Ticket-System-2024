#ifndef COMMANDSYSTEM_HPP
#define COMMANDSYSTEM_HPP
#include "Systhesissystem.hpp"
#include"easyinclude.hpp"
#include "../include/vector.hpp" // Add the missing include statement for the 'sjtu' namespace.
#include "mytype.hpp"
#include <cstdlib>
#include <istream>
#include <ostream>
#include <random>
#include <string>
#include<iostream>
// #include<sstream>

// Your code goes here
struct Command {
    int timestamp;
  command_type type;
  int auguementNum;
  sjtu::vector<char> key;
    sjtu::vector<std::string> auguement;
    Command() = default;
    Command(int timestamp, command_type type, int auguementNum, const sjtu::vector<char> &key, const sjtu::vector<std::string> &auguement)
        : timestamp(timestamp), type(type), auguementNum(auguementNum), key(key), auguement(auguement) {}
    Command(const Command &command) = default;
    Command &operator=(const Command &command) = default;
    Command &operator=(Command &&command){
        if(this==&command)
            return *this;
        timestamp=command.timestamp;
        type=command.type;
        auguementNum=command.auguementNum;
        key=std::move(command.key);
        auguement=std::move(command.auguement);
        return *this;
    }
    void inputstring(const std::string &s);
    Command(const std::string &s);
    friend std::ostream& operator<<(std::ostream &os, const Command &command);

};
std::ostream& operator<<(std::ostream &os, const Command &command) {
    os << "[" << command.timestamp << "] ";
    switch (command.type) {
        case command_type::add_user:
            os << "add_user";
            break;
        case command_type::login:
            os << "login";
            break;
        case command_type::logout:
            os << "logout";
            break;
        case command_type::query_profile:
            os << "query_profile";
            break;
        case command_type::modify_profile:
            os << "modify_profile";
            break;
        case command_type::add_train:
            os << "add_train";
            break;
        case command_type::release_train:
            os << "release_train";
            break;
        case command_type::query_train:
            os << "query_train";
            break;
        case command_type::delete_train:
            os << "delete_train";
            break;
        case command_type::query_ticket:
            os << "query_ticket";
            break;
        case command_type::query_transfer:
            os << "query_transfer";
            break;
        case command_type::buy_ticket:
            os << "buy_ticket";
            break;
        case command_type::query_order:
            os << "query_order";
            break;
        case command_type::refund_ticket:
            os << "refund_ticket";
            break;
        case command_type::clean:
            os << "clean";
            break;
        case command_type::exit:
            os << "exit";
            break;
        default:
            os << "invalid";
            break;
    }
    for (int i = 0; i < command.auguementNum; ++i) {
        os << " -" << command.key[i] << " " << command.auguement[i];
    }
    return os;
}
command_type string_to_command_type(const std::string &s) {
    if (s == "add_user") {
        return command_type::add_user;
    } else if (s == "login") {
        return command_type::login;
    } else if (s == "logout") {
        return command_type::logout;
    } else if (s == "query_profile") {
        return command_type::query_profile;
    } else if (s == "modify_profile") {
        return command_type::modify_profile;
    } else if (s == "add_train") {
        return command_type::add_train;
    } else if (s == "release_train") {
        return command_type::release_train;
    } else if (s == "query_train") {
        return command_type::query_train;
    } else if (s == "delete_train") {
        return command_type::delete_train;
    } else if (s == "query_ticket") {
        return command_type::query_ticket;
    } else if (s == "query_transfer") {
        return command_type::query_transfer;
    } else if (s == "buy_ticket") {
        return command_type::buy_ticket;
    } else if (s == "query_order") {
        return command_type::query_order;
    } else if (s == "refund_ticket") {
        return command_type::refund_ticket;
    } else if (s == "clean") {
        return command_type::clean;
    } else if (s == "exit") {
        return command_type::exit;
    } else {
        return command_type::invalid;
    }
}
std::istream &operator>>(std::istream &iss, Command &command) {
    throw TrainSystemError("not use it");
    std::string timestamp;
    iss >>  timestamp;
    command.timestamp=std::stoi(timestamp.substr(1, timestamp.size() - 2)); // Read the timestamp
    command.auguement.clear();
    std::string cmd;
    iss >> cmd; // Read the command type
    command.type = string_to_command_type(cmd); // Convert the command type to an enum

    std::string key;
    while (iss >> key) {
        if (!key.size()||key[0] != '-') {
            break; // Stop reading if the key is not preceded by '-'
        }
        command.key.push_back(key[1]); // Add the key to the vector

        std::string argument;
        iss >> argument; // Read the argument
        command.auguement.push_back(argument); // Add the argument to the vector
    }

    command.auguementNum = command.auguement.size(); // Set the auguementNum

    return iss;
}
Command::Command(const std::string &s) {
    std::string str[30];
    int size=splittos(s, str, ' ');
    if(size==0){
        this->type=command_type::inanity;
        return ;
    }
    if(size<2)
        throw TrainSystemError("Invalid command:too few arguments");
    this->timestamp = std::stoi(str[0].substr(1, str[0].size() - 2)); // Read the timestamp
    this->type = string_to_command_type(str[1]); // Convert the command type to an enum
    for (int i = 2; i < size; ++i) {
        if (str[i].size() <2 || str[i][0] != '-') {
            throw TrainSystemError("Invalid command:invalid key");
        }
        this->key.push_back(str[i][1]); // Add the key to the vector
        ++i;
        if(i>=size)
            throw TrainSystemError("Invalid command:odd number of arguments");
        this->auguement.push_back(str[i]); // Add the argument to the vector
    }
    this->auguementNum = this->auguement.size(); // Set the auguementNum
}
void Command::inputstring(const std::string &s) {
    this->auguementNum=0;
    this->key.clear();
    this->auguement.clear();
    
    std::string str[30];
    int size=splittos(s, str, ' ');
    if(size==0){
        this->type=command_type::inanity;
        return ;
    }
    if(size<2)
        throw TrainSystemError("Invalid command:too few arguments");
    this->timestamp = std::stoi(str[0].substr(1, str[0].size() - 2)); // Read the timestamp
    this->type = string_to_command_type(str[1]); // Convert the command type to an enum
    for (int i = 2; i < size; ++i) {
        if (str[i].size() <2 || str[i][0] != '-') {
            throw TrainSystemError("Invalid command:invalid key");
        }
        this->key.push_back(str[i][1]); // Add the key to the vector
        ++i;
        if(i>=size)
            throw TrainSystemError("Invalid command:odd number of arguments");
        this->auguement.push_back(str[i]); // Add the argument to the vector
    }
    this->auguementNum = this->auguement.size(); // Set the auguementNum
}
class Commandsystem {
    sjtu::vector<Command> commandList;
public:
    Commandsystem() = default;
    void readcommand(Command& command)
    {
        // char s[1000];
        string s;
        std::getline(std::cin,s);    
        command.inputstring(s);
        return;
    }
    void getcommand(Command& command,const std::string &s)
    {
        command=std::move(Command(s));
        return;
    }
};
    


#endif // COMMANDSYSTEM_HPP