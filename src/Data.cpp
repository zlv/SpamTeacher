//далее написано для каких функций или классов включается библиотека /
//every discriptions for include files is neccessary functions from this libraries
#include "Data.h"
//class Receiver
#include "Receiver.h"
//окно отправки
#include "interface/qt/SendWindow.h"
#include <fstream>
#include <sstream>
#include <iostream>
#ifdef QT
#include <QDir>
#include <QApplication>
#endif
/////////////////////////
const int MAX = 200;
#ifndef _WIN32
std::string confdir = ("/home/" + (QDir::home()).dirName()+"/.MassDelivery/").toStdString();
#else
std::string confdir = "";
#endif
////////////////////////
//файл для данных
std::string fileaddr = "data";
//файл для сообщения
std::string filemess = "message";
//файл для вложений
std::string atach = "atach";
//стандартные переменные
std::string stdfile =
"#sender adress\n"
"senderEmail = sender@server\n"
"#sender name\n"
"senderName = \n"
"#title\n"
"theme = title\n"
"#login, on serv\n"
"server = someserver\n"
"user = login\n"
"#password = \n"
"#receivers\n"
"dest =\n"
"z_lezhnin@scalpnet.ru + м receiver1\n"
"adress1@mail2 + м receiver2\n";
/////////////////////////
#ifndef _WIN32 //Linux
#    include <stdlib.h>
#    include <sys/stat.h>
#    include <sys/types.h>
#endif
Data::Data() : senderName("")
{
    if (confdir!="") (new QDir)->mkdir(QString(confdir.c_str()));
    //////////////////////////////
    fileaddr = confdir + fileaddr;
    filemess = confdir + filemess;
    atach = confdir + atach;
    //пробуем открыть файл
    std::istream* file = new std::ifstream(fileaddr.c_str());
    //если он не открывается, подменяем поток из файла на поток, связанный со строкой
    if (!(*file))
    {
        file = new std::istringstream(stdfile);
    }

    //находим значение переменных / find variables
    //"senderEmail", "senderName", "theme", "server", "user", "password"
    while (!(*file).eof())
    {
        //сначала считываем название переменной / get name of variable first
        std::string var;
        (*file) >> var;

        //пропускаем символы до первой буквы / skip symbols before first letter
        char ch = (char)(*file).peek();
        while (ch==' ' || ch=='=')
        {
            (*file).get();
            ch = (char)(*file).peek();
        }

        //если переменная -- dest, значит до конца файла идут значения этой переменной и нужно переходить к следующему этапу /
        //if it is dest string, then go to the next step
        if (var=="dest") break;

        //читаем значение переменной / get variable
        char buffer[MAX+1];
        (*file).getline(buffer,MAX,'\n');

        //если это комментарий, идём дальше / if it is comment
        if (var[0]=='#') continue;

        //если нет, то присваиваем соответствующей переменной значение / else assign a value to a variable
        if (var == "senderEmail")
        {
                senderEmail = buffer;
        }
        else if (var == "senderName")
        {
                senderName = buffer;
        }
        else if (var == "theme")
        {
                theme = buffer;
        }
        else if (var == "server")
        {
                server = buffer;
        }
        else if (var == "user")
        {
                user = buffer;
        }
        /*else if (var == "password")
        {
                password = buffer;
                password = decode(password);
        }*///пока не реализовано / not working awhile
    }
    //если есть получатели, обязательно после "dest = " идёт '\n' / it is always '\n' symbol after "dest = "
    (*file).ignore(1,'\n');
    //определяет, написаны ли в файле только адреса, либо с дополнительной информацией /
    //if onlyaddress is 0, there is information about receiver in file (name,gender)
    bool onlyaddress=0;
    while (!(*file).eof())
    {
        char buffer[MAX+1];
        //читаем адрес / get address
        if (onlyaddress) (*file).getline(buffer,MAX,'\n');
        else (*file).getline(buffer,MAX,' ');
        if ((*file).eof())
        {
            break;
        }
        std::string text = buffer;
        //слово #only в файле "data" определяет, что в следующих строках будут только адреса /
        //"#only" means, that next lines will be contain only e-mail address
        if (text[0]=='#')
        {
            if (text=="#only")
            {
            onlyaddress=1;
            (*file).getline(buffer,MAX,'\n');
            }
            continue;
        }
        recvEmail.push_back(text);
        if (onlyaddress)
        {
            Receiver rec;
            rec.enabled = 1;
            rec.male = 1;
            rec.name = text;
            recvData.push_back(rec);
            continue;
        }

        std::string gender;

        //сначала читаем включен ли адрес (+ или -) потом уже пол
        (*file) >> gender;
        Receiver rec;
        if (gender == "+")
        {
            rec.enabled = 1;
        }
        else
        {
            rec.enabled = 0;
        }
        (*file) >> gender;
        if (gender == "ж")
        {
            rec.male = 0;
        }
        else
        {
            rec.male = 1;
        }

        //игнорируем пробелы
        char ch = (char)(*file).peek();
        while (ch==' ')
        {
            (*file).get();
            ch = (char)(*file).peek();
        }

        //читаем имя до конца строки
        (*file).getline(buffer,MAX,'\n');
        rec.name = buffer;
        recvData.push_back(rec);
    }
    delete file;

    file = new std::ifstream(atach.c_str());
    int amount;
    (*file) >> amount;
    (*file).ignore(1,'\n');
    for (int i=0; i<amount; i++)
    {
        //читаем адрес файла / get location of file
        char buffer[MAX+1];
        (*file).getline(buffer,MAX,'\n');
        if (!(*file).good()) break;
        std::string text = buffer;
        if (text[0]=='#') continue;
        fileatach.push_back(text);
    }
    delete file;

    //некоторые проблемы с qt не дают возможности прочитать из этого файла / can't realize it because of some bugs
    //далее сообщение
    /*file = new std::ifstream(filemess.c_str());
    char bigbuffer[BIGMAX+1];
    int index=0;
    while (!(*file).eof())
    {
        int c = (*file).get();
        if (c==-1)
        {
            break;
        }
        bigbuffer[index++] = (char)c;
    }
    message = bigbuffer;
    delete file;*/
}

