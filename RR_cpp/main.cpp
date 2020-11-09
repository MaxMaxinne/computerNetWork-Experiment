#include "common.h"

int main(int argc,char** argv){
    producer p[PROD_NUM];
    double lamda[PROD_NUM]={atof(argv[2]),atof(argv[3]),atof(argv[4])};
    double Mu[PROD_NUM]={atof(argv[5]),atof(argv[6]),atof(argv[7])};
    cout<<"M/M/"<<PROD_NUM<<" queueLen: "<<QUEUE_LEN<<endl;
    for(int i=0;i<PROD_NUM;i++)
        p[i]=producer(i,lamda[i],Mu[i]);
    servemm3(p);
    return 0;
}
