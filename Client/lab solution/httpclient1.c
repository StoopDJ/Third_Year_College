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
        char sendbuffer[BUFSIZE], recvbuffer[BUFSIZE], discard[20]; // I/O buffers
        int numBytes = 0; //numBytes is the return value from recv

        if (argc < 3 || argc > 4) // Test for correct number of arguments
    DieWithUserMessage("Parameter(s)",
        "<Server Address> <Server Port> <HTTP Request>");  //state how the arguments are presented at the command-line
                                                           // e.g. To run: ./httpClient 127.0.0.1 5001 $'G$

  char *servIP = argv[1];     // Second arg: server IP address (dotted quad)
  char *httpRequest = argv[3]; // Fourth arg: HTTP Request

  // Third arg: server port (numeric).
  in_port_t servPort = atoi(argv[2]);

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

        snprintf(sendbuffer, sizeof(sendbuffer), "%s", httpRequest);

        ssize_t numBytesSent = send(sock, sendbuffer, strlen(sendbuffer), 0);

    if (numBytesSent < 0)
      DieWithSystemMessage("send() failed");


  while ((numBytes = recv(sock, recvbuffer, BUFSIZE, 0)) > 0) {
    /* Receive up to the buffer size (minus 1 to leave space for
     a null terminator) bytes from the sender */
                recvbuffer[numBytes] = '\0';    // Terminate the string!
        fputs(recvbuffer, stdout);      // Print the contents of the recvbuffer to the local terminal
        }
     if (numBytes < 0)
                DieWithSystemMessage("recv() failed");

        fputc('\n', stdout); // Print a final linefeed

        close(sock); //close the connected socket
        exit(0);
}


