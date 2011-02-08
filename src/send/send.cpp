//далее написано для каких функций или классов включается библиотека /
//every discriptions for include files is neccessary functions from this libraries
///////////////////
#include "send.hpp"
//base64_encode()
#include "base64.h"
#include "u82w1251.hpp"
#include <sstream>
//std::cerr
#include <iostream>
//memcpy()
#include <string.h>
//exit()
#include <stdlib.h>
//для загрузки вложений / for loading atachments
#include <fstream>
//информация о файлах / class QFileInfo
#include <QFileInfo>
#ifndef _WIN32
//функции работы с сетью / network functions
#include <netdb.h>
#else
//замена функций для работы с сетью / replace network functions in windows
#include <winsock.h>
//переименование / renaming
int read(int sock, char* buf, int BUFSIZE)
{
    return recv(sock, buf, BUFSIZE, 0);
}
void write(int sock, const char* buf, int l)
{
    send(sock, buf, l, 0);
}
void close(int sock)
{
    closesocket(sock);
}
#endif
//разделитель для файлов вложений / delimeter for atachments
std::string delimiter = "delimiter";
//конец строки / end of line
std::string END = "\r\n";
//инициализация / initialization
Sender::Sender(Info& info) : utf8(0)
{
#ifdef _WIN32
    //информация для винды / windows socket information
    WSADATA WsaData;
    int err = WSAStartup (0x0101, &WsaData);
    if (err == SOCKET_ERROR)
    {
        std::cerr << "WSAStartup() failed:"  << GetLastError () << std::endl;
        return;
    }
#endif

    //установливаем log
    log = new std::ostringstream();
    logout = 1;

    int i;

    senderMail = info.senderMail;
    server = info.server;
    username = info.username;
    password = info.password;
    receiverMail = info.receiverMail;

    senderName = info.senderName;
    receiverName = info.receiverName;
    theme = info.theme;
    text = info.text;

    //подсоединение к серверу
    struct sockaddr_in sockserver;

    sockserver.sin_family = AF_INET;
    sock = socket(sockserver.sin_family, SOCK_STREAM, 0);
    if (sock==-1)
    {
        std::cerr << "opening stream socket";
        return;
    }

    struct hostent *hp;

    hp = gethostbyname(server.c_str());
    if (hp==(struct hostent *) 0)
    {
        (*log) << server << " : unknown host\n";
        std::cerr << server << " : unknown host\n";
        close(sock);
        throw BadServer((*log).str());
    }
    memcpy((char *) &sockserver.sin_addr, (char *) hp->h_addr, hp->h_length);

    //установить порт
    for (i=server.length(); i>0; i--)
    {
        if (receiverMail[i]==':')
        {
            break;
        }
    }
    int port;
    if (i!=0)
    {
        std::istringstream istm(server.substr(i));

        istm >> port;
    }
    else
    {
        port=25;
    }

    sockserver.sin_port=htons(port);

    if (connect(sock, (struct sockaddr *) &sockserver, sizeof sockserver)==-1)
    {
        (*log) << server << " : connection error";
        std::cerr << server << " : connection error";
        close(sock);
        throw BadServer((*log).str());
    }

    read_socket();
    //приветствие (HELLO)
    send_socket(std::string("EHLO ")+server+END);
    read_socket();

    //идентификация
    send_socket(std::string("AUTH LOGIN")+END);
    read_socket();

    std::string* s = &username;
    std::string dest64;
    dest64 = base64_encode(
        reinterpret_cast<const unsigned char*>(s->c_str()),
        s->length());
    send_socket(dest64 + '\n');
    read_socket();

    s = &password;
    dest64 = base64_encode(
        reinterpret_cast<const unsigned char*>(s->c_str()),
        s->length());
    send_socket(dest64+END);
    read_socket_with_pass_check();
}

std::string Sender::encode_theme(std::string& theme, bool utf8)
{
    return std::string("=?") + (utf8 ? "utf-8" : "windows-1251") + "?B?" + base64_encode(reinterpret_cast<const unsigned char*>(theme.c_str()),theme.length()) + "?=";
}

