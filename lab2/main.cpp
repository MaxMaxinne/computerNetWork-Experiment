#include "common.h"

#define ACK_DELAY 0
#define BASIC_RANGE 4
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
        key_t[i]=0;
    }
    //包退出占用信道的时间
    int end_t=INT32_MAX;
    //信道忙碌标志
    bool busy=false;
    int totalCollisionPack[PROD_NUM]={0,0,0,0,0};
    int totalTries[PROD_NUM]={0,0,0,0,0};
    int trans_t=0;
    //等待时间
    // vector<int> res[PROD_NUM];
    int totalTime[PROD_NUM];
    //指数回退
    int backoffCount[PROD_NUM]={0,0,0,0,0};
    int transPackNum[PROD_NUM]={0,0,0,0,0};
    int through[PROD_NUM]={0,0,0,0,0};
    int backoffRange[PROD_NUM]={BASIC_RANGE,BASIC_RANGE,BASIC_RANGE,BASIC_RANGE,BASIC_RANGE};
    for(int i=0;i<PROD_NUM;i++){
        backoffCount[i]=rand()%backoffRange[i];
    }
    while(transPackNum[0]<QUEUE_LEN&&transPackNum[1]<QUEUE_LEN&&transPackNum[2]<QUEUE_LEN&&transPackNum[3]<QUEUE_LEN&&transPackNum[4]<QUEUE_LEN){
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
            ++totalTries[key_ptr];
            int collisionNum=0;
            for(int i=0;i<PROD_NUM;i++){
                if(i!=key_ptr&&key_t[key_ptr]==key_t[i]&&backoffCount[i]==0){
                    ++totalTries[i];
                    ++totalCollisionPack[i];
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
                ++totalCollisionPack[key_ptr];
                // totalCollisionPack+=collisionNum;
                continue;
            }

            // pack* next_send=p[key_ptr]._queue[p[key_ptr].front_ptr];
            // next_send->leaveTime=cur_t;
            // next_send->waitingTime=next_send->leaveTime-next_send->comeTime;
            // res[key_ptr].push_back(next_send->waitingTime);
            int weight=(int)ceil(-log((double)rand()/(RAND_MAX))*1);
            trans_t+=weight;
            end_t=cur_t+weight;
            through[key_ptr]+=weight;
            transPackNum[key_ptr]++;
            // if(backoffRange[key_ptr]>BASIC_RANGE)
            //     backoffRange[key_ptr]=backoffRange[key_ptr]/1.414;
            // else 
            //     backoffRange[key_ptr]=BASIC_RANGE;
            backoffRange[key_ptr]=BASIC_RANGE;
            backoffCount[key_ptr]=rand()%backoffRange[key_ptr];
            busy=true;
            if((transPackNum[key_ptr])%(QUEUE_LEN/10)==0)
                cout<<"站"<<key_ptr+1<<": 包"<<transPackNum[key_ptr]<<" currentTime: "<<cur_t<<" transTime: "<<weight<<endl;
            // ++p[key_ptr].front_ptr;
            if(0){
                key_t[key_ptr]=INT32_MAX;
                totalTime[key_ptr]=cur_t;
            }else{
                int nextCome_t=0;
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
    for(int i=0;i<PROD_NUM;i++){
        printf("站%d: 吞吐量:%d 发包数:%d 碰撞率:%.2f%% 碰撞包数:%d 尝试次数:%d\n",i+1,through[i],transPackNum[i],(double)totalCollisionPack[i]/totalTries[i]*100,totalCollisionPack[i],totalTries[i]);
    }
    int tc=0,tt=0,tp=0;
    for(int i=0;i<PROD_NUM;i++){
        tc+=totalCollisionPack[i];
        tt+=totalTries[i];
        tp+=transPackNum[i];
    }
    printf("TotalTime:%d TransTime:%d Efficiency:%.2f%% collisionRate:%.2f%% collisionPack:%d totalTries:%d\n",cur_t,trans_t,(double)tp/cur_t*100,(double)tc/tt*100,tc,tt);
    
    // res_output_mm3(res);
    // queueLen_output_mm3(p,totalTime);
}
int main(int argc,char** argv){
    producer p[PROD_NUM];
    double lamda[PROD_NUM]={atof(argv[1]),atof(argv[2]),atof(argv[3]),atof(argv[4]),atof(argv[5])};
    double Mu[PROD_NUM]={atof(argv[6]),atof(argv[7]),atof(argv[8]),atof(argv[9]),atof(argv[10])};
    // for(int i=0;i<PROD_NUM;i++)
    //     p[i]=producer(i,lamda[i],Mu[i]);
    run(p);
    return 0;
}