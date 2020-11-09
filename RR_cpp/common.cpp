#include "common.h"

void queueLen_output(map<int,double> m,char* filename,double totalTime){
    ofstream out(filename);
    if(!out.is_open()){
        cout<<"文件打开失败"<<endl;
        return;
    }
    // for(int i=0;i<l.size();i++){
    //     out<<i<<"    "<<l[i]/totalTime<<endl;
    // }
    for(auto iter=m.begin();iter!=m.end();iter++){
        
        out<<iter->first<<"    "<<iter->second/totalTime<<endl;
    }
    out.close();
}
void res_output(vector<double>& res,char* filename){
    sort(res.begin(),res.end());
    int len=res.size();
    int count=0;//区间计数
    double range=res[len-1]/100,min=0,max=range;//区间范围
    cout<<"区间范围 "<<range<<endl;
    ofstream out(filename);
    if(!out.is_open()){
        cout<<"文件打开失败"<<endl;
        return;
    }

    for(int j=0;j<len;j++){
        if(res[j]>=min&&res[j]<=max){
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
void res_output_mm3(vector<double>* res){
    char filename[40];
    for(int i=0;i<PROD_NUM;i++){
        sprintf(filename,"./res_output/res%d.dat",i+1);
        double t=0.0;
        for(int j=0;j<res[i].size();j++)
            t+=res[i][j];
        printf("队列%d平均等待时间: %.5f\n",i+1,t/QUEUE_LEN);
        res_output(res[i],filename);
    }
}
void queueLen_output_mm3(producer* prod,double totalTime){
    char filename[40];
    for(int i=0;i<PROD_NUM;i++){
        sprintf(filename,"./res_output/len%d.dat",i+1);
        map<int,double> m;
        int p1=0,p2=0;//p1指向下一个到达包，p2指向下一个离开包
        double nextLeave_t=prod[i]._queue[0]->leaveTime,nextArr_t=prod[i]._queue[0]->comeTime,cur_t=0;
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
                    nextArr_t=prod[i]._queue[p2]->comeTime;
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
                    nextLeave_t=prod[i]._queue[p1]->leaveTime;
                else
                    nextLeave_t=__DBL_MAX__;
            }
        }
        queueLen_output(m,filename,totalTime);
        double t=0.0;
        // for(int j=0;j<queueLen[i].size();j++)
        //     t+=queueLen[i][j]*j;
        for(auto iter=m.begin();iter!=m.end();iter++){
            t+=iter->first*iter->second;
        }
        printf("队列%d平均长度: %.5f\n",i+1,t/totalTime);
    }
}