///////////////////////////
std::string Data::get_message_about_sender()
{
    return "\"" + senderEmail + "\" (\"" + server + "\", {\"" +
        user + "\"})";
}

///////////////////////////
std::string Data::get_message_about_receiver()
{
    std::string text;

    text = "{";
    int i;
    if (recvEmail.size()!=0)
    {
        for (i=0; i<(int)recvEmail.size()-1; i++)
        {
            text += '\"' + recvEmail[i] + "\", ";
        }
        text += '\"' + recvEmail[i] + '\"';
    }
    text += "}";

    return text;
}
///////////////////////////
std::string Data::get_message_about_receiver(int i)
{
    std::string text;

    if (recvEmail.size()!=0)
    {
        text += '\"' + recvEmail[i] + "\"";
    }

    return text;
}
///////////////////////////
std::string Data::get_message_about_atachment()
{
    std::string text;

    text = "{";
    int i;
    if (fileatach.size()!=0)
    {
        for (i=0; i<(int)fileatach.size()-1; i++)
        {
            text += '\"' + fileatach[i] + "\", ";
        }
        text += '\"' + fileatach[i] + '\"';
    }
    text += "}";

    return text;
}

/////////////////////////////////
static volatile bool stoped = 0;

//////////////////////////////////////
//отправка сообщения в зависимости от установленных данных
void Data::send()
{
    //текст для вывода при отправке
    std::string logtext;
    //начальная установка отправителя / basic sender information and initialization
    senderInfo.senderName = senderName;
    senderInfo.server = server;
    senderInfo.senderMail = senderEmail;
    senderInfo.username = user;
    senderInfo.password = password;
    sender = new Sender(senderInfo);
    logtext += sender->get_log();
    sender->initialise_files(this->fileatach.size());
    for (int i=0; i<sender->nFiles; i++)
    {
        sender->fileNames[i] = fileatach[i];
    }
    sender->load_files();
    ////////////////////////////////
    //не отправляет сообщения, если нет текста. Если вы хотите это исправить уберите return
    if (message=="")
    {
        std::cerr << tr("No message!\n").toStdString();

        SendEvent* pe = new SendEvent();
        pe->setValue(SendEvent::SendData(
                tr("Nothing to send!\n").toStdString() ,100));
        QApplication::postEvent(window, pe);
        return;
    }
    int counter=0;
    //по очереди для всех адресатов / for all receivers
    for (int i=0; i<(int)recvEmail.size(); i++)
    {
        std::string logtextPart;
        if (stoped) return;
        //выполнять следующие действия только если включен адрес / send only if address enabled
        if (!recvData[i].enabled) continue;

        //получение настоящего сообщения из шаблона / get message from template
        std::string mes = get_mes(recvData[i]);
        //отправка
        sendEmail(recvEmail[i],recvData[i].name,theme,mes,logtextPart);

        logtext+=logtextPart;
        //отправка информации в окно send / send event to sendWindow
        int size = enabledCount();
        std::ostringstream omem;
        omem << "(" << counter++ << "/" << size << ")"
            << get_message_about_receiver(i);
        int percent = int(double(i)/double(size)*100);
        SendEvent* pe = new SendEvent();
        std::cout << logtextPart;
        pe->setValue(SendEvent::SendData(omem.str(),logtext,percent));
        QApplication::postEvent(window, pe);
    }
    std::ostringstream omem;
    //получение настоящего сообщения из шаблона / get message from template
    int size = enabledCount();
    //сообщение об окончании передачи / send 'end of transmission' event to sendWindow
    omem << "(" << size << "/" << size << tr(") -- end of transmission").toStdString();
    SendEvent* pe = new SendEvent();
    pe->setValue(SendEvent::SendData(omem.str(),logtext,101));
    QApplication::postEvent(window, pe);
}

