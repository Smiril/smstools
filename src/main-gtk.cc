#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <memory>
#include <errno.h>
#include <sstream>
#include <fstream>
#include <gtk/gtk.h>

  #ifdef __linux__
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
  #error "not support your OS!"
  #endif

using namespace std;

const char* Versionx() {
#ifdef VERSION
  return VERSION;
#else
  return "";
#endif
}

char *text = NULL;

std::string a = "";
std::string b = "";
std::string c = "";
std::string d = "";
std::string dx = "";
std::string dd = "";
std::string dx1 = "y";
std::string dd1 = "y";
std::string dx2 = "n";
std::string dd2 = "n";
std::string aa = "Send as Flash SMS";
std::string bb = "Send with Priority";

GtkWidget *CreateCheckBox (GtkWidget *box, char *szLabel)
{
    GtkWidget *check;

    /* --- Get the check button --- */
    check = gtk_check_button_new_with_label (szLabel);

    /* --- Pack the checkbox into the vertical box (box). --- */
    gtk_box_pack_start (GTK_BOX (box), check, FALSE, FALSE, 2);

    /* --- Show the widget --- */
    gtk_widget_show (check);

    return (check);
}

void flashsms(GtkWidget *, gpointer *datac){
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(datac)))
       dx = dx1;
   else
    dx = dx2;
}

void prioritysms(GtkWidget *, gpointer *datad){
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(datad)))
       dd = dd1;
   else
    dd = dd2;
}

void about(GtkButton *,gpointer *){
    	GtkWidget *aboutx;
	char *weabout = NULL;
	weabout = g_strdup_printf("Version: %s \n\n\n\n		Developed 2015-Sept\n\nWWW: https://github.com/Smiril/smstools/",Versionx());
        aboutx = gtk_message_dialog_new_with_markup(NULL,GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, weabout );
	gtk_window_set_title(GTK_WINDOW(aboutx), "About");
	gtk_dialog_run(GTK_DIALOG(aboutx));
	gtk_widget_destroy( GTK_WIDGET(aboutx) );
}

void getnumberx(GtkButton *,gpointer dash11)
{
  a = gtk_entry_get_text(GTK_ENTRY(dash11));
}

void getmsgx(GtkButton *,gpointer dash22)
{
  b = gtk_entry_get_text(GTK_ENTRY(dash22));
}

void getuserx(GtkButton *,gpointer dash33)
{
  c = gtk_entry_get_text(GTK_ENTRY(dash33));
}

void getpassx(GtkButton *,gpointer dash44)
{
  d = gtk_entry_get_text(GTK_ENTRY(dash44));
}
void change_label(GtkButton *,gpointer label)
{
    gtk_label_set_text(GTK_LABEL(label), text);
}
 
void change_text(GtkButton *,gpointer numberx)
{	 
      g_free(text);
      text = g_strdup(gtk_entry_get_text(GTK_ENTRY(numberx)));
}  

void sendsms()
{
      const char *pch;
      int value = 0;
      pch = strtok ((char *)a.c_str(),",");
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
	char filename[] = "/var/spool/sms/GSM/GSM1/smstools-gtk.XXXXXX"; // template for our file.        
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
         fprintf ( fx, "\n%s \n", b.c_str() );// <<< write:Message
         // Max. sms per message just Message count
	    
	    if(fclose(fx)){
  //unlink(filename);
	 // Test message?
	GtkWidget *msgboxxx;
	char *wexx = NULL;
	wexx = g_strdup_printf("Message: %s \n\nTo Number: %lu \n\nStatus message: %s\nStatus code: %d\n",b.c_str(),strtol(pch,NULL,value),"Message: OK",2000);
        msgboxxx = gtk_message_dialog_new_with_markup(NULL,GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, wexx );
	gtk_window_set_title(GTK_WINDOW(msgboxxx), "INFO");
	gtk_dialog_run(GTK_DIALOG(msgboxxx));
	gtk_widget_destroy( GTK_WIDGET(msgboxxx) );
	fprintf(stderr,"Message: %s \n\nTo Number: %lu \n\nStatus: %s \n",b.c_str(),strtol(pch,NULL,value),"Message OK");
	}
	pch = strtok (NULL, ",");
      } // while end
      } // for end
}

int main(int argc, char *argv[]) {
    //freopen( "/var/log/Smiril-websms-error.log", "a+", stderr );
    
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *label,*label1,*label2;
    GtkWidget *button,*buttonx,*button2;
    GtkWidget *number;
    GtkWidget *msg;
    GtkWidget *check1;
    GtkWidget *check2;
    gtk_init(&argc, &argv);
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Smiril-smstools");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 250);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    //gtk_window_set_icon(GTK_WINDOW(window), gtk_xpm);
    gtk_window_has_toplevel_focus(GTK_WINDOW(window));
    
    vbox = gtk_vbox_new(FALSE, 2);
    gtk_container_add(GTK_CONTAINER(window), vbox);
 
    label = gtk_label_new("Give me Data!");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    
    label1 = gtk_label_new("Number");
    gtk_box_pack_start(GTK_BOX(vbox), label1, FALSE, FALSE, 0);
    number = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(number),201330687);
    gtk_box_pack_start(GTK_BOX(vbox), number, FALSE, FALSE, 0);
    
    label2 = gtk_label_new("Text Message");
    gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 0);
    msg = gtk_entry_new_with_max_length(155);
    gtk_box_pack_start(GTK_BOX(vbox), msg, FALSE, FALSE, 0);
    
    check1 = CreateCheckBox (vbox, (char *)aa.c_str());
    //gtk_box_pack_start(GTK_BOX(vbox), check1, FALSE, FALSE, 0);
    gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (check1), FALSE);
 
    check2 = CreateCheckBox (vbox, (char *)bb.c_str());
    //gtk_box_pack_start(GTK_BOX(vbox), check2, FALSE, FALSE, 0);
    gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (check2), FALSE);
 

    button = gtk_button_new_with_label("Send");
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
    
    button2 = gtk_button_new_with_label("About");
    gtk_box_pack_start(GTK_BOX(vbox), button2, FALSE, FALSE, 0);
    
    buttonx = gtk_link_button_new_with_label("http://127.0.0.1","Visit the SMS Provider");
    gtk_box_pack_start(GTK_BOX(vbox), buttonx, FALSE, FALSE, 0);
     
    fprintf(stderr,"Status message: %s \n", strerror(errno));
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(button, "clicked", G_CALLBACK(change_text), number);
    g_signal_connect(button, "clicked", G_CALLBACK(change_label), label);
    g_signal_connect(button, "clicked", G_CALLBACK(getnumberx), number);
    g_signal_connect(button, "clicked", G_CALLBACK(getmsgx), msg);
    g_signal_connect(button, "clicked", G_CALLBACK(flashsms), check1);
    g_signal_connect(button, "clicked", G_CALLBACK(prioritysms), check2);
    g_signal_connect(button, "clicked", G_CALLBACK(sendsms), NULL);
    g_signal_connect(button2, "clicked", G_CALLBACK(about), NULL);
    
    gtk_widget_show_all(window);
 
    gtk_main();
    
    g_free(text);
    
    return 0;
} 
