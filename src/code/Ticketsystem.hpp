#ifndef TICKETSYSTEM_HPP
#define TICKETSYSTEM_HPP
#include"../include/B_plustree.hpp"
// #include"easyinclude.hpp"
#include"Trainsystem.hpp"
#include "mytype.hpp"
#include <utility>
#include <ctime>
#include <iostream>
extern int TIME;
class Ticket{
    UserName_type user_name;
    TrainID_type train_id;
    //just the rank of station
    Stationname_type startstation;
    Stationname_type endstation;
    int startstation_date;
    Mydate leavetime;
    Mydate arrivetime;
    int timestamp;
    int order_num;
    int price;
    int seatnum;
public:
    enum class TicketType{
        success,pending,refunded
    };
private:
    TicketType ticket_type;
public:
    Ticket()=default;
    Ticket(const UserName_type &user_name,const TrainID_type &train_id,const Stationname_type &startstation,const Stationname_type &endstation,const Mydate &leavetime,const Mydate &arrivetime,const int &price,const int &seatnum,const TicketType &ticket_type,const int &startstation_date,const int &timestamp,const int &ord_num):user_name(user_name),train_id(train_id),startstation(startstation),endstation(endstation),leavetime(leavetime),arrivetime(arrivetime),price(price),seatnum(seatnum),ticket_type(ticket_type),startstation_date(startstation_date),timestamp(timestamp),order_num(ord_num){}
    Ticket(const Ticket& other) = default;
    Ticket& operator=(const Ticket& other) = default;
    const UserName_type &getUserName() const{
        return user_name;
    }
    const TrainID_type &getTrainID() const{
        return train_id;
    }
    const Stationname_type &getStartStation() const{
        return startstation;
    }
    const Stationname_type &getEndStation() const{
        return endstation;
    }
    const Mydate &getLeaveTime() const{
        return leavetime;
    }
    const Mydate &getArriveTime() const{
        return arrivetime;
    }
    const int &getPrice() const{
        return price;
    }
    const int &getSeatNum() const{
        return seatnum;
    }
    const TicketType &getTicketType() const{
        return ticket_type;
    }
    const int &getStartStationDate() const{
        return startstation_date;
    }
    const int &getTimestamp() const{
        return timestamp;
    }
    const int &getOrderNum() const{
        return order_num;
    }
    void setUserName(const UserName_type &user_name){
        this->user_name=user_name;
    }
    void setTrainID(const TrainID_type &train_id){
        this->train_id=train_id;
    }
    void setStartStation(const Stationname_type &startstation){
        this->startstation=startstation;
    }
    void setEndStation(const Stationname_type &endstation){
        this->endstation=endstation;
    }
    void setLeaveTime(const Mydate &leavetime){
        this->leavetime=leavetime;
    }
    void setArriveTime(const Mydate &arrivetime){
        this->arrivetime=arrivetime;
    }
    void setPrice(const int &price){
        this->price=price;
    }
    void setSeatNum(const int &seatnum){
        this->seatnum=seatnum;
    }
    void setTicketType(const TicketType &ticket_type){
        this->ticket_type=ticket_type;
    }
    void setStartStationDate(const int &startstation_date){
        this->startstation_date=startstation_date;
    }
    void setTimestamp(const int &timestamp){
        this->timestamp=timestamp;
    }
    void setOrderNum(const int &order_num){
        this->order_num=order_num;
    }
    friend std::ostream& operator <<(std::ostream &os,const Ticket &ticket);
};
/*格式为 [<STATUS>] <trainID> <FROM> <LEAVING_TIME> -> <TO> <ARRIVING_TIME> <PRICE> <NUM>，其中 <NUM> 为购票数量， <STATUS> 表示该订单的状态，可能的值为：success（购票已成功）、pending（位于候补购票队列中）和 refunded（已经退票）。*/
std::ostream& operator <<(std::ostream &os,const Ticket &ticket){
    os<<ticket.timestamp<<' '<<ticket.order_num<<' '<<ticket.user_name<<" ";
    os<<"[";
    if(ticket.ticket_type==Ticket::TicketType::success) os<<"success";
    else if(ticket.ticket_type==Ticket::TicketType::pending) os<<"pending";
    else if(ticket.ticket_type==Ticket::TicketType::refunded) os<<"refunded";
    os<<"] "<<ticket.train_id<<" "<<ticket.startstation<<" "<<ticket.leavetime<<" -> "<<ticket.endstation<<" "<<ticket.arrivetime<<" "<<ticket.price<<" "<<ticket.seatnum;
    return os;
}
namespace sjtu{
class release_bpt{
    public:
    const Mydate BEGIN_DATE;

