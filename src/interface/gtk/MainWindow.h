#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <gtk/gtk.h>
//главное окно
class MainWindow
{
	GtkWidget *window;
	//разные контейнеры
	GtkWidget *box[4];
	GtkWidget *vbox;
	////////////////////
	GtkWidget *senderButton,*receiverButton,*atachButton,*messageButton;
	////////////////////
	GtkWidget* get_menubar_menu(GtkWidget *window);
public:
	////////////////////
	GtkWidget *messageEdit,*senderEdit,*receiverEdit,*atachEdit;
	GtkWidget *themeEdit;
	MainWindow();
	void clear();
};
void clear_signal(GtkWidget* b, gpointer data);
#endif
