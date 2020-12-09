#include "common.h"

#define TIMEPIECE 0.4
#define OLD1

double currentTime=0,workingTime=0;
double remainTime[3]={TIMEPIECE,TIMEPIECE,TIMEPIECE};
double finishTime[3]={0,0,0};
double throughoutput[PROD_NUM]={0,0,0};
vector<double> res[PROD_NUM];
#ifdef OLD1
//处理一个时间片
void serveTimePiece(producer* prod){
    int queue_index=prod->index;
    // if(remainTime[queue_index]<TIMEPIECE)
        remainTime[queue_index]+=TIMEPIECE;
    while(!prod->empty()&&remainTime[queue_index]>0){
        pack* p=prod->_queue[prod->front_ptr];
        //包未到达
        if(p->comeTime>currentTime)
            break;
        //剩余时间不足，赤字
        if(p->weight>remainTime[queue_index]){
            // remainTime[queue_index]+=TIMEPIECE;
            break;
        }

        p->waitingTime=currentTime-p->comeTime;
        currentTime+=p->weight;
        p->leaveTime=currentTime;
        remainTime[queue_index]-=p->weight;
        throughoutput[queue_index]+=p->weight;
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
    if(prod->empty()&&finishTime[queue_index]==0)
        finishTime[queue_index]=currentTime;
}
void servemm3(producer* prod){
    
    cout<<"Start simulation.Using RR.\ntimePiece: "<<TIMEPIECE<<endl;
    int pointer=0;
    bool isExecuted=false;
    while((prod[0].front_ptr+prod[1].front_ptr+prod[2].front_ptr)<=QUEUE_LEN){
        serveTimePiece(prod+pointer);
        // if(!isExecuted&&(prod[0].front_ptr>QUEUE_LEN*0.5)){
        //     isExecuted=true;
        //     for(int i=0;i<PROD_NUM;i++){
        //         cout<<"队列"<<i+1<<"吞吐量: "<<throughoutput[i]<<endl;
        //     }
        // }
        pointer=(pointer+1)%3;
        double min_t=__DBL_MAX__;
        int min_p=0;
        bool flag=false;
        //使currentTime等于totalTime
        if(prod[0].empty()||prod[1].empty()||prod[2].empty())
            break;
        for(int i=0;i<PROD_NUM;i++){
            double local_t=(prod[i].empty())?__DBL_MAX__:prod[i]._queue[prod[i].front_ptr]->comeTime;
            if(local_t<=currentTime){
                flag=true;
                break;
            }
            if(local_t<min_t){
                min_t=local_t;
                // min_p=i;
            }
        }
        if(!flag){
            currentTime=min_t;
            // pointer=min_p;
        }
            
    }
    for(int i=0;i<PROD_NUM;i++){
        cout<<"队列"<<i+1<<"吞吐量: "<<throughoutput[i]<<endl;
        cout<<remainTime[i]<<endl;
        finishTime[i]=currentTime;
    }
    // cout<<((prod[0].front_ptr+prod[1].front_ptr+prod[2].front_ptr)<=10000000)<<endl;
    printf("WoringTime: %.2f\nTotalTime: %.2f\nUtilization: %.2f%%\n",workingTime,currentTime,workingTime/currentTime*100);
    res_output_mm3(res);
    
    queueLen_output_mm3(prod,finishTime);
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
void servemm1(producer* p){
    
    vector<double> res_mm1;
    printf("234");
    while(!p->empty()){
        pack* head=p->_queue[p->front_ptr];
        if(currentTime<head->comeTime){
            currentTime=head->comeTime;
        }
        head->leaveTime=currentTime;
        head->waitingTime=currentTime-head->comeTime;
        currentTime+=head->weight;
        workingTime+=head->weight;
        res_mm1.push_back(head->waitingTime);
        p->front_ptr++;
    }
    printf("占用率:%.5f%%\n",workingTime/currentTime);
    res_output(res_mm1,"./res_output/res_mm1.dat");

    map<int,double,greater<int>> m;
    int p1=0,p2=0;
    double nextLeave_t=p->_queue[0]->leaveTime,nextArr_t=p->_queue[0]->comeTime,cur_t=0;
    while(p1<QUEUE_LEN||p2<QUEUE_LEN){
        if(nextArr_t<nextLeave_t){
            auto iter=m.find(p2-p1);
            if(iter!=m.end()){
                iter->second+=nextArr_t-cur_t;
            }else{
                m[p2-p1]=nextArr_t-cur_t;
            }
            cur_t=nextArr_t;
            p2++;
            if(p2<QUEUE_LEN)
                nextArr_t=p->_queue[p2]->comeTime;
            else
                nextArr_t=__DBL_MAX__;
        }else{
            auto iter=m.find(p2-p1);
            if(iter!=m.end()){
                iter->second+=nextLeave_t-cur_t;
            }else{
                m[p2-p1]=nextLeave_t-cur_t;
            }
            cur_t=nextLeave_t;
            p1++;
            if(p1<QUEUE_LEN)
                nextLeave_t=p->_queue[p1]->leaveTime;
            else
                nextLeave_t=__DBL_MAX__;
        }
    }
    queueLen_output(m,"./res_output/len_mm1.dat",currentTime);
    double t=0.0;
    // for(int j=0;j<queueLen[i].size();j++)
    //     t+=queueLen[i][j]*j;
    for(auto iter=m.begin();iter!=m.end();iter++){
        t+=iter->first*iter->second;
    }
    printf("队列平均长度: %.5f\n",t/currentTime);
}
