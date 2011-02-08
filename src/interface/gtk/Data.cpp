#include "Data.h"
#include "Receiver.h"
#include <fstream>
#include <sstream>
/////////////////////////
std::string confdir = "etc/SpamTeacher";
char confdirs[][40] = {"etc","SpamTeacher"};
//файл для данных
std::string fileaddr = "/data";
//файл для сообщения
std::string filemess = "/message";
//файл для вложений
std::string atach = "/atach";
//стандартные переменные
std::string stdfile = 
"#адрес для отправки\n"
"senderEmail = отправитель@почта\n"
"#заголовок\n"
"theme = Заголовок\n"
"#сервер, пользователь, пароль\n"
"server = сервер\n"
"user = пользователь\n"
"#password = Ђ\n"
"#далее адреса\n"
"dest =\n"
"адрес1@почта1 + м Получатель1\n"
"адрес2@почта2 + м Получатель2\n";
/////////////////////////
#ifndef _WIN32 //Linux
#    include <stdlib.h>
#    include <sys/stat.h>
#    include <sys/types.h>
#endif
Data::Data()
{
    //////////////////////////////    
#ifdef _WIN32
    confdir = "etc";
    fileaddr = "\\data";
    filemess = "\\message";
    atach = "\\atach";
#endif
    //////////////////////////////
#ifndef _WIN32 //Linux
    confdir = getenv("HOME")+("/"+confdir);
    std::string dir = getenv("HOME");
    for (int i=0; i<sizeof(confdirs)/sizeof(confdirs[0]); i++)
    {
        dir+=("/"+std::string(confdirs[i]));
        mkdir(dir.c_str(),0750);
    }
#endif
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
    
    //находим значение переменных "senderEmail", "theme", "server", "user", "password"
    while (!(*file).eof())
    {
        //сначала считываем название переменной
        std::string var;
        (*file) >> var;
        
        //пропускаем символы до первой буквы
        char ch = (char)(*file).peek();
        while (ch==' ' || ch=='=')
        {
            (*file).get();
            ch = (char)(*file).peek();
        }
        
        //если переменная -- dest, значит до конца файла идут значения этой переменной и нужно переходить к следующему этапу
        if (var=="dest") break;
        
        //читаем значение переменной
        char buffer[MAX+1];
        (*file).getline(buffer,MAX,'\n');
        
        //если это комментарий, идём дальше
        if (var[0]=='#') continue;
        
        //если нет, то присваиваем соответствующей переменной значение
        if (var == "senderEmail")
        {
                senderEmail = buffer;
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
        else if (var == "password")
        {
                password = buffer;
                password = decode(password);
        }
    }
    //если есть получатели, обязательно после "dest = " идёт '\n'
    (*file).ignore(1,'\n');
    while (!(*file).eof())
    {
        //читаем адрес
        char buffer[MAX+1];
        (*file).getline(buffer,MAX,' ');
        if ((*file).eof())
        {
            break;
        }
        std::string text = buffer;
        if (text[0]=='#') continue;
        recvEmail.push_back(text);
        
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
    while (!(*file).eof() && !(*file).fail())
    {
        //читаем адрес
        char buffer[MAX+1];
        (*file).getline(buffer,MAX,'\n');
        /*if ((*file).eof())
        {
            break;
        }*/
        std::string text = buffer;
        if (text[0]=='#') continue;
        fileatach.push_back(text);
    }
    delete file;
    
    //далее сообщение
    file = new std::ifstream(filemess.c_str());
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
    delete file;
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
        for (i=0; i<recvEmail.size()-1; i++)
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
        for (i=0; i<fileatach.size()-1; i++)
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

#include <iostream>
//отправка сообщения в зависимости от установленных данных
void Data::send()
{
    ////////////////////////////////
    if (message=="")
    {
        std::cerr << "Нет сообщения!\n";
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress),1.0);
        gtk_label_set_text(GTK_LABEL(label),"Нет сообщения!\n");
        return;
    }
    //по очереди для всех адресатов
    for (int i=0; i<recvEmail.size(); i++)
    {
        if (stoped) return;
        int size = recvEmail.size();
        std::ostringstream omem;
        omem << "(" << i << "/" << size << ")"
            << get_message_about_receiver(i);
        gtk_label_set_text(GTK_LABEL(label),omem.str().c_str());
        double percent = double(i)/double(size);
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress),percent);
        //только если адрес включен
        if (recvData[i].enabled)
        {
            //получение настоящего сообщения из шаблона
            std::string mes = get_mes(recvData[i]);
            //отправка
            sendEmail(recvEmail[i],theme,mes);
        }
    }
    std::ostringstream omem;
    int size = recvEmail.size();
    omem << "(" << size << "/" << size << ") -- конец отправки сообщений";
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress),1.0);
    gtk_label_set_text(GTK_LABEL(label),omem.str().c_str());
}

