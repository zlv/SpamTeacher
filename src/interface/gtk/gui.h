#ifndef GUI_H_
#define GUI_H_
//gui
#include <gtk/gtk.h>
//class std::string
#include <string>
#include <iostream>
////////////////////////////////////
//настройки отправителя
void sender_signal(GtkWidget* b, gpointer data);
//получателей
void receiver_signal(GtkWidget* b, gpointer data);
//вложений
void atach_signal(GtkWidget* b, gpointer data);
//отправить сообщение
void message_signal(GtkWidget* b, gpointer data);
////////////////////////////////////
//при нажатии на кнопку "Имя"
void name_signal(GtkWidget* b, gpointer data);
//при нажатии на кнопку "Пол"
void gender_signal(GtkWidget* b, gpointer data);
///////////////////////////////
void celled_signal(GtkCellRendererText*, const gchar*, const gchar*, gpointer data);
///////////////////////////////
void del_signal(GtkWidget* b, gpointer data);
///////////////////////////////
void about(GtkWidget* b, gpointer data);
//сделать строку с информацией о поле
std::string get_gender_message(std::string m,std::string f,std::string t);
//количество настроек
const int setNum = 2;
//указатель на функцию
typedef void (*PFunc)(GtkWidget* b, gpointer data);
//массив сигналов настроек
const PFunc settings_signal[setNum] = {name_signal, gender_signal};
////////////////////////////////////
//закрыть
void closeApp ( GtkWidget *window, gpointer data);
////////////////////////////////////
//глобальные
//главное окно
class MainWindow;
//отправителя
class SenderWindow;
//о поле
class GenderWindow;
//получатель
class ReceiverWindow;
///////////////
class SendWindow;
//данные
class Data;
//главное окно
extern MainWindow* mainWindow;
//отправителя
extern SenderWindow* senderWindow;
//о поле
extern GenderWindow* genderWindow;
//получатель
extern ReceiverWindow* receiverWindow;
/////////////
extern SendWindow* sendWindow;
//данные
extern Data* data;
const std::string version = "0.2";
extern std::string program;
#endif
