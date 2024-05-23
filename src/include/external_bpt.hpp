#ifndef EXTERNAL_BPT_HPP
#define EXTERNAL_BPT_HPP

#include <fstream>
#include <string>
#include "../include/external_array.hpp"
#include "../include/external_map.hpp"
#include"Hash.hpp"
extern int TIME;
namespace sjtu{
template <class key_t,class val_t,bool is_hash=false>
class external_bpt {
    std::string file_name;
    public:
    external_map<key_t,int>index;
    external_map<Hash::hash_t,int>index_hash;
    external_array<val_t>data;
    
    external_bpt(std::string file_name,bool isnew=false):file_name(file_name),index(file_name+"index",isnew),data(file_name+"data",isnew),index_hash(file_name+"index_hash",isnew){

    }
    void insert(const key_t &key,const val_t &val){
        int pos=data.getnum();
        data.add_num();
        if(is_hash){
            
            index_hash.insert(Hash::Hash(key),pos);
        }
        else index.insert(key,pos);
        data.write(val,pos);
    }
    bool search(const key_t &key,val_t &val){
        int pos;
        // if(index.search(key,pos)==false)return false;
        if(is_hash){
            if(index_hash.search(Hash::Hash(key),pos)==false)return false;
        }
        else {
            if(index.search(key,pos)==false)return false;
        }
        data.read(val,pos);
        return true;
    }
    bool search(const key_t &key,val_t &val,int offset1,int offset2,int len){
        int pos;
        // if(index.search(key,pos)==false)return false;
        if(is_hash){
            if(index_hash.search(Hash::Hash(key),pos)==false)return false;
        }
        else {
            if(index.search(key,pos)==false)return false;
        }
        data.read(val,pos,offset1,offset2,len);
        return true;
    }
    

    bool modify(const key_t &key,const val_t &val){
        int pos;
        // if(index.search(key,pos)==false)return false;
        if(is_hash){
            if(index_hash.search(Hash::Hash(key),pos)==false)return false;
        }
        else {
            if(index.search(key,pos)==false)return false;
        }
        data.write(val,pos);
        return true;
    }
    bool modify(const key_t &key,const val_t &val,int offset1,int offset2,int len){
        int pos;
        // if(index.search(key,pos)==false)return false;
        if(is_hash){
            if(index_hash.search(Hash::Hash(key),pos)==false)return false;
        }
        else {
            if(index.search(key,pos)==false)return false;
        }
        data.write(val,pos,offset1,offset2,len);
        return true;
    }
    
    void clear(){
        index.clear();
        index_hash.clear();
        data.clear();
    }
    bool remove(const key_t &key){
        int pos;
        // if(index.search(key,pos)==false)return false;
        if(is_hash){
            if(index_hash.search(Hash::Hash(key),pos)==false)return false;
            index_hash.remove(Hash::Hash(key));
        }
        else {
            if(index.search(key,pos)==false)return false;
            index.remove(key);
        }
        return true;
    }
    int size()const {
        if(is_hash){
            return index_hash.size();
        }
        return index.mp.size();
    }
    void searchall(const key_t &l,const key_t &r,sjtu::vector<val_t>&val){
        if(is_hash){
            throw "not supported";
        }
        val.clear();
        for(auto it=index.mp.lower_bound(l);it!=index.mp.end()&&it->first<=r;it++){
            val_t tmp;
            data.read(tmp,it->second);
            val.push_back(tmp);
        }
    }
    bool lower_bound(const key_t &key,val_t &val){
        if(is_hash){
            throw "not supported";
        }
        auto it=index.mp.lower_bound(key);
        if(it==index.mp.end())return false;
        data.read(val,it->second);
        return true;
    }
    ~external_bpt(){}
};
}

#endif // EXTERNAL_BPT_HPP