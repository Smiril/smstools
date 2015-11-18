/*
 * daemonize.cc
 * This example daemonizes a process, writes a few log messages,
 * sleeps 20 seconds and terminates afterwards.
 */
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <pthread.h> //for threading , link with lpthread
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <memory>
#include <errno.h>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

  #ifdef __linux__
  #include <termios.h>
  #include <unistd.h>
  #define PATH_MAX        4096    /* # chars in a path name including nul */
  #include <linux/limits.h>
  #elif _WIN32
  #include <unistd.h>
  #include <windows.h>
  #elif _WIN64
  #include <unistd.h>
  #include <windows.h>
  #else 
  #error "SDK not support your OS!"
  #endif
//the thread function
void *connection_handler(void *);
void sendsms(std::string a1,std::string b1,std::string dx,std::string dd,std::string dc);

static void skeleton_daemon()
{
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>0; x--)
    {
        close (x);
    }

    /* Open the log file */
    openlog ("Smiril-smstools-deamon", LOG_PID, LOG_DAEMON);
}
int main()
{
    skeleton_daemon();
    
     int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        syslog (LOG_NOTICE, "Could not create socket");
        printf("Could not create socket");
    }
    syslog (LOG_NOTICE, "Socket created");
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 1131 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        syslog (LOG_NOTICE, "bind failed. Error");
        perror("bind failed. Error");
        return 1;
    }
    syslog (LOG_NOTICE, "bind done");
    puts("bind done");
    //TODO: Insert daemon code here.
     //Listen
    listen(socket_desc , 3); 
    //while (1)
    //{
     
     
    //Accept and incoming connection
    syslog (LOG_NOTICE, "Waiting for incoming connections...");
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        syslog (LOG_NOTICE, "Connection accepted");
        puts("Connection accepted");
         
        pthread_t sniffer_thread;
        new_sock = (int*)malloc(1);
        *new_sock = client_sock;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
	    syslog (LOG_NOTICE, "could not create thread");
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        pthread_join( sniffer_thread , NULL);
        syslog (LOG_NOTICE, "Handler assigned");
        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        syslog (LOG_NOTICE, "accept failed");
        perror("accept failed");
        return 1;
    }
        sleep (20);
        //break;
    //}

    syslog (LOG_NOTICE, "VPN Helper Daemon terminated.");
    closelog();

    return EXIT_SUCCESS;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{	
    // create the buffer with space for the data
    const unsigned int MAX_BUF_LENGTH = 4096;
    char buffer[PATH_MAX];
    const char client_version[] = "V1_STANDARD_LINUX";
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    
    char *message , client_message[PATH_MAX];
    
    	std::string a = "";
	std::string b = "";
	std::string e = "";
	std::string f = "";
	std::string g = "";
	
    //Send some messages to the client
    std::string messageg = "Greetings! I am your Smiril smstools SMS Server v 0.1\n";
    write(sock , messageg.c_str() , strlen(messageg.c_str()));
    
    //Send Get Client version
    std::string versionxx = "VERSION\n";
    write(sock , versionxx.c_str() , strlen(versionxx.c_str()));
    	  
      //Receive a message from client
      while( (read_size = read(sock , buffer , PATH_MAX )) > 0 )
      {
	syslog (LOG_NOTICE, buffer);
	// number
	char *hx0 = strtok(buffer,"\n"); 
	if(hx0 != "0"){
	a = hx0;
	// number
	// message
	char *hx1 = strtok(NULL,"\n"); 
	b = hx1;
	// message
	// yny nny nnn yyn nyn yyy nyy ynn
	char *hx2 = strtok(NULL,"\n"); 
	e = hx2;
	char *hx3 = strtok(NULL,"\n"); 
	f = hx3;
	char *hx4 = strtok(NULL,"\n"); 
	g = hx4;
	// yny nny nnn yyn nyn yyy nyy ynn

	sendsms(hx0,hx1,hx2,hx3,hx4);
	//Send Get Client version
	std::string messagegood = "2000\n\0";
	write(sock , messagegood.c_str() , strlen(messagegood.c_str()));	
	close(sock);
	}
	else{
	  //Send Get Client version
	std::string messagegood = "ERROR\n\0";
	write(sock , messagegood.c_str() , strlen(messagegood.c_str()));	
	close(sock);
	}
	
	if(read_size == 0)
	{
        syslog (LOG_NOTICE, "Client disconnected");
        puts("Client disconnected");
        fflush(stdout);
	close(sock);
	}
	else if(read_size == -1)
	{
        syslog (LOG_NOTICE, "recv failed");
        perror("recv failed");
	}    
    } //while end 
    
    close(sock);  
    //Free the socket pointer
    free(socket_desc);
      
    return 0;
}

