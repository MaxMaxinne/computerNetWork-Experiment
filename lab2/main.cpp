#include "common.h"

#define ACK_DELAY 0
#define BASIC_RANGE 32
int min(int* arr){
    int minp=0;
    int mint=INT32_MAX;
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
    int cur_t=0;
    //五个站下一个请求时间(SLOT)
    int key_t[PROD_NUM]={INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX,INT32_MAX};
    for(int i=0;i<PROD_NUM;i++){
        key_t[i]=p[i]._queue[0]->comeTime;
    }
    //包退出占用信道的时间
    int end_t=INT32_MAX;
    //信道忙碌标志
    bool busy=false;
    int totalCollisionPack=0;
    int trans_t=0;
    //等待时间
    vector<int> res[PROD_NUM];
    int totalTime[PROD_NUM];
    //指数回退
    int backoffCount[PROD_NUM]={0,0,0,0,0};
    int backoffRange[PROD_NUM]={BASIC_RANGE,BASIC_RANGE,BASIC_RANGE,BASIC_RANGE,BASIC_RANGE};
    for(int i=0;i<PROD_NUM;i++){
        backoffCount[i]=rand()%backoffRange[i];
    }
    while(!p[0].empty()||!p[1].empty()||!p[2].empty()||!p[3].empty()||!p[4].empty()){
        //指向下一个事件发生的站
        int key_ptr=min(key_t);
        //cout<<key_ptr<<endl;
        if(!busy){
            cur_t=key_t[key_ptr];
            if(backoffCount[key_ptr]>0){
                key_t[key_ptr]+=SLOT;
                backoffCount[key_ptr]--;
                continue;
            }
            //判断是否碰撞
            int collisionNum=0;
            for(int i=0;i<PROD_NUM;i++){
                if(i!=key_ptr&&key_t[key_ptr]==key_t[i]&&backoffCount[i]==0){
                    ++collisionNum;
                    backoffRange[i]=backoffRange[i]*2;
                    backoffCount[i]=rand()%backoffRange[i];
                    if(backoffCount[i]>0){
                        key_t[i]+=SLOT;
                        --backoffCount[i];
                    }
                }
            }
            if(collisionNum){
                backoffRange[key_ptr]=backoffRange[key_ptr]*2;
                backoffCount[key_ptr]=rand()%backoffRange[key_ptr];
                if(backoffCount[key_ptr]>0){
                    key_t[key_ptr]+=SLOT;
                    --backoffCount[key_ptr];
                }
                totalCollisionPack+=collisionNum;
                continue;
            }

            pack* next_send=p[key_ptr]._queue[p[key_ptr].front_ptr];
            next_send->leaveTime=cur_t;
            next_send->waitingTime=next_send->leaveTime-next_send->comeTime;
            res[key_ptr].push_back(next_send->waitingTime);
            trans_t+=next_send->weight;
            end_t=cur_t+next_send->weight;
            backoffRange[key_ptr]=BASIC_RANGE;
            backoffCount[key_ptr]=rand()%BASIC_RANGE;
            busy=true;
            if((next_send->index+1)%(QUEUE_LEN/10)==0)
                cout<<"站"<<p[key_ptr].index+1<<": 包"<<next_send->index+1<<" waitingTime: "<<next_send->waitingTime<<" comeTime: "<<next_send->comeTime<<" currentTime: "<<cur_t<<" transTime: "<<next_send->weight<<endl;
            ++p[key_ptr].front_ptr;
            if(p[key_ptr].empty()){
                key_t[key_ptr]=INT32_MAX;
                totalTime[key_ptr]=cur_t;
            }else{
                int nextCome_t=p[key_ptr]._queue[p[key_ptr].front_ptr]->comeTime;
                //没包发的时候会countdown吗
                key_t[key_ptr]=(nextCome_t>end_t)?nextCome_t:end_t;
            }
        }else{
            if(end_t<=key_t[key_ptr]){
                busy=false;
                end_t=INT32_MAX;
                cur_t=end_t;
            }else{
                cur_t=key_t[key_ptr];
                key_t[key_ptr]=end_t;
            }
        }
    }
    printf("TotalTime:%d TransTime:%d Utilization:%.2f%% collisionRate:%.2f%%\n",cur_t,trans_t,(double)trans_t/cur_t*100,(double)totalCollisionPack/(3*QUEUE_LEN)*100);
    
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