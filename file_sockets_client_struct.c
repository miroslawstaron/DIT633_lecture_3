/*
* This is the second part of the network communication - the client.
* The client will send a message to the server, but the message is now
* a specific structure
*/

// so, we need to include the structure definition
// it should be the same structure for both the client and the server
#include "structure_definition.h"

// the code below shows the use of preprocessor directives
// when compiling the code
// this particular code says that different libraries/headers should be included
// if the system is WIN32, i.e. Visual Studio compiler
#ifdef WIN32
#include <winsock2.h>                // link to the winsock library, only for VC cl 
#pragma comment(lib,"ws2_32.lib")   // Winsock Library
#else
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#define PORT 8088 

int main_client_struct(void)
{
#ifdef WIN32
    WSADATA mywsadata; //your wsadata struct, it will be filled by WSAStartup

    if (WSAStartup(MAKEWORD(2, 2), &mywsadata) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
#endif

    int iSocket;
    struct sockaddr_in server_addr;

    struct_person sPersonToSend;

    sPersonToSend.iAge = 12;
    strcpy(sPersonToSend.address, "John's street 12");
    strcpy(sPersonToSend.name, "MS");

    // Create socket:
    iSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (iSocket < 0) {
        printf("Unable to create socket\n");
        return -1;
    }

    printf("Socket created successfully\n");

    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Send connection request to server:
    if (connect(iSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Unable to connect\n");
        return -1;
    }
    printf("Connected with server successfully\n");

    // Send the message to server:
    if (send(iSocket, &sPersonToSend, sizeof(sPersonToSend), 0) < 0) {
        printf("Unable to send message\n");
        return -1;
    }

    // Close the socket
#ifdef WIN32  
    closesocket(iSocket);
    WSACleanup();
#else
    close(iSocket);
#endif
}