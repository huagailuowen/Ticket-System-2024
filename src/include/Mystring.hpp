#ifndef MYSTRING_HPP
#define MYSTRING_HPP
#include<string>
template <const int len> struct Mystring {
private:
#ifdef DEBUG
public:
#endif
  unsigned long long myhash = 0;
  char c[len + 1];
  int l = 0;

public:
  Mystring();
  Mystring(std::string s);
  Mystring(const Mystring<len>& other){
    for(int i=0;i<other.l;i++)
      c[i]=other.c[i];
    l=other.l;
    myhash=other.myhash;
  }
  Mystring<len>& operator=(const Mystring<len>& other){
    if(this==&other)
      return *this;
    for(int i=0;i<other.l;i++)
      c[i]=other.c[i];
    l=other.l;
    myhash=other.myhash;
    return *this;
  }
  operator std::string() const;

  friend std::ostream &operator<<(std::ostream &out, const Mystring<len> &the) {
    for (int i = 0; i < the.l; i++)
      out << the.c[i];
    return out;
  }

  //???
  std::string tostr();
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
  // friend Mystring<len> MAXMystring(int L);
  friend class AccountData;
  friend class BookData;
  template <class Tkey, class Tvalue> friend class MyMemoryClass;
  std::string MAXstring() {
    std::string s;
    for (int i = 0; i < len; i++)
      s += char(127);
    return s;
  }
  static Mystring<len>setmin(){
    Mystring<len>tmp;
    for(int i=0;i<len;i++)
      tmp[i]=0;
    return tmp;
  }
  static Mystring<len>setmax(){
    Mystring<len>tmp;
    for(int i=0;i<len;i++)
      tmp[i]=127;
    return tmp;
  }
};
template <const int len> Mystring<len>::Mystring() {
  c[0] = '\0';
  l = 0;
}
template <const int len> void Mystring<len>::gethash() {
  myhash = 0;
  for (int i = 0; i < l; i++)
    myhash = (myhash << 7) + myhash + c[i];
}

template <const int len> Mystring<len>::Mystring(std::string s) {
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
template <const int len> Mystring<len>::operator std::string() const {
  std::string s;
  for (int i = 0; i < l; i++)
    s += c[i];
  return s;
}
template <const int len> std::string Mystring<len>::tostr() {
  std::string s;
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

#endif // Mystring_HPP