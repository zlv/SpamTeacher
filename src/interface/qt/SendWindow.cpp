#include "SendWindow.h"
#include "MainWindow.h"
#include "../func.h"
#include "../../Data.h"
#include "PassWindow.h"
#include <iostream>
#include <QProgressBar>
SendWindow::SendWindow(QWidget* w) : BaseWindow(), window(w)
{
    ////////////////////////////////////////////
    this->setWindowTitle(tr("Delivery process"));
    /////////////////////////////////
    statusLabel = new_label(tr("Send message: "));
    progress = new QProgressBar();
    /////////////////////////////////////
    box[0] = new QHBoxLayout();
    box[1] = new QHBoxLayout();
    /////////////////////////////////////
    view = new QTextEdit();
    /////////////////////////////////////
    ((QLayout*)box[0])->addWidget(statusLabel);
    ((QLayout*)box[1])->addWidget(progress);
    /////////////////////////////////////
    vbox = new QVBoxLayout();
    for (int i=0; i<count; i++)
    {
        ((QBoxLayout*)vbox)->addLayout(box[i]);
    }
    ((QBoxLayout*)vbox)->addWidget(view);

    this->setLayout(vbox);

    this->show();
    ok = 0;
    if (((MainWindow*)window)->data->password=="")
    {
        new PassWindow(this);
    }
    else
    {
        ok=1;
    }
    if (ok)
    {
        ((MainWindow*)window)->data->startThread(this);
    }
}
SendWindow::~SendWindow()
{
}

void SendWindow::closeEvent(QCloseEvent* e)
{
    ((MainWindow*)window)->data->stop();
    e->accept();
}

void SendWindow::customEvent(QEvent* pe)
{
    if ((int)pe->type() == SendEvent::SendType)
    {
        SendEvent::SendData sendData = (((SendEvent*)(pe))->value());
        if (sendData.value==101)
        {
            writeLog(sendData.qtext);
            ((QLabel*)statusLabel)->setText(QString(sendData.text.c_str()));
            ((QProgressBar*)progress)->setValue(100);
        }
        else
        {
            writeLog(sendData.qtext);
            ((QProgressBar*)progress)->setValue(sendData.value);
            ((QLabel*)statusLabel)->setText(QString(sendData.text.c_str()));
        }
    }
    else if ((int)pe->type() == PassEvent::PassType)
    {
        ((MainWindow*)window)->data->startThread(this);
    }
    else if ((int)pe->type() == WrongPassEvent::WrongPassType)
    {
        ((QLabel*)statusLabel)->setText(tr("Error"));
        writeLog(((WrongPassEvent*)pe)->data);
        new PassWindow(this,tr("Wrong password! Please, try again"));
    }
    else if ((int)pe->type() == BadServerEvent::BadServerType)
    {
        ((QLabel*)statusLabel)->setText(tr("Error"));
        writeLog(((BadServerEvent*)pe)->data);
    }
    QWidget::customEvent(pe);
}

void SendWindow::writeLog(std::string text)
{
    ((QTextEdit*)view)->setPlainText(text.c_str());
}
