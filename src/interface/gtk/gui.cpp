#include "gui.h"
//разные окна
#include "MainWindow.h"
#include "SenderWindow.h"
#include "GenderWindow.h"
#include "ReceiverWindow.h"
#include "AtachmentWindow.h"
//окно добавления получателя
#include "AddReceiverWindow.h"
//необходимые структуры
#include "Receiver.h"
#include "Data.h"
#include "About.h"
///////////////////////
#include "SendWindow.h"
MainWindow* mainWindow=NULL;
SenderWindow* senderWindow=NULL;
GenderWindow* genderWindow=NULL;
ReceiverWindow* receiverWindow=NULL;
AtachmentWindow* atachWindow=NULL;
AddReceiverWindow* addReceiverWindow=NULL;
About* aboutWindow=NULL;
Data* data;
std::string program;
///////////////////////
SendWindow* sendWindow;
#ifdef _WIN32
#    define PNAME "sendEmail"
#else
#    define PNAME "sendEmail.pl"
#endif
int main (int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    program = argv[0];

    g_thread_init(NULL);
    gdk_threads_init();
    
    data = new Data();
    //"./sendEmail.pl" или "sendEmail" -- sendEmail v1.56 by Brandon Zehm
    data->program = std::string(PNAME);
    mainWindow = new MainWindow();
    
    gdk_threads_enter();
    gtk_main ();
    gdk_threads_leave();

    return 0;
}

//единственное действие при нажатии многих кнопок -- создание окна
//////////////////////////////////////////
void sender_signal(GtkWidget* b, gpointer data)
{
    senderWindow = new SenderWindow();
}

void receiver_signal(GtkWidget* b, gpointer data)
{
    receiverWindow = new ReceiverWindow();
}

void atach_signal(GtkWidget* b, gpointer data)
{    
    atachWindow = new AtachmentWindow();
}

//отправка сообщения
void message_signal(GtkWidget* b, gpointer)
{
    GtkTextBuffer *buffer;

    //получение GtkTextBuffer    
    GtkWidget* view = mainWindow->messageEdit;
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

    //получение строки из GtkTextBuffer
    GtkTextIter *start = new GtkTextIter(),*end = new GtkTextIter();
    gtk_text_buffer_get_start_iter(buffer, start);
    gtk_text_buffer_get_end_iter(buffer, end);
    data->message = gtk_text_buffer_get_text (buffer,start, end, 1);
    
    ////////////////////////////////////
    data->theme = gtk_entry_get_text(GTK_ENTRY(mainWindow->themeEdit));
    
    //посылка сообщения
    //data->send();
    
    sendWindow = new SendWindow();
}

//при нажатии на "Имя"
void name_signal(GtkWidget*, gpointer data)
{
    //получение GtkTextBuffer    
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(mainWindow->messageEdit));
    
    //и вставка в него nameCode
    gtk_text_buffer_insert_at_cursor(buffer,nameCode.c_str(),nameCode.length());
}

//при нажатии на "Пол"
void gender_signal(GtkWidget* b, gpointer data)
{
    genderWindow = new GenderWindow();
}

//закрытие и сохранение данных
void closeApp ( GtkWidget *window, gpointer)
{
    GtkTextBuffer *buffer;
    
    //получение GtkTextBuffer
    GtkWidget* view = mainWindow->messageEdit;
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

    //получение строки из GtkTextBuffer
    GtkTextIter *start = new GtkTextIter(),*end = new GtkTextIter();
    gtk_text_buffer_get_start_iter(buffer, start);
    gtk_text_buffer_get_end_iter(buffer, end);
    data->message = gtk_text_buffer_get_text (buffer,start, end, 1);
    
    ////////////////////////////////////
    data->theme = gtk_entry_get_text(GTK_ENTRY(mainWindow->themeEdit));
    
    //сохранение данных
    data->serial();
    
    //выход
    gtk_main_quit();
}

////////////////////////////////////////////
void gender_cancel_signal(GtkWidget* b, gpointer data)
{
    gtk_widget_destroy(genderWindow->window);
}
void gender_destroy_signal(GtkWidget* b, gpointer data)
{
    delete genderWindow;
    genderWindow = NULL;
}

