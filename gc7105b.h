/*
   This is a simple wrapper file for those who want use JDSU GC7105B as a spectrume analyzer with remote control(only working when 
   GC7105B is in Spectrume analyzer mode).
   
   before use it ,please be awared that this code currently it is just a data capture tool, it is not possible to set 
   center frequency,frequency span(start frequency,stop frequency),you should set it by hand.
   one possible way is to set by send key pressing simultion to the instrument, but currently it is not work for number key.
   		
    
   
   		
   su qin 2020.09.08			e-mail:	1076195767@qq.com

	
*/

#ifndef GC7105B_H
#define GC7105B_H
#include <stdio.h>

#include <stdlib.h>
#include <string>
 
#include <sstream> 
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#define JD_SERV_PORT   66  
#define SPECTRUME_LEN  4496 
#define SPEC_SIZE	501
struct msg_header
{
    unsigned char hh;
    unsigned char type0;
    unsigned char type1;
    unsigned char size;
    unsigned char index;
    // msg body
    //char checksum
    // tail;
};

struct cap_header
{
	char filename[94];	
	char mode[80];
	char devname[20];
	char model[20];
	char sn[10];
	char reserved0[48];
	unsigned long long span;
	//int _span;
	unsigned long long start_freq;
	//int _start_freq;
	unsigned long long stop_freq;
	//int _stop_freq;
	unsigned long long center_freq;
	//int _center_freq;
	int reserved;
	int reserved1;
	char dummy[96];
	char spec_header[176];
};
 


using namespace std;
class GC7105B
{
public:
	explicit GC7105B();
	~GC7105B();
	int init(string ip);
	int disconnect();
	int querydevinfo();
	int capture(std::vector<double>&  freq,std::vector<double>&  power);
	int testbyfile(string file);
	int KeyPress(string key);
	float PeakFreq();
	float PeakPower();
	float CurrentSpan();
	float CurrentCenterFreq();
	float CurrentStopFreq();
	float CurrentStartFreq();

private:
	int analyzedata(unsigned char *data,int size,std::vector<double>&  freq,std::vector<double>&  power);
	int sockfd;
	float peak_freq;
	float peak_power;
	cap_header lastHeader;
};



#endif
