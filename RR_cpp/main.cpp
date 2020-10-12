#include "common.h"

int main(){
    producer p[PROD_NUM];
    double lamda[PROD_NUM]={3,4,5};
    double Mu[PROD_NUM]={13,14,11};
    cout<<"M/M/"<<PROD_NUM<<" queueLen: "<<QUEUE_LEN<<endl;
    for(int i=0;i<PROD_NUM;i++)
        p[i]=producer(i,lamda[i],Mu[i]);
    serve(p);    
    return 0;
}
