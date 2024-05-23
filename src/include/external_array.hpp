#ifndef EXTERNAL_ARRAY_HPP
#define EXTERNAL_ARRAY_HPP
// #include <cassert>
#include<fstream>
#include<string>
#include <cmath>
#include <iostream>
template <class T>
class external_array {
    using size_t = std::size_t;
    std::fstream file;
    std::string filename; 
    const size_t incrsize=4096;
    const size_t MAXSIZE=1<<30;
    struct config{
        size_t filesize;
        size_t num;
        size_t las;
    }cfg;
    
    public:
    void add_num(){
        cfg.num++;
        cfg.las++;
    }
    void sub_num(){
        cfg.num--;
        // cfg.las--;
    }
    // int& getnum(){
    //     return cfg.num;
    // }  
    int getnum()const{
        return cfg.num;
    } 
    // int &getlas(){
    //     return cfg.las;
    // }
    int getlas()const{
        return cfg.las;
    }
    void clear(){
        file.open(filename,std::ios::out);
        if(!file.is_open()){
            throw "open file failed";
        }
        cfg.filesize=sizeof(config);
        cfg.num=0;
        cfg.las=0;
        file.write(reinterpret_cast<const char*>(&cfg),sizeof(config));
        file.close();
        return;
    }
    external_array(const external_array &a) = delete;
    external_array &operator=(const external_array &a) = delete;
    external_array(const std::string &filename,bool is_new):filename(filename){
        if(is_new){
            file.open(filename,std::ios::out);
            if(!file.is_open()){
                throw "open file failed";
            }
            cfg.filesize=sizeof(config);
            cfg.num=0;
            cfg.las=0;
            file.write(reinterpret_cast<const char*>(&cfg),sizeof(config));
            file.close();
            return;
        }
        file.open(filename,std::ios::app);
        if(file.tellp()==0){
            cfg.filesize=sizeof(config);
            cfg.num=0;
            cfg.las=0;
            file.write(reinterpret_cast<const char*>(&cfg),sizeof(config));
            file.close();
            return;
        }else file.close();
        file.open(filename,std::ios::in|std::ios::out);
        file.read(reinterpret_cast<char*>(&cfg),sizeof(config));
        file.close();
        return;
    }
    ~external_array(){
        file.open(filename,std::ios::in|std::ios::out);
        // std::cerr<<cfg.filesize<<std::endl;
        file.write(reinterpret_cast<const char*>(&cfg),sizeof(config));
        file.close();
    }
    void write(const T &a,int pos){
        file.open(filename,std::ios::in|std::ios::out);
        size_t repos=sizeof(config)+pos*sizeof(T);
        if(repos>MAXSIZE){
            throw "out of range";
        }
        

        if(repos+sizeof(T)>=cfg.filesize){
            file.seekp(0,std::ios::end);
            cfg.filesize=file.tellp();
            char buffer[incrsize];
            while(repos+sizeof(T)>=cfg.filesize){
                file.write(buffer,incrsize);
                cfg.filesize+=incrsize;
            }
            // file.seekp(0);
            // file.write(reinterpret_cast<const char*>(&cfg.filesize),sizeof(size_t));
        }
        file.seekp(repos);
        file.write(reinterpret_cast<const char*>(&a),sizeof(T));
        file.close();
    }
    void write(const T &a,int pos,int offset1,int offset2,int len){
        file.open(filename,std::ios::in|std::ios::out);
        if(pos>MAXSIZE){
            throw "out of range";
        }
        size_t repos=sizeof(config)+pos*sizeof(T)+offset1;
        if(repos+len>=cfg.filesize){
            file.seekp(0,std::ios::end);
            cfg.filesize=file.tellp();
            // assert(file.tellp()==cfg.filesize);
            char buffer[incrsize];
            while(repos+len>=cfg.filesize){
                file.write(buffer,incrsize);
                cfg.filesize+=incrsize;
            }
            // file.seekp(0);
            // file.write(reinterpret_cast<const char*>(&cfg.filesize),sizeof(size_t));
        }
        file.seekp(repos);
        file.write((reinterpret_cast<const char*>(&a))+offset2,len);
        file.close();
    }
    void read(T &a,int pos){
        file.open(filename,std::ios::in|std::ios::out);
        size_t repos=sizeof(config)+pos*sizeof(T);
        if(repos>MAXSIZE){
            throw "out of range";
        }
        // std::cerr<<file.tellp()<<"||"<<std::endl;
        if(repos+sizeof(T)>=cfg.filesize){
            file.seekp(0,std::ios::end);
            cfg.filesize=file.tellp();
            // assert(file.tellp()==cfg.filesize);
            char buffer[incrsize];
            while(repos+sizeof(T)>=cfg.filesize){
                file.write(buffer,incrsize);
                cfg.filesize+=incrsize;
            }
            // file.seekp(0);
            // file.write(reinterpret_cast<const char*>(&cfg.filesize),sizeof(size_t));
        }
        file.seekg(repos);
        file.read(reinterpret_cast<char*>(&a),sizeof(T));
        file.close();
    }
    void read(T &a,int pos,int offset1,int offset2,int len){
        file.open(filename,std::ios::in|std::ios::out);
        int repos=sizeof(config)+pos*sizeof(T)+offset1;
        if(repos>MAXSIZE){
            throw "out of range";
        }
        if(repos+len>=cfg.filesize){
            file.seekp(0,std::ios::end);
            cfg.filesize=file.tellp();
            char buffer[incrsize];
            while(repos+len>=cfg.filesize){
                file.write(buffer,incrsize);
                cfg.filesize+=incrsize;
            }
        }
        file.seekg(repos);
        file.read((reinterpret_cast<char*>(&a))+offset2,len);
        file.close();
    }
    
};

#endif // EXTERNAL_ARRAY_HPP