#include "common.h"

#define TIMEPIECE 0.4
#define OLD1

double currentTime=0,workingTime=0;
#ifdef OLD1
//处理一个时间片
void serveTimePiece(producer* prod,vector<double>* res){
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
            cout<<"队列"<<prod->index+1<<": 包"<<p->index+1<<" waitingTime: "<<p->waitingTime<<" comeTime: "<<p->comeTime<<" currentTime: "<<currentTime<<" serveTime: "<<p->weight<<endl;
        res[prod->index].push_back(p->waitingTime);

        prod->_queue.pop();
        delete p;
    }
}
void servemm3(producer* prod){
    vector<double> res[PROD_NUM];
    cout<<"Start simulation.Using RR.\ntimePiece: "<<TIMEPIECE<<endl;
    int pointer=0;
    while(!prod[0]._queue.empty()||!prod[1]._queue.empty()||!prod[2]._queue.empty()){
        serveTimePiece(prod+pointer,res);
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
#endif
// 原调度方法
// void servemm1(producer* prod){ 
//     double currentTime=0;
//     double workingTime=0;
//     vector<double> res;
//     while(!prod->_queue.empty()){
//         pack* p=prod->_queue.front();
//         if(currentTime<p->comeTime)
//             currentTime=p->comeTime;
//         currentTime+=p->weight;
//         workingTime+=p->weight;
//         p->waitingTime=currentTime-p->comeTime;
//         res.push_back(p->waitingTime);
//         prod->_queue.pop();
//         delete p;
//     }
//     res_output(res,"res_mm1.dat");
//     printf("Utilization: %.2f%%\n",workingTime/currentTime*100);
// }
#ifdef OLD2
int nextComingQueue(producer* prod,double& nextArrive_t){
    int queue_p=0;
    double minCome_t=__DBL_MAX__;
    for(int i=0;i<PROD_NUM;i++){
        //TODO 处理队列为空的情况
        if(!prod[i]._queue.empty()){
            if(prod[i]._queue.front()->comeTime<minCome_t){
                queue_p=i;
                minCome_t=prod[i]._queue.front()->comeTime;
            }
        }
    }
    nextArrive_t=minCome_t;
    return queue_p;
}
//下一个得到服务的包
//TODO debug
pack* nextServingPack(queue<pack*>* q,double& nextLeave_t,int& serve_p){
    pack* servingPack=nullptr;
    if(q[0].empty()&&q[1].empty()&&q[2].empty()){
        nextLeave_t=__DBL_MAX__;
        return servingPack;
    }
    while(1){
        if(!q[(serve_p+1)%3].empty()){
            if(q[(serve_p+1)%3].front()->serveTime<TIMEPIECE){
                serve_p=(serve_p+1)%3;
                servingPack=q[serve_p].front();
                nextLeave_t=currentTime+q[serve_p].front()->weight;
                return servingPack;
            }else{
                q[(serve_p+1)%3].front()->serveTime-=TIMEPIECE;
            }
        }
        if(!q[(serve_p+2)%3].empty()){
            if(q[(serve_p+2)%3].front()->serveTime<TIMEPIECE){
                serve_p=(serve_p+2)%3;
                servingPack=q[serve_p].front();
                nextLeave_t=currentTime+q[serve_p].front()->weight;
                return servingPack;
            }else{
                q[(serve_p+2)%3].front()->serveTime-=TIMEPIECE;
            }
        }
        if(!q[serve_p].empty()){
            if(q[serve_p].front()->serveTime<TIMEPIECE){
                servingPack=q[serve_p].front();
                nextLeave_t=currentTime+q[serve_p].front()->weight;
                return servingPack;
            }else{
                q[serve_p].front()->serveTime-=TIMEPIECE;
            }
        }
    }
}
void servemm3(producer* prod){
    //double currentTime=0,workingTime=0;
    cout<<"Start simulation.Using RR.\ntimePiece: "<<TIMEPIECE<<endl;
    double nextArrive_t=0,nextLeave_t=__DBL_MAX__,remainTime=TIMEPIECE;
    vector<double> res[PROD_NUM];
    int queLen=100;
    vector<double> queueLen[3]={vector<double>(queLen,0),vector<double>(queLen,0),vector<double>(queLen,0)};
    int serve_p=0,queue_p=0;//多队列指针，指向获得当前时间片的队列与下一个有包到达的队列
    queue<pack*> q[3];
    int maxlen=0;
    pack* servingPack=nullptr;
    queue_p=nextComingQueue(prod,nextArrive_t);
    bool isExecuted=false;
    while(!prod[0]._queue.empty()||!prod[1]._queue.empty()||!prod[2]._queue.empty()||!q[0].empty()||!q[1].empty()||!q[2].empty()){
        
        if(!isExecuted&&(prod[0]._queue.size()+q[0].size())<QUEUE_LEN*0.5){
            isExecuted=true;
            for(int i=0;i<PROD_NUM;i++){
                cout<<"队列"<<i+1<<"吞吐量: "<<QUEUE_LEN-(prod[i]._queue.size()+q[i].size())<<endl;
            }
        }
        if(nextArrive_t<nextLeave_t){
            pack* nextPack=nullptr;
            if(!prod[queue_p]._queue.empty()){
                nextPack=prod[queue_p]._queue.front();
                prod[queue_p]._queue.pop();
                //更新队列长度
                for(int i=0;i<PROD_NUM;i++){
                    queueLen[i][q[i].size()]+=nextPack->comeTime-currentTime;
                }
                currentTime=nextPack->comeTime;
            }
            if(q[queue_p].empty()){
                if(servingPack){
                    q[queue_p].push(nextPack);
                    if(q[queue_p].size()>maxlen)
                        maxlen=q[queue_p].size();
                    queue_p=nextComingQueue(prod,nextArrive_t);
                }else{
                    serve_p=queue_p;
                    q[queue_p].push(nextPack);
                    if(q[queue_p].size()>maxlen)
                        maxlen=q[queue_p].size();
                    //if(nextPack->serveTime<TIMEPIECE){
                        servingPack=nextPack;
                        nextLeave_t=currentTime+servingPack->weight;
                    // }else{
                    //     nextPack->serveTime-=TIMEPIECE;
                    //     servingPack=nextServingPack(q,nextLeave_t,serve_p);
                    //}
                    queue_p=nextComingQueue(prod,nextArrive_t);
                }
            }else{
                q[queue_p].push(nextPack);
                if(q[queue_p].size()>maxlen)
                    maxlen=q[queue_p].size();
                queue_p=nextComingQueue(prod,nextArrive_t);
            }
        }else{
            if(remainTime<servingPack->serveTime){
                servingPack->serveTime-=remainTime;
                remainTime=TIMEPIECE;
                servingPack=nextServingPack(q,nextLeave_t,serve_p);
                continue;
            }
            //更新队列长度
            for(int i=0;i<PROD_NUM;i++){
                queueLen[i][q[i].size()]+=nextLeave_t-currentTime;
            }
            currentTime=nextLeave_t;
            remainTime-=servingPack->serveTime;
            servingPack->waitingTime=currentTime-servingPack->comeTime;
            if((servingPack->index+1)%(QUEUE_LEN/10)==0)
                cout<<"队列"<<serve_p+1<<": 包"<<servingPack->index+1<<" waitingTime: "<<servingPack->waitingTime<<" comeTime: "<<servingPack->comeTime<<" currentTime: "<<currentTime<<" serveTime: "<<servingPack->weight<<endl;
            workingTime+=servingPack->weight;
            res[serve_p].push_back(servingPack->waitingTime);
            q[serve_p].pop();
            
            if(q[serve_p].empty()){
                servingPack=nextServingPack(q,nextLeave_t,serve_p);
                remainTime=TIMEPIECE;
            }else{
                servingPack=q[serve_p].front();
                nextLeave_t=currentTime+servingPack->weight;
            }
        }
    }
    printf("WoringTime: %.2f\nTotalTime: %.2f\nUtilization: %.2f%%\n",workingTime,currentTime,workingTime/currentTime*100);
    cout<<"max queue Length:"<<maxlen<<endl;
    res_output_mm3(res);
    queueLen_output_mm3(queueLen,currentTime);
}
#endif
#ifdef NEW
void servemm3(producer* prod){
    cout<<"Start simulation.Using RR.\ntimePiece: "<<TIMEPIECE<<endl;
    double remainTime[3]={TIMEPIECE,TIMEPIECE,TIMEPIECE};
    vector<double> res[PROD_NUM];
    int queueLen[3]={0,0,0};
    while(!prod[0]._queue.empty()||!prod[1]._queue.empty()||!prod[2]._queue.empty()){
        //cout<<prod[1]._queue.size()<<endl;
        // for(int i=0;i<PROD_NUM;i++){
        //     while(!prod[i]._queue.empty()&&prod[i]._queue.front()->comeTime<currentTime)
        //         queueLen[i]++;
        // }
        //时间前进
        bool flag=false;
        double min_t=__DBL_MAX__;
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
        //handle working
        for(int i=0;i<PROD_NUM;i++){
            remainTime[i]+=TIMEPIECE;
            while(!prod[i]._queue.empty()&&prod[i]._queue.front()->weight<remainTime[i]){
                if(prod[i]._queue.front()->comeTime>currentTime){
                    remainTime[i]=0;
                    break;
                }
                pack* servingPack=prod[i]._queue.front();
                remainTime[i]-=servingPack->weight;
                res[i].push_back(currentTime-servingPack->comeTime);
                currentTime+=servingPack->weight;
                if((servingPack->index+1)%(QUEUE_LEN/10)==0)
                    cout<<"队列"<<i+1<<": 包"<<servingPack->index+1<<" waitingTime: "<<currentTime-servingPack->comeTime<<" comeTime: "<<servingPack->comeTime<<" currentTime: "<<currentTime<<" serveTime: "<<servingPack->weight<<endl;
                workingTime+=servingPack->weight;
                queueLen[i]--;
                prod[i]._queue.pop();
                delete servingPack;
            }
        }
    }
    printf("WorkingTime: %.2f\nTotalTime: %.2f\nUtilization: %.2f%%\n",workingTime,currentTime,workingTime/currentTime*100);
    res_output_mm3(res);
}
#endif 
void servemm1(producer* prod){
    //double currentTime=0,workingTime=0;
    queue<pack*> q;
    pack* servingPack=nullptr;
    double nextArrive_t=0,nextLeave_t=__DBL_MAX__;
    vector<double> res;
    vector<double> queLen(220,0);
    //int maxLen=0;
    while(!prod->_queue.empty()||!q.empty()){
        // if(maxLen<q.size())
        //     maxLen=q.size();
        if(nextArrive_t<nextLeave_t){
            pack* nextPack=nullptr;
            if(!prod->_queue.empty()){
                nextPack=prod->_queue.front();
                queLen[q.size()]+=nextPack->comeTime-currentTime;
                prod->_queue.pop();
                currentTime=nextPack->comeTime;
            }
            if(q.empty()){
                if(servingPack){
                    q.push(nextPack);
                    //nextArrive_t=nextPack->comeTime;
                    if(!prod->_queue.empty())
                        nextArrive_t=prod->_queue.front()->comeTime;
                    else
                        nextArrive_t=__DBL_MAX__;
                }else{
                    servingPack=nextPack;
                    //currentTime+=nextPack->weight;
                    nextLeave_t=currentTime+nextPack->weight;
                    if(!prod->_queue.empty())
                        nextArrive_t=prod->_queue.front()->comeTime;
                    else
                        nextArrive_t=__DBL_MAX__;
                }
            }else{
                q.push(nextPack);
                //nextArrive_t=nextPack->comeTime;
                if(!prod->_queue.empty())
                    nextArrive_t=prod->_queue.front()->comeTime;
                else
                    nextArrive_t=__DBL_MAX__;
            }
        }else{
            queLen[q.size()]+=nextLeave_t-currentTime;
            currentTime=nextLeave_t;
            workingTime+=servingPack->weight;
            servingPack->waitingTime=currentTime-servingPack->comeTime;
            if((servingPack->index+1)%(QUEUE_LEN/10)==0)
                cout<<"包"<<servingPack->index+1<<" waitingTime: "<<servingPack->waitingTime<<" comeTime: "<<servingPack->comeTime<<" currentTime: "<<currentTime<<" serveTime: "<<servingPack->weight<<endl;
            res.push_back(servingPack->waitingTime);
            delete servingPack;
            if(q.empty()){
                nextLeave_t=__DBL_MAX__;
                servingPack=nullptr;
            }else{
                servingPack=q.front();
                q.pop();
                nextLeave_t=currentTime+servingPack->weight;
            }
        }
    }
    res_output(res,"./res_output/res_mm1.dat");
    queueLen_output(queLen,"./res_output/len_mm1.dat",currentTime);
    printf("Utilization: %.2f%%\n",workingTime/currentTime*100);
    // cout<<maxLen<<endl;
}