//при нажатии на "готово" в окне выбора пола
void gender_ok_signal(GtkWidget* b, gpointer data)
{
    //получение строк из виджетов
    std::string sMale = gtk_entry_get_text(GTK_ENTRY(genderWindow->maleEdit));
    std::string sFemale = gtk_entry_get_text(GTK_ENTRY(genderWindow->femaleEdit));
    //удаление окна
    gtk_widget_destroy(genderWindow->window);
    
    //получение текста из шаблона genderCode
    std::string text = get_gender_message(sMale,sFemale,genderCode);
    
    //получение GtkTextBuffer
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(mainWindow->messageEdit));
    //и вставка в него текста
    gtk_text_buffer_insert_at_cursor(buffer,text.c_str(),text.length());
}

//получение информации о выборе текста в зависимости от пола из шаблона t
std::string get_gender_message(std::string m,std::string f,std::string t)
{
    std::string r = t;
    int pos;
    
    pos = r.find("%ms")+2;
    r.replace(pos,1,m);
    pos = r.find("%fs")+2;
    r.replace(pos,1,f);
    return r;
}

void sender_ok_signal(GtkWidget* b, gpointer)
{
    //получение информации из виджетов и запись их в data
    data->senderEmail = gtk_entry_get_text(GTK_ENTRY(senderWindow->senderEdit));
    data->server = gtk_entry_get_text(GTK_ENTRY(senderWindow->serverEdit));
    data->user = gtk_entry_get_text(GTK_ENTRY(senderWindow->userEdit));
    data->password = gtk_entry_get_text(GTK_ENTRY(senderWindow->passEdit));
    //удаление окна
    gtk_widget_destroy(senderWindow->window);

    std::string text;
    
    //получение полной информации об отправителе в text    
    text = data->get_message_about_sender();
    //и вставка этой информации в виджет в главном окне
    gtk_entry_set_text(GTK_ENTRY(mainWindow->senderEdit),text.c_str());
}
#include <iostream>
void sender_cancel_signal(GtkWidget* b, gpointer data)
{    
    gtk_widget_destroy(senderWindow->window);
}

void sender_destroy_signal(GtkWidget* b, gpointer data)
{
    delete senderWindow;
    senderWindow = NULL;
}

//при нажатие "Готово" в окне выбора получателей
void receiver_ok_signal(GtkWidget* b, gpointer m)
{
    //очистка
    data->recvEmail.clear();
    data->recvData.clear();
    
    //итератор для списка
    GtkTreeIter toplevel;
    //информация
    gchar *name,*addr; gboolean male,enab;
    
    //получение итератора первого элемента списка
    bool valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(m), &toplevel);
    
    int count=0;
    
    //пока есть элементы
    while (valid)
    {
        //получение информации
        gtk_tree_model_get(GTK_TREE_MODEL(m), &toplevel,
                COL_NAME, &name,
                COL_ADRESS, &addr,
                COL_MALE, &male,
                COL_ENABLED, &enab,
                -1);
        //увеличение итератора
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(m), &toplevel);
        //запись информации в recvEmail
        data->recvEmail.push_back(addr);
        //и recvData структуры data
        Receiver rec;
        rec.name = name;
        rec.male = male;
        rec.enabled = enab;
        data->recvData.push_back(rec);
        
        count++;
    }
    gtk_widget_destroy(receiverWindow->window);
    
    std::string text;
    
    //вставка информации одной строкой в виджет главного окна
    text = data->get_message_about_receiver();
    gtk_entry_set_text(GTK_ENTRY(mainWindow->receiverEdit),text.c_str());
}

void receiver_cancel_signal(GtkWidget* b, gpointer data)
{    
    gtk_widget_destroy(receiverWindow->window);
}

