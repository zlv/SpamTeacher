#include "AddReceiverWindow.h"
#include "../func.h"
AddReceiverWindow::AddReceiverWindow()
{
    window = gtk_window_new(    GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(       GTK_WINDOW(window), "Добавить получателя");
    gtk_window_set_position(    GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    /////////////////////////////////
    box[0] = gtk_hbox_new(0,5);
    box[1] = gtk_hbox_new(0,5);
    box[2] = gtk_hbox_new(0,15);
    /////////////////////////////////
    nameEdit =    new_edit ("Имя");
    senderEdit =  new_edit ("Почта");
    nameLabel =   new_label("Имя получателя");
    senderLabel = new_label("Почтовый адрес получателя");
    /////////////////////////////////
    okButton =     new_button("Готово");
    cancelButton = new_button("Отмена");
    g_signal_connect(GTK_OBJECT(okButton),     "clicked", GTK_SIGNAL_FUNC(add_receiver_ok_signal),     NULL);
    g_signal_connect(GTK_OBJECT(cancelButton), "clicked", GTK_SIGNAL_FUNC(add_receiver_cancel_signal), NULL);
    /////////////////////////////////////
    gtk_box_pack_start(GTK_BOX(box[0]), nameLabel,   0,0,5);
    gtk_box_pack_start(GTK_BOX(box[0]), nameEdit,    0,0,5);
    gtk_box_pack_start(GTK_BOX(box[1]), senderLabel, 0,0,5);
    gtk_box_pack_start(GTK_BOX(box[1]), senderEdit,  0,0,5);
    gtk_box_pack_start(GTK_BOX(box[2]), okButton,    1,1,5);
    gtk_box_pack_start(GTK_BOX(box[2]), cancelButton,1,1,5);
    /////////////////////////////////////
    GtkWidget *vbox = gtk_vbox_new(0,5);
    for (int i=0; i<count; i++)
    {
        gtk_box_pack_start(GTK_BOX(vbox), box[i], 1,0,5);
    }
    
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    gtk_widget_show_all(window);
}
////////////////////////////////////
AddReceiverWindow::~AddReceiverWindow()
{
    gtk_widget_destroy(window);
}
