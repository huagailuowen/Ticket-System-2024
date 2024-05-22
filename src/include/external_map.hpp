#ifndef EXTERNAL_MAP_HPP
#define EXTERNAL_MAP_HPP
#include<fstream>
#include<string>
#include"map.hpp"
#include "utility.hpp"
#include"vector.hpp"
namespace sjtu {


template <class key_t,class val_t>
class external_map {
    std::string file_name;
    std::fstream file;
    public:
    sjtu::map<key_t,val_t>mp;
    external_map(std::string file_name,bool isnew=false):file_name(file_name){
        if(isnew==true){
            file.open(file_name,std::ios::out);
            file.close();
            return;
        }
        file.open(file_name,std::ios::app);
        if(file.tellp()==0){
            int n=0;
            file.write(reinterpret_cast<char*>(&n),sizeof(int));
            file.close();
            return;
        }else file.close();
        file.open(file_name,std::ios::in|std::ios::out);
        int n;
        file.read(reinterpret_cast<char*>(&n),sizeof(int));
        sjtu::vector<sjtu::pair<key_t,val_t>>tmp;
        tmp.resize(n);
        if(n>0)
            file.read(reinterpret_cast<char*>(tmp.begin_p()),sizeof(sjtu::pair<key_t,val_t>)*n);
        for(int i=0;i<n;i++){
            mp[tmp[i].first]=tmp[i].second;
        }
        file.close();
    }
    void insert(const key_t &key,const val_t &val){
        mp[key]=val;
    }
    bool search(const key_t &key,val_t &val){
        if(mp.find(key)==mp.end())return false;
        val=mp[key];
        return true;
    }
    bool modify(const key_t &key,const val_t &val){
        if(mp.find(key)==mp.end())return false;
        mp[key]=val;
        return true;
    }
    void clear(){
        mp.clear();
    }
    bool remove(const key_t &key){
        if(mp.find(key)==mp.end())return false;
        mp.erase(key);
        return true;
    }
    int size()const {
        return mp.size();
    }
    void searchall(const key_t &l,const key_t &r,sjtu::vector<val_t>&val){
        val.clear();
        for(auto it=mp.lower_bound(l);it!=mp.end()&&it->first<=r;it++){
            val.push_back(it->second);
        }
    }
    bool lower_bound(const key_t &key,val_t &val){
        auto it=mp.lower_bound(key);
        if(it==mp.end())return false;
        val=it->second;
        return true;
    }
    ~external_map(){
        file.open(file_name,std::ios::out);
        int n=mp.size(),cnt=0;
        file.write(reinterpret_cast<char*>(&n),sizeof(int));
        sjtu::vector<sjtu::pair<key_t,val_t>>tmp;
        tmp.resize(n);
        for(auto it=mp.begin();it!=mp.end();it++){
            tmp[cnt++]=*it;
        }
        file.write(reinterpret_cast<char*>(tmp.begin_p()),sizeof(sjtu::pair<key_t,val_t>)*n);
        file.close();
    }
};
}
#endif // EXTERNAL_MAP_HPP