    struct release_block{
        ReleasedTrain trains[92];
        ReleasedTrain& operator[](const int &i){
            return trains[i];
        }
        
    };
    
    release_bpt()=delete;
    release_bpt(const release_bpt &release_bpt)=delete;
    release_bpt &operator=(const release_bpt &release_bpt)=delete;
    sjtu::external_bpt<TrainID_type,release_block,true> data;
    public:
    release_block buffer;
    release_bpt(std::string name,bool isnew=false):data(name,isnew),BEGIN_DATE(153,0){}
    void insert(const Train& train,int begin_date,int end_date){
        int begin_index=begin_date-BEGIN_DATE.day();
        int end_index=end_date-BEGIN_DATE.day();
        if(begin_index<0||end_index>91||begin_date>end_date) throw TrainSystemError("Invalid date");
        release_block block;
        ReleasedTrain released_train;
        // released_train.setTrainID(train.getTrainID());
        released_train.setStationNum(train.getStationNum());
        for(int j=0;j<(int)train.getStationNum()-1;j++){
            released_train.setSeat(j,train.getSeatNum());
        }
        for(int i=begin_index;i<=end_index;i++){
            released_train.setDate(i+BEGIN_DATE.day());
            block[i]=released_train;
        }
        data.insert(train.getTrainID(),block);
    }
    bool search(const sjtu::pair<TrainID_type,int> &key,ReleasedTrain &released_train){
        int index=key.second-BEGIN_DATE.day();
        if(index<0||index>91) return false;
        // bool res=data.search(key.first,buffer);
        // std::cerr<<"----------------------\n";
        // std::cerr<<(&buffer)<<std::endl;
        buffer[index].setDate(0);
        bool res=data.search(key.first,buffer,index*sizeof(ReleasedTrain),index*sizeof(ReleasedTrain),sizeof(ReleasedTrain));
        if(res==false) return false;
        // if(buffer[index].getTrainID()!=key.first) return false;
        if(buffer[index].getDate()!=key.second) return false;
        if(buffer[index].getDate()!=key.second) return false;
        released_train=buffer[index];
        
        return true;
    }
    bool modify(const sjtu::pair<TrainID_type,int> &key,const ReleasedTrain &released_train){
        int index=key.second-BEGIN_DATE.day();
        if(key.second!=released_train.getDate()) throw TrainSystemError("Invalid date");
        if(index<0||index>91) return false;
        buffer[index]=released_train;
        return data.modify(key.first,buffer,index*sizeof(ReleasedTrain),index*sizeof(ReleasedTrain),sizeof(ReleasedTrain));
    }
    void clear(){
        data.clear();
    }
    // bool lower_bound(const sjtu::pair<TrainID_type,int> &key,ReleasedTrain &released_train){
    //     int index=key.second-BEGIN_DATE.day();
    //     bool res=data.search(key.first,buffer,index*sizeof(ReleasedTrain),0,sizeof(ReleasedTrain));
    //     if(res==false) return false;
    //     released_train=buffer[index];
    //     return true;
    // }

};
}
class Ticketsystem{
private:
#ifdef DEBUG
public:
#endif
    // sjtu::BPlusTree<sjtu::pair<TrainID_type,int >, ReleasedTrain,200,8>released_train_info;
    sjtu::release_bpt released_train_info;
    sjtu::BPlusTree<sjtu::pair<UserName_type,int>,Ticket,200,40> UserTicket;
    // sjtu::external_bpt<sjtu::pair<UserName_type,int>,Ticket>UserTicket;

