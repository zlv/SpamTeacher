#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <QMenuBar>
#include "BaseWindow.h"
#include "../../Data.h"
//главное окно
class MainWindow : public BaseWindow
{
    Q_OBJECT

    //русский?
    bool ru;
    QWidget* senderWindow;
    QWidget* receiverWindow;
    QWidget* atachWindow;
public:
    Data* data;
private:
    QWidget *window;
    //разные контейнеры
    QLayout *box[4];
    QLayout *vbox;
    ////////////////////
    QWidget *senderButton,*receiverButton,*atachButton,*messageButton;
public:
    ////////////////////
    QWidget *messageEdit,*senderEdit,*receiverEdit,*atachEdit;
    QWidget *themeEdit;
    MainWindow(bool);
    void clear();
    void lang();
    void closeEvent(QCloseEvent*);
private slots:
    void clear_slot();
    void lang_slot();
    //настройки отправителя
    void sender_slot();
    //получателей
    void receiver_slot();
    //вложений
    void atach_slot();
    //отправить сообщение
    void message_slot();
    //при нажатии на кнопку "Имя" // when press button "Name"
    void name_slot();
    //при нажатии на кнопку "Пол" / when press "Gender"
    void gender_slot();
    //при нажатии на кнопку "Цитата" / when press "Quote"
    void quote_slot();
    //О программе
    void about_slot();
private:
    static const int setNum=2;//3;
    //массив сигналов настроек
private:
    void create_actions();
    //действия
public:
    //смена языка
    QAction* langAct;
    //очистка
    QAction* clearAct;
    //настройки отправителя
    QAction* senderAct;
    //получателей
    QAction* receiverAct;
    //вложений
    QAction* atachAct;
    //отправить сообщение
    QAction* messageAct;
    //О программе
    QAction* aboutAct;
    //////////////////////////
    QAction* exitAct;
private:
    void toQuote(std::string&, int, int);
};
class Menu : public QMenuBar
{
    Q_OBJECT

public:
    Menu(QWidget *w = 0);
    ~Menu(){}
};
typedef void (*PFunc)(void*);
#endif
