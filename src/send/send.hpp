#ifndef SEND_HPP
#define SEND_HPP
#include <string>
//Адрес и имя / address and name
class Info
{
public:
    std::string senderMail, //почта отправителя
    senderName, //его имя
    server, //название сервера
    username, //имя пользователя на этом сервере
    password, //пароль
    receiverMail, //адрес получателя
    receiverName, //имя для отправки
    theme; //заголовок сообщения
    std::string text; //само сообщение / message itself
};
class Sender
{
public:
    //та же информация / this same fields as in Info
    std::string senderMail, //почта отправителя
        senderName, //его имя
        server, //название сервера
        username, //имя пользователя на этом сервере
        password, //пароль
        receiverMail, //адрес получателя
        receiverName, //имя для отправки
        theme; //заголовок сообщения
    std::string text;
    bool utf8; //кодировка
    Sender(Info&);
    std::string send();

    int sock;
    //отправка сообщения на сервер
    void send_socket(std::string s);
    //прочитать сообщение из сокета
    void read_socket();
    //прочитать из сокета, проверяя пароль
    void read_socket_with_pass_check();
    //конец отправки
    void end();
    //файлы
    int nFiles;
    std::string* fileNames;
    std::string* files;
    void load_files();
    void load_file(int);
    std::string get_type(std::string&);
/////////////////////////////
    void initialise_files(int n)
    {
        nFiles = n;
        fileNames = new std::string[nFiles];
        files = new std::string[nFiles];
    }
private:
    std::ostringstream* log;
    bool logout;
public:
    std::string get_log();
    class WrongPassword
    {public: std::string data; WrongPassword(std::string a) : data(a){}};
    class BadServer
    {public: std::string data; BadServer(std::string a) : data(a){}};
    std::string encode_theme(std::string&,bool);
};
#endif
