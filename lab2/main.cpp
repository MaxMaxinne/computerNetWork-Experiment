#include "common.h"
double getRetryTime(int n){
    return random()*0.02;
    
}
int min(double* arr){
    int minp=0;
    double mint=__DBL_MAX__;
    for(int i=0;i<PROD_NUM;i++){
        if(arr[i]<mint){
            minp=i;
            mint=arr[i];
        }
    }
    return minp;
}
void run(producer* p){
    //当前时间
    double cur_t=0.0;
    //五个站下一个请求发包时间
    double send_t[PROD_NUM]={__DBL_MAX__,__DBL_MAX__,__DBL_MAX__,__DBL_MAX__,__DBL_MAX__};
    for(int i=0;i<PROD_NUM;i++){
        send_t[i]=p[i]._queue[0]->comeTime;
    }
    //包退出占用信道的时间
    double end_t=__DBL_MAX__;
    //信道忙碌标志
    bool busy=false;
    int totalCrashPack=0;
    double trans_t=0.0;
    //等待时间
    vector<double> res[PROD_NUM];
    double totalTime[PROD_NUM];
    //指数回退
    int backoff[PROD_NUM]={0,0,0,0,0};
    while(!p[0].empty()||!p[1].empty()||!p[2].empty()||!p[3].empty()||!p[4].empty()){
        int send_ptr=min(send_t);
        if(!busy){
            cur_t=send_t[send_ptr];
            //判断是否碰撞
            int crashNum=0;
            for(int i=0;i<PROD_NUM;i++){
                if(i!=send_ptr&&send_t[send_ptr]==send_t[i]){
                    ++crashNum;
                    send_t[i]+=getRetryTime(backoff[send_ptr]++);
                }
            }
            if(crashNum){
                send_t[send_ptr]+=getRetryTime(backoff[send_ptr]++);
                totalCrashPack+=crashNum;
                continue;
            }

            pack* next_send=p[send_ptr]._queue[p[send_ptr].front_ptr];
            next_send->leaveTime=cur_t;
            next_send->waitingTime=next_send->leaveTime-next_send->comeTime;
            res[send_ptr].push_back(next_send->waitingTime);
            trans_t+=next_send->weight;
            end_t=cur_t+next_send->weight;
            backoff[send_ptr]=0;
            busy=true;
            if((next_send->index+1)%(QUEUE_LEN/10)==0)
                cout<<"站"<<p[send_ptr].index+1<<": 包"<<next_send->index+1<<" waitingTime: "<<next_send->waitingTime<<" comeTime: "<<next_send->comeTime<<" currentTime: "<<cur_t<<" transTime: "<<next_send->weight<<endl;
            ++p[send_ptr].front_ptr;
            if(p[send_ptr].empty()){
                send_t[send_ptr]=__DBL_MAX__;
                totalTime[send_ptr]=cur_t;
            }else{
                double nextCome_t=p[send_ptr]._queue[p[send_ptr].front_ptr]->comeTime;
                send_t[send_ptr]=(nextCome_t>cur_t)?nextCome_t:cur_t;
            }
        }else{
            if(end_t<=send_t[send_ptr]){
                busy=false;
                end_t=__DBL_MAX__;
                cur_t=end_t;
            }else{
                cur_t=send_t[send_ptr];
                send_t[send_ptr]+=getRetryTime(backoff[send_ptr]++);
            }
        }
    }
    printf("TotalTime:%.2f TransTime:%.2f Utilization:%.2f%% crashRate:%.2f%%\n",cur_t,trans_t,trans_t/cur_t*100,(double)totalCrashPack/(3*PROD_NUM)*100);
    res_output_mm3(res);
    queueLen_output_mm3(p,totalTime);
}
int main(int argc,char** argv){
    producer p[PROD_NUM];
    double lamda[PROD_NUM]={atof(argv[1]),atof(argv[2]),atof(argv[3]),atof(argv[4]),atof(argv[5])};
    double Mu[PROD_NUM]={atof(argv[6]),atof(argv[7]),atof(argv[8]),atof(argv[9]),atof(argv[10])};
    for(int i=0;i<PROD_NUM;i++)
        p[i]=producer(i,lamda[i],Mu[i]);
    run(p);
    return 0;
}