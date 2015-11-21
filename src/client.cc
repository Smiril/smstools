/*
    C ECHO client example using sslsockets
    sending some strings for auth and message
    TLS SSL Auth Mechamism is used for this
*/
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <memory.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <memory>
#include <errno.h>
#include <sstream>
#include <openssl/rsa.h>
#include <openssl/crypto.h> // link with -lcrypto
#include <openssl/tls1.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h> // link with -lssl
#include <openssl/err.h>
#define PATH_MAX        4096    /* # chars in a path name including nul */

#define CHK_NULL(x) if ((x)==NULL) exit (1)
#define CHK_ERR(err,s) if ((err)==-1) { perror(s); exit(1); }
#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(2); }

using namespace std;

int main(int argc , char *argv[])
{
  int err;
  int sd;
  int read_size;
  struct sockaddr_in sa;
  SSL_CTX* ctx;
  SSL*     ssl;
  X509*    server_cert;
  char*    str;
  char     buf [PATH_MAX];
  char 	   server_reply[PATH_MAX];
  const SSL_METHOD *meth;

  OpenSSL_add_ssl_algorithms();
  meth = TLSv1_client_method();
  SSL_load_error_strings();
  ctx = SSL_CTX_new (meth);                        
  CHK_NULL(ctx);

  /* ----------------------------------------------- */
  /* Create a socket and connect to server using normal socket calls. */

    
    //Create socket
    sd = socket(AF_INET , SOCK_STREAM , 0);
    if (sd == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    CHK_ERR(sd, "socket");
 
    memset(&sa, 0, sizeof(sa)); 
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");
    sa.sin_family = AF_INET;
    sa.sin_port = htons( 1131 );              
  
 
    //Connect to remote server
    if ( err = connect(sd , (struct sockaddr *)&sa , sizeof(sa)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    CHK_ERR(err, "connect"); 
    puts("Connected");
/* Now we have TCP conncetion. Start SSL negotiation. */
  
  ssl = SSL_new (ctx);                         
  CHK_NULL(ssl);    
  SSL_set_fd (ssl, sd);
  err = SSL_connect (ssl);                    
  CHK_SSL(err);
    
  /* Following two steps are optional and not required for
     data exchange to be successful. */
  
    while(read_size = SSL_read(ssl, server_reply, sizeof(server_reply) ) > 0){
	
        CHK_SSL(read_size);
	printf("%s",server_reply);
	// PAM AUTH USER DETAILS
	err = SSL_write (ssl, "user", strlen("user")); 
	CHK_SSL(err);
	printf("sending username\n");
	//send(sd , "user" , strlen("user") , 0);
	err = SSL_write (ssl, "password", strlen("password")); 
	CHK_SSL(err);
	printf("sending password\n");
	//send(sd , "password" , strlen("password") , 0);
	//PAM AUTH END
	err = SSL_write (ssl, "43676xxxxxxx,43676xxxxxxx\ntest message\ny\ny\nn\n", strlen("43676xxxxxxx,43676xxxxxxx\ntest message\ny\ny\nn\n")); 
	CHK_SSL(err);
	printf("sending sms\n");
	//send(sd , "43676xxxxxxx,43676xxxxxxx\ntest message\ny\ny\nn\n" , strlen("43676xxxxxxx,43676xxxxxxx\ntest message\ny\ny\nn\n") , 0);
		
	if(read_size == 0)
	{
        printf("Server disconnect's");
        //fflush(stdout);
	break;
	}
	else if(read_size == -1)
	{
        perror("recv failed");
	}
    } //while recv end

    close(sd);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    return 0;
}