//преобразование шаблона в настоящее сообщение / translate template to real message
std::string Data::get_mes(Receiver& r)
{
    std::string mes = message;

    //поиск открывающей и закрывающей скобок
    int index = mes.find("[%");
    int last = mes.find("]",index);
    while (index!=(int)std::string::npos)
    {
        //вставляемая строка
        std::string add;

        //если значение вставляемой вместо шаблона строки зависит от пола
        if (mes[index+2]=='m')
        {
            char* text = new char[last-index+1];
            int count=0;
            //если получатель мужчина, то нам интересна первая часть шаблона
            if (r.male)
            {
                for (int i=index+3; i<last; i++)
                {
                    if (mes[i]=='|')
                    {
                        break;
                    }
                    text[count++] = mes[i];
                }
            }
            //если нет, то вторая, которая заканчивается на ']'
            else
            {
                int first = mes.find("|%f",index);
                for (int i=first+3; i<last; i++)
                {
                    if (mes[i]==']')
                    {
                        break;
                    }
                    text[count++] = mes[i];
                }
            }
            text[count] = 0;
            add = text;
            delete[] text;
        }
        //если нет, то вставляемый текст -- это имя получателя
        else
        {
            add = r.name;
        }
        //заменяем шаблон на вставляемый текст
        mes.replace(index,last-index+1,add);
        //ищем следующий шаблон
        last = index+add.size();
        index = mes.find("[%",last);
        last = mes.find("]",last);
    }
    return mes;
}
//отправка сообщения
void Data::sendEmail(std::string& rec, std::string& n, 
             std::string& t, std::string& m,
             std::string& l)
{
    sender->receiverMail = rec;
    sender->receiverName = n;
    sender->theme = t;
    sender->text = m;
    l = sender->send();
}

