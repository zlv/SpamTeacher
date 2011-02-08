#ifndef ABOUT_H
#define ABOUT_H
#include <gtk/gtk.h>
class About
{
    GtkWidget *window;
    //поле, в котором размещается содержимое README, field that contains text from README
    GtkWidget *messageEdit;
    /////////////////////////////////
public:
    About();
    ~About();
};
void about_destroy_signal(GtkWidget* b, gpointer data);
#endif
