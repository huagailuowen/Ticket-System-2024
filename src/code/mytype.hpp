#ifndef MYTYPE
#define MYTYPE
#include "../include/fastinclude.hpp"
#include "../include/Mystring.hpp"

#include<string>
using std::string;
class AccountData;
class BookData;
#include "../include/Mystring.hpp" // Include the header file that defines the Mystring template class.

enum class quiry_type {
  All /*共有*/,
  User_type,
  Password_type,
  Name_type,
  MailAddr_type,
  TrainID_type,
  Stationname_type,
  Train_type,
};

using UserName_type = Mystring<20>;
using Password_type = Mystring<30>;
using RealName_type = Mystring<20>;
using MailAddr_type = Mystring<30>;
using TrainID_type = Mystring<20>;
using Stationname_type = Mystring<30>;
enum class command_type {
  add_user,
  login,
  logout,
  query_profile,
  modify_profile,
  add_train,
  delete_train,
  release_train,
  query_train,
  query_ticket,
  query_transfer,
  buy_ticket,
  query_order,
  refund_ticket,
  clean,
  exit,
  invalid
};
const int Month_day [13] = {0,31,29,31,30,31,30,31,31,30,31,30,31};
int Date_to_int(std::string date)
{
  int year = 2024;
  int month, day;
  sscanf(date.c_str(), "%d-%d", &month, &day);
  int days = 0;
  for (int i = 1; i < month; i++) {
    days += Month_day[i];
  }
  days += day;
  return days;
}
std::string int_to_Date(int days)
{
  int year = 2024;
  int month = 1;
  int day = days;
  while (day > Month_day[month]) {
    day -= Month_day[month];
    month++;
  }
  std::string date = std::to_string(month) + "-" + std::to_string(day);
  return date;
}

#endif
