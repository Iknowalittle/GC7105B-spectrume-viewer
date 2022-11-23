 #include<errno.h>
 #include<sys/types.h>
 #include<sys/socket.h>
 #include<netinet/in.h>
 #include<arpa/inet.h>
 #include<unistd.h>

#include  "gc7105b.h"
#include <string.h> 
using namespace std;
//#include "stringutil.h"
GC7105B::GC7105B()
{
	sockfd=0;
}
GC7105B::~GC7105B()
{
       disconnect();
}

int GC7105B::init(string ip)
{
      disconnect();
      
 struct sockaddr_in  servaddr;

      if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
          printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
          return 0;
      }
  

      memset(&servaddr, 0, sizeof(servaddr));
      servaddr.sin_family = AF_INET;
      servaddr.sin_port = htons(66);
      if( inet_pton(AF_INET, ip.c_str(), &servaddr.sin_addr) <= 0){
          printf("inet_pton error for %s\n",ip.c_str());
          return 0;
      }
  
      if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
          printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
          return 0;
      }

      querydevinfo();
      return sockfd;
}
int GC7105B::querydevinfo()
{
   unsigned char data[]={0x7f,0x41,0x50,0x01,0x00,0x92,0x7e};
   if( send(sockfd, data, sizeof(data) , 0) < 0)
   {
	   printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
          return 0;
   }  
   char buff[255];// 4096
   memset(buff,0,255);
   int n = recv(sockfd, buff, 255, 0);
   if (n)
   {
 	 // hexdump(buff,n);
	//  buff[n]=0;//
	  printf("recv msg from client: %s\n", buff);
   }
   return n;
}

