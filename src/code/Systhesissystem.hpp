#ifndef SYSTHESSISSYSTEM_HPP
#define SYSTHESSISSYSTEM_HPP
#include <utility> // Include the <utility> header file

#include "Commandsystem.hpp"
#include "Ticketsystem.hpp"
#include "Usersystem.hpp"
#include "Trainsystem.hpp"
class Systhesissystem{
public:
    Commandsystem commandsystem;
    Ticketsystem ticketsystem;
    Usersystem usersystem;
    Trainssystem trainssystem;
    Systhesissystem(){
        commandsystem = Commandsystem();
        ticketsystem = Ticketsystem();
        usersystem = Usersystem();
        trainssystem = Trainssystem();
    }
    void run(){
        commandsystem.run();
    }
};



#endif