/* 
 * 	Description:	get and read or delete SMS in Incoming Folder 
 */ 
    #include <dirent.h>
    #include <cstdlib>
    #include <iostream>
    #include <fstream>
    
    std::string core1 = ".";
    std::string core2 = "..";
    
    using namespace std;
    
    const char* Version() {
    #ifdef VERSION
      return VERSION;
    #else
      return "";
    #endif
    }
    int main () {
	printf("%s\n",Version());
        const char *TROZ = "/var/spool/sms/incoming/";;
        
        struct dirent *pDirent;
        DIR *pDir;
       pDir = opendir (TROZ);
        if (pDir == NULL) {
            printf ("Cannot open directory '%s'\n", TROZ);
            return 1;
        }

        while ((pDirent = readdir(pDir)) != NULL){ 
	if(pDirent->d_name != core1 && pDirent->d_name != core2)
	{
    printf ("\x1B[32m%s\n", pDirent->d_name);
    char START[512];
    snprintf(START,512,"%s%s",TROZ, pDirent->d_name);
    std::string STRING;
    ifstream myfile;
    myfile.open(START, ios::in);
    int a=0;
    string previousLine="";
    while(a<1) // To get you all the lines.
    {
      for(int i = 0;i < 13;i++){
        getline(myfile,STRING); // Saves the line in STRING.
        if (STRING != previousLine)
        {
            previousLine=STRING;
            cout<<STRING<<endl; // Prints our STRING.
        }
      }
     myfile.close(); 
     string delx = "";
     string delc = "y";
     printf("\n\x1B[33mDelete sms? (y/n): \x1B[39m");
     getline(cin,delx);
     if(delx.c_str() == delc){
       std::remove(START);
       //DeleteFile(START);
     }
     else {
      printf("Leave in Inbox ...\n");
     }
     break;
    } // if end 
	}
	else {
	continue;
	} // else end 
   }
   closedir(pDir);
   return 0;
    }
