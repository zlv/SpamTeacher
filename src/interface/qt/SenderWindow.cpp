#include "SenderWindow.h"
//функции создания виджетов
#include "../func.h"
//для data и для сигналов
#include "../../Data.h"
#include "MainWindow.h"
#include <iostream>
SenderWindow::SenderWindow(QWidget* w) : BaseWindow(), mainWindow(w)
{
    /////////////////////////////////////
    this->setWindowTitle(tr("Sender options"));
    /////////////////////////////////
    Data* data = ((MainWindow*)mainWindow)->data;
    /////////////////////////////////
    senderEdit = new_edit(data->senderEmail.c_str());
    senderLabel = new_label(tr("Sender e-mail"));
    /////////////////////////////////
    nameEdit = new_edit(data->senderName.c_str());
    nameLabel = new_label(tr("Your name"));
    ///////////////////////////////////////
    serverEdit = new_edit(data->server.c_str());
    serverLabel = new_label(tr("Outgoing server(smtp)"));
    ///////////////////////////////////////
    userEdit = new_edit(data->user.c_str());
    userLabel = new_label(tr("Username"));
    ////////////////////////////////
    passEdit = new_edit(data->password.c_str());
    passLabel = new_label(tr("Password"));
    ((QLineEdit*)passEdit)->setEchoMode(QLineEdit::Password);
    /////////////////////////////////
    okButton = new_button(tr("&Ok"));
    cancelButton = new_button(tr("&Cancel"));
    /////////////////////////////////
    connect(okButton,SIGNAL(clicked()),this,SLOT(ok_slot()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    /////////////////////////////////
    box[0] = new QHBoxLayout();
    box[1] = new QHBoxLayout();
    box[2] = new QHBoxLayout();
    box[3] = new QHBoxLayout();
    box[4] = new QHBoxLayout();
    /////////////////////////////////////
    ((QLayout*)box[0])->addWidget(nameLabel);
    ((QLayout*)box[0])->addWidget(nameEdit);
    ((QLayout*)box[1])->addWidget(senderLabel);
    ((QLayout*)box[1])->addWidget(senderEdit);
    ((QLayout*)box[2])->addWidget(serverLabel);
    ((QLayout*)box[2])->addWidget(serverEdit);
    ((QLayout*)box[3])->addWidget(userLabel);
    ((QLayout*)box[3])->addWidget(userEdit);
    ((QLayout*)box[3])->addWidget(passLabel);
    ((QLayout*)box[3])->addWidget(passEdit);
    ((QLayout*)box[4])->addWidget(okButton);
    ((QLayout*)box[4])->addWidget(cancelButton);
    /////////////////////////////////////
    vbox = new QVBoxLayout();
    for (int i=0; i<count; i++)
    {
        ((QBoxLayout*)vbox)->addLayout(box[i]);
    }

    this->setLayout(vbox);

    this->show();
    ///////коментариев не нужно///////////(всё и так понятно)
}

SenderWindow::~SenderWindow()
{
}

void SenderWindow::clear()
{
    ((QLineEdit*)senderEdit)->setText("");
    ((QLineEdit*)serverEdit)->setText("");
    ((QLineEdit*)userEdit)->setText("");
    ((QLineEdit*)passEdit)->setText("");
}

//"Готово"
void SenderWindow::ok_slot()
{
    Data* data = ((MainWindow*)mainWindow)->data;
    //получение информации из виджетов и запись их в data
    data->senderEmail = ((QLineEdit*)senderEdit)->text().toStdString();
    data->senderName = ((QLineEdit*)nameEdit)->text().toStdString();
    data->server = ((QLineEdit*)serverEdit)->text().toStdString();
    data->user = ((QLineEdit*)userEdit)->text().toStdString();
    data->password = ((QLineEdit*)passEdit)->text().toStdString();
    //удаление окна
    emit close();

    std::string text;
    //получение полной информации об отправителе в text
    text = data->get_message_about_sender();
    //и вставка этой информации в виджет в главном окне
    ((QLineEdit*)((MainWindow*)mainWindow)->senderEdit)->setText(text.c_str());
}
///////////////////
void SenderWindow::closeEvent(QCloseEvent* event)
{
    event->accept();
}
