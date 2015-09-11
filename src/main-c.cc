#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <memory>
#include <errno.h>
#include <sstream>


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
  
using namespace std;


const char* Versionx() {
#ifdef VERSION
  return VERSION;
#else
  return "";
#endif
}

void iam()
{
  	  printf(".------..------..------..------..------..------.\n");
	  printf("|S.--. ||M.--. ||I.--. ||R.--. ||I.--. ||L.--. |\n");
	  printf("| '--'S|| '--'M|| '--'S|| '--TO|| '--OL|| '--'S|\n");
	  printf("`------'`------'`------'`------'`------'`------'\n");
}

void sendsms(std::string a1,std::string b1,std::string dx,std::string dd)
{     
      const char *pch;
      int value = 0;
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
	   printf("Error make TMP-Filename\n");
	   exit(1);
	}          // Check we managed to open the file.
	close(fd);
	fx = fopen (filename,"w+");                        // Xs are replaced with a unique number.
        if (fx == NULL){
	   printf("Error write to Filename\n");
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
      if(dx.c_str() == god && dd.c_str() != devil)
      {
      fprintf ( fx, "Flash: %s \n", "yes" ); // <<< write:Flash:yes
      printf("\x1B[32mSending as \x1B[33mFlash\x1B[32m SMS with \x1B[33mLow Priority!\x1B[39m\n");
      }
      else if(dx.c_str() == god && dd.c_str() == devil){
      fprintf ( fx, "Priority: %s \n", "yes" );
      fprintf ( fx, "Flash: %s \n", "yes" );// <<< write:Flash:yesPriority:yes
      printf("\x1B[32mSending as \x1B[33mFlash\x1B[32m SMS with \x1B[33mHigh Priority!\x1B[39m\n");
      }
      else if(dx.c_str() != god && dd.c_str() == devil){
      fprintf ( fx, "Priority: %s \n", "yes" );// <<< write:Priority:yes
      printf("\x1B[32mSending as \x1B[33mRegular\x1B[32m SMS with \x1B[33mHigh Priority!\x1B[39m\n");
      }
      else{
      printf("\x1B[32mSending as \x1B[33mRegular\x1B[32m SMS with \x1B[33mLow Priority!\x1B[39m\n");
      }
     
        // Send the message.
         fprintf ( fx, "\n%s \n", b1.c_str() );// <<< write:Message
         // Max. sms per message just Message count
	    
	    fclose(fx);
  //unlink(filename);
	 // Test message
    
	pch = strtok (NULL, ",");
	
	  
      } // while end
      } // for end
}
	

int main(/*int argc, char *argv[]*/) {
	freopen( "/var/log/Smiril-websms-error.log", "a+", stderr );
	printf("%s \n",Versionx());
	int x = 0;  // Don't forget to declare variables
	printf("\x1B[33mLoading...");
	while ( x < 11 ) { // While x is less than 11 
	printf("%d",x);
	x++;             // Update x so the condition can be met eventually
	}
	//sleep(1);
	printf(" \x1B[32mFinished\x1B[39m\n");
	// Uncomment version you want!
  // v.1.4
	Again:
	std::string a = "";
	std::string b = "";
	std::string e = "n";
	std::string f = "n";
	std::string h = "n";
	std::string hh = "y";
	
	printf("Number: \x1B[32m");
	getline(cin,a);
	int valx = 0;
	printf("Number: %s\n",a.c_str());
	if(strtol(a.c_str(),NULL,valx) == 0){
	  printf("\x1B[31mERROR Number! \x1B[39m\n");
	  iam();
	  exit(0);
	}
	printf("\x1B[39mMessage: \x1B[33m");
	getline(cin,b);
	printf("\x1B[32mSend as Flash SMS? (y/n) Default n : \x1B[39m");
	getline(cin,e);
	printf("\x1B[32mSend as Priority SMS? (y/n) Default n : \x1B[39m");
	getline(cin,f);
	
	sendsms(a,b,e,f);
	
	printf("\x1B[32mSend again SMS? (y/n) Default n : \x1B[39m");
	getline(cin,h);
	
	if(h.c_str() == hh)
	{
	  goto Again;
	}
	else{
	  iam();
	  exit(0);
	}
	
	return 0;
}

