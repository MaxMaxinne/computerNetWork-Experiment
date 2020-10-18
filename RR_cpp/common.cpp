#include "common.h"
void queueLen_output(vector<double>& l,char* filename,double totalTime){
    ofstream out(filename);
    if(!out.is_open()){
        cout<<"文件打开失败"<<endl;
        return;
    }
    for(int i=0;i<l.size();i++){
        out<<i<<"    "<<log10(l[i]/totalTime)<<endl;
    }
    out.close();
}
void res_output(vector<double>& res,char* filename){
    sort(res.begin(),res.end());
    int len=res.size();
    int count=0;//区间计数
    double range=res[len-1]/70,min=0,max=range;//区间范围
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
            out<<(min+max)/2<<"    "<<log10((double)count/len)<<endl;
            min+=range;
            max+=range;
            count=0;
        }
    }
    out.close();
}
void res_output_mm3(vector<double>* res){
    char filename[20];
    for(int i=0;i<PROD_NUM;i++){
        sprintf(filename,"res%d.dat",i+1);
        res_output(res[i],filename);
    }
}