//здесь список серверов, на которых плохо просматриваются сообщения с utf8
char goodServer[][51] = {"gmail.com", "mail2000.ru", "mail.ru", "list.ru"};
int goodServerSize = sizeof(goodServer)/sizeof(goodServer[0]);
std::string Sender::send()
{
    //установливаем log
    log = new std::ostringstream();
    logout = 1;
    //подготовка кодировки
    int i;
    for (i=receiverMail.length(); i>0; i--)
    {
        if (receiverMail[i]=='@')
        {
            break;
        }
    }
    if (i+1>(int)receiverMail.length())
    {
        std::string error = "empty address";
        std::cerr << error << std::endl;
        return error;
    }
    std::string recServer = receiverMail.substr(i+1);

    //for a while
    utf8=0;
    for (int i=0; i<goodServerSize; i++) if (recServer==goodServer[i]) utf8=1;
    std::string senderName,
        receiverName,
        theme, text;
    if (utf8)
    {
        senderName = this->senderName;
        receiverName = this->receiverName;
        theme = this->theme;
        text = this->text;
    }
    else
    {
        senderName = utf8_2_win1251(this->senderName);
        receiverName = utf8_2_win1251(this->receiverName);
        theme = utf8_2_win1251(this->theme);
        text = utf8_2_win1251(this->text);
    }
    ///////////////////////////////////////
    ///////////////////////////////////////
    //от кого
    send_socket(std::string("MAIL From: <") +
    senderMail + '>' + END);
    read_socket();
    //кому
    send_socket(std::string("RCPT To: <") +
    receiverMail + '>' + END);
    read_socket();

    //начало сообщения
    send_socket(std::string("DATA")+END);
    read_socket();
    if (nFiles!=0)
    {
        //заголовок
        send_socket(std::string("From: ") + senderName +
                " <" + senderMail + '>' + END + "To: " +
                receiverName + " <" + receiverMail + '>' +
                END + "Subject: " + encode_theme(theme,utf8) + END +
                "Content-Type: multipart/mixed; boundary=\""+delimiter+"\"" + END +
                END + "multipart message" + END +
                END + "--"+delimiter + END +
                "Content-Type: text/plain; charset=\"" +
                (utf8 ? "utf-8" : "windows-1251") + "\"" +
                END + "Content-Transfer-Encoding: 7bit" + END +
        //сообщение
                END + text + END);

        for (int i=0; i<nFiles; i++)
        {
            send_socket(files[i]);
        }
        send_socket(END + "--"+delimiter+"--" + END);
    }
    else
    {
        //заголовок
        send_socket(std::string("From: ") + senderName +
                " <" + senderMail + '>' + END + "To: " +
                receiverName + " <" + receiverMail + '>' +
                END + "Subject: " + encode_theme(theme,utf8) +
                END + "Content-Type: text/plain; charset=\"" +
                (utf8 ? "utf-8" : "windows-1251") + "\"" +
                END + "Content-Transfer-Encoding: 7bit" + END +
        //сообщение
                END + text + END);
    }
    //конец отправки
    send_socket(END+'.'+END);
    read_socket();

    return get_log();
}
void Sender::end()
{
    send_socket(std::string("QUIT")+END);
    read_socket();
    close(sock);
}
////////////////////////////////////////
////////////////////////////////////////
void Sender::send_socket(std::string s)
{
    write(sock,s.c_str(),s.length());
    if (logout)
    {
        (*log) << s;
    }
}
const int BUFSIZE=4096;
void Sender::read_socket()
{
    char buf[BUFSIZE+1];
    int len;
    len = read(sock,buf,BUFSIZE);
    if (logout)
    {
        (*log) << buf;
    }
}
//прочитать из сокета, проверяя пароль
void Sender::read_socket_with_pass_check()
{

    char buf[BUFSIZE+1];
    int len;
    len = read(sock,buf,BUFSIZE);
    std::string bu = buf;
    if (bu.find("235")==std::string::npos)
    {
        end();
        (*log) << "Wrong password!";
        throw WrongPassword((*log).str());
    }
    if (logout)
    {
        (*log) << buf;
    }
}
////////////////////////////////////////
void Sender::load_files()
{
    for (int i=0; i<nFiles; i++)
    {
        load_file(i);
    }
}

