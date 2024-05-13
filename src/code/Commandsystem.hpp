#ifndef COMMANDSYSTEM_HPP
#define COMMANDSYSTEM_HPP
#include"easyinclude.hpp"
#include "../include/vector.hpp" // Add the missing include statement for the 'sjtu' namespace.
#include "mytype.hpp"
#include <cstdlib>
#include <istream>
// Your code goes here
struct Command {
    int timestamp;
  command_type type;
  int parameterNum;
  sjtu::vector<char> key;
    sjtu::vector<std::string> parameter;
    Command() = default;
    Command(int timestamp, command_type type, int parameterNum, const sjtu::vector<char> &key, const sjtu::vector<std::string> &parameter)
        : timestamp(timestamp), type(type), parameterNum(parameterNum), key(key), parameter(parameter) {}

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
        command.parameter.push_back(argument); // Add the argument to the vector
    }

    command.parameterNum = command.parameter.size(); // Set the parameterNum

    return iss;
}




#endif // COMMANDSYSTEM_HPP