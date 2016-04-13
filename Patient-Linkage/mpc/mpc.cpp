// main.cpp
#include "../util/typedefs.h"
#include "../util/config.h"
#include "../circuit/circuit.h"
#include "party.h"
#include <ctime>
#include <stdio.h>
 #include <unistd.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <string.h>
 #include <arpa/inet.h>
 #include <netdb.h>
 #include <netinet/in.h>
 #include <stdlib.h>
#include <fstream> 
#include <unistd.h>
#include <pthread.h>
#define BUFSIZE 0xFF00
 
#define BACKLOG 10  
#define MAXSIZE 1024  

#define SERVPORT 3333  
#define MAXDATASIZE 100  
#define SERVER_IP "169.228.52.135"  
#define patientnum 100
#define IDlength 7
#define NUM_THREADS 8
 

int getserverpoint(char *url)
{

 int i=0,j=0;
 int t[10];
 int num=0,num1=0;;
 int totalnum=0;
 char tempurl[MAXSIZE];
 while(url[i]!='!')
 {
    t[i]=url[i]-48;
    i++;
 }
 num=i;
 i++;
 while(url[i]!='\0')
 {  
   tempurl[j]=url[i];
   j++;
   i++;
 }
 tempurl[j]='\0';
 num1=j;
 for(i=0;i<num;i++)
 {
   totalnum=t[i]*pow(10,num-i-1)+totalnum;
 }
 for(i=0;i<num1;i++)
   url[i]=tempurl[i];
 url[i]='\0';
 return totalnum;
}




void server(char *msg1,char *msg,int port) {  
    int sockfd, client_fd;  
    struct sockaddr_in my_addr;  
    struct sockaddr_in remote_addr;  
    //创建套接字  
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  
        perror("socket create failed!");  
        exit(1);  
    }  
  
    //绑定端口地址  
    my_addr.sin_family = AF_INET;  
    my_addr.sin_port = htons(port);  
    my_addr.sin_addr.s_addr = INADDR_ANY;  
    bzero(&(my_addr.sin_zero), 8);  
    int on,ret;
     on = 1;
     ret = setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
    while (bind(sockfd, (struct sockaddr*) &my_addr, sizeof(struct sockaddr))  
            == -1) {  
        perror("bind error!");  
      //  exit(1);  
    }  
    
    //监听端口  
    if (listen(sockfd, BACKLOG) == -1) {  
        perror("listen error");  
        exit(1);  
    }  
    cout<<"Listening the port "<<endl;
    int time=1;
    while (time>0) {  
        int sin_size = sizeof(struct sockaddr_in);  
        if ((client_fd = accept(sockfd, (struct sockaddr*) &remote_addr, (socklen_t*)&sin_size)) == -1) {  
            perror("accept error!");  
            
            continue;  
        } 
        
        printf("Received a connection from %s\n", (char*) inet_ntoa(  
                remote_addr.sin_addr));  
        time--;
        
     
    //    if (!fork()) { 
       
            int rval;  
           // char buf[MAXSIZE];  
            if ((rval = read(client_fd, msg, MAXSIZE)) < 0) {  
               perror("reading stream error!");  
               continue;  
            }  
            printf("%s\n", msg);  
           
          
            if (send(client_fd, msg1, strlen(msg1), 0) == -1)  
                perror("send error!");  
            close(client_fd);  
            close(sockfd);
          // exit(0);  
         }
   
        //close(client_fd);  
     
}  

void client(char *msg1,char *msg,int port){  
  
 
    int sockfd, recvbytes;  
   char buf[MAXDATASIZE];  
    struct hostent *host;  
   struct sockaddr_in serv_addr;  
  
   if (( sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  
        perror("socket error!");  
        exit(1);  
   }  
   bzero(&serv_addr,sizeof(serv_addr));  
    serv_addr.sin_family    = AF_INET;  
    serv_addr.sin_port      = htons(port);  
    serv_addr.sin_addr.s_addr= inet_addr(SERVER_IP);  
  
    while (connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) == -1) {  
        perror("connect error!");  
       //exit(1);  
       return;
    }  
  
    write(sockfd,msg1, sizeof(msg1));  
   if ((recvbytes = recv(sockfd, buf, MAXDATASIZE,0)) == -1) {  
        perror("recv error!");  
    //    exit(1);  
        return;
    }  
  
    buf[recvbytes] = '\0';  
    msg[0]=buf[0];
    printf("Received: %s",buf);  
    close(sockfd);  
}  




