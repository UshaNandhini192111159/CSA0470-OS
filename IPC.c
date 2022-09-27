#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
int main()
{
int fd1[2],fd2[2],childpid;
pipe(fd1);
pipe(fd2);
childpid=fork();
if(childpid==0)
{
close(fd2[0]);
close(fd1[1]);
server(fd1[0],fd2[1]);
}
else
{
close(fd1[0]);
close(fd2[1]);
client(fd2[0],fd1[1]);
}
}

void client(int readfd,int writefd)
{
int n,length;
struct message mesg;
printf("Enter the Filenamen");
fgets(mesg.message_data,MAXMESSAGEDATA,stdin);
length=strlen(mesg.message_data);
if(mesg.message_data[length-1]=='n') length--;
mesg.message_length=length;
write(writefd,&mesg,MESGHDRSIZE+mesg.message_length);
while(1)
{
read(readfd,&mesg,MESGHDRSIZE);
length=mesg.message_length;
n=read(readfd,mesg.message_data,length);
write(STDOUT_FILENO,mesg.message_data,n);
}
}

void server(int readfd,int writefd)
{
int n,length;
struct message mesg;
FILE *fp;
n=read(readfd,&mesg,MESGHDRSIZE);
length=mesg.message_length;
n=read(readfd,mesg.message_data,length);
mesg.message_data[n]='';
fp=fopen(mesg.message_data,"r");
while(fgets(mesg.message_data,MAXMESSAGEDATA,fp)!=NULL)
{
mesg.message_length=strlen(mesg.message_data);
write(writefd,&mesg,MESGHDRSIZE+mesg.message_length);
}
}
