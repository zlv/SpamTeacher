#ifndef ADD_RECEIVER_WINDOW_H
#define ADD_RECEIVER_WINDOW_H
#include <gtk/gtk.h>
//окно для добавления элемента в список получателей
class AddReceiverWindow
{
    GtkWidget *window;
    static const int count = 3;
    GtkWidget *box[count];
    GtkWidget* nameLabel;
    GtkWidget* senderLabel;
    /////////////////////////////////
    GtkWidget* okButton;
    GtkWidget* cancelButton;
public:
    GtkWidget* nameEdit;
    GtkWidget* senderEdit;
    AddReceiverWindow();
    ~AddReceiverWindow();
};
void add_receiver_ok_signal(GtkWidget* b, gpointer data);
void add_receiver_cancel_signal(GtkWidget* b, gpointer data);
#endif