    //用户可以在同一时间下单吗？？？
    //int 代表时间
    sjtu::BPlusTree<sjtu::pair<sjtu::pair<TrainID_type,int>,sjtu::pair<UserName_type,int>>, Ticket,80,40>ticket_queue;
    // sjtu::external_bpt<sjtu::pair<sjtu::pair<TrainID_type,int>,sjtu::pair<UserName_type,int>>, Ticket>ticket_queue;
    // sjtu::BPlusTree<UserName_type,int,80,400>order_num;
    sjtu::external_bpt<UserName_type,int,true>order_num;
    Trainsystem trainsystem;
public:
    Ticketsystem()=delete;
    Ticketsystem(const Ticketsystem &ticketsystem)=delete;
    Ticketsystem &operator=(const Ticketsystem &ticketsystem)=delete;
    Ticketsystem(std::string name,bool isnew=false):released_train_info(name+"released_train_info",isnew),UserTicket(name+"UserTicket",isnew),ticket_queue(name+"ticket_queue",isnew),trainsystem(name+"trainsystem",isnew),  order_num(name+"order_num",isnew){}
    void add_user(const UserName_type &username){
        order_num.insert(username,0);
    }
    void delete_user(const UserName_type &username){
        order_num.remove(username);
    }
    int get_order_num(const UserName_type &username){
        int num;
        bool res=order_num.search(username,num);
        if(res==false) throw TrainSystemError("No such user in order");
        return num;
    }
    bool modify_order_num(const UserName_type &username,const int &num){
        return order_num.modify(username,num);
    }

    bool add_train(const Train &train){
        return trainsystem.add_train(train);
    }
    bool delete_train(const TrainID_type &trainID){
        return trainsystem.delete_train(trainID);
    }
    bool release_train(const TrainID_type &trainID){
        bool res=trainsystem.release_train(trainID);
        if(res==false) return false;
        Train train;
        trainsystem.find_train(trainID,train,1);
        // ReleasedTrain releasedTrain_tmp;
        // releasedTrain_tmp.setTrainID(trainID);
        // releasedTrain_tmp.setStationNum(train.getStationNum());
        // Mydate curdate;
        // for(int i=0;i<train.getStationNum()-1;i++){
        //     //there must minus one because the last station is the terminal station
        //     releasedTrain_tmp.setSeat(i,train.getSeatNum());
        // }
        // for(int i=train.getSaleDate(0);i<=train.getSaleDate(1);i++){
        //     //端点？？？？
        //     releasedTrain_tmp.setDate(i);
        //     released_train_info.insert(sjtu::make_pair(trainID,i),releasedTrain_tmp);
        // }

        released_train_info.insert(train,train.getSaleDate(0),train.getSaleDate(1));
        //i do not detect if ir
        return true;
    }
    bool query_train(const TrainID_type &trainID,const int &date,Train& train,ReleasedTrain  &released_train) {
        bool res=trainsystem.find_train(trainID,train,1);
        if(res==false){
            res=trainsystem.find_train(trainID,train,2);
            if(res==false) return false;
            if(date<train.getSaleDate(0)||date>train.getSaleDate(1)) return false;
            // released_train.setTrainID(trainID);
            released_train.setStationNum(train.getStationNum());
            for(int i=0;i<(int)train.getStationNum()-1;i++){
                //there must minus one because the last station is the terminal station
                released_train.setSeat(i,train.getSeatNum());
            }
            released_train.setDate(date);
            return res;
        }
        res=released_train_info.search(sjtu::make_pair(trainID,date),released_train);
        return res;
    }
    bool query_train(const TrainID_type &trainID,Train& train) {
        return trainsystem.find_train(trainID,train,1);
    }
    void clear()
    {
        trainsystem.clear();
        released_train_info.clear();
        UserTicket.clear();
        ticket_queue.clear();
        order_num.clear();
    }
    void getallsmalltrain_bystation(const Stationname_type &station,sjtu::vector<Smalltrain> &trains){
        trainsystem.getallsmalltrain_bystation(station,trains);
    }
    void getalltrain_bystation(const Stationname_type &station,sjtu::vector<Train> &trains){
        sjtu::vector<Smalltrain> smalltrains;
        getallsmalltrain_bystation(station,smalltrains);
        trains.resize(smalltrains.size());
        for(int i=0;i<(int)smalltrains.size();i++){
            query_train(smalltrains[i].getTrainID(),trains[i]);
        }
    }
    
