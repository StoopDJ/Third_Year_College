#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"
#include <sys/stat.h>

static const int MAXPENDING = 5; // Maximum outstanding connection requests

int main(int argc, char *argv[]){
        char sendbuffer[BUFSIZE], recvbuffer[BUFSIZE]; // Buffer for sending data and receive  to the client
        int char_in, numBytes = 0, size = 0, count = 0;
        char discard1[50], discard2[50], path[200]= {'.'};
        FILE * hFile;
        struct stat st;
        socklen_t clntAddrLen;//new variable to hold length of address struct
        struct sockaddr_in servaddr, cliaddr; //new address structure
        char clntName[INET_ADDRSTRLEN]; //buffer to hold the client address

        if (argc != 2) // Test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<Server Port>");

        in_port_t servPort =atoi( argv[1]);  //First arg: local port
                //Create socket for incoming connections
        int servSock; // Socket descriptor for server
        if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
                DieWithSystemMessage("socket() failed");

        // Construct the local address structure
        struct sockaddr_in servAddr;            // loacl address
        memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
        servAddr.sin_family = AF_INET;          //IPv4 address family
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
        servAddr.sin_port = htons(servPort); // Local port
        clntAddrLen = sizeof(cliaddr);
   // Bind to the local address
        if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
                DieWithSystemMessage("bind() failed");

        //Mark the socket so it will list for incoming connection
        if (listen(servSock, MAXPENDING) < 0)
                DieWithSystemMessage("listen() failed");


        for(;;)
         { //run forerver

                // Wait fro a client to connect
                int clntSock = accept(servSock, (struct sockaddr *) &cliaddr, &clntAddrLen);
                if(clntSock < 0){
                DieWithSystemMessage("accept() failed");
                }
                printf("connecttion from %s, port%d\n", inet_ntop(AF_INET,&cliaddr.sin_addr,clntName,sizeof(clntName)),ntohs(cliaddr.sin_port));//print client address
                while((numBytes = recv(clntSock, recvbuffer, BUFSIZE-1, 0))> 0)
                {
                        recvbuffer[numBytes];
                        fputs(recvbuffer, stdout);
                        recvbuffer[numBytes]='\0';
                        if(strstr(recvbuffer, "\r\n\r\n") > 0)
                        break;
                }
                sscanf(recvbuffer,"%s %s %s",discard1, (path+1), discard2);
                if(strcmp(path,"./")==0){
                strcpy(path,"./index.html");
                }
                hFile = fopen(path,"r");
                if(hFile == NULL){
                strcpy(path, "./error.html");
                hFile = fopen(path, "r");
                stat(path, &st);
                size = st.st_size;
  snprintf(sendbuffer, sizeof(sendbuffer),"HTTP/1.1 404 Error\r\nCache-Control: no-cache\r\nContent-length:%d\r\nConnection: close\r\n\r\n", size);
                }
                else{
                stat(path, &st);
                size = st.st_size;
                snprintf(sendbuffer, sizeof(sendbuffer), "HTTP/1.1 200 OK\r\nCache-Control: no-cache\r\nContent-length:%d\r\nConnection: close\r\n\r\n", size);
                }

                ssize_t numBytesSent = send(clntSock, sendbuffer, strlen(sendbuffer), 0);
                if (numBytesSent < 0)
                DieWithSystemMessage("send failed");

                strcpy(sendbuffer,"");

                while((char_in=fgetc(hFile))!= EOF){
                sendbuffer[count] = char_in;
                count++;
                }

                // clntsock is connected to a clinet!
                numBytesSent = send(clntSock, sendbuffer, strlen(sendbuffer), 0);
                if (numBytesSent < 0)
                DieWithSystemMessage("send failed");
                strcpy(path,".");
                count = 0;
                size = 0;
                strcpy(recvbuffer,"");

                fclose(hFile);
                close(clntSock); // Close client socket
        }

}


