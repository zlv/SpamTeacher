#ifndef GENDER_WINDOW_H
#define GENDER_WINDOW_H
#include <gtk/gtk.h>
class GenderWindow
{
	static const int count = 3;
	GtkWidget *box[count];
	GtkWidget* maleLabel;
	GtkWidget* femaleLabel;
	/////////////////////////////////
	GtkWidget* okButton;
	GtkWidget* cancelButton;
public:
	GtkWidget *window;
	GtkWidget* maleEdit;
	GtkWidget* femaleEdit;
	GenderWindow();
	~GenderWindow();
};
void gender_ok_signal(GtkWidget* b, gpointer data);
void gender_cancel_signal(GtkWidget* b, gpointer data);
void gender_destroy_signal(GtkWidget* b, gpointer data);
#endif
