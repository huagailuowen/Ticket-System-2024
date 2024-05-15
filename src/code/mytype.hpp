#ifndef MYTYPE
#define MYTYPE
#include "../include/Mystring.hpp"
#include "../include/fastinclude.hpp"
#include "../include/vector.hpp"
#include <ostream>
#include <string>

using std::string;
class AccountData;
class BookData;
class TrainSystemError {
  string msg;

public:
  TrainSystemError(const std::string &s) : msg(s) {}
  string what() { return msg; }
};
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
const int Month_day[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int Date_to_int(std::string date) {
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
int time_to_int(const std::string &time) {
  int hour, minute;
  sscanf(time.c_str(), "%d:%d", &hour, &minute);
  return hour * 60 + minute;
}
std::string int_to_Date(int days) {
  int year = 2024;
  int month = 1;
  int day = days;
  while (day > Month_day[month]) {
    day -= Month_day[month];
    month++;
  }
  std::string date = std::to_string(year) + "-";
  if (month < 10)
    date += "0";
  date += std::to_string(month) + "-";
  if (day < 10)
    date += "0";
  date += std::to_string(day);
  return date;
}
std::string int_to_time(int time) {
  int hour = time / 60;
  int minute = time % 60;
  std::string t;
  if (hour < 10)
    t += "0";
  t += std::to_string(hour) + ":";
  if (minute < 10)
    t += "0";
  t += std::to_string(minute);
  return t;
}
struct Mydate {
  int day, time;
  Mydate() {}
  Mydate(int d, int t) {
    day = d + t / 1440;
    time = t % 1440;
    if (time < 0) {
      time += 1440;
      day--;
    }
  }
  Mydate(const std::string &date, const std::string &time)
      : day(Date_to_int(date)), time(time_to_int(time)) {}
  Mydate operator+(const Mydate &a) {
    return Mydate(day + a.day, time + a.time);
  }
  operator std::string() { return int_to_Date(day) + " " + int_to_time(time); }
};
std::ostream &operator<<(std::ostream &os, const Mydate &a) {
  os << int_to_Date(a.day) << " " << int_to_time(a.time);
  return os;
}
template <class T>
void splittos(const std::string &str, T *res, const char &token) {
  int len = str.length();
  int cnt = 0;
  int last = 0;
  for (int i = 0; i < len; i++) {
    if (str[i] == token) {
      res[cnt++] = T(str.substr(last, i - last));
      last = i + 1;
    }
  }
  res[cnt++] = T(str.substr(last, len - last));
}
template <class T>
void splittoi(const std::string &str, T *res, const char &token,
              int isdate_time = 0)
// isdate_time=0 means int,1 means date,2 means time
{
  int len = str.length();
  int cnt = 0;
  int last = 0;
  for (int i = 0; i < len; i++) {
    if (str[i] == token) {
      if (isdate_time == 1)
        res[cnt++] = Date_to_int(str.substr(last, i - last));
      else if (isdate_time == 2)
        res[cnt++] = time_to_int(str.substr(last, i - last));
      else
        res[cnt++] = T(str.substr(last, i - last));
      last = i + 1;
    }
  }
  res[cnt++] = T(str.substr(last, len - last));
}

#endif