void receiver_destroy_signal(GtkWidget* b, gpointer data)
{
    delete receiverWindow;
    receiverWindow = NULL;
}
//при нажатии на флажок в списке
void receiver_toggle_signal(GtkCellRendererToggle *cell, gchar* path_str, gpointer data)
{
    //Получение информации о списке в GtkTreeModel
    GtkTreeModel *model = (GtkTreeModel *)data;
    //Получение информации о местанахождении виджета флажка в модели
    GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
    
    //итератор для доступа к виджету
    GtkTreeIter iter;
    //статус
    gboolean toggle_item;
    //колонка (данные пользователя)
    gint* column;
    column = (gint*)(g_object_get_data (G_OBJECT (cell), "column"));

    /* get toggled iter */
    //получение информации о состоянии объекта
    gtk_tree_model_get_iter (model, &iter, path);
    gtk_tree_model_get (model, &iter, column, &toggle_item, -1);

    /* do something with the value */
    //изменить значение на противоположное
    toggle_item ^= 1;

    /* set new value */
    gtk_tree_store_set (GTK_TREE_STORE (model), &iter, column,
                    toggle_item, -1);

    /* clean up */
    gtk_tree_path_free (path);
}

//сигнал изменения ячейки
void celled_signal(GtkCellRendererText* cell, const gchar *path_str, const gchar *new_text, gpointer data)
{
    //Получение информации о списке в GtkTreeModel
    GtkTreeModel *model = (GtkTreeModel *)data;
    //Получение информации о местанахождении виджета в модели
    GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
    
    //итератор для доступа к виджету
    GtkTreeIter iter;

    //колонка (данные пользователя)
    gint column = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (cell), "column"));

    //получение итератора
    gtk_tree_model_get_iter (model, &iter, path);
    
    gchar *old_text;

    //получение  старого текста и его удаление
    gtk_tree_model_get (model, &iter, column, &old_text, -1);
    g_free (old_text);

    //установка нового текста в ячейке
    gtk_tree_store_set (GTK_TREE_STORE (model), &iter, column, g_strdup (new_text), -1);
}

//добавить получателя
void receiver_add_signal(GtkWidget* b, gpointer data)
{    
    addReceiverWindow = new AddReceiverWindow;
}

//удалить элемент списка
void del_signal(GtkWidget* b, gpointer data)
{
    GtkTreeIter iter;
    //список GtkTreeView
    GtkTreeView *treeview = (GtkTreeView *)data;
    
    GtkTreeModel *model = gtk_tree_view_get_model (treeview);
    
    //получить выделенную ячейку и удалить её
    GtkTreeSelection *selection = gtk_tree_view_get_selection (treeview);
    if (gtk_tree_selection_get_selected (selection, NULL, &iter))
    {
        gtk_tree_store_remove (GTK_TREE_STORE (model), &iter);
    }
}

void add_receiver_cancel_signal(GtkWidget* b, gpointer data)
{
    delete addReceiverWindow;
}
//добавить элемент в список
void add_receiver_ok_signal(GtkWidget* b, gpointer data)
{
    std::string sName = gtk_entry_get_text(GTK_ENTRY(addReceiverWindow->nameEdit));
    std::string sSender = gtk_entry_get_text(GTK_ENTRY(addReceiverWindow->senderEdit));
    delete addReceiverWindow;
    GtkTreeIter iter;
    GtkTreeModel *model = (GtkTreeModel *)receiverWindow->model;
    gtk_tree_store_append (GTK_TREE_STORE (model), &iter, NULL);
    gtk_tree_store_set (GTK_TREE_STORE (model), &iter, collist[0], sName.c_str(), collist[1], sSender.c_str(), collist[2], 1, collist[3], 1, -1);
}
//очистка окон
void clear_signal(GtkWidget* b, gpointer)
{
    data->clear();
    if (mainWindow) mainWindow->clear();
    if (senderWindow) senderWindow->clear();
    if (receiverWindow) receiverWindow->clear();
    if (atachWindow) atachWindow->clear();
}

void about(GtkWidget* b, gpointer data)
{
    aboutWindow = new About();
}
void about_destroy_signal(GtkWidget* b, gpointer data)
{
    delete aboutWindow;
    aboutWindow = NULL;
}
