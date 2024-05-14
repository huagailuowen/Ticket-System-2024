#ifndef COMMANDSYSTEM_HPP
#define COMMANDSYSTEM_HPP
#include"easyinclude.hpp"
#include "../include/vector.hpp" // Add the missing include statement for the 'sjtu' namespace.
#include "mytype.hpp"
#include <cstdlib>
#include <istream>
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
    
    Command(const std::string &s);

};
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
    std::string timestamp;
    iss >>  timestamp;
    command.timestamp=std::stoi(timestamp.substr(1, timestamp.size() - 2)); // Read the timestamp

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
    std::string::size_type pos = 0;
    std::string timestamp;
    pos = s.find(' ');
    timestamp = s.substr(0, pos);
    this->timestamp = std::stoi(timestamp.substr(1, timestamp.size() - 2)); // Read the timestamp
    std::string cmd;
    pos = s.find(' ', pos + 1);
    cmd = s.substr(pos + 1, s.find(' ', pos + 1) - pos - 1); // Read the command type
    this->type = string_to_command_type(cmd); // Convert the command type to an enum
    std::string key;
    while (pos != std::string::npos) {
        pos = s.find(' ', pos + 1);
        if (pos == std::string::npos) {
            break;
        }
        key = s.substr(pos + 1, 1);
        if (key[0] != '-') {
            break; // Stop reading if the key is not preceded by '-'
        }
        this->key.push_back(key[0]); // Add the key to the vector
        std::string argument;
        pos = s.find(' ', pos + 1);
        if (pos == std::string::npos) {
            argument = s.substr(pos + 1);
        } else {
            argument = s.substr(pos + 1, s.find(' ', pos + 1) - pos - 1); // Read the argument
        }
        this->auguement.push_back(argument); // Add the argument to the vector
    }

    this->auguementNum = this->auguement.size(); // Set the auguementNum
}
class Commandsystem {
    sjtu::vector<Command> commandList;
public:
    Commandsystem() = default;
    void readcommand(Command& command)
    {
        std::cin>>command;
        return;
    }
    void getcommand(Command& command,const std::string &s)
    {
        command=std::move(Command(s));
        return;
    }
};


#endif // COMMANDSYSTEM_HPP