using namespace std;

void MyBinary(vector <int>& data,int start,int len,int inputnum)
{
  int temp1,temp2;
  temp2=inputnum;
  for(int i=start;i<start+len;i++)
   { 
       temp1=temp2%2;
       temp2=floor(temp2/2);
       data[i]=temp1;
    }
   return;
}


int PrintOutput(const vector<int>& vOutput) 
{

	int size=vOutput.size();
  int result=0;
	for (int i=0;i<size;i++)
    result=pow(2,i)*vOutput[i]+result;

	return result;


}


void geturl(const char* url,char *mytext)
//本篇文章来源于 Linux公社网站(www.linuxidc.com)  原文链接：http://www.linuxidc.com/Linux/2014-05/101455.htm
{

        int cfd;
        struct sockaddr_in cadd;
        struct hostent *pURL = NULL;
        char myurl[BUFSIZE];
        char *pHost = 0;
        char host[BUFSIZE],GET[BUFSIZE];
        char request[BUFSIZE];
        static char text[BUFSIZE];
        int i,j;
 

        //分离主机中的主机地址和相对路径
        memset(myurl,0,BUFSIZE);
        memset(host,0,BUFSIZE);
        memset(GET,0,BUFSIZE);
        
        strcpy(myurl,url);
        for(pHost = myurl;*pHost != '/' && *pHost != '\0';++pHost);
        

        //获取相对路径保存到GET中
        if((int)(pHost-myurl) == strlen(myurl))
        {
                strcpy(GET,"/");//即url中没有给出相对路径，需要自己手动的在url尾
 //部加上/
        }
        else
        {
                strcpy(GET,pHost);//地址段pHost到strlen(myurl)保存的是相对路径
        }
        
        //将主机信息保存到host中
        //此处将它置零，即它所指向的内容里面已经分离出了相对路径，剩下的为host信
 //息(从myurl到pHost地址段存放的是HOST)
        *pHost = '\0';
        strcpy(host,myurl);
        
        //设置socket参数
        if(-1 == (cfd = socket(AF_INET,SOCK_STREAM,0)))
        {
                printf("create socket failed of client!\n");
                exit(-1);
        }
 
        pURL = gethostbyname(host);
      
      
        bzero(&cadd,sizeof(struct sockaddr_in));
    
        cadd.sin_family = AF_INET;
    
        cadd.sin_addr.s_addr = *((unsigned long*)pURL->h_addr_list[0]);
       
        cadd.sin_port = htons(80);
       
        //向WEB服务器发送URL信息
        memset(request,0,BUFSIZE);
        strcat(request,"GET ");
        strcat(request,GET);
        strcat(request," HTTP/1.1\r\n");//至此为http请求行的信息
        strcat(request,"HOST: ");
        strcat(request,host);
        strcat(request,"\r\n");
        strcat(request,"Cache-Control: no-cache\r\n\r\n");
        //连接服务器

       int cc;
        if(-1 == (cc = connect(cfd,(struct sockaddr*)&cadd,(socklen_t)sizeof(cadd))))
        {
                printf("connect failed of client!\n");
                exit(1);
        }
        printf("connect success!\n");
      
        //向服务器发送url请求的request
        int cs;
        if(-1 == (cs = send(cfd,request,strlen(request),0)))
        {
                printf("向服务器发送请求的request失败!\n");
                exit(1);
        }
        printf("Send successfully:%d\n",cs);
 
        //客户端接收服务器的返回信息
        memset(text,0,BUFSIZE);
        memset(mytext,0,BUFSIZE);
        int cr;
        if(-1 == (cr = recv(cfd,text,BUFSIZE,0)))
        {
                printf("recieve failed!\n");
                exit(1);
        }
        else
        {
                printf("receive succecc!\n");
        }
        close(cfd);
       
      //  cout<<"Recevied: "<<text<<endl;
        strcpy(mytext,text);

}
//style 1 zyc  style 0 jiangchao
vector<vector<int> > getdata(char* data,int style1)
{
   int i=0;
   while(data[i]!='[')
     i++;
   int j=0;
   int sum=0;
   int t=0;
   sum=i;
   vector <int> temp(IDlength*4+21);
   vector<vector< int> > result;
   int style=style1;
   if (style1==2)
     style=0;
   
   
   

   sum=sum-style;
   while(1)
   {
      if (data[i+1]!=']')
      {
      sum=sum+2+style;
      t=0;
     
      for(j=sum;j<sum+IDlength;j++)
      {  
       
         switch (data[j])
         {
           case '0': MyBinary(temp,t,4,0);
                     break;
           case '1': MyBinary(temp,t,4,1);
                     break;
           case '2': MyBinary(temp,t,4,2);
                     break;
           case '3': MyBinary(temp,t,4,3);
                     break;
           case '4': MyBinary(temp,t,4,4);
                     break;
           case '5': MyBinary(temp,t,4,5);
                     break;
           case '6': MyBinary(temp,t,4,6);
                     break;
           case '7': MyBinary(temp,t,4,7);
                     break;
           case '8': MyBinary(temp,t,4,8);
                     break;
           case '9': MyBinary(temp,t,4,9);
                     break;
           case 'H': MyBinary(temp,t,4,10);
                     break;
           case 'G': MyBinary(temp,t,4,11);
                     break;
           case 'N': MyBinary(temp,t,4,12);
                     break;
           case 'A': MyBinary(temp,t,4,13);
                     break;
           
        }
        t=t+4;
      }                  
      result.push_back(temp);
      sum=sum+IDlength;
      sum=sum+1;
      
      if(data[sum]==']')
      {
           t=IDlength*4+style1*7;
           j=result.size();
           for(i=j;i<patientnum;i++)
           {
              MyBinary(temp,t,7,i);
              result.push_back(temp);
           }
           
         return result;
      }
      }
      else
      {
          t=IDlength*4+style1*7;
          j=0;
          for(i=j;i<patientnum;i++)
           {
              MyBinary(temp,t,7,i);
              result.push_back(temp);
           }
          return result;
      
      }
   }
   return result;
}

