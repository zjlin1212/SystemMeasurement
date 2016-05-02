#include "network.h"

#define BUFSIZE 512

#define ORWL_NANO (+1.0E-9)
#define ORWL_GIGA UINT64_C(1000000000)
using namespace std;

network::network(){
	host = "127.0.0.1";
    mach_timebase_info(&sTimebaseInfo);
}

network::~network(){
    cout << "network test over" << endl;
}

void DieWithSystemMessage(string s){
	cout << s << endl;
	exit(-1);
}

void network::roundTrip(){
    
    char *echoString = "hello"; // Second arg:  server IP address (dotted quad)

    // Create a reliable, stream socket using TCP
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
        DieWithSystemMessage("socket() failed");

    // Construct the server address structure
    struct sockaddr_in servAddr;            // Server address
    memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
    servAddr.sin_family = AF_INET;          // IPv4 address family
    // Convert address
    int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
    if (rtnVal == 0)
        DieWithSystemMessage("inet_pton() failed invalid address string");
    else if (rtnVal < 0)
        DieWithSystemMessage("inet_pton() failed");
    servAddr.sin_port = htons(servPort);    // Server port

    // Establish the connection to the echo server
    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        DieWithSystemMessage("connect() failed");

    size_t echoStringLen = strlen(echoString); // Determine input length

    start = mach_absolute_time();
    // Send the string to the server
    ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
    if (numBytes < 0)
        DieWithSystemMessage("send() failed");
    else if (numBytes != echoStringLen)
        DieWithSystemMessage("send() sent unexpected number of bytes");

    // Receive the same string back from the server
    unsigned int totalBytesRcvd = 0; // Count of total bytes received

    while (totalBytesRcvd < echoStringLen) {
        char buffer[BUFSIZE]; // I/O buffer
        /* Receive up to the buffer size (minus 1 to leave space for
        a null terminator) bytes from the sender */
        numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
        if (numBytes < 0)
            DieWithSystemMessage("recv() failed");
        else if (numBytes == 0)
            DieWithSystemMessage("recv() connection closed prematurely");
        totalBytesRcvd += numBytes; // Keep tally of total bytes
        buffer[numBytes] = '\0';    // Terminate the string!
        //cout << buffer << endl;     // Print the echo buffer
    }

    end = mach_absolute_time();
    //cout << endl; // Print a final linefeed

    close(sock);
    
    elapsedNano = (end-start) * sTimebaseInfo.numer / sTimebaseInfo.denom;
    cout << "round trip:" << elapsedNano*timeScale << endl;
}

void network::peakBandwidth(){
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
        DieWithSystemMessage("socket() failed");

    // Construct the server address structure
    struct sockaddr_in servAddr;            // Server address
    memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
    servAddr.sin_family = AF_INET;          // IPv4 address family
    // Convert address
    int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
    if (rtnVal == 0)
        DieWithSystemMessage("inet_pton() failed invalid address string");
    else if (rtnVal < 0)
        DieWithSystemMessage("inet_pton() failed");
    servAddr.sin_port = htons(servPort);    // Server port

    // Establish the connection to the echo server
    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        DieWithSystemMessage("connect() failed");
    for(int i=6; i<14; i++){
        //size_t dataLen = 1 << (4+i*2); // Determine input length
        size_t dataLen = 64 << 10;
        int* data;
        data = (int *)malloc(dataLen);

        
        int times = 1<<(i-6);
        start = mach_absolute_time();
        // Send the string to the server
        for(int j=0; j<times; j++){
            ssize_t numBytes = send(sock, data, dataLen, 0);
            if (numBytes < 0)
                DieWithSystemMessage("send() failed");
            else if (numBytes != dataLen)
                DieWithSystemMessage("send() sent unexpected number of bytes");
        }

        end = mach_absolute_time();
        //cout << endl; // Print a final linefeed


        
        elapsedNano = (end-start) * sTimebaseInfo.numer / sTimebaseInfo.denom;
        cout << "peak bandwidth for " << (1<<i) << "KB is:"<< (1<<i)*1.0/1024/(elapsedNano*timeScale*0.001) << "MB/s" << endl;
    }
    close(sock);
}

void network::connectionOverhead(){
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
        DieWithSystemMessage("socket() failed");

    // Construct the server address structure
    struct sockaddr_in servAddr;            // Server address
    memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
    servAddr.sin_family = AF_INET;          // IPv4 address family
    // Convert address
    int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
    if (rtnVal == 0)
        DieWithSystemMessage("inet_pton() failed invalid address string");
    else if (rtnVal < 0)
        DieWithSystemMessage("inet_pton() failed");
    servAddr.sin_port = htons(servPort);    // Server port

    // Establish the connection to the echo server
    
    start = mach_absolute_time();
    // Send the string to the server
    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        DieWithSystemMessage("connect() failed");
    end = mach_absolute_time();
    close(sock);

    //cout << endl; // Print a final linefeed


    
    elapsedNano = (end-start) * sTimebaseInfo.numer / sTimebaseInfo.denom;
    cout << "connection overhead:" << elapsedNano*timeScale << endl;
}