    //根据寻址连续性，这种方式实际上是比较快的，因为选中的trainid也是升序排列的？
    //但是还有更快的方法？，就是在station中直接放releasedtrain，这样就不用再次查询了
    //这两种方法哪个更快？？
    //我先写第一种吧
    bool getreleasedtrain_bysmalltrain(const Smalltrain& train,const Stationname_type&station,const Mydate &date,ReleasedTrain& released_train,bool havetobethatday=true){
        //这一出有关一些Transfer的事项好没有搞清楚，以后还得再返工
        //had fixed
        //这里有一个问题，就是如果这个车次在这一天没有开，那么就会出现问题
        int station_index=-1;
        // for(int j=0;j<train.getStationNum();j++){
        //     if(station==train.getStation(j)){
        //         station_index=j;
        //         break;
        //     }
        // }
        station_index=train.getStationindex();
        // if(station_index==-1||station_index+1==train.getStationindex()) return false;
        //不走终点站
        if(havetobethatday==true){
            //不是换车的情况，date.time()==0
            if(date.time()!=0)throw   TrainSystemError("Invalid date,not transfering");
            //这里不是arrivetime，而是leavetime吧？？
            Mydate tmp=train.getleavetime();
            int takedays=tmp.day();
            if(date.day()-takedays<train.getSaleDate(0)||date.day()-takedays>train.getSaleDate(1)) return false;
            return released_train_info.search(sjtu::make_pair(train.getTrainID(),date.day()-takedays),released_train);
        }else{
            Mydate curstartdate=date-(train.getleavetime()-train.getStartTime());
            //始发时间>=curstartdate
            int daydate=curstartdate.day();
            if(curstartdate.time()>train.getStartTime()) daydate++;
            // if(TIME==379506&&train.getTrainID()==TrainID_type("aparadoxappearsth")
            // &&station==Stationname_type("北京市")) {
            //     std::cerr<<":::::::::::::::::::::::::::::::::::\n";
            //     std::cerr<<int_to_Date(daydate)<<std::endl;
            //     std::cerr<<int_to_Date(train.getSaleDate(0))<<" "<<int_to_Date(train.getSaleDate(1))<<std::endl;
            // }
            
            if(train.getSaleDate(1)<daydate)return false;
            if(train.getSaleDate(0)>daydate)
                daydate=train.getSaleDate(0);
            bool res=released_train_info.search(sjtu::make_pair(train.getTrainID(),daydate),released_train);
            if(res==false) throw TrainSystemError("Impossible Invalid train");
            // if(released_train.getTrainID()!=train.getTrainID()) throw TrainSystemError("Impossible Invalid train");
            // if(TIME==379506&&train.getTrainID()==TrainID_type("aparadoxappearsth")
            // &&station==Stationname_type("北京市")) {
            //     std::cerr<<":::::::::::::::::::::::::::::::::::\n";
            //     std::cerr<<int_to_Date(daydate)<<std::endl;
            //     std::cerr<<int_to_Date(train.getSaleDate(0))<<" "<<int_to_Date(train.getSaleDate(1))<<std::endl;
            // }
            //找不到了
            return true;
        }
    
    }
    void getallsmalltrain_bystation_time(const Stationname_type &station,const Mydate &date,sjtu::vector<sjtu::pair<Smalltrain,ReleasedTrain>> &trains,bool havetobethatday=true,bool is_debug=false){
        sjtu::vector<Smalltrain> possible_train;
        trains.clear();
        getallsmalltrain_bystation(station,possible_train);
        ReleasedTrain released_train;
        for(auto &i:possible_train){
            //&加速
            //可以不用get函数调用来加速
            if(getreleasedtrain_bysmalltrain(i,station,date,released_train,havetobethatday)==false) 
                continue;
            trains.push_back(sjtu::make_pair(i,released_train));
            // if(is_debug&&TIME==379506&&trains.back().first.getTrainID()==TrainID_type("aparadoxappearsth")
            // &&station==Stationname_type("北京市")&&date==Mydate(Date_to_int("07-27"),time_to_int("17:42"))){
            //     std::cerr<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
            //     std::cerr<<trains.back().first.getTrainID()<<std::endl;
            //     std::cerr<<trains.back().second.getTrainID()<<std::endl;
            // }
            // if(is_debug){
            //     // std::cerr<<"#############################\n";
            //     std::cerr<<trains.back().first.getTrainID()<<std::endl;
            //     // std::cerr<<trains.back().second.getTrainID()<<std::endl;
            // }
            
        }
        // if(is_debug){
        //     std::cerr<<trains.size()<<std::endl;
        //     std::cerr<<"#############################\n";
        // }
    }
    void getallTrain_bystation_time(const Stationname_type &station,const Mydate &date,sjtu::vector<sjtu::pair<Train,ReleasedTrain>> &trains,bool havetobethatday=true){
        sjtu::vector<sjtu::pair<Smalltrain,ReleasedTrain>> smalltrains;
        getallsmalltrain_bystation_time(station,date,smalltrains,havetobethatday);
        trains.resize(smalltrains.size());
        for(int i=0;i<(int)smalltrains.size();i++){
            query_train(smalltrains[i].first.getTrainID(),trains[i].first);
            getreleasedtrain_bysmalltrain(smalltrains[i].first,station,date,trains[i].second,havetobethatday);
        }
    }
    bool buy_ticket(const TrainID_type& trainID,const Stationname_type& startstation,const Stationname_type& endstation,const Train&train,ReleasedTrain released_train,const UserName_type& user_name,const int price,const int& num,const int& timestamp){
        int start_station_num=-1;
        int end_station_num=-1;
        
        // int price = 0;
        for(int i=0;i<train.getStationNum();i++){
            if(train.getStation(i)==startstation) start_station_num=i;
            if(train.getStation(i)==endstation) {end_station_num=i;break;}
            if(i+1==train.getStationNum()) throw TrainSystemError("buyti_No such station");
            if(start_station_num!=-1){
                if((released_train.getSeat(i)-=num)<0)throw TrainSystemError("buyti_No enough seat");
                // price+=train.getPrice(i);
                
            }
        }
        if(start_station_num==-1||end_station_num==-1) throw TrainSystemError("buyti_No such station");
        int ord_num;
        modify_order_num(user_name,ord_num=get_order_num(user_name)+1);
        released_train_info.modify(sjtu::make_pair(trainID,released_train.getDate()),released_train);
        UserTicket.insert(sjtu::make_pair(user_name,ord_num),Ticket(user_name,trainID,startstation,endstation,Mydate(released_train.getDate(),train.getleavetime(start_station_num)),Mydate(released_train.getDate(),train.getarrivetime(end_station_num)),price,num,Ticket::TicketType::success,released_train.getDate(),timestamp,ord_num));
        return true;
    }
    bool add_queue( const TrainID_type& trainID,const Stationname_type& startstation,const Stationname_type& endstation,const Train&train,ReleasedTrain released_train,const UserName_type& user_name,const int &price,const int& num,const int& timestamp){
        int start_station_num=-1;
        int end_station_num=-1;
        // int price = 0;
        for(int i=0;i<train.getStationNum();i++){
            if(train.getStation(i)==startstation) start_station_num=i;
            if(train.getStation(i)==endstation) {end_station_num=i;break;}
            if(i+1==train.getStationNum()) throw TrainSystemError("addq_No such station");
        }
        if(start_station_num==-1||end_station_num==-1) throw TrainSystemError("add_qNo such station");
        // Ticket ticket(  user_name,trainID,startstation,endstation,
        // train.getleavetime(start_station_num)+Mydate(released_train.getDate(),0),
        // train.getarrivetime(end_station_num)+Mydate(released_train.getDate(),0)
        // ,price*num,num,Ticket::TicketType::pending,released_train.getDate(),timestamp,order_num);
        int ord_num;
        Ticket  ticket(user_name,trainID,startstation,endstation,Mydate(released_train.getDate(),train.getleavetime(start_station_num)),Mydate(released_train.getDate(),train.getarrivetime(end_station_num)),price,num,Ticket::TicketType::pending,released_train.getDate(),timestamp,ord_num=get_order_num(user_name)+1);
        ticket_queue.insert(    sjtu::make_pair(sjtu::make_pair(trainID,timestamp),sjtu::make_pair(user_name,ord_num=get_order_num(user_name)+1)),ticket);
        bool res=modify_order_num(user_name,ord_num);
        if(res==false) throw TrainSystemError("addq_No such user");
        UserTicket.insert(sjtu::make_pair(user_name,ord_num),ticket);
        return true;
    }
    void query_order(const UserName_type& user_name,sjtu::vector<Ticket> &tickets){
        tickets.clear();
        UserTicket.searchall(sjtu::make_pair(user_name,0),sjtu::make_pair(user_name,INT_MAX),tickets);
    }
    bool try_buy_ticket(const UserName_type  user_name,const int &index,const Ticket &ticket){
        // Ticket ticket;
        // UserTicket.search(sjtu::make_pair(user_name,index),ticket);
        if(ticket.getTicketType()!=Ticket::TicketType::pending) throw TrainSystemError("Invalid ticket");
        Train train;
        ReleasedTrain released_train;
        query_train(ticket.getTrainID(),ticket.getStartStationDate(),train,released_train);
        int start_station_num=-1;
        int end_station_num=-1;
        int curseatnum=100000;
        for(int i=0;i<train.getStationNum();i++){
            if(train.getStation(i)==ticket.getStartStation()) start_station_num=i;
            if(train.getStation(i)==ticket.getEndStation()) {end_station_num=i;break;}
            if(start_station_num!=-1)curseatnum=std::min(curseatnum,released_train.getSeat(i));
        }
        if(start_station_num==-1||end_station_num==-1) throw TrainSystemError("trybuy_No such station");
        // std::cerr<<"AHAHHAHAHAHAHA"<<std::endl;
        // std::cerr<<"!!!"<<curseatnum<<" "<<ticket.getSeatNum()<<std::endl;
        if(curseatnum<ticket.getSeatNum()) return false;
        // std::cerr<<"-------------------------------------------\n";
        // std::cerr<<ticket<<std::endl;

        for(int i=start_station_num;i<end_station_num;i++){
            released_train.setSeat(i,released_train.getSeat(i)-ticket.getSeatNum());
        }
        released_train_info.modify(sjtu::make_pair(ticket.getTrainID(),ticket.getStartStationDate()),released_train);
        Ticket newticket(ticket);
        newticket.setTicketType(Ticket::TicketType::success);
        // UserName_type user_name=ticket.getUserName();
        // int index=ticket.getOrderNum();
        // sjtu::pair<UserName_type,int> user_order=sjtu::make_pair(user_name,index);
        UserTicket.modify(sjtu::make_pair(user_name,index),newticket);
        //这里ticket_queue 也删除
        ticket_queue.remove(sjtu::make_pair(sjtu::make_pair(ticket.getTrainID(),ticket.getTimestamp()),sjtu::make_pair(ticket.getUserName(),index)));
        return true;
    }
    bool refund_ticket(const UserName_type& user_name,const int &num_by_last){
        #ifdef DEBUG
        if(num_by_last<=0||num_by_last>get_order_num(user_name)) throw TrainSystemError("Invalid num");
        #endif
        
        Ticket ticket;
        int ord_num=get_order_num(user_name);
        UserTicket.search(sjtu::make_pair(user_name,ord_num-num_by_last+1),ticket);
        if(ticket.getTicketType()==Ticket::TicketType::refunded) return false;
        TrainID_type trainID=ticket.getTrainID();
        bool is_extra_seat=false;
        if(ticket.getTicketType()==Ticket::TicketType::success){
            Train train;
            ReleasedTrain released_train;
            query_train(trainID,ticket.getStartStationDate(),train,released_train);
            int start_station_num=-1;
            int end_station_num=-1;
            for(int i=0;i<train.getStationNum();i++){
                if(train.getStation(i)==ticket.getStartStation()) start_station_num=i;
                if(train.getStation(i)==ticket.getEndStation()) {end_station_num=i;break;}
            }
            if(start_station_num==-1||end_station_num==-1) throw TrainSystemError("refund_No such station");
            for(int i=start_station_num;i<end_station_num;i++){
                released_train.setSeat(i,released_train.getSeat(i)+ticket.getSeatNum());
            }
            bool res=released_train_info.modify(sjtu::make_pair(trainID, ticket.getStartStationDate()),released_train);
            if(!res){
                throw   TrainSystemError("refund_No such train");
            }
            is_extra_seat=true;
            
        }else{
            ticket.setTicketType(Ticket::TicketType::refunded);
            bool res=ticket_queue.remove(sjtu::make_pair(sjtu::make_pair(trainID,ticket.getTimestamp()),sjtu::make_pair(user_name,ord_num-num_by_last+1)));
            if(!res) throw TrainSystemError("refund_No such ticket");
        }
        ticket.setTicketType(Ticket::TicketType::refunded);

        bool res=UserTicket.modify(sjtu::make_pair(user_name,ord_num-num_by_last+1),ticket);
        if(!res) throw TrainSystemError("refund_No such ticket");
        if(is_extra_seat==false)return true;
        sjtu::vector<Ticket>    tickets;
        ticket_queue.searchall(sjtu::make_pair(sjtu::make_pair(trainID,0),  sjtu::make_pair(UserName_type::setmin(),0)),
        sjtu::make_pair(sjtu::make_pair(trainID,INT_MAX),  sjtu::make_pair(UserName_type::setmax(),INT_MAX)),tickets);
        // sjtu::vector<sjtu::pair<UserName_type,int>>delete_tickets;
        for(auto &i:tickets){

            
            if(i.getTicketType()!=Ticket::TicketType::pending) throw TrainSystemError("Invalid ticket");
            try_buy_ticket(i.getUserName(),i.getOrderNum(),i);
        }
        return true;
    }

};
#endif