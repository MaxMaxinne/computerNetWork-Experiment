#include<iostream>
#include<queue>
#include<stdlib.h>
#include<cmath>
#include<time.h>
#include<iomanip>
#include<vector>
#include<fstream>
#include<algorithm>
#include<map>
using namespace std;

#define PROD_NUM 5
#define QUEUE_LEN 1000000
#define random(n) rand()%n
#define SLOT 1

#ifndef COMMON
#define COMMON

struct pack{
    int index;
    int comeTime;
    int leaveTime;
    int weight;//包服务所需时间（const)
    int waitingTime;
    pack(int _index,int _weight,int _comeTime):index(_index),weight(_weight),comeTime(_comeTime){}
};

struct producer{
    double Mu;
    double lamda;
    int index;
    int front_ptr;
    vector<pack*> _queue;
    //int length;
    producer(){};
    producer(int _index,double _lamda,double _mu):Mu(_mu),index(_index),lamda(_lamda),front_ptr(0){
        int currentTime=0;
        for(int i=0;i<QUEUE_LEN;i++){
            if((i+1)%(QUEUE_LEN/10)==0)
                cout<<"队列"<<index+1<<": "<<"包"<<i+1<<" currentTime: "<<setprecision(5)<<fixed<<currentTime<<" lamda: "<<lamda<<" Mu: "<<Mu<<endl;
            //修改除为乘
            pack* p=new pack(i,(int)ceil(-log((double)rand()/(RAND_MAX))*Mu),currentTime);
            currentTime+=(int)ceil(-log((double)rand()/(RAND_MAX))*lamda);//泊松过程
            //cout<<currentTime<<endl;
            _queue.push_back(p);
        }
    }
    bool empty(){
        return front_ptr>=QUEUE_LEN;
    }
};

void res_output(vector<int>&,char*);
void res_output_mm3(vector<int>*);
void queueLen_output(map<int,int>,char*,int);
void queueLen_output_mm3(producer*,int*);
#endif