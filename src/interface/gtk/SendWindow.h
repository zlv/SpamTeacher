#ifndef SEND_WINDOW_H
#define SEND_WINDOW_H
#include <gtk/gtk.h>
//ÑÄÑÑÑÅÑÇÑpÑrÑyÑÑÑé
class SendWindow
{
    static const int count = 2;
    GtkWidget *box[count];
    /////////////////////////////////
    GtkWidget *statusLabel;
    GtkWidget *progress;
    /////////////////////////////////
public:
    GtkWidget *window;
    SendWindow();
    ~SendWindow();
};
void send_destroy_signal(GtkWidget* b, gpointer data);
#endif
