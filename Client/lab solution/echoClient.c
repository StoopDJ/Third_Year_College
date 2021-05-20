#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

int main(int argc, char *argv[]) {
        char recvbuffer[BUFSIZE]; // I/O buffer
        char sendbuffer[BUFSIZE]; // I/O buffer
        int numBytes = 0; //numBytes is the return value from recv

        if (argc < 3 || argc > 4 ) // Test for correct number of arguments
    DieWithUserMessage("Parameter(s)",
        "<Server Address> <Server Port> <String Message>");  //state how the arguments are presented at the command-line
                                                                                                                        // e.g. To run: ./echoClient 127.0.0.1 5001 $'H$
        char *servIP = argv[1];     // Second arg: server IP address (dotted quad)
        char *echoString = argv[3];     // Third  arg: string to echo

        in_port_t servPort = atoi(argv[2]);  // Third arg: server port (numeric).

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
                DieWithUserMessage("inet_pton() failed", "invalid address string");
        else if (rtnVal < 0)
 DieWithSystemMessage("inet_pton() failed");
        servAddr.sin_port = htons(servPort);    // Server port

  // Establish the connection to the echo server
        if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
                DieWithSystemMessage("connect() failed");

        size_t echoStringLen  = strlen(echoString);

    // clntSock is connected to a client!
    snprintf(sendbuffer, sizeof(sendbuffer), "%s\r\n", echoString); //Store the echo string in the outgoing buffer, terminate with \r\n
    ssize_t numBytesSent = send(sock, sendbuffer, strlen(sendbuffer), 0); //Send echo string to the server
    if (numBytesSent < 0)
      DieWithSystemMessage("send() failed");

        while ((numBytes = recv(sock, recvbuffer, BUFSIZE - 1, 0)) > 0) {  //receive echo string from server into the recvbuffer
                recvbuffer[numBytes] = '\0';    // Terminate the string!
                fputs(recvbuffer, stdout);      // Print the echo string to the local terminal
                        /* Receive up to the buffer size (minus 1 to leave space for
                                a null terminator) bytes from the sender */
                }
    if (numBytes < 0)
      DieWithSystemMessage("recv() failed");

        fputc('\n', stdout); // Print a final linefeed

        close(sock);//close the connected socket
        exit(0);
}

