#ifndef SENDER_WINDOW_H
#define SENDER_WINDOW_H
#include <gtk/gtk.h>
//настройка отправителей
class SenderWindow
{
	////////////////////////////////
	static const int count = 4;
	GtkWidget *box[count];
	GtkWidget* senderLabel;
	GtkWidget* serverLabel;
	GtkWidget* userLabel;
	GtkWidget* passLabel;
	/////////////////////////////////
	GtkWidget* okButton;
	GtkWidget* cancelButton;
public:
	///////////////
	GtkWidget *window;
	/////////////////////
	GtkWidget* senderEdit;
	GtkWidget* serverEdit;
	GtkWidget* userEdit;
	GtkWidget* passEdit;
	SenderWindow();
	~SenderWindow();
	void clear();
};
//"Готово"
void sender_ok_signal(GtkWidget* b, gpointer data);
//"Отмена"
void sender_cancel_signal(GtkWidget* b, gpointer data);
void sender_destroy_signal(GtkWidget* b, gpointer data);
#endif
