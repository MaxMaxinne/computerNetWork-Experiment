#include<iostream>
#include<queue>
#include<stdlib.h>
#include<cmath>
#include<time.h>
#include<iomanip>
#include<vector>
#include<fstream>
#include<algorithm>
using namespace std;

#define PROD_NUM 3
#define QUEUE_LEN 1000000
#define random() (double)rand()/RAND_MAX

#ifndef COMMON
#define COMMON

struct pack{
    int index;
    double comeTime;
    double serveTime;//包服务所需时间(可变)
    double weight;//包服务所需时间（const)
    double waitingTime;
    pack(int _index,double _weight,double _comeTime):index(_index),weight(_weight),serveTime(_weight),comeTime(_comeTime){}
};

struct producer{
    double Mu;
    double lamda;
    int index;
    queue<pack*> _queue;
    //int length;
    producer(){};
    producer(int _index,double _lamda,double _mu):Mu(_mu),index(_index),lamda(_lamda){
        double currentTime=0;
        for(int i=0;i<QUEUE_LEN;i++){
            if((i+1)%(QUEUE_LEN/10)==0)
                cout<<"队列"<<index<<": "<<"包"<<i+1<<" currentTime: "<<setprecision(5)<<fixed<<currentTime<<" lamda: "<<lamda<<" Mu: "<<Mu<<endl;
            //TODO 修改乘为除
            pack* p=new pack(i,-log(random())/Mu,currentTime);
            currentTime+=-log(random())/lamda;//泊松过程
            _queue.push(p);
        }
    }
    
};

void servemm3(producer*);
// void serve(producer*);
void servemm1(producer*);
void res_output(vector<double>&,char*);
void res_output_mm3(vector<double>*);
void queueLen_output(vector<double>&,char*,double);
void queueLen_output_mm3(vector<double>*,double);
#endif