char vars[][30] = {"senderEmail", "senderName", "theme", "server", "user", "password"};
//запись в файл / write info to file
void Data::serial()
{
    std::string outstr = stdfile;
    for (int i=0; i<(int)(sizeof(vars)/sizeof(vars[0])); i++)
    {
        std::string var = vars[i];
        int first = outstr.find(var)+3+var.size();
        int last = outstr.find('\n',first);
        std::string text;
        if (var == "senderEmail")
        {
            text = senderEmail;
        }
        else if (var == "senderName")
        {
            text = senderName;
        }
        else if (var == "theme")
        {
            text = theme;
        }
        else if (var == "server")
        {
            text = server;
        }
        else if (var == "user")
        {
            text = user;
        }
        /*else if (var == "password")
        {
            text = encode(password);
        }*/
        outstr.replace(first,last-first,text);
    }
    int first = outstr.find("dest")+3+4;
    int last = outstr.size();
    std::string text;
    int i;
    std::string sp = " ";
    if (recvEmail.size()!=0)
    {
        for (i=0; i<(int)recvEmail.size()-1; i++)
        {
            text += recvEmail[i] + sp + (recvData[i].enabled?"+":"-") + sp + (recvData[i].male?"м":"ж") + sp + recvData[i].name;
            text += "\n";
        }
        text += recvEmail[i] + sp + (recvData[i].enabled?"+":"-") + sp + (recvData[i].male?"м":"ж") + sp + recvData[i].name;
    }

    outstr.replace(first,last-first,text);
    std::ostream* file = new std::ofstream(fileaddr.c_str());
    (*file) << outstr;
    delete file;

    file = new std::ofstream(atach.c_str());
    std::ostringstream omem;
    omem << fileatach.size() << std::endl;
    for (i=0; i<(int)fileatach.size(); i++)
    {
        omem << fileatach[i] << std::endl;
    }
    (*file) << omem.str();
    delete file;

    //not neccessary now
    /*file = new std::ofstream(filemess.c_str());
    (*file) << message;
    delete file;*/
}

void Data::clear()
{
    senderEmail = senderName = theme = message = server = user = password = "";
    fileatach.clear();
    recvEmail.clear();
    recvData.clear();
}

#define DEBUG 0
/*std::string Data::encode(std::string& s)
{
    char *result= new char[s.length()+2];
    if (DEBUG) std::cerr << s.length() << std::endl;
    result[0] = s.length()-128;
    for (int i=0; i<(int)s.length(); i++)
    {
        result[i+1] = int(s[i]+s.length()-15+i*32+128)%256-128;
        if (DEBUG) std::cerr << (int)s[i] << " " << (int)result[i+1] << '\n';
    }
    result[s.length()+1]=0;
    if (DEBUG) std::cerr << result << '\n';

    return std::string(result);
}

std::string Data::decode(std::string& s)
{
    if (DEBUG) std::cerr << s << '\n';
    int s0 = s[0]+128;
    char *result = new char[s0+1];
    for (int i=0; i<s0; i++)
    {
        result[i] = s[i+1]-s0+15-i*32;
        if (DEBUG) std::cerr << (int)s[i+1] << " " << (int)result[i] << '\n';
    }
    result[s0]=0;

    if (DEBUG) std::cerr << result << '\n' << std::endl;

    return std::string(result);
}*/

#ifdef QT
void Data::startThread(QWidget* w)
{
    window = w;
    stoped = 0;
    this->start();
}
#endif
#ifdef GTK
void Data::start(PWidget p, PWidget l)
{
    progress = p;
    label = l;
    GError* error;
    stoped = 0;
    g_thread_create(Data::func, this, 0, &error);
}
#endif

void Data::run(void)
{
    try
    {
        this->send();
    }
    catch (Sender::WrongPassword e)
    {
        WrongPassEvent* pe = new WrongPassEvent(e.data);
        QApplication::postEvent(window, pe);
    }
    catch (Sender::BadServer e)
    {
        BadServerEvent* se = new BadServerEvent(e.data);
        QApplication::postEvent(window, se);
    }
}
void Data::stop()
{
    stoped = 1;
}
/////////////////////////////
//подсчитать количество доступных
int Data::enabledCount()
{
    int count=0;
    for (int i=0; i<(int)this->recvData.size(); i++)
    {
        count += (int)this->recvData[i].enabled;
    }
    return count;
}
