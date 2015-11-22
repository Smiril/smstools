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

  /* define HOME to be dir for key and cert files... */
#ifdef __linux__
#define HOME "/home/github/smstools/src/"
#elif _WIN32 || _WIN64
#define HOME "C:\\Users\\github\\smstools\\src\\"
#else 
#error "SDK not support your OS!"
#endif

/* Make these what you want for cert & key files */
// openssl req -x509 -days 365 -nodes -newkey rsa:1024 -keyout valid-client-cakey.pem -out valid-client-ca.pem
#define CERTF	HOME	 "valid-client-ca.pem"
#define KEYF	HOME	 "valid-client-cakey.pem"

using namespace std;

int main(int argc , char *argv[])
{
  if (argv[1] == 0){ // argv[1] should be 0 for correct execution
    // We print argv[0] assuming it is the program name
    cout<<"usage: "<< argv[0] <<" 127.0.0.1\n";
    exit(1);
  }
    const char *serv = argv[1];
    int err;
    int sd;
    int read_size;
    struct sockaddr_in sa;
    SSL_CTX* ctx;
    SSL*     ssl;
    X509*    server_cert;
    char*    str;
    char     buf [PATH_MAX];
    char     server_reply[PATH_MAX];
    const SSL_METHOD *meth;

    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    meth = TLSv1_client_method();
    ctx = SSL_CTX_new (meth);                        
    CHK_NULL(ctx);
    /* ----------------------------------------------- */
    /* Load and use CERTF and KEYF for encrypt the connection */
    if (!ctx) {
    printf("There's NO Crypto Method choosen\n");
    exit(2);
    }
    if (SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM) <= 0) {
    printf("PEM Cert File is NOT Valid\n");
    exit(3);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, KEYF, SSL_FILETYPE_PEM) <= 0) {
    printf("PEM CertKey File is NOT Valid\n");
    exit(4);
    }
    if (!SSL_CTX_check_private_key(ctx)) {
    printf("Private key does not match the certificate public key\n");
    exit(5);
    }
    /* ----------------------------------------------- */
    /* Create a socket and connect to server using normal socket calls. */
    //Create socket
    sd = socket(AF_INET , SOCK_STREAM , 0);
    if (sd == -1)
    {
        printf("Could not create socket");
    }
    CHK_ERR(sd, "socket");
    puts("Socket created");
    
    memset(&sa, 0, sizeof(sa)); 
    sa.sin_addr.s_addr = inet_addr(serv);
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
    ssl = SSL_new(ctx);                         
    CHK_NULL(ssl);    
    SSL_set_fd(ssl, sd);
    err = SSL_connect(ssl);                    
    CHK_SSL(err);
    
    /* Get the cipher - opt */

    printf ("SSL connection using %s\n", SSL_get_cipher (ssl));
  
  /* Get server's certificate (note: beware of dynamic allocation) - opt */

    server_cert = SSL_get_peer_certificate (ssl);       
    CHK_NULL(server_cert);
    printf ("Server certificate:\n");
  
    str = X509_NAME_oneline (X509_get_subject_name (server_cert),0,0);
    CHK_NULL(str);
    printf ("\t subject: %s\n", str);
    OPENSSL_free (str);

    str = X509_NAME_oneline (X509_get_issuer_name (server_cert),0,0);
    CHK_NULL(str);
    printf ("\t issuer: %s\n", str);
    OPENSSL_free (str);

  /* We could do all sorts of certificate verification stuff here before
     deallocating the certificate. */

    X509_free (server_cert);
  
  /* --------------------------------------------------- */
  /* DATA EXCHANGE - Send a message and receive a reply. */
  
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
