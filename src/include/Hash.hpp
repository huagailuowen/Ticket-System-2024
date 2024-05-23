#ifndef HASH_HPP
#define HASH_HPP

#include "utility.hpp"
#include "Mystring.hpp"

namespace Hash {

    using hash_t = unsigned long long;

    // Forward declaration
    hash_t Hash(const int &x);
    hash_t Hash(const unsigned &x);
    
    template <int len>
    hash_t Hash(const Mystring<len> &x);

    // Hash for pair
    template <class T1, class T2>
    hash_t Hash(const sjtu::pair<T1, T2>& x) {
        return ::Hash::Hash(x.first) ^ ::Hash::Hash(x.second);
    }

    // Hash for unsigned
    hash_t Hash(const unsigned &x) {
        return x;
    }

    // Hash for int
    hash_t Hash(const int &x) {
        return static_cast<hash_t>(x);
    }

    // Hash for Mystring
    template<int len>
    hash_t Hash(const Mystring<len> &x) {
        hash_t res = 0;
        for (int i = 0; i < x.size(); i++) {
            res = res * 131 + x[i];
        }
        return res;
    }

} // namespace Hash

#endif // HASH_HPP