void writefile(vector<vector<int> > data,int pid, vector<int> num,int threadnum)
{ 
   int i=0,j=0;
   int size=data.size();
   int datasize=(IDlength*4+21)*size;
   char n1[100]="config_Multiparty/configs1 .txt";
   char n2[100]="config_Multiparty/configs2 .txt";
   char n3[100]="config_Multiparty/configs3 .txt";
   char d1[100]="config_Multiparty/inputs1 .txt";
   char d2[100]="config_Multiparty/inputs2 .txt";
   char d3[100]="config_Multiparty/inputs3 .txt";
   char k1[100]="config_Multiparty/address1 .txt";
   char k2[100]="config_Multiparty/address2 .txt";
   char k3[100]="config_Multiparty/address3 .txt";
   
   
   
   char *configname;
   char *dataname;
   char *addressname;
   if (pid==0)
   {
      configname=n1;
      dataname=d1;
      addressname=k1;
   }
   else if(pid==1)
   {
      configname=n2;
      dataname=d2;
      addressname=k2;
   }
   else
   {
      configname=n3;
      dataname=d3;
      addressname=k3;
   }
   configname[26]=threadnum+48;
   dataname[25]=threadnum+48;
   addressname[26]=threadnum+48;
   
   ofstream file(configname);
   file<<endl;
   file<<"num_parties "<<num.size()<<endl;
   file<<"pid"<<" "<<pid<<endl;
   file<<"address-book	"<<addressname<<endl;
   file<<"create-circuit Multipartya ";
   for(i=0;i<num.size()-1;i++)
     file<<num[i]<<" ";
   file<<num[num.size()-1]<<endl<<endl;
   file<<"num_input"<<" "<<datasize<<endl;
   file<<"input"<<" "<<dataname<<endl<<endl;
   file<<"seed 4373984170943781378434349"<<endl<<endl;
   file<<"p 8936097950764538541647693880373941060412422053330581106416547022143872696986309618392492140173286901378451169263228782167065246660381840799235731486130087"<<endl<<endl;
   file<<"g 7660915846360464914746169975675514711978996378800371841846530440188167304050642048045944447898172503094027848348928324282770185052123039167705188057761352"<<endl<<endl;
   file.close();
   ofstream file1(dataname);
   for(i=0;i<data.size();i++)
   {
   
     for(j=0;j<IDlength*4+21;j++)
       file1<<data[i][j]<<" ";
     file1<<endl;
   }
   file1.close();
   ofstream file2(addressname);
   file2<<"0 132.239.245.154 "<<8920+threadnum<<endl;
   file2<<"1 169.228.52.144 "<<8920+threadnum<<endl;
   file2<<"2 169.228.52.145 "<<8920+threadnum<<endl;

   file2.close();
   return;
}
   
