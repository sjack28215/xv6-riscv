#include "kernel/types.h"
#include "user/user.h"
#define PRIME_NUM 35
#define READEND 0
#define WRITEEND 1

void child(int * pl);
int main(int argc,char *argv[]){
    int p[2];
    pipe(p);
    if(fork() == 0){
        child(p);
    }else{
        close(p[READEND]);

        for(int i = 2;i < PRIME_NUM;i ++){
            write(p[WRITEEND],&i,sizeof(int));
        }
        close(p[WRITEEND]);
        wait(0);
    }
    exit(0);
}
void run_child(int *p) {
    child(p);
}
void child(int* pl){
    int pr[2];
    int n;

    close(pl[WRITEEND]);
    int read_result = read(pl[READEND],&n,sizeof(int));
    if(read_result == 0) exit(0);

    pipe(pr);

    if(fork() == 0){
       run_child(pr);
        exit(0);
    }else{
        close(pr[READEND]);
        printf("prime %d\n",n);
        int prime = n;
        while(read(pl[READEND],&n,sizeof(int))!=0){
            if(n%prime != 0){
                write(pr[WRITEEND],&n,sizeof(int));
            }
        }
        close(pr[WRITEEND]);
        wait((int*)0);
        exit(0);
    }
}