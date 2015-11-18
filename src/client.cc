/*
    C ECHO client example using sockets
*/
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <memory>
#include <errno.h>
#include <sstream>

using namespace std;

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    int read_size;
    std::string v2 = "VERSION";
    
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 1131 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected");

    while(read_size = recv(sock , server_reply , 2000 , 0) > 0){
	
	printf("%s",server_reply);
	
	
	send(sock , "42676xxxxxxx\ntest message\ny\ny\nn\n" , strlen("42676xxxxxxx\ntest message\ny\ny\nn\n") , 0);
	printf("DATA SENT !\n");
	
	
	if(read_size == 0)
	{
        puts("Client disconnected");
        fflush(stdout);
	break;
	}
	else if(read_size == -1)
	{
        perror("recv failed");
	}
    
    } //while recv end
    //recv(sock , server_reply , 6 , 0);
    //printf("%s",server_reply);
    close(sock);
    return 0;
}
