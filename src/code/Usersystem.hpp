#ifndef USERSYSTEM_HPP
#define USERSYSTEM_HPP
#include "easyinclude.hpp"
#include "mytype.hpp"
class User {
private:
#ifdef DEBUG
public:
#endif
  UserName_type username;
  Password_type password;
  RealName_type realName;
  MailAddr_type mailAddr;
  int privilege;

public:
  User() = default;
  User(const UserName_type &username, const Password_type &password, const RealName_type &realName, const MailAddr_type &mailAddr, int privilege)
      : username(username), password(password), realName(realName), mailAddr(mailAddr), privilege(privilege) {}
  UserName_type getUserName() const { return username; }
  Password_type getPassword() const { return password; }
  RealName_type getRealName() const { return realName; }
  MailAddr_type getMailAddr() const { return mailAddr; }
  int getPrivilege() const { return privilege; }
  void setUserName(const UserName_type &username) { this->username = username; }
  void setPassword(const Password_type &password) { this->password = password; }
  void setRealName(const RealName_type &realName) { this->realName = realName; }
  void setMailAddr(const MailAddr_type &mailAddr) { this->mailAddr = mailAddr; }
  void setPrivilege(int privilege) { this->privilege = privilege; }
  bool operator==(const User &rhs) const { return username == rhs.username; }
  bool operator!=(const User &rhs) const { return !(rhs == *this); }
  bool operator<(const User &rhs) const { return username < rhs.username; }
  bool operator>(const User &rhs) const { return rhs < *this; }
  bool operator<=(const User &rhs) const { return !(rhs < *this); }
  bool operator>=(const User &rhs) const { return !(*this < rhs); }  
};

class Usersystem {};
// Your code goes here

#endif // USERSYSTEM_HPP