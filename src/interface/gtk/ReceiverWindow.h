#ifndef RECEIVER_WINDOW_H
#define RECEIVER_WINDOW_H
#include <gtk/gtk.h>
class ReceiverWindow
{
	static const int count = 2;
	GtkWidget *box[count];
	/////////////////////////////////
	GtkWidget* addButton;
	GtkWidget* delButton;
	/////////////////////////////////
	GtkWidget* okButton;
	GtkWidget* cancelButton;
	/////////////////////////////////
	//создание модели списка
	GtkTreeModel* create_and_fill_model();
	//представления списка (должно вызываться после создания модели)
	GtkTreeView* create_and_fill_store();
public:
	/////////////////////////////////
	GtkWidget *window;
	//список
	GtkWidget* store;
	//модель списка
	GtkTreeModel* model;
	ReceiverWindow();
	~ReceiverWindow();
	void clear();
};
//содержание
enum
{
	COL_NAME,
	COL_ADRESS,
	COL_MALE,
	COL_ENABLED,
	NUM_COLS
};
//строки
const char titles[][30+1] = {"Имя","Почта","Мужчина?","Включен"};
//содержание
const int collist[NUM_COLS] = {COL_NAME, COL_ADRESS, COL_MALE, COL_ENABLED};
//все необходимые сигналы
void receiver_ok_signal(GtkWidget* b, gpointer data);
void receiver_cancel_signal(GtkWidget* b, gpointer data);
void receiver_toggle_signal(GtkCellRendererToggle*, gchar*, gpointer data);
void receiver_add_signal(GtkWidget* b, gpointer data);
//////////////////////////
void receiver_destroy_signal(GtkWidget* b, gpointer data);

#endif
