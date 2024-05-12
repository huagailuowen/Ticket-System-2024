#ifndef MYTYPE
#define MYTYPE
#include <set>
#include <string>
#include <vector>

#include "memory.h"
using std::string;
class AccountData;
class BookData;
enum class quiry_type {
  All /*共有*/,
  ISBN,
  name,
  author,
  keyword,
  userid,
  username,
  realISBN,
  price
};
enum class power_type { none = 0, customer = 1, crew = 3, owner = 7 };

enum class command_type {
  quit,
  exit,
  su,
  logout,
  regi,
  passwd,
  useradd,
  del,
  show,
  buy,
  select,
  modify,
  import,
  showfinance,
  log,
  reportfinance,
  reportemployee
};

template <const int len> struct Mystring {
private:
  unsigned long long myhash = 0;
  char c[len + 1];
  int l = 0;

public:
  Mystring();
  Mystring(string s);
  operator string() const;

  friend std::ostream &operator<<(std::ostream &out, const Mystring<len> &the) {
    for (int i = 0; i < the.l; i++)
      out << the.c[i];
    return out;
  }

  //???
  string tostr();
  void gethash();
  int size() const;
  char &operator[](const int &index);
  char operator[](const int &index) const;

  bool operator<(const Mystring &b) const;
  bool operator==(const Mystring &b) const;
  bool operator>(const Mystring &b) const;
  bool operator>=(const Mystring &b) const;
  bool operator<=(const Mystring &b) const;
  bool operator!=(const Mystring &b) const;
  // template <const int len>
  // friend Mystring<len> MAXMYSTRING(int L);
  friend class AccountData;
  friend class BookData;
  template <class Tkey, class Tvalue> friend class MyMemoryClass;
  string MAXSTRING() {
    string s;
    for (int i = 0; i < len; i++)
      s += char(127);
    return s;
  }
};
using MyISBN = Mystring<20>;
using Userid = Mystring<30>;
using Myauthor = Mystring<60>;
using Bookname = Mystring<60>;
using Username = Mystring<30>;
using Password = Mystring<30>;
using Keyword = Mystring<60>;
bool IsISBN(const string &s, MyISBN &);
bool Isuserid(const string &s, Userid &);
bool Isauthor(const string &s, Myauthor &);
bool Isbookname(const string &s, Bookname &);
bool Isusername(const string &s, Username &);

bool Ispassword(const string &s, Password &);

bool Iskeyword(const string &s, Keyword &, bool one = false);
bool Isprivilege(const string &s, power_type &a);
bool Isquantity(const string &s, long long &);
bool Isprice(const string &s, double &);

void Getkeywordlist(const Keyword &, std::vector<string> &);

template <const int len> Mystring<len>::Mystring() {
  c[0] = '\0';
  l = 0;
}
template <const int len> void Mystring<len>::gethash() {
  myhash = 0;
  for (int i = 0; i < l; i++)
    myhash = (myhash << 7) + myhash + c[i];
}

template <const int len> Mystring<len>::Mystring(string s) {
  for (int i = 0; i < s.size(); i++)
    c[i] = s[i];
  l = s.size();
  c[s.size()] = '\0';
  gethash();
}
template <const int len> char &Mystring<len>::operator[](const int &index) {
  return c[index];
}
template <const int len>
char Mystring<len>::operator[](const int &index) const {
  return c[index];
}

template <const int len> int Mystring<len>::size() const { return l; }
template <const int len> Mystring<len>::operator string() const {
  string s;
  for (int i = 0; i < l; i++)
    s += c[i];
  return s;
}
template <const int len> string Mystring<len>::tostr() {
  string s;
  for (int i = 0; i < l; i++)
    s += c[i];
  return s;
}
template <const int len>
bool Mystring<len>::operator<(const Mystring &b) const {
  for (int i = 0; i < std::max(l, b.l); i++) {
    if (c[i] != b.c[i])
      return c[i] < b.c[i];
  }
  return 0;
}
template <const int len>
bool Mystring<len>::operator==(const Mystring<len> &b) const {
  if (b.l != l)
    return 0;
  for (int i = 0; i < l; i++) {
    if (c[i] != b.c[i])
      return 0;
  }
  return 1;
}
template <const int len>
bool Mystring<len>::operator>(const Mystring &b) const {
  return b < (*this);
}
template <const int len>
bool Mystring<len>::operator>=(const Mystring &b) const {
  return !((*this) < b);
}
template <const int len>
bool Mystring<len>::operator<=(const Mystring &b) const {
  return !((*this) > b);
}
template <const int len>
bool Mystring<len>::operator!=(const Mystring &b) const {
  return !((*this) == b);
}

#endif
