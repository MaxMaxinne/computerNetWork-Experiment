#include "common.h"
void queueLen_output(vector<double>& l,char* filename,double totalTime){
    ofstream out(filename);
    if(!out.is_open()){
        cout<<"文件打开失败"<<endl;
        return;
    }
    for(int i=0;i<l.size();i++){
        out<<i<<"    "<<l[i]/totalTime<<endl;
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
        printf("队列%d平均等待时间: %.2f\n",i+1,t/QUEUE_LEN);
        res_output(res[i],filename);
    }
}
void queueLen_output_mm3(vector<double>* queueLen,double totalTime){
    char filename[40];
    for(int i=0;i<PROD_NUM;i++){
        sprintf(filename,"./res_output/len%d.dat",i+1);
        queueLen_output(queueLen[i],filename,totalTime);
        double t=0.0;
        for(int j=0;j<queueLen[i].size();j++)
            t+=queueLen[i][j]*j;
        printf("队列%d平均长度: %.2f\n",i+1,t/totalTime);
    }
}