void* mythread( void* args)
{
int result=0;
int pid=(int)(*((int*)args));
int threadnum=(int)(*((int*)args+1));
int serverpoint=(int)(*((int*)args+2));
cout<<"pid:"<<pid<<" threadnum: "<<threadnum<<" serverpoint:  "<<serverpoint<<endl;
char *configpath;
	double tt = clock();
char text[20]="6";
char msg1[MAXSIZE];
	CConfig* pConfig = new CConfig();
    char n1[100]="config_Multiparty/configs1 .txt";
   char n2[100]="config_Multiparty/configs2 .txt";
   char n3[100]="config_Multiparty/configs3 .txt";
 if(pid==0)
  { configpath=n1;
   // myurl=url1;
  
  }
  else if(pid==1)
  { configpath=n2;
   
  
  }
  else
  {  configpath=n3;
   
  }
   configpath[26]=threadnum+48;
 
	if(!pConfig->Load(configpath))
	{
		cout << "failure in opening the config file: " << configpath << endl;
		return 0;
	}

	CParty* pParty = new CParty();
	pParty->Run();
  
 
   result=PrintOutput( pParty->GetOutput() );
   cout<<"The result is: "<<result<<endl;
	 
	delete pParty; 
 	 
	double tt1 = clock();
	cout << endl << "elapsed " <<  (tt1-tt)/CLOCKS_PER_SEC << " seconds." << endl;
  text[0]=result;
  if (pid==0)
  {
  client(text,msg1,serverpoint);
  }
 // sleep(32);
 
 return NULL;
}



int main(int argc, char** argv)
{
  int pid;
  pid=int(argv[1][0])-48;
  char msg1[MAXSIZE];
   char url[MAXSIZE];
   int para[3];
  int threadnum=0;
  pthread_t thread[NUM_THREADS];
  int rc=0;
 
  const char *myurl;
  int serverpoint;
  int port;

 // const char *url1="win3.ucsd-dbmi.org/query.php?A.sex_cd=M&B.length_of_stay=4";
 // const char *url2="gpu1.ucsd-dbmi.org/cgi-bin/ucscBeacon/query?chromosome=1&varID=rs555500075&referenceBases=T&alternateBases=TA&format=text";
 // const char *url3="win4.ucsd-dbmi.org/query.php?sleep.notes=great";
  char mytext[BUFSIZE];
  if(pid==0)
  { 
   // myurl=url1;
    port=2010;
  }
  else if(pid==1)
  { 
   // myurl=url2;
    port=2011;
  }
  else
  {  
   //   myurl=url3;
      port=2012;
  }
  while(1)
  {
  server(msg1,url,port);
 
  serverpoint=getserverpoint(url);
 
  
  myurl=url;
  geturl(myurl,mytext);

  
  vector<vector<int> > mydata=getdata(mytext,pid);

  
  /*
  for (int i=0;i<mydata.size();i++)
  {
    for(int j=0;j<IDlength*4+21;j++)
      cout<<mydata[i][j]<<" ";
    cout<<endl;
  }
  cin>>pid;
  */
 // cout<<mydata.size()<<endl;
  vector <int> num(3);
  /*
  num[pid]=mydata.size();

  msg1[0]=mydata.size();
  msg1[1]='\0';

  if (pid==0)
  {
     client(msg1,msg2,3333);
     num[1]=int(msg2[0]);
     cout<<"done"<<endl;
      usleep(3000000);
      server(msg1,msg2,3334);
      cout<<"done"<<endl;
  }
     
  else
  { 
     server(msg1,msg2,3333);
     cout<<"done"<<endl;
      usleep(3000000);
     client(msg1,msg2,3334);
     num[0]=int(msg2[0]);
     cout<<"done"<<endl;
     
  }
 // usleep(3000000);
  cout<<num[0]<<" "<<num[1]<<endl;
 */
  num[0]=patientnum;
  num[1]=patientnum;
  num[2]=patientnum;
  writefile(mydata,pid,num,threadnum);
  para[0]=pid;
  para[1]=threadnum;
  para[2]=serverpoint;
  pthread_join(thread[threadnum],NULL);
   rc = pthread_create(&thread[threadnum], NULL,mythread, &para);
   threadnum++;
   if (threadnum==NUM_THREADS)
     threadnum=0;
  }
	return 0;
}