//преобразование шаблона в настоящее сообщение
std::string Data::get_mes(Receiver& r)
{
    std::string mes = message;
    
    //поиск открывающей и закрывающей скобок
    int index = mes.find("[%");
    int last = mes.find("]",index);
    while (index!=std::string::npos)
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

#include <stdlib.h>
#include <iostream>
//отправка сообщения
void Data::sendEmail(std::string rec, std::string t, std::string m)
{
    //добавление параметров строки
    std::string buf;
    buf +=(program);
    buf +=(" -f \"");
    buf +=(senderEmail);
    buf +=("\" -o ");
    //сообщение в Unicode
    buf +=("message-charset=""UTF-8");
    buf +=(" -u \"");
    buf +=(t);
#ifndef _WIN32 //Linux
    buf +=("\" -m \"");
    buf +=(m);
#else
    serial();
    buf += "\" -o message-file=\"";
    buf += filemess;
#endif
    buf +=("\" -s \"");
    buf +=(server);
    buf +=("\" -xu \"");
    buf +=(user);
    buf +=("\" -xp \"");
    buf +=(password);
    buf +=("\" ");
    for (int i=0; i<fileatach.size(); i++)
    {
        buf +=("-a \"");
        buf +=(fileatach[i]);
        buf +=("\" ");
    }
    buf +=("-t \"");
    buf +=(rec);
    buf +=("\"");
    system(buf.c_str());
}

char vars[][30] = {"senderEmail", "theme", "server", "user", "password"};
//запись в файл
void Data::serial()
{
    std::string outstr = stdfile;
    for (int i=0; i<sizeof(vars)/sizeof(vars[0]); i++)
    {
        std::string var = vars[i];
        int first = outstr.find(var)+3+var.size();
        int last = outstr.find('\n',first);
        std::string text;
        if (var == "senderEmail")
        {
            text = senderEmail;
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
        else if (var == "password")
        {
            text = encode(password);
        }
        outstr.replace(first,last-first,text);
    }
    int first = outstr.find("dest")+3+4;
    int last = outstr.size();
    std::string text;
    int i;
    std::string sp = " ";
    if (recvEmail.size()!=0)
    {
        for (i=0; i<recvEmail.size()-1; i++)
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
    for (i=0; i<fileatach.size()-1; i++)
    {
        omem << fileatach[i] << std::endl;
    }
    omem << fileatach[i];
    (*file) << omem.str();
    delete file;
    
    file = new std::ofstream(filemess.c_str());
    (*file) << message;
    delete file;
}

void Data::clear()
{
    senderEmail = theme = message = server = user = password = "";
    fileatach.clear();
    recvEmail.clear();
    recvData.clear();
}

#define DEBUG 0
std::string Data::encode(std::string& s)
{
    char *result= new char[s.length()+2];
    if (DEBUG) std::cerr << s.length() << std::endl;
    result[0] = s.length()-128;
    for (int i=0; i<s.length(); i++)
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
        while (result[i]>=128)
        {
            result[i]-=256;
        }
        while (result[i]<-128)
        {
            result[i]+=256;
        }
    }
    result[s0]=0;

    if (DEBUG) std::cerr << result << '\n' << std::endl;
    
    return std::string(result);
}

void Data::set_password(std::string& s)
{
    password = s;
}

void Data::start(GtkWidget* p, GtkWidget* l)
{
    progress = p;
    label = l;
    GError* error;
    stoped = 0;
    g_thread_create(Data::func, this, 0, &error);
}

void* Data::func(void* d)
{    
    ((Data*)d)->send();
}
void Data::stop()
{
    stoped = 1;
}