////////////////////////////////////////
//здесь список форматов текстовых файлов / text formats
std::string text[] = {"exe", "odt", "doc", "qm", "rar"};
int textSize = 5;
bool check_txt(std::string s)
{
    QFileInfo info(QString::fromStdString(s));

    for (int i=0; i<textSize; i++) if (info.suffix().toStdString()==text[i])
    {
        return 0;
    }
    return 1;
}
////////////////////////////////////////
void Sender::load_file(int fi)
{
    std::string filename = fileNames[fi];
    std::string* content = &files[fi];
    int i;
    for (i=filename.length(); i>0; i--)
    {
        if (filename[i]=='.')
        {
            break;
        }
    }
    std::string encoding;
    if (i<=0)
    {
        encoding = "";
    }
    else
    {
        encoding = filename.substr(i+1);
    }

    //установка MIME-типа
    std::string contentType = get_type(encoding);
    std::string name;
    for (i=filename.length(); i>0; i--)
    {
        if (filename[i]=='/' || filename[i]=='\\')
        {
            break;
        }
    }
    if (i<=0)
    {
        name = filename;
    }
    else
    {
        name = filename.substr(i+1);
    }

    (*content) += END + "--"+delimiter + END +
        "Content-Type: " + contentType + ";" + END +
        "        name=\"" + name + "\"" + END +
        "Content-Transfer-Encoding: base64" + END +
        "Content-Disposition: attachment; filename=\"" + name + "\"" + END +
        END;
    std::ifstream* fstream;
    std::string temp;
    if (check_txt(filename))
    {
        fstream = new std::ifstream(filename.c_str());
        const int BUFSIZE=4096;
        char buffer[BUFSIZE+1];

        while ((*fstream).good())
        {
            (*fstream).get(buffer,BUFSIZE,-1);
            temp += buffer;
        }
    }
    else
    {
        fstream = new std::ifstream(filename.c_str(), std::ios::binary|std::ios::in);

        while ((*fstream).good())
        {
            char ch=' ';
            (*fstream).read((char*)ch, 1);
            temp += ch;
        }
    }
    (*fstream).close();
    delete fstream;
    (*content) += base64_encode((const unsigned char*)temp.c_str(), temp.length()) + END;
}
///////////////////////////////////////
std::string Sender::get_type(std::string& encoding)
{
    std::string contentType;
    if (encoding == "txt" || encoding == "text" || encoding == "log" || encoding == "conf" || encoding == "c" || encoding == "cpp" || encoding == "h" || encoding == "inc" || encoding == "m3u")
    {
        contentType = "text/plain";
    }
    else if (encoding == "html" || encoding == "htm" || encoding == "shtml" || encoding == "shtm" || encoding == "asp" || encoding == "php" || encoding == "cfm")
    {
        contentType = "text/html";
    }
    else if (encoding == "sh")
    {
        contentType = "application/x-sh";
    }
    else if (encoding == "tcl")
    {
        contentType = "application/x-tcl";
    }
    else if (encoding == "pl")
    {
        contentType = "application/x-perl";
    }
    else if (encoding == "js")
    {
        contentType = "application/x-javascript";
    }
    else if (encoding == "man")
    {
        contentType = "application/x-troff-man";
    }
    else if (encoding == "gif")
    {
        contentType = "image/gif";
    }
    else if (encoding == "jpg" || encoding == "jpeg" || encoding == "jpe" || encoding == "jfif" || encoding == "pjpeg" || encoding == "pjp")
    {
        contentType = "image/jpeg";
    }
    else if (encoding == "tif" || encoding == "tiff")
    {
        contentType = "image/tiff";
    }
    else if (encoding == "xpm")
    {
        contentType = "image/x-xpixmap";
    }
    else if (encoding == "bmp")
    {
        contentType = "image/x-MS-bmp";
    }
    else if (encoding == "pcd")
    {
        contentType = "image/x-photo-cd";
    }
    else if (encoding == "png")
    {
        contentType = "image/png";
    }
    else if (encoding == "aif" || encoding == "aiff")
    {
        contentType = "audio/x-aiff";
    }
    else if (encoding == "wav")
    {
        contentType = "audio/x-wav";
    }
    else if (encoding == "mp2" || encoding == "mp3" || encoding == "mpa")
    {
        contentType = "audio/x-mpeg";
    }
    else if (encoding == "ra" || encoding == "ram")
    {
        contentType = "audio/x-pn-realaudio";
    }
    else if (encoding == "mpeg" || encoding == "mpg")
    {
        contentType = "video/mpeg";
    }
    else if (encoding == "mov" || encoding == "qt")
    {
        contentType = "video/quicktime";
    }
    else if (encoding == "avi")
    {
        contentType = "video/x-msvideo";
    }
    else if (encoding == "zip")
    {
        contentType = "application/x-zip-compressed";
    }
    else if (encoding == "tar")
    {
        contentType = "application/x-tar";
    }
    else if (encoding == "jar")
    {
        contentType = "application/java-archive";
    }
    else if (encoding == "exe" || encoding == "bin")
    {
        contentType = "application/octet-stream";
    }
    else if (encoding == "ppt" || encoding == "pot" || encoding == "ppa" || encoding == "pps" || encoding == "pwz")
    {
        contentType = "application/vnd.ms-powerpoint";
    }
    else if (encoding == "mdb" || encoding == "mda" || encoding == "mde")
    {
        contentType = "application/vnd.ms-access";
    }
    else if (encoding == "xls" || encoding == "xlt" || encoding == "xlm" || encoding == "xld" || encoding == "xla" || encoding == "xlc" || encoding == "xlw" || encoding == "xll")
    {
        contentType = "application/vnd.ms-excel";
    }
    else if (encoding == "doc" || encoding == "dot")
    {
        contentType = "application/msword";
    }
    else if (encoding == "odt")
    {
        contentType = "application/vnd.oasis.opendocument.text";
    }
    else if (encoding == "ogg")
    {
        contentType = "application/ogg";
    }
    else if (encoding == "mkv")
    {
        contentType = "video/x-matroska";
    }
    else if (encoding == "rtf")
    {
        contentType = "application/rtf";
    }
    else if (encoding == "pdf")
    {
        contentType = "application/pdf";
    }
    else if (encoding == "tex")
    {
        contentType = "application/x-tex";
    }
    else if (encoding == "latex")
    {
        contentType = "application/x-latex";
    }
    else if (encoding == "vcf")
    {
        contentType = "application/x-vcard";
    }
    else
    {
        contentType = "application/octet-stream";
    }

    return contentType;
}
std::string Sender::get_log()
{
    std::string res = log->str();
    delete log;
    return res;
}
