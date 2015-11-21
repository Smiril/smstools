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
#include <memory.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h> //for threading , link with -lpthread
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <memory>
#include <errno.h>
#include <sstream>
#include <sys/stat.h>
#include <syslog.h>


  #ifdef __linux__
  #include <openssl/crypto.h> // link with -lopenssl
  #include <openssl/x509.h> // link with -lopenssl
  #include <openssl/pem.h> // link with -lopenssl
  #include <openssl/ssl.h> // link with -lopenssl
  #include <openssl/err.h> // link with -lopenssl
  #include <fstream>
  #include <security/pam_appl.h> // link with -lpam
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
    
  int err;
  int socket_desc;
  struct sockaddr_in sa;
  SSL_CTX* ctx;
  SSL*     ssl;
  X509*    server_cert;
  char*    str;
  char     buf [4096];
  SSL_METHOD *meth;
  /* ----------------------------------------------- */
#define CHK_NULL(x) if ((x)==NULL) exit (1)
#define CHK_ERR(err,s) if ((err)==-1) { perror(s); exit(1); }
//#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(2); }

  //the thread function
void *connection_handler(void *);
void sendsms(std::string a1,std::string b1,std::string dx,std::string dd,std::string dc);
const char* Versionx() {
#ifdef VERSION
  return VERSION;
#else
  return "";
#endif
}
struct pam_response *reply;

