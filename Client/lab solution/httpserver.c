#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"
#include <unistd.h>
#include <sys/stat.h>


static const int MAXPENDING = 5; // Maximum outstanding connection requests

int main(int argc, char *argv[]) {

int numBytes = 0, char_in, size = 0, count = 0;
char recvbuffer[BUFSIZE], sendbuffer[BUFSIZE], path[200] ={'.'}, discard1[50], discard2[50]; // I/O buffers and buffers for parsing HTTP Request string
        FILE * hFile;
struct stat st;
socklen_t clntAddrLen; //new variable to hold length of address struct
char clntName[INET_ADDRSTRLEN]; // buffer to hold the client address
 if (argc != 2) // Test for correct number of arguments
    DieWithUserMessage("Parameter(s)", "<Server Port>");

  in_port_t servPort = atoi(argv[1]); // First arg:  local port

  // Create socket for incoming connections
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

  for (;;) { // Infinite for loop; runs forever

    // Wait for a client to connect
    int clntSock = accept(servSock, (struct sockaddr *) NULL, NULL);
    if (clntSock < 0)
      DieWithSystemMessage("accept() failed");

    // clntSock is connected to a client!

    while ((numBytes = recv(clntSock, recvbuffer, BUFSIZE - 1, 0)) > 0) {  //receive HTTP Request string from client into the recvbuffer
        recvbuffer[numBytes] = '\0';    // Terminate the string!
        fputs(recvbuffer, stdout);      // Print the contents of the recvbuffer to the local terminal
        if(strstr(recvbuffer, "\r\n\r\n") > 0)  //look for end of HTTP Request header i.e. terminating characters
                break;                                                          // break from recv() loop
    /* Receive up to the buffer size (minus 1 to leave space for
     a null terminator) bytes from the sender */
  }
    if (numBytes < 0)
      DieWithSystemMessage("recv() failed");

        if(strstr(recvbuffer, "favicon.ico")>0)
                continue;

        sscanf(recvbuffer, "%s %s %s", discard1, (path+1), discard2);  //parse HTTP Request string

        if(strcmp(path,"./")==0)  //check if client requested homepage
        {
        strcpy(path, "./index.html");
        }
hFile = fopen(path,"r");
if(hFile == NULL)
{
strcpy(path, "./error.html");
hFile = fopen(path,"r");
stat(path, &st);
size = st.st_size;
snprintf(sendbuffer, sizeof(sendbuffer), "HTTP/1.1 404 Error\r\nContent-Length: 91\r\nConnection:close\r\n\r\n<HTML><HEAD><TITLE>File Not Found</TITLE></HEAD><BODY><h2$


}
else
{
stat(path, &st);
size = st.st_size;
snprintf(sendbuffer, sizeof(sendbuffer), "HTTP/1.1 200 OK\r\nContent-Length: 83\r\nConnection:close\r\n\r\n<HTML><HEAD><TITLE>File Found</TITLE></HEAD><BODY><h2>File F$
}
        ssize_t numBytesSent = send(clntSock, sendbuffer, strlen(sendbuffer), 0);
    if (numBytesSent < 0)
      DieWithSystemMessage("send() failed");

        strcpy (sendbuffer, "");  //reset buffer for next client request
        while((char_in = fgetc(hFile))!= EOF)
{
sendbuffer[count] = char_in;
count++;
}
numBytesSent = send(clntSock, sendbuffer, strlen(sendbuffer), 0);
if(numBytesSent < 0)
DieWithSystemMessage("send failed");
strcpy(path,".");
count = 0;
size = 0;
strcpy(recvbuffer,"");
fclose(hFile);

        close(clntSock); // Close client socket
  }  // end infinite for loop
  // NOT REACHED
}

