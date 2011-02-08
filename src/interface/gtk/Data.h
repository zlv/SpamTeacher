#ifndef DATA_H
#define DATA_H
#include <string>
#include <vector>
#include <gtk/gtk.h>
//структура для хранения информации о получателе
class Receiver;
//Информация, необходимая для отправки, и сама отправка
class Data
{
    static const int MAX = 1000;
    static const int BIGMAX = MAX*10;
    //отправить сообщение с помощью программы sendEmail
    void sendEmail(std::string rec, std::string t, std::string m);
    //обработка шаблона под структуру Receiver
    std::string get_mes(Receiver&);
    std::string encode(std::string&);
    std::string decode(std::string&);
    /////////////////////////////////////////
    GtkWidget* progress;
    GtkWidget* label;
    //отправить
    void send();
    /////////////////////////////////////////
    static void* func(void*);
    /////////////////////////////////////////
public:
    void start(GtkWidget*,GtkWidget*);
    void stop();
    void set_password(std::string&);
    //название программы sendEmail
    std::string program;
    //название текстовых файлов
    std::vector<std::string> fileatach;
    //адрес отправителя
    std::string senderEmail;
    //тема
    std::string theme;
    //сообщение
    std::string message;
    //сервер
    std::string server;
    //пользователь
    std::string user;
    //пароль
    std::string password;
    //адреса получателей
    std::vector<std::string> recvEmail;
    //информация о получателях
    std::vector<Receiver> recvData;
    //записать информацию на жёсткий диск
    void serial();
    ///////////////////////////////////
    Data();
    //получение информации для отображения в виджетах главного окна
    std::string get_message_about_sender();
    std::string get_message_about_receiver();
    std::string get_message_about_receiver(int);
    std::string get_message_about_atachment();
    ////////////////////////////
    void clear();
};
//шаблон опций имени и пола
const std::string nameCode = "[%name]";
const std::string genderCode = "[%ms|%fs]";
#endif
