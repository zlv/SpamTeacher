#ifndef SEND_WINDOW_H
#define SEND_WINDOW_H
#include <string>
#include <QEvent>
#include "BaseWindow.h"
class SendWindow : BaseWindow
{
    Q_OBJECT

    static const int count = 2;
    QLayout* box[count];
    QLayout* vbox;
    /////////////////////////////////
    QWidget *statusLabel;
    QWidget *progress;
    /////////////////////////////////
    QWidget *view;
    /////////////////////////////////
    bool ok;
public:
    QWidget *window;
    SendWindow(QWidget* w);
    ~SendWindow();
    void closeEvent(QCloseEvent*);
    void customEvent(QEvent* pe);
    void writeLog(std::string);
};
///////////////////////////////
class SendEvent : public QEvent {
public:
    class SendData
    {
    public:
        int value;
        std::string text;
        std::string qtext;
        SendData(){}
        SendData(std::string t, int v) : value(v), text(t)
        {}
        SendData(std::string t, std::string t2, int v) : value(v), text(t), qtext(t2)
        {}
    };
private:
    SendData d;

public:

    enum {SendType = User + 1};

    SendEvent() : QEvent((Type)SendType)
    {
    }

    void setValue(SendData n)
    {
        d = n;
    }

    SendData value() const
    {
        return d;
    }
};
class PassEvent : public QEvent {
public:

    enum {PassType = User + 2};

    PassEvent() : QEvent((Type)PassType)
    {
    }
};

class WrongPassEvent : public QEvent {
public:

    std::string data;

    enum {WrongPassType = User + 3};

    WrongPassEvent(std::string a) : QEvent((Type)WrongPassType), data(a)
    {
    }
};

class BadServerEvent : public QEvent {
public:

    std::string data;

    enum {BadServerType = User + 4};

    BadServerEvent(std::string a) : QEvent((Type)BadServerType), data(a)
    {
    }
};
#endif
