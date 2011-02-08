#ifndef DATA_H
#define DATA_H
#include "interface/interface.h"
#include "interface/func.h"
#ifdef QT
#include <QThread>
#endif
#include "send/send.hpp"
#include <vector>
//структура для хранения информации о получателе
class Receiver;
//Информация, необходимая для отправки, и сама отправка / information for email send, and sending functions
class Data
#ifdef QT
             : QThread
#endif
{
    ///////////////////////////////////////
    //для текста / for text
    static const int MAX = 1000;
    static const int BIGMAX = MAX*MAX;
    //отправить сообщение
    void sendEmail(std::string& rec, std::string& n, 
               std::string& t, std::string& m,
               std::string& l);
    //обработка шаблона под структуру Receiver
    std::string get_mes(Receiver&);
    //отправить
    void send();
    /////////////////////////////////////////
    void run();
public:
    //необходимые для потока функции
#ifdef QT
    PWidget window;
    void startThread(PWidget);
#endif
#ifdef GTK
    PWidget progress;
    PWidget label;
    void start(PWidget, PWidget);
    /////////////////////////////////////////
    static void* func(void*);
#endif
    void stop();
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
private:
    //Структура для отправки сообщений
    Sender* sender;
    //Информация для неё
    Info senderInfo;
public:
    std::string senderName;
private:
    int enabledCount();
    //not used
private:
    //закодировать пароль (в данный момент не используется) / encode and decode password (now not used)
    //std::string encode(std::string&);
    //std::string decode(std::string&);
};
//шаблон опций имени и пола
const std::string nameCode = "[%name]";
const std::string genderCode = "[%ms|%fs]";
#endif
