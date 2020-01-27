#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<fcntl.h>
#define PORT 1024
#define PORT1 2024
#define PORT2 3024
int main()
{

// ROOT

 
int i,sockid;
sockid=socket(AF_INET,SOCK_DGRAM,0);
if(sockid==-1)
{
printf("Socket Error");
return 0;
}
int recvbytes,sentbytes;
struct sockaddr_in client,client1,client2;
client.sin_family=AF_INET;
client.sin_port=htons(PORT);
client.sin_addr.s_addr=INADDR_ANY;
for(i=0;i<8;i++)
client.sin_zero[i]='\0';
printf("\nConnected to Origin\n");
char hostname[100],ip[100],ip1[100],ip2[100];
printf("\nEnter HostName : ");
scanf(" %s",hostname);
int size=sizeof(struct sockaddr);
sentbytes=sendto(sockid,hostname,strlen(hostname),0,(struct sockaddr*)&client,size);
recvbytes=recvfrom(sockid,ip,sizeof(ip),0,NULL,NULL);
ip[recvbytes]='\0';
printf("\nIp from originserver is %s\n",ip);
close(sockid);
return 0;
}
