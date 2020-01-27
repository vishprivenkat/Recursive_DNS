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
#define PORT 2024
#define PORT2 3024
struct details
{	char hostname[100],ip[100];
}x[1],y[1];
int main()
{	char hostname[100],ip[100],ip1[100],ip2[100];

	int i,sockid;
	sockid=socket(AF_INET,SOCK_DGRAM,0);
	if(sockid==-1)
	{
		printf("Socket Error");
		return 0;
	}
	int sentbytes,recvbytes;
	struct sockaddr_in server,dummy, client2;
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
	server.sin_addr.s_addr=INADDR_ANY;
	for(i=0;i<8;i++)
		server.sin_zero[i]='\0';
	int bindstatus=bind(sockid,(struct sockaddr*)&server,sizeof(struct sockaddr));
	if(bindstatus==-1)
	{
		printf("Bind Error");
		close(sockid);
	}
	
	int size=sizeof(struct sockaddr);
	recvbytes=recvfrom(sockid,y,sizeof(y),0,(struct sockaddr*)&dummy,&size);
	
	printf("\nHostname From Client : %s ",y[0].hostname);
	printf("\nIP from root: %s ",y[0].ip);
	int j,k,count;
	count=0;
	char name[100];
	for(i=0;y[0].hostname[i]!='\0';i++)
	{
		if(y[0].hostname[i]=='.')
		{
			break;
		}
	}
	strcpy(name,y[0].hostname+i+1);
	//printf("%s",name);
	int fd=open("tld.txt",O_RDWR | S_IREAD | S_IWRITE);
	if(fd==-1)
	{
		printf("FIle Error");
		close(sockid);
		return 0;
	}
	char filecontent[300];
	lseek(fd,0,0);
	read(fd,filecontent,300);
	int flag=0;
	//printf("%s",filecontent);
	count=0;
	for(i=0;i<strlen(filecontent);i++)
	{
		count=0;
		for(j=0;j<strlen(name);j++)
		{
			if(name[j]==filecontent[i+j])
			{
				count++;
				if(count==strlen(name))
				{
					flag=1;
					break;
				}
			}
		}
		if(flag)
		{
			k=0;
			for(j=i+strlen(name)+1;filecontent[j]!='\n';j++)
			{
				ip[k++]=filecontent[j];
			}
			break;
		}
	}
	ip[k]='\0';
	//printf("%s",ip);
	strcpy(x[0].hostname,y[0].hostname);
	strcpy(x[0].ip,ip);

	int sockid2;
	sockid2=socket(AF_INET,SOCK_DGRAM,0);
	client2.sin_family=AF_INET;
	client2.sin_port=htons(PORT2);
	client2.sin_addr.s_addr=INADDR_ANY;
	for(i=0;i<8;i++)
	client2.sin_zero[i]='\0';
	printf("\nConnected to Authoritative\n");
	sentbytes=sendto(sockid2,x,sizeof(x),0,(struct sockaddr*)&client2,size);
	recvbytes=recvfrom(sockid2,ip2,sizeof(ip2),0,NULL,NULL);
	ip2[recvbytes]='\0';
	printf("\nIp from Authoritative is %s\n",ip2);
	close(sockid2);

	sentbytes=sendto(sockid,ip2,strlen(ip2),0,(struct sockaddr*)&dummy,size);
	close(sockid);
	return 0;
}
