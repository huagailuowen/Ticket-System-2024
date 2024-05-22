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
  std::string what() const{ return msg; }
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
  invalid,
  inanity,
};
const int Month_day[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int Month_sum[13] = {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305,
                           335};
int Date_to_int(std::string date) {
  int year = 2024;
  int month, day;
  sscanf(date.c_str(), "%d-%d", &month, &day);
  int days = 0;
  days=Month_sum[month]+day;
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
  std::string date ;//= std::to_string(year) + "-";
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
  // int day, time;
  int date;
  Mydate() {date=0;}
  int day() const { return date / 1440; }
  int time() const { return date % 1440; }
  Mydate(int date){this->date=date;}
  Mydate(int d, int t) {
    date=d*1440+t;
  }
  Mydate(const Mydate& a):date(a.date){}
  Mydate(const std::string &date, const std::string &time)
  {
    this->date=Date_to_int(date)*1440+time_to_int(time);
  }
  Mydate& operator=(const Mydate& a) {
    date=a.date;
    return *this;
  }
  Mydate operator+(const Mydate &a)const {
    return Mydate(date + a.date);
  }
  Mydate operator-(const Mydate &a)const {
    return Mydate(date - a.date);
  }
  static Mydate max() { return Mydate(100000, 0); }
  operator std::string() const { return int_to_Date(date/1440) + " " + int_to_time(date%1440); }
  operator int () const { return date; }
};
std::ostream &operator<<(std::ostream &os, const Mydate &a) {
  os << std::string(a);
  return os;
}
template <class T>
int splittos(const std::string &str, T *res, const char &token) {
  int len = str.length();
  int cnt = 0;
  int last = 0;
  for (int i = 0; i < len; i++) {
    if (str[i] == token) {
      if(i-last>0)
        res[cnt++] = T(str.substr(last, i - last));
      last = i + 1;
    }
  }
  if(len-last>0)
    res[cnt++] = T(str.substr(last, len - last));
  return cnt;
}
// template <class T>
void splittoi(const std::string &str, int *res, const char &token,
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
        // res[cnt++] = T(str.substr(last, i - last));
        sscanf(str.substr(last, i - last).c_str(), "%d", &res[cnt++]);
      last = i + 1;
    }
  }
  // res[cnt++] = T(str.substr(last, len - last));
  if(len-last>0){
    if (isdate_time == 1)
      res[cnt++] = Date_to_int(str.substr(last, len - last));
    else if (isdate_time == 2)
      res[cnt++] = time_to_int(str.substr(last, len - last));
    else
      sscanf(str.substr(last, len - last).c_str(), "%d", &res[cnt++]);
  
  }
}
void splittoshort(const std::string &str, short *res, const char &token,
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
        // res[cnt++] = T(str.substr(last, i - last));
        sscanf(str.substr(last, i - last).c_str(), "%d", &res[cnt++]);
      last = i + 1;
    }
  }
  // res[cnt++] = T(str.substr(last, len - last));
  if(len-last>0){
    if (isdate_time == 1)
      res[cnt++] = Date_to_int(str.substr(last, len - last));
    else if (isdate_time == 2)
      res[cnt++] = time_to_int(str.substr(last, len - last));
    else
      sscanf(str.substr(last, len - last).c_str(), "%d", &res[cnt++]);
  
  }
}
void splittodate(const std::string &str, Mydate *res, const char &token,
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
        // res[cnt++] = T(str.substr(last, i - last));
        sscanf(str.substr(last, i - last).c_str(), "%d", &res[cnt++]);
      last = i + 1;
    }
  }
  // res[cnt++] = T(str.substr(last, len - last));
  if(len-last>0){
    if (isdate_time == 1)
      res[cnt++] = Date_to_int(str.substr(last, len - last));
    else if (isdate_time == 2)
      res[cnt++] = time_to_int(str.substr(last, len - last));
    else
      sscanf(str.substr(last, len - last).c_str(), "%d", &res[cnt++]);
  
  }
}
template <typename T,typename CMP>
void quickSort(sjtu::vector<T>& arr, int low, int high) {
  if (low < high) {
    int l = low, r = high;
    T key = arr[l];
    while (l < r) {
      while (l < r && !CMP()(arr[r], key)) r--;
      if (l < r) arr[l++] = arr[r];
      while (l < r && !CMP()(key, arr[l])) l++;
      if (l < r) arr[r--] = arr[l];
    }
    arr[l] = key;
    quickSort<T,CMP>(arr, low, l - 1);
    quickSort<T,CMP>(arr, l + 1, high);
  }
}

#endif
