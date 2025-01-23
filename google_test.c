/*
* This file contains the example of how to use sockets on the client side
* It fetches the website from google.com
* The code is based on the tutorial from: https://stackoverflow.com/questions/6769981/windows-socket-programming-in-c
*/

#include <stdio.h>
#include <errno.h>      // library containing error codes
#include <stdlib.h>
#include <string.h>

// Here we use defines to make the compilation conditional
// for windows and other systems (e.g. Linux)
#ifdef WIN32
    #include <winsock2.h>                // link to the winsock library, only for VC cl 
    #pragma comment(lib,"ws2_32.lib")   // Winsock Library
#else
    #include <sys/socket.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
#endif

#define MSG_SIZE 1024
#define REPLY_SIZE 65536

void google_test(void)
{
    int iSocket = -1;               // id of the socket, -1 indicates that the socket does not exist
    struct sockaddr_in addrServer;  // address of the server as a dedicated structure
    char strMessage[MSG_SIZE] = { 0 };  // message to send (will be a HTTP GET)        
    char strReply[REPLY_SIZE] = { 0 };  // buffer for the reply
    int recv_size = 0;

    // for windows, we need to use a dedicated library
    // WSADATA
#if defined(_WIN32) || defined(_WIN64)    
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("\nError Code: %d. Exiting..\n", WSAGetLastError());
        exit(1);
    }
#endif

    // Create a socket, but not yet bind it to the address yet
    // i.e we only create the "file" which will help us to get the data from the 
    // internet
    // AF_INET means that we can communicate with all addresses on the internet using IPv4
    // SOCK_STREAM means that we will use TCP, SOCK_DGRAM means that we use UDP
    if ((iSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error: Could not create socket: %s. Exiting...\n", strerror(errno));
        exit(1);
    }

    // reserving the place for the server address
    // and filling it with 0s (memset)
    memset(&addrServer, 0, sizeof(addrServer));

    // fill in the server address
    // addrServer.sin_addr.s_addr = inet_addr("204.106.240.53");      // One of public DNS servers
    addrServer.sin_addr.s_addr = inet_addr("130.241.39.203");    // www.gu.se 
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(80);

    // Here we connect to the server, but we do not send/request anything yet
    if (connect(iSocket, (struct sockaddr*)(&addrServer), sizeof(addrServer)) < 0) {
        printf("Error: Could not connect to server: %s. Exiting..\n", strerror(errno));
        exit(1);
    }

    // Here we send the data to the server
    // first we copy a string to the message buffer
    strcpy(strMessage, "GET / HTTP/1.1\r\n\r\n");
    // strcpy(strMessage, "GET / HTTP/1.0\r\n\r\n");

    // and we send the content of the buffer to the socket
    if (send(iSocket, strMessage, strlen(strMessage), 0) < 0) {
        printf("Error: Could not send http request to server: %s. Exiting..\n", strerror(errno));
        exit(1);
    }

    // Receive a reply from the server
    printf("\nWaiting for server reply..\n");

    // here we receive the data and put it into the buffer again
    // which is a character array in our case
    if ((recv_size = recv(iSocket, strReply, REPLY_SIZE, 0)) < 0) {
        printf("Error: Something wrong happened while getting reply from server: %s. Exiting..\n", strerror(errno));
        exit(1);
    }
    
	printf("Response size: %d\n", recv_size);

    // we add the 0 to the end of the string
    strReply[REPLY_SIZE - 1] = 0;

    // print the server reply
    printf("\nServer Reply:\n\n");
    printf("%s\n", strReply);

    // Close the socket
#if defined(_WIN32) || defined(_WIN64)  
    closesocket(iSocket);
    WSACleanup();
#else
    close(iSocket);
#endif

    exit(0);
} 