//function used to get user input
int function_conversation(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr)
{
  *resp = reply;
  return PAM_SUCCESS;
}

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
    
     int client_sock , c , *new_sock;
    struct sockaddr_in server , client;
  OpenSSL_add_ssl_algorithms();
  meth = NULL;//TLS_client_method();
  SSL_load_error_strings();
  ctx = SSL_CTX_new (meth);                        
  CHK_NULL(ctx);
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        syslog (LOG_NOTICE, "Could not create socket");
        printf("Could not create socket");
    }
    syslog (LOG_NOTICE, "Socket created");
    puts("Socket created");
    CHK_ERR(socket_desc, "socket");
    memset(&sa, 0, sizeof(sa));
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 1131 );
     
    //Bind
    if( bind(socket_desc, (struct sockaddr*) &sa,sizeof(sa)) < 0)
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
 err = connect(socket_desc, (struct sockaddr*) &sa,sizeof(sa));                   
 CHK_ERR(err, "connect");

  /* ----------------------------------------------- */
  /* Now we have TCP conncetion. Start SSL negotiation. */
  
  ssl = SSL_new (ctx);                         
  CHK_NULL(ssl);    
  SSL_set_fd(ssl, socket_desc);
  err = SSL_connect(ssl);                     
  //CHK_SSL(err);
    
  /* Following two steps are optional and not required for
     data exchange to be successful. */
  
  /* Get the cipher - opt */
  char *chip;
  sprintf (chip,"SSL connection using %s\n", SSL_get_cipher (ssl));
  syslog (LOG_NOTICE, chip);
  /* Get server's certificate (note: beware of dynamic allocation) - opt */

  server_cert = SSL_get_peer_certificate (ssl);       
  CHK_NULL(server_cert);
  char *cert;
  sprintf (cert,"Server certificate:\n");
  syslog (LOG_NOTICE, cert);
 /* 
  str = X509_NAME_oneline (X509_get_subject_name (server_cert),0,0);
  CHK_NULL(str);
  char *subj;
  sprintf (subj,"subject: %s\n", str);
  syslog (LOG_NOTICE, subj);
  OPENSSL_free (str);

  str = X509_NAME_oneline (X509_get_issuer_name  (server_cert),0,0);
  CHK_NULL(str);
  char *issu;
  sprintf (issu,"issuer: %s\n", str);
  syslog (LOG_NOTICE, issu);
  OPENSSL_free (str);
*/
  /* We could do all sorts of certificate verification stuff here before
     deallocating the certificate. */

  //X509_free (server_cert);
  
  /* --------------------------------------------------- */
    //Accept and incoming connection
    syslog (LOG_NOTICE, "Waiting for incoming connections...");
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = SSL_accept(ssl) ))
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

    syslog (LOG_NOTICE, "SMS Daemon terminated.");
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
    char bufferA[PATH_MAX];
    char bufferB[PATH_MAX];
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
	char vers[512];
    snprintf(vers,512,"Greetings! I am your %s\n",Versionx());
    SSL_write(ssl, vers, strlen(vers));
    //write(sock , vers , strlen(vers));
    SSL_read(ssl, (char *)bufferA, PATH_MAX);
    //read(sock , bufferA , PATH_MAX );
  const char *username;
  username = bufferA;

  const struct pam_conv local_conversation = { function_conversation, NULL };
  pam_handle_t *local_auth_handle = NULL; // this gets set by pam_start

  int retval;

  // local_auth_handle gets set based on the service
  retval = pam_start("common-auth", username, &local_conversation, &local_auth_handle);

  if (retval != PAM_SUCCESS)
  {
    std::cout << "pam_start returned " << retval << std::endl;
    return 0;
  }

  SSL_read(ssl, (char *)bufferB, PATH_MAX);
  //read(sock , bufferB , PATH_MAX );
  reply = (struct pam_response *)malloc(sizeof(struct pam_response));

  // *** Get the password by any method, or maybe it was passed into this function.
  reply[0].resp = bufferB;
  reply[0].resp_retcode = 0;

  
  retval = pam_authenticate(local_auth_handle, 0);

  if (retval != PAM_SUCCESS)
  {
    if (retval == PAM_AUTH_ERR)
    {
      std::cout << "Authentication failure." << std::endl;
    }
    else
    {
      std::cout << "pam_authenticate returned " << retval << std::endl;
    }
    return 0;
  }
    //Send some messages to the client
    std::string messagegx = "\x1B[31mAuthenticated.\x1B[39m\n";
    SSL_write(ssl, messagegx.c_str(), strlen(messagegx.c_str()));
    //write(sock , messagegx.c_str() , strlen(messagegx.c_str()));
    
  retval = pam_end(local_auth_handle, retval);

  if (retval == PAM_SUCCESS)
  {
    //Send some messages to the client
    std::string messagegs = "\x1B[32mSuccess.\x1B[39m\n";
    SSL_write(ssl, messagegs.c_str(), strlen(messagegs.c_str()));
  }
  else{
    //Send some messages to the client
    std::string messagegy = "\x1B[33mFailure.\x1B[39m\n";
    SSL_write(ssl, messagegy.c_str(), strlen(messagegy.c_str()));
    //write(sock , messagegy.c_str() , strlen(messagegy.c_str()));
    
    close(sock);  
    //Free the socket pointer
    free(socket_desc);
      SSL_free (ssl);
      SSL_CTX_free (ctx);
      
    return 0;
      }
      
      //Receive a message from client
      while( (read_size = SSL_read(ssl, (char *)buffer, PATH_MAX)) > 0 )
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
	SSL_write(ssl, messagegood.c_str(), strlen(messagegood.c_str()));
	//write(sock , messagegood.c_str() , strlen(messagegood.c_str()));	
	close(sock);
	}
	else{
	  //Send Get Client version
	std::string messagegoodx = "ERROR\n\0";
	SSL_write(ssl, messagegoodx.c_str(), strlen(messagegoodx.c_str()));
	//write(sock , messagegoodx.c_str() , strlen(messagegoodx.c_str()));	
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
      SSL_free (ssl);
      SSL_CTX_free (ctx);
      
    return 0;  
    //return 0;
}

void sendsms(std::string a1,std::string b1,std::string dx,std::string dd,std::string dc)
{     
      const char *pch;
      char *value; 
      long ret;
      
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
	printf("Message: %s \nTo: %s \nStatus message: %s\nStatus code: %d\n",b1.c_str(),pch,"Error make TMP-Filename",4010);
	exit(1);
	}          // Check we managed to open the file.
	close(fd);
	fx = fopen (filename,"w+");                        // Xs are replaced with a unique number.
        if (fx == NULL){
	printf("Message: %s \nTo: %s \nStatus message: %s\nStatus code: %d\n",b1.c_str(),pch,"Error could not Write",4030);
	exit(1);
	}          // Check we managed to open the file.
	
	if (fx != NULL){
        #ifdef __linux__
	fprintf ( fx, "To: %s \n", pch );// <<< write to:
        #elif _WIN32 || _WIN64
	fprintf ( fx, "To: %s \n", pch );// <<< write to:
        #else 
	#error "OS not supported!"
	#endif
	
      printf("Outgoing SMS ... %s\n",pch );
      
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
	
	printf("Message: %s \nTo: %s \nStatus message: %s\n Status code: %d\n",b1.c_str(),pch,"Message OK",2000);
	
	}
	} // if file not null to force
	pch = strtok (NULL, ",");
	
	  
      } // while end
      } // for end
}

