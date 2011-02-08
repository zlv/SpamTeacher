#include "MainWindow.h"
//функции создания виджетов
#include "../func.h"
//для data и для сигналов
#include "../../Data.h"
#include <iostream>
#include <QTextEdit>
#include <QApplication>
#include <QSettings>
#include <QMessageBox>
#include <sstream>
///////////////////////////////////////
void MainWindow::closeEvent(QCloseEvent *event)
{
    data->message = ((QTextEdit*)this->messageEdit)->toPlainText().toStdString();
    data->theme = ((QLineEdit*)this->themeEdit)->text().toStdString();
    data->serial();
    emit ((QApplication*)qApp)->quit();
    event->accept();
}
//////////////////////////////////
MainWindow::MainWindow(bool r) : BaseWindow(), ru(r)
{
    /////////////////////////////////////
    data = new Data();
    //SpamTeacher -- рабочее название
    this->setWindowTitle(tr("MassDelivery"));

    /////////////////////////////////////
    //установка текста в поля ввода из data
    std::string text;
    text = data->get_message_about_sender();
    senderEdit = new_edit(text.c_str());
    text = data->get_message_about_receiver();
    receiverEdit = new_edit(text.c_str());
    text = data->get_message_about_atachment();
    atachEdit = new_edit(text.c_str());
    /////////////////////////////////////
    ((QLineEdit*)senderEdit)->setReadOnly(1);
    ((QLineEdit*)receiverEdit)->setReadOnly(1);
    ((QLineEdit*)atachEdit)->setReadOnly(1);
    /////////////////////////////////////
    //установка текста в поле для ввода сообщения
    QWidget *view;
    view = new QTextEdit();
    text = data->message;
    ((QTextEdit*)view)->setPlainText(text.c_str());
    /////////////////////////////////////////
    messageEdit = view;
    ////////////////////////////////////////
    //создание кнопок и подцепление к ним сигналов
    senderButton = new_button(tr("Sender options").toStdString().c_str());
    receiverButton = new_button(tr("Receivers options").toStdString().c_str());
    atachButton = new_button(tr("Atachments options(files)").toStdString().c_str());
    messageButton = new_button(tr("Send message").toStdString().c_str());
    ////////////////////////////////////////
    connect(senderButton,SIGNAL(clicked()),this,SLOT(sender_slot()));
    connect(receiverButton,SIGNAL(clicked()),this,SLOT(receiver_slot()));
    connect(atachButton,SIGNAL(clicked()),this,SLOT(atach_slot()));
    connect(messageButton,SIGNAL(clicked()),this,SLOT(message_slot()));
    ////////////////////////////////////////
    //создание кнопок для настроек
    QWidget* settingsButton[setNum];
    settingsButton[0] = new_button(tr("Name").toStdString().c_str());
    settingsButton[1] = new_button(tr("Gender").toStdString().c_str());
    //settingsButton[2] = new_button(tr("Quote").toStdString().c_str());
    ////////////////////////////////////////
    connect(settingsButton[0],SIGNAL(clicked()),this,SLOT(name_slot()));
    connect(settingsButton[1],SIGNAL(clicked()),this,SLOT(gender_slot()));
    //connect(settingsButton[2],SIGNAL(clicked()),this,SLOT(quote_slot()));
    ////////////////////////////////////////
    QLayout* messageSettings = new QGridLayout();
    for (int i=0; i<setNum; i++)
    {
        ((QLayout*)messageSettings)->addWidget(settingsButton[i]);
    }
    QWidget* label;
    label = new_label(tr("Title:"));
    themeEdit = new_edit(data->theme.c_str());
    ((QLayout*)messageSettings)->addWidget(label);
    ((QLayout*)messageSettings)->addWidget(themeEdit);
    /////////////////////////////////////
    //упаковка всего
    box[0] = new QHBoxLayout();
    box[1] = new QHBoxLayout();
    box[2] = new QHBoxLayout();
    box[3] = new QVBoxLayout();
    /////////////////////////////////////
    ((QLayout*)box[0])->addWidget(senderEdit);
    ((QLayout*)box[0])->addWidget(senderButton);
    ((QLayout*)box[1])->addWidget(receiverEdit);
    ((QLayout*)box[1])->addWidget(receiverButton);
    ((QLayout*)box[2])->addWidget(atachEdit);
    ((QLayout*)box[2])->addWidget(atachButton);
    ((QBoxLayout*)box[3])->addLayout(messageSettings);
    ((QLayout*)box[3])->addWidget(messageEdit);
    ((QLayout*)box[3])->addWidget(messageButton);
    /////////////////////////////////////
    vbox = new QVBoxLayout();
    /////////////////////////////////////
    create_actions();
    QWidget* menu = new Menu(this);
    vbox->setMenuBar(menu);

    for (int i=0; i<4; i++)
    {
        ((QBoxLayout*)vbox)->addLayout(box[i]);
    }

    this->setLayout(vbox);
    
    this->show();
}
//////////////////////////////////////////////
void MainWindow::create_actions()
{
    langAct = new QAction(tr("&to Russian"), this);
    langAct->setShortcut(tr("Ctrl+L"));
    connect(langAct, SIGNAL(triggered()), this, SLOT(lang_slot()));
    ///////////////////////////////////////////////
    clearAct = new QAction(tr("&Clear all"), this);
    clearAct->setShortcut(tr("Ctrl+N"));
    connect(clearAct, SIGNAL(triggered()), this, SLOT(clear_slot()));
    ///////////////////////////////////////////////
    senderAct = new QAction(tr("&Sender options"), this);
    senderAct->setShortcut(tr("Ctrl+S"));
    connect(senderAct, SIGNAL(triggered()), this, SLOT(sender_slot()));
    ///////////////////////////////////////////////
    receiverAct = new QAction(tr("&Receiver options"), this);
    receiverAct->setShortcut(tr("Ctrl+R"));
    connect(receiverAct, SIGNAL(triggered()), this, SLOT(receiver_slot()));
    ///////////////////////////////////////////////
    atachAct = new QAction(tr("&Atachment options"), this);
    atachAct->setShortcut(tr("Ctrl+E"));
    connect(atachAct, SIGNAL(triggered()), this, SLOT(atach_slot()));
    ///////////////////////////////////////////////
    messageAct = new QAction(tr("Send &message"), this);
    connect(messageAct, SIGNAL(triggered()), this, SLOT(message_slot()));
    ///////////////////////////////////////////////
    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about_slot()));
    ///////////////////////////////////////////////
    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
}
//////////////////////////////////////////////
void MainWindow::lang()
{
    QSettings settings;
    ru = !ru;
    settings.setValue("/Settings/ru",ru);
    QMessageBox msgBox;
    msgBox.setText(tr("Settings will be changed after restart"));
    msgBox.exec();
}
//////////////////////////////////////////////
void MainWindow::clear()
{
    data->clear();
    ((QLineEdit*)senderEdit)->setText("");
    ((QLineEdit*)receiverEdit)->setText("");
    ((QLineEdit*)atachEdit)->setText("");
    //////////////////////////////////
    ((QLineEdit*)themeEdit)->setText("");

    ((QTextEdit*)messageEdit)->setPlainText("");
}
///////////////////////////////////////////
///////////////Menu////////////////////////
///////////////////////////////////////////
Menu::Menu(QWidget* w) : QMenuBar()
{
    const int mcount=2;
    QMenu* pm[mcount];
    pm[0] = new QMenu(tr("&File"));
    pm[0]->addAction(((MainWindow*)w)->langAct);
    //pm[0]->addAction(((MainWindow*)w)->clearAct);
    pm[0]->addAction(((MainWindow*)w)->senderAct);
    pm[0]->addAction(((MainWindow*)w)->receiverAct);
    pm[0]->addAction(((MainWindow*)w)->atachAct);
    pm[0]->addAction(((MainWindow*)w)->messageAct);
    pm[0]->addSeparator();
    pm[0]->addAction(((MainWindow*)w)->exitAct);
    ////////////////////////////////////////////
    pm[1] = new QMenu(tr("&Help"));
    pm[1]->addAction(((MainWindow*)w)->aboutAct);
    for (int i=0; i<mcount; i++)
    {
        this->addMenu(pm[i]);
    }
}
///////////////////////////////////////////
/////////////Сигналы///////////////////////
///////////////////////////////////////////
#include "SenderWindow.h"
#include "ReceiverWindow.h"
#include "AtachmentWindow.h"
#include "GenderWindow.h"
#include "About.h"
#include "SendWindow.h"
void MainWindow::lang_slot()
{
    this->lang();
}
void MainWindow::clear_slot()
{
    this->clear();
    if (senderWindow!=NULL && senderWindow->isEnabled())
    {
        ((SenderWindow*)senderWindow)->clear();
    }
    if (receiverWindow!=NULL && receiverWindow->isEnabled())
    {
        ((ReceiverWindow*)receiverWindow)->clear();
    }
}
//отправителя
void MainWindow::sender_slot()
{
    senderWindow = new SenderWindow(this);
}
//получателя
void MainWindow::receiver_slot()
{
    receiverWindow = new ReceiverWindow(this);
}
//вложений
void MainWindow::atach_slot()
{
    atachWindow = new AtachmentWindow(this);
}
//при нажатии на кнопку "Имя"
void MainWindow::name_slot()
{
    ((QTextEdit*)messageEdit)->insertPlainText(nameCode.c_str());
}
//при нажатии на кнопку "Пол"
void MainWindow::gender_slot()
{
    new GenderWindow(this);
}
#define DEBUG 1
//при нажатии на кнопку "Цитата"
//не работает для русских символов Unicode!
void MainWindow::quote_slot()
{
    std::string text = ((QTextEdit*)messageEdit)->toPlainText().toStdString();
    QTextCursor cursor = ((QTextEdit*)messageEdit)->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd()-1;
#ifdef DEBUG
    std::cerr << "start: " << start << " end: " << end << std::endl;
#endif
    QString tex = ((QTextEdit*)messageEdit)->toPlainText();
    tex.chop(tex.length()-end);
    end = tex.toStdString().length();
    tex.chop(tex.length()-start);
    start = tex.toStdString().length();
#ifdef DEBUG
    std::cerr << "start: " << start << " end: " << end << std::endl;
#endif
    toQuote(text, start, end);
#ifdef DEBUG
    std::cerr << text << std::endl;
#endif

    ((QTextEdit*)messageEdit)->setPlainText(QString::fromStdString(text));
}
//О программе
void MainWindow::about_slot()
{
    new About();
}
//отправить сообщение
void MainWindow::message_slot()
{
    data->message = ((QTextEdit*)this->messageEdit)->toPlainText().toStdString();
    data->theme = ((QLineEdit*)this->themeEdit)->text().toStdString();
    new SendWindow(this);
}
//создать цитату / create a quote
void MainWindow::toQuote(std::string& s, int b, int e)
{
#ifdef DEBUG
    std::cerr << "begin: " << b << " end: " << e << std::endl;
    std::cerr << s.substr(b,e) << std::endl;
#endif
    std::string quote = "> ";
    char delim = '\n';
    std::ostringstream omem;
    std::istringstream imem(s.substr(b,e));
    if (b!=0)
    {
        if (s[b-1]!=delim)
        {
            omem << std::endl;
        }
    }
    //imem.ignore(5, delim);
    const int MAX = 255;
    while (imem.good())
    {
        char line[MAX+1];
        imem.getline(line, MAX, delim);
        omem << quote << line << std::endl;
    }

    s = s.substr(0,b) + omem.str() + s.substr(e+1);
}