void network::setClient(){
    
    char *servIP = "127.0.0.1";     // First arg:string to echo
  	char *echoString = "hello"; // Second arg:  server IP address (dotted quad)

  	// Third arg (optional): server port (numeric).  7 is well-known echo port
  	in_port_t servPort = 9999;

  	// Create a reliable, stream socket using TCP
  	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  	if (sock < 0)
    	DieWithSystemMessage("socket() failed");

  	// Construct the server address structure
  	struct sockaddr_in servAddr;            // Server address
  	memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
  	servAddr.sin_family = AF_INET;          // IPv4 address family
  	// Convert address
  	int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
  	if (rtnVal == 0)
    	DieWithSystemMessage("inet_pton() failed invalid address string");
  	else if (rtnVal < 0)
    	DieWithSystemMessage("inet_pton() failed");
  	servAddr.sin_port = htons(servPort);    // Server port

  	// Establish the connection to the echo server
  	if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
    	DieWithSystemMessage("connect() failed");

  	size_t echoStringLen = strlen(echoString); // Determine input length

    start = mach_absolute_time();
  	// Send the string to the server
  	ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
  	if (numBytes < 0)
    	DieWithSystemMessage("send() failed");
  	else if (numBytes != echoStringLen)
    	DieWithSystemMessage("send() sent unexpected number of bytes");

  	// Receive the same string back from the server
  	unsigned int totalBytesRcvd = 0; // Count of total bytes received

  	while (totalBytesRcvd < echoStringLen) {
    	char buffer[BUFSIZE]; // I/O buffer
    	/* Receive up to the buffer size (minus 1 to leave space for
     	a null terminator) bytes from the sender */
    	numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
    	if (numBytes < 0)
      		DieWithSystemMessage("recv() failed");
    	else if (numBytes == 0)
      		DieWithSystemMessage("recv() connection closed prematurely");
    	totalBytesRcvd += numBytes; // Keep tally of total bytes
    	buffer[numBytes] = '\0';    // Terminate the string!
    	//cout << buffer << endl;     // Print the echo buffer
  	}

    end = mach_absolute_time();
  	//cout << endl; // Print a final linefeed

  	close(sock);
    
    elapsedNano = (end-start) * sTimebaseInfo.numer / sTimebaseInfo.denom;
    cout << "round trip:" << elapsedNano << endl;
  	exit(0);
}

void network::setServer(){
    in_port_t servPort=9999;
	int servSock; // Socket descriptor for server
 	if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    	DieWithSystemMessage("socket() failed");
	// Construct local address structure

 	struct sockaddr_in servAddr;                  // Local address
 	memset(&servAddr, 0, sizeof(servAddr));       // Zero out structure
 	servAddr.sin_family = AF_INET;                // IPv4 address family
 	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
 	servAddr.sin_port = htons(servPort);          // Local port

	// Bind to the local address
	if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
    DieWithSystemMessage("bind() failed");

	// Mark the socket so it will listen for incoming connections
	if (listen(servSock, MAXPENDING) < 0)
		DieWithSystemMessage("listen() failed");

	while(233) { // Run forever
    	struct sockaddr_in clntAddr; // Client address
    	// Set length of client address structure (in-out parameter)
    	socklen_t clntAddrLen = sizeof(clntAddr);

    	// Wait for a client to connect
    	int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
    	if (clntSock < 0)
      		DieWithSystemMessage("accept() failed");

    	// clntSock is connected to a client!

    	char clntName[INET_ADDRSTRLEN]; // String to contain client address
    	if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
        	sizeof(clntName)) != NULL)
      		printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
    	else
      		puts("Unable to get client address");
        char *buf  = (char *)malloc(64 << 10);
        int readed;
        for(int i=6; i<14; i++){
            readed =  1 << (i+10);
            start = mach_absolute_time();
            while(readed > 0){
                readed -= recv(clntSock, buf, 64 << 10, 0);
            }
            end = mach_absolute_time();
            elapsedNano = (end-start) * sTimebaseInfo.numer / sTimebaseInfo.denom;
            cout << "peak bandwidth for " << (1<<i) << "KB is:"<< (1<<i)*1.0/1024/(elapsedNano*timeScale*0.001) << "MB/s" << endl;
        }
        // while ((readed = read(clntSock, buf, sizeof(buf))) > 0){
        //     if (write(clntSock, buf, readed) != readed){
        //         fprintf(stderr, "Write failed\n");
        //     }
        // }
    	//HandleTCPClient(clntSock);
        close(clntSock);
    }
}
