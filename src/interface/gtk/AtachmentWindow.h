#ifndef ATACH_WINDOW_H
#define ATACH_WINDOW_H
#include <gtk/gtk.h>
//настройка отправителей
class AtachmentWindow
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
    GtkWidget *window;
    //список
    GtkWidget* store;
    //модель списка
    GtkTreeModel* model;
    AtachmentWindow(GtkWidget*);
    ~AtachmentWindow();
    void clear();
    ////////////////////////////////
    GtkWidget* mainWindow;
};
//все необходимые сигналы
void atach_celled_signal(GtkCellRendererText*, const gchar*, const gchar*, gpointer data);
void atach_add_signal(GtkWidget* b, gpointer data);
void atach_del_signal(GtkWidget* b, gpointer data);
//"Готово"
void atach_ok_signal(GtkWidget* b, gpointer data);
//"Отмена"
void atach_cancel_signal(GtkWidget* b, gpointer data);
void atach_destroy_signal(GtkWidget* b, gpointer data);
#endif
