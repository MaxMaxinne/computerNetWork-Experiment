#include "common.h"

#define TIMEPIECE 0.4
#define OLD1

double currentTime=0,workingTime=0;
double remainTime[3]={TIMEPIECE,TIMEPIECE,TIMEPIECE};
#ifdef OLD1
//处理一个时间片
void serveTimePiece(producer* prod,vector<double>* res){
    int queue_index=prod->index;
    while(!prod->empty()&&remainTime[queue_index]>0){
        pack* p=prod->_queue[prod->front_ptr];
        //包未到达
        if(p->comeTime>currentTime)
            break;
        //剩余时间不足，赤字
        if(p->weight>remainTime[queue_index]){
            remainTime[queue_index]+=TIMEPIECE;
            break;
        }

        p->waitingTime=currentTime-p->comeTime;
        currentTime+=p->weight;
        p->leaveTime=currentTime;
        remainTime[queue_index]-=p->weight;
        workingTime+=p->weight;
        
        //输出结果
        if((p->index+1)%(QUEUE_LEN/10)==0)
            cout<<"队列"<<queue_index+1<<": 包"<<p->index+1<<" waitingTime: "<<p->waitingTime<<" comeTime: "<<p->comeTime<<" currentTime: "<<currentTime<<" serveTime: "<<p->weight<<endl;
        res[queue_index].push_back(p->waitingTime);

        //prod->_queue.pop();
        prod->front_ptr++;
        //cout<<prod->front_ptr<<endl;
        //delete p;
    }
}
void servemm3(producer* prod){
    vector<double> res[PROD_NUM];
    cout<<"Start simulation.Using RR.\ntimePiece: "<<TIMEPIECE<<endl;
    int pointer=0;
    bool isExecuted=false;
    while(!prod[0].empty()||!prod[1].empty()||!prod[2].empty()){
        serveTimePiece(prod+pointer,res);
        if(!isExecuted&&(prod[0].front_ptr>QUEUE_LEN*0.5)){
            isExecuted=true;
            for(int i=0;i<PROD_NUM;i++){
                cout<<"队列"<<i+1<<"吞吐量: "<<prod[i].front_ptr<<endl;
            }
        }
        pointer=(pointer+1)%3;
        double min_t=__DBL_MAX__;
        bool flag=false;
        //使currentTime等于totalTime
        if(prod[0].empty()&&prod[1].empty()&&prod[2].empty())
            break;
        for(int i=0;i<PROD_NUM;i++){
            double local_t=(prod[i].empty())?__DBL_MAX__:prod[i]._queue[prod[i].front_ptr]->comeTime;
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
    queueLen_output_mm3(prod,workingTime);
}
#endif

// void servemm1(producer* prod){
//     //double currentTime=0,workingTime=0;
//     queue<pack*> q;
//     pack* servingPack=nullptr;
//     double nextArrive_t=0,nextLeave_t=__DBL_MAX__;
//     vector<double> res;
//     vector<double> queLen(220,0);
//     //int maxLen=0;
//     while(!prod->_queue.empty()||!q.empty()){
//         // if(maxLen<q.size())
//         //     maxLen=q.size();
//         if(nextArrive_t<nextLeave_t){
//             pack* nextPack=nullptr;
//             if(!prod->_queue.empty()){
//                 nextPack=prod->_queue.front();
//                 queLen[q.size()]+=nextPack->comeTime-currentTime;
//                 prod->_queue.pop();
//                 currentTime=nextPack->comeTime;
//             }
//             if(q.empty()){
//                 if(servingPack){
//                     q.push(nextPack);
//                     //nextArrive_t=nextPack->comeTime;
//                     if(!prod->_queue.empty())
//                         nextArrive_t=prod->_queue.front()->comeTime;
//                     else
//                         nextArrive_t=__DBL_MAX__;
//                 }else{
//                     servingPack=nextPack;
//                     //currentTime+=nextPack->weight;
//                     nextLeave_t=currentTime+nextPack->weight;
//                     if(!prod->_queue.empty())
//                         nextArrive_t=prod->_queue.front()->comeTime;
//                     else
//                         nextArrive_t=__DBL_MAX__;
//                 }
//             }else{
//                 q.push(nextPack);
//                 //nextArrive_t=nextPack->comeTime;
//                 if(!prod->_queue.empty())
//                     nextArrive_t=prod->_queue.front()->comeTime;
//                 else
//                     nextArrive_t=__DBL_MAX__;
//             }
//         }else{
//             queLen[q.size()]+=nextLeave_t-currentTime;
//             currentTime=nextLeave_t;
//             workingTime+=servingPack->weight;
//             servingPack->waitingTime=currentTime-servingPack->comeTime;
//             if((servingPack->index+1)%(QUEUE_LEN/10)==0)
//                 cout<<"包"<<servingPack->index+1<<" waitingTime: "<<servingPack->waitingTime<<" comeTime: "<<servingPack->comeTime<<" currentTime: "<<currentTime<<" serveTime: "<<servingPack->weight<<endl;
//             res.push_back(servingPack->waitingTime);
//             delete servingPack;
//             if(q.empty()){
//                 nextLeave_t=__DBL_MAX__;
//                 servingPack=nullptr;
//             }else{
//                 servingPack=q.front();
//                 q.pop();
//                 nextLeave_t=currentTime+servingPack->weight;
//             }
//         }
//     }
//     res_output(res,"./res_output/res_mm1.dat");
//     queueLen_output(queLen,"./res_output/len_mm1.dat",currentTime);
//     printf("Utilization: %.2f%%\n",workingTime/currentTime*100);
//     // cout<<maxLen<<endl;
// }
