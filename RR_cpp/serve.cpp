#include "common.h"
#include<vector>
#include<fstream>
#include<algorithm>
#define TIMEPIECE 7

double currentTime=0;
vector<double> res[PROD_NUM];
void res_output();
//处理一个时间片
void serveTimePiece(producer* prod){
    double remainTime=TIMEPIECE;
    while(!prod->_queue.empty()&&remainTime>0){
        pack* p=prod->_queue.front();
        //包未到达
        if(p->comeTime>currentTime)
            break;
        //剩余时间不足，赤字
        if(p->serveTime>remainTime){
            p->serveTime-=remainTime;
            //cout<<"队列"<<prod->index<<": 包"<<p->index<<"出现赤字"<<endl;
            break;
        }

        p->waitingTime=currentTime-p->comeTime;
        currentTime+=p->weight;
        remainTime-=p->serveTime;

        //输出结果
        if((p->index+1)%(QUEUE_LEN/10)==0)
            cout<<"队列"<<prod->index<<": 包"<<p->index+1<<" waitingTime: "<<p->waitingTime<<" comeTime: "<<p->comeTime<<" currentTime: "<<currentTime<<" serveTime: "<<p->weight<<endl;
        res[prod->index].push_back(p->waitingTime);
        
        prod->_queue.pop();
        delete p;
    }
}
void serve(producer* prod){
    cout<<"Start simulation.Using RR.\ntimePiece: "<<TIMEPIECE<<endl;
    int pointer=0;
    while(!prod[0]._queue.empty()||!prod[1]._queue.empty()||!prod[2]._queue.empty()){
        serveTimePiece(prod+pointer);
        pointer=(pointer+1)%3;
        double min_t=__DBL_MAX__;
        bool flag=false;
        for(int i=0;i<PROD_NUM;i++){
            double local_t=(prod[i]._queue.empty())?__DBL_MAX__:prod[i]._queue.front()->comeTime;
            if(local_t<=currentTime){
                flag=true;
                break;
            }
            if(local_t<min_t)
                min_t=local_t;
        }
        if(!flag)
            currentTime=min_t;
    }
    res_output();
}
//输出结果
void res_output(){
    char filename[20];
    for(int i=0;i<PROD_NUM;i++){
        sort(res[i].begin(),res[i].end());

        int len=res[i].size();
        int count=0;//区间计数
        double range=res[i][len-1]/100,min=0,max=range;//区间范围
        cout<<"区间范围 "<<range<<endl;

        sprintf(filename,"res%d.dat",i+1);
        ofstream out(filename);
        if(!out.is_open()){
            cout<<"文件打开失败"<<endl;
            return;
        }

        for(int j=0;j<len;j++){
            if(res[i][j]>=min&&res[i][j]<=max){
                count++;
            }else{
                out<<(min+max)/2<<"    "<<(double)count/len<<endl;
                min+=range;
                max+=range;
                count=0;
            }
        }
        out.close();
    }
}
