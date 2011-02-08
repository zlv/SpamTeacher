#include "AddReceiverWindow.h"
#include "../func.h"
#include "ReceiverWindow.h"
AddReceiverWindow::AddReceiverWindow(QWidget* w) : BaseWindow(), window(w)
{
    //////////////////////////////////
    this->setWindowTitle(tr("AddReceiver"));
    /////////////////////////////////
    nameEdit = new_edit(tr("Name"));
    nameLabel = new_label(tr("Receiver name"));
    senderEdit = new_edit(tr("e-mail"));
    senderLabel = new_label(tr("Receiver e-mail"));
    /////////////////////////////////
    okButton = new_button(tr("ok"));
    cancelButton = new_button(tr("cancel"));
    /////////////////////////////////////
    connect(okButton,SIGNAL(clicked()),this,SLOT(ok_slot()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    /////////////////////////////////////
    box[0] = new QHBoxLayout();
    box[1] = new QHBoxLayout();
    box[2] = new QHBoxLayout();
    /////////////////////////////////////
    ((QLayout*)box[0])->addWidget(nameLabel);
    ((QLayout*)box[0])->addWidget(nameEdit);
    ((QLayout*)box[1])->addWidget(senderLabel);
    ((QLayout*)box[1])->addWidget(senderEdit);
    ((QLayout*)box[2])->addWidget(okButton);
    ((QLayout*)box[2])->addWidget(cancelButton);
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
AddReceiverWindow::~AddReceiverWindow()
{
}
////////////////////////////////////
void AddReceiverWindow::ok_slot()
{
    ((ReceiverWindow*)window)->add(((QLineEdit*)nameEdit)->text(), ((QLineEdit*)senderEdit)->text());
    emit close();
}
