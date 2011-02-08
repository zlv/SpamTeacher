#include "SendWindow.h"
#include "../func.h"
#include "gui.h"
#include "../../Data.h"
SendWindow::SendWindow()
{
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Отправка сообщений");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    /////////////////////////////////
    box[0] = gtk_hbox_new(0,5);
    box[1] = gtk_hbox_new(0,15);
    /////////////////////////////////
    gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(send_destroy_signal), NULL);
    /////////////////////////////////////
    statusLabel = new_label("Отправка сообщений: ",NULL);
    progress = gtk_progress_bar_new();
    /////////////////////////////////////
    gtk_box_pack_start(GTK_BOX(box[0]),statusLabel,0,0,5);
    gtk_box_pack_start(GTK_BOX(box[1]),progress,0,0,5);
    /////////////////////////////////////
    GtkWidget *vbox = gtk_vbox_new(0,5);
    for (int i=0; i<count; i++)
    {
        gtk_box_pack_start(GTK_BOX(vbox),box[i],1,0,5);
    }
    
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    gtk_widget_show_all(window);
    data->start(progress,statusLabel);
}

SendWindow::~SendWindow()
{
    data->stop();
}

void send_destroy_signal(GtkWidget* b, gpointer data)
{
    delete sendWindow;
    sendWindow = NULL;
}
