#include "PassWindow.h"
#include "SendWindow.h"
#include "MainWindow.h"
#include "../func.h"
#include <iostream>
#include <QApplication>
PassWindow::PassWindow(QWidget* w, QString mess) : BaseWindow(), window(w)
{
    //////////////////////////////////
    if (mess=="")
    {
        std::cerr << tr("Enter password").toStdString();
        this->setWindowTitle(tr("Enter password"));
    }
    else
    {
        std::cerr << mess.toStdString();
        this->setWindowTitle(mess);
    }
    /////////////////////////////////
    edit = new_edit("");
    label = new_label(tr("Password:"));
    ((QLineEdit*)edit)->setEchoMode(QLineEdit::Password);
    /////////////////////////////////
    okButton = new_button(tr("&Ok"));
    cancelButton = new_button(tr("&Cancel"));
    /////////////////////////////////////
    connect(okButton,SIGNAL(clicked()),this,SLOT(ok_slot()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    /////////////////////////////////////
    box[0] = new QHBoxLayout();
    box[1] = new QHBoxLayout();
    /////////////////////////////////////
    ((QLayout*)box[0])->addWidget(label);
    ((QLayout*)box[0])->addWidget(edit);
    ((QLayout*)box[1])->addWidget(okButton);
    ((QLayout*)box[1])->addWidget(cancelButton);
    /////////////////////////////////////
    QLayout* vbox = new QVBoxLayout();
    for (int i=0; i<count; i++)
    {
        ((QBoxLayout*)vbox)->addLayout(box[i]);
    }

    this->setLayout(vbox);

    this->show();
}
////////////////////////////////////
PassWindow::~PassWindow()
{
}
////////////////////////////////////
void PassWindow::ok_slot()
{
    //получение строк из виджетов
    std::string sPassword = ((QLineEdit*)edit)->text().toStdString();
    //удаление окна
    emit close();

    ((MainWindow*)((SendWindow*)window)->window)->data->password = sPassword;
    PassEvent* pe = new PassEvent();
    QApplication::postEvent(window, pe);
}

