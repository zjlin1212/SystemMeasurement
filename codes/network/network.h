#ifndef _NETWORK_H_
#define _NETWORK_H_

#define MSGLEN 2048
#include <time.h>

#include <iostream>
#include <string>
#include <mach/mach_time.h>
#include <mach/mach.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <CoreServices/CoreServices.h>
#include <unistd.h>

class network{
public:
	network();
	~network();
	void setServer();
	void setClient();
	void roundTrip();
	void peakBandwidth();
	void connectionOverhead();
private:
	std::string host;
	uint64_t        start;
    uint64_t        end;
    mach_timebase_info_data_t sTimebaseInfo;
    uint64_t        elapsedNano;
    static const int MAXPENDING = 5; 
    //char *servIP = "192.168.0.13";
    char *servIP = "127.0.0.1";
    in_port_t servPort = 9999;
    double timeScale = 0.000001;
};

#endif