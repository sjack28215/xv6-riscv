#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char *argv[]){
 int p1[2],p2[2];
 pipe(p1),pipe(p2);
 char buf[5];
 int size;
 int pid = fork();
 if(pid == 0){
    close(p1[1]);//关闭其中一个写端，使其作为子进程读端
    //close(p2[0]);//关闭其中一个读端，使其作为子进程的写端
    if((size = read(p1[0],buf,sizeof buf)) > 0){
      printf("%d:received ",getpid());
      write(1,buf,size);
    }else{
      printf("%d:received \n",getpid());
    }
    close(p2[0]);
    write(p2[1],"pong\n",5);
    exit(0);
 }
 else if(pid > 0){
   close(p1[0]);
   write(p1[1],"ping\n",5);
   wait(0);
   close(p2[1]);
   if((size = read(p2[0],buf,sizeof buf))>0){
      printf("%d:received ",getpid());
      write(1,buf,size);
   }else{
      printf("%d:received failed\n",getpid());
   }
   
 }
   else{
      printf("fork error\n");
 }
 exit(0);
}
