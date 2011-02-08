#include "About.h"
#include <fstream>
#include <string>
const int MAX=200;
const int SUPERMAX=100000;
#ifndef _WIN32
    char readme[][MAX+1] = {"README","/usr/local/share/MassDelivery/doc/README"};
#else
    char readme[][MAX+1] = {"Readme.txt"};
#endif
About::About()
{
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "О программе");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(about_destroy_signal), NULL);
    
    //чтение из файла
    std::string text;
    std::ifstream file;
    int size = sizeof(readme)/sizeof(readme[0]);
    for (int i=0; i<size; i++)
    {
        file.open(readme[i]);
        if (file.good()) break;
    }
    char buf[SUPERMAX+1];
    file.getline(buf,SUPERMAX,'\n');
    text=buf;
    while (file.good())
    {
        file.getline(buf,SUPERMAX,'\n');
        if (file.eof()) break;
        text+=std::string("\n")+buf;
    }
    file.close();
    
    //установка текста в поле
    GtkWidget *view = gtk_text_view_new();
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    
    gtk_text_buffer_set_text (buffer, text.c_str(), -1);
    GtkWidget* frame = gtk_frame_new (NULL);
    gtk_frame_set_label (GTK_FRAME (frame), "Из README");
    gtk_container_add(GTK_CONTAINER(frame), view);
    messageEdit = frame;
    
    GtkWidget *vbox = gtk_vbox_new(0,5);
    gtk_box_pack_start(GTK_BOX(vbox),messageEdit,1,0,5);
    
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    gtk_widget_show_all(window);
}
////////////////////////////////////
About::~About()
{
    gtk_widget_destroy(window);
}