/*
  send keystroke to GC7105, not work properly.
*/
int GC7105B::KeyPress(string key)
{
  unsigned char data[135];
  memset(data,0,135);
  data[0]=0x7f;
  data[134]=0x7e;

  data[1]=0x41;
  data[2]=0x61;    
  data[3]=0x01;    
  data[5]=0x4b;    //K
  data[6]=0x45;    //E
  data[7]=0x59;    //Y
  data[8]=0x50;    //P
  data[9]=0x3a;    //:
  //append key name entery and 0x0A
  //finnal fill up checksum
  memcpy(data+10,key.c_str(),key.size());   
  data[10+key.size()]=0x0A; 


  unsigned char csum=0;
  int i;
  for (i=1;i<133;i++)
  {
	csum+=data[i];
  }
  data[133]=csum;
  //hexdump((char*)data,135);
   if( send(sockfd, data, sizeof(data)/sizeof(unsigned char), 0) < 0)
   {
	  printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
          return 0;
   }
  printf("command send\n");
  unsigned char c;
  scanf("%d",&c);


  return 0;
}
int GC7105B::capture(std::vector<double>&  freq,std::vector<double>&  power)
{
   unsigned char data[]=
	{			      0x7f,0x41,  0x11,0x01,0x00,0x64,0x65,0x66,0x61,0x75,
	0x6c,0x74,0x2e,0x74,0x72,0x61,0x00,0x58,  0xd7,0xd7,0x02,0x00,0x00,0x00,0x00,0xf8,
	0x08,0xd2,0x02,0x00,0x00,0x00,0x00,0x01,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,
	0x76,0x8d,0x01,0x00,0xee,0x19,0x00,0xcc,  0x46,0xa8,0x00,0x00,0x00,0xd7,0x02,0x00,
	0x00,0x00,0x00,0xeb,0x46,0xa8,0x00,0xe7,  0x3f,0x0d,0x89,0xec,0xf3,0x19,0x00,0x00,
	0x00,0x00,0x00,0x20,0x4c,0x52,0x00,0x14,  0xc2,0x97,0x00,0xd4,0xed,0x19,0x00,0x88,
	0x13,0x00,0x00,0xf0,0xee,0x19,0x00,0x10,  0x9f,0xa8,0x00,0xaf,0xdb,0xd8,0x89,0xfe,
	0xff,0xff,0xff,0xeb,0x46,0xa8,0x00,0xf3,  0x1b,0x98,0x00,0x58,0xd7,0xd7,0x02,0x1c,
	0xee,0x19,0x00,0x30,0x25,0x40,0x00,0x58,  0xd7,0xd7,0x02,0x00,0x00,0x00,0x00,0x40,
	0x7e
	};

 
  if( send(sockfd, data, sizeof(data)/sizeof(unsigned char), 0) < 0)
   {
	  printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
          return 0;
   }

   unsigned char buff[1500];// 4096
   unsigned char acc_buffer[6000];
   memset(acc_buffer,0,6000);

   int n=0;
    
   int acc_size=0;
   while (acc_size<4500)
   {
   	 memset(buff,0,1500);
         n = recv(sockfd, buff, 1500, 0);
 	 memcpy(acc_buffer+acc_size,buff,n);
 	//  hexdump((char*)buff,n,false);
 	 acc_size+=n;
   } 
    
   unsigned char packetheader[3]={0x7f,0x53,0x11}; //05 00//
   unsigned char final_buffer[5000];

   memset(final_buffer,0,5000);

   int i,j;
   int st=5;	
   int ed;
   int buffer_pos=0;
   int element_size;
   unsigned int count,index;
   bool match=false;
   
   for (i=5;i<acc_size;i++)
   {
	 
	if (acc_buffer[i]==packetheader[0])
	{
		match=true;

	 	for (j=1;j<sizeof(packetheader);j++)
		{
			if (i+j<acc_size)
			{
			    if (acc_buffer[i+j]!=packetheader[j])
				  match=false;
			}else
			   match=false;	 
			if (!match) break;	   
		}

		if (match)
		{
			  ed=i-2;	//remove the tail 0x7e and checksum

			  count=acc_buffer[i+3];
			  index=acc_buffer[i+4];

			  element_size=ed-st;

 			  memcpy(final_buffer+buffer_pos, acc_buffer+st, element_size);
			
			 //  printf("seg: at: %d - %d, size: %d, %d of %d\n",st,ed,element_size,index,count);

			 // hexdump((char*)final_buffer+buffer_pos,element_size,false);

			  buffer_pos+=element_size;			//
 			  st=i+5;		//next segment start position..
 			  //i=i+5;
 			  
			  
		}
 	}
   }
   //check from 0x7d.
   //start

  // printf("checking....upto %d\n",buffer_pos);
   unsigned char sByte;
   
   string ss;
   memset(acc_buffer,0,6000);
   //memcpy(acc_buffer,final_buffer,488);
 
   count=0;
   for (i=0;i<buffer_pos;i++)
   {
	match=false;
	if (final_buffer[i]==0x7d)
	{
	   //next 0x50---0x5f
	    sByte=final_buffer[i+1];
	    if ((sByte>=0x50) && (sByte<=0x5F))
	    {
		sByte=sByte-0x50+0x70;
		acc_buffer[count]=sByte;

		// printf("%d..%d: 0x7d: %02x=>%02x\n",i,count,final_buffer[i+1],sByte);
		i++;
		count++;
		match=true;
	    } 
	      
	} 
	if (!match){
           acc_buffer[count]=final_buffer[i]; 
	   count++;
	}
   }

   //  printf("count: %d\n",count );

  //  hexdump((char*)acc_buffer+488,count-488);


   return analyzedata(acc_buffer,count, freq, power);
}

 
int GC7105B::analyzedata(unsigned char *data,int size,std::vector<double>&  freq,std::vector<double>& power)
{
   memcpy(&lastHeader,data,sizeof(cap_header));
  
   printf("start  freq: %lldHz\n",lastHeader.start_freq);
   printf("stop   freq: %lldHz\n",lastHeader.stop_freq);
   printf("center freq: %lldHz\n",lastHeader.center_freq);
   printf("span   freq: %lldHz\n",lastHeader.span);
 
   float spec_data[SPEC_SIZE];

   int data_size=SPEC_SIZE*sizeof(float);

   float f_step=(lastHeader.span)/(SPEC_SIZE-1);	

   float fv=lastHeader.start_freq;
   
   int i;
   memset(spec_data,0,data_size);
   memcpy(spec_data,data+488,data_size);

   peak_power=-9999;
   peak_freq=-1;
   for (i=0;i<SPEC_SIZE;i++)
   {
 	fv=lastHeader.start_freq+i*f_step;
 	freq.push_back(fv/1e6);
 	power.push_back(spec_data[i]);

	if (spec_data[i]>peak_power)
	{
		peak_power=spec_data[i];
		peak_freq=fv;
	}
   }
	
   return SPEC_SIZE;
}
float GC7105B::CurrentSpan()
{
  return lastHeader.span/1e6;
}

float GC7105B::CurrentCenterFreq()
{
  return lastHeader.center_freq/1e6;
}
float GC7105B::CurrentStartFreq()
{
   return lastHeader.start_freq/1e6;
}
float GC7105B::CurrentStopFreq()
{
   return lastHeader.stop_freq/1e6;
}

float GC7105B::PeakFreq()
{
  return peak_freq/1e6;
}
float GC7105B::PeakPower()
{
  return peak_power;
}
int GC7105B::disconnect()
{
  if (sockfd)  {
	close(sockfd);
	sockfd=0;
  }
}


