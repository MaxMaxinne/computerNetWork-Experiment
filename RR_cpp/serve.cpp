#include "common.h"

#define TIMEPIECE 7

double currentTime=0;
double workingTime=0;
vector<double> res[PROD_NUM];
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
        workingTime+=p->weight;
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
        //使currentTime等于totalTime
        if(prod[0]._queue.empty()&&prod[1]._queue.empty()&&prod[2]._queue.empty())
            break;
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
    printf("WoringTime: %.2f\nTotalTime: %.2f\nUtilization: %.2f%%\n",workingTime,currentTime,workingTime/currentTime*100);
    res_output_mm3(res);
}
void servemm1(producer* prod){ 
    double currentTime=0;
    double workingTime=0;
    while(!prod->_queue.empty()){
        pack* p=prod->_queue.front();
        if(currentTime<p->comeTime)
            currentTime=p->comeTime;
        currentTime+=p->weight;
        workingTime+=p->weight;
        p->waitingTime=currentTime-p->comeTime;
        prod->_queue.pop();
        delete p;
    }
    printf("Utilization: %.2f%%\n",workingTime/currentTime*100);
}