void sendsms(std::string a1,std::string b1,std::string dx,std::string dd,std::string dc)
{     
      const char *pch;
      int value;
      pch = strtok ((char *)a1.c_str(),",");
      // 4096 phonenumbers max per sms sending sequence
      #ifdef __linux__
      for(int f = 0;f < PATH_MAX;f++){
      #elif _WIN32 || _WIN64
      for(int f = 0;f < MAX_PATH;f++){
      #else 
      #error "OS not supported!"
      #endif
	
      while (pch != NULL)
      {
	FILE * fx;
	char filename[] = "/var/spool/sms/GSM/GSM1/smstools.XXXXXX"; // template for our file.        
	int fd = mkstemp(filename);    // Creates and opens a new temp file r/w.
        if (fd == -1){
	printf("Message: %s \nTo: %lu \nStatus message: %s\nStatus code: %d\n",b1.c_str(),strtol(pch,NULL,value),"Error make TMP-Filename",4010);
	exit(1);
	}          // Check we managed to open the file.
	close(fd);
	fx = fopen (filename,"w+");                        // Xs are replaced with a unique number.
        if (fx == NULL){
	printf("Message: %s \nTo: %lu \nStatus message: %s\nStatus code: %d\n",b1.c_str(),strtol(pch,NULL,value),"Error could not Write",4030);
	exit(1);
	}          // Check we managed to open the file.
        #ifdef __linux__
	fprintf ( fx, "To: %lu \n", strtol(pch,NULL,value) );// <<< write to:
        #elif _WIN32 || _WIN64
	fprintf ( fx, "To: %lu \n", strtol(pch,NULL,value) );// <<< write to:
        #else 
	#error "OS not supported!"
	#endif
      printf("Outgoing SMS ... %lu\n",strtol(pch,NULL,value));
      
      // TextMessage recipient_address_list();
      // God's will welcome but User vote and got some Sympathy with the Devil ...
      std::string god = "y";
      std::string devil = "y";
      std::string human = "y";
      if(dx.c_str() == god && dd.c_str() != devil && dc.c_str() != human)
      {
      fprintf ( fx, "Flash: %s \n", "yes" ); // <<< write:Flash:yes
      printf("\x1B[32mSending as \x1B[33mFlash\x1B[32m SMS with \x1B[33mLow Priority!\x1B[39m\n");
      }
      else if(dx.c_str() == god && dd.c_str() != devil && dc.c_str() == human)
      {
      fprintf ( fx, "Flash: %s \n", "yes" ); // <<< write:Flash:yes
      fprintf ( fx, "Report: %s \n", "yes" );
      printf("\x1B[32mSending as \x1B[33mFlash\x1B[32m SMS with \x1B[33mLow Priority!\x1B[39m\n");
      }
      else if(dx.c_str() == god && dd.c_str() == devil && dc.c_str() != human){
      fprintf ( fx, "Priority: %s \n", "yes" );
      fprintf ( fx, "Flash: %s \n", "yes" );// <<< write:Flash:yesPriority:yes
      printf("\x1B[32mSending as \x1B[33mFlash\x1B[32m SMS with \x1B[33mHigh Priority!\x1B[39m\n");
      }
      else if(dx.c_str() == god && dd.c_str() == devil && dc.c_str() == human){
      fprintf ( fx, "Priority: %s \n", "yes" );
      fprintf ( fx, "Flash: %s \n", "yes" );// <<< write:Flash:yesPriority:yes
      fprintf ( fx, "Report: %s \n", "yes" );
      printf("\x1B[32mSending as \x1B[33mFlash\x1B[32m SMS with \x1B[33mHigh Priority!\x1B[39m and getting Report\n");
      }
      else if(dx.c_str() != god && dd.c_str() == devil && dc.c_str() == human){
      fprintf ( fx, "Priority: %s \n", "yes" );// <<< write:Priority:yes
      fprintf ( fx, "Report: %s \n", "yes" );
      printf("\x1B[32mSending as \x1B[33mRegular\x1B[32m SMS with \x1B[33mHigh Priority!\x1B[39m and getting Report\n");
      }
      else if(dx.c_str() != god && dd.c_str() == devil && dc.c_str() != human){
      fprintf ( fx, "Priority: %s \n", "yes" );// <<< write:Priority:yes
      printf("\x1B[32mSending as \x1B[33mRegular\x1B[32m SMS with \x1B[33mHigh Priority!\x1B[39m\n");
      }
      else if(dx.c_str() != god && dd.c_str() != devil && dc.c_str() == human){
      fprintf ( fx, "Report: %s \n", "yes" );// <<< write:Priority:yes
      printf("\x1B[32mSending as \x1B[33mRegular\x1B[32m SMS with \x1B[33mLow Priority!\x1B[39m and getting Report\n");
      }
      else{
      printf("\x1B[32mSending as \x1B[33mRegular\x1B[32m SMS with \x1B[33mLow Priority!\x1B[39m\n");
      }
     
        // Send the message.
         fprintf ( fx, "\n%s \n", b1.c_str() );// <<< write:Message
         // Max. sms per message just Message count
	    
	    if(fclose(fx) == 0){
  //unlink(filename);
	 // Test message?
	
	printf("Message: %s \nTo: %lu \nStatus message: %s\nStatus code: %d\n",b1.c_str(),strtol(pch,NULL,value),"Message OK",2000);
	
	}
    
	pch = strtok (NULL, ",");
	
	  
      } // while end
      } // for end
}

