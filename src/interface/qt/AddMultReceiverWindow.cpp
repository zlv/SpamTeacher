#include "AddMultReceiverWindow.h"
#include "../func.h"
#include "ReceiverWindow.h"
#include <sstream>
AddMultReceiverWindow::AddMultReceiverWindow(QWidget* w) : BaseWindow(), window(w)
{
    //////////////////////////////////
    this->setWindowTitle(tr("Adding many receivers"));
    /////////////////////////////////
    edit = new QTextEdit();
    instLabel = new_label(tr("Enter addreses to this field, they would be added to list"));;
    /////////////////////////////////
    okButton = new_button(tr("&Ok"));
    cancelButton = new_button(tr("&Cancel"));
    /////////////////////////////////////
    connect(okButton,SIGNAL(clicked()),this,SLOT(ok_slot()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    /////////////////////////////////////
    box[0] = new QVBoxLayout();
    box[1] = new QHBoxLayout();
    /////////////////////////////////////
    ((QLayout*)box[0])->addWidget(instLabel);
    ((QLayout*)box[0])->addWidget(edit);
    ((QLayout*)box[1])->addWidget(okButton);
    ((QLayout*)box[1])->addWidget(cancelButton);
    /////////////////////////////////////
    vbox = new QVBoxLayout();
    for (int i=0; i<count; i++)
    {
        ((QBoxLayout*)vbox)->addLayout(box[i]);
    }

    this->setLayout(vbox);

    this->show();
}
////////////////////////////////////
AddMultReceiverWindow::~AddMultReceiverWindow()
{
}
////////////////////////////////////
void AddMultReceiverWindow::ok_slot()
{
    QString text = ((QTextEdit*)edit)->toPlainText();
    std::string str = text.toStdString();
    std::istringstream im(str);
    while (im.good())
    {
        std::string temp;
        im >> temp;
        ((ReceiverWindow*)window)->add(temp);
    }

    emit close();
}
