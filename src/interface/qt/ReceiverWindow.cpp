#include "MainWindow.h"
#include "ReceiverWindow.h"
#include "../../Receiver.h"
#include "../func.h"
#include "../../Data.h"
#include "AddReceiverWindow.h"
#include "AddMultReceiverWindow.h"
#include <iostream>
#include <QTreeView>
//строки
QString titles[4];
ReceiverWindow::ReceiverWindow(QWidget* w) : BaseWindow()
{
    ////////////////////////////////////////////
    titles[0] = tr("Name");
    titles[1] = tr("e-mail");
    titles[2] = tr("Man?");
    titles[3] = tr("Enabled");
    mainWindow = w;
    this->setWindowTitle(tr("Set receivers"));
    /////////////////////////////////
    box[0] = new QHBoxLayout();
    box[1] = new QHBoxLayout();
    /////////////////////////////////
    model = create_and_fill_model();
    store = create_and_fill_store();
    /////////////////////////////////
    addButton = new_button(tr("&Add"));
    delButton = new_button(tr("&Delete"));
    /////////////////////////////////
    okButton = new_button(tr("&Ok"));
    cancelButton = new_button(tr("&Cancel"));
    /////////////////////////////////
    connect(addButton,SIGNAL(clicked()),this,SLOT(add_slot()));
    connect(delButton,SIGNAL(clicked()),this,SLOT(delete_slot()));
    /////////////////////////////////////
    connect(okButton,SIGNAL(clicked()),this,SLOT(ok_slot()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    /////////////////////////////////////
    ((QLayout*)box[0])->addWidget(store);
    ((QLayout*)box[1])->addWidget(okButton);
    ((QLayout*)box[1])->addWidget(addButton);
    ((QLayout*)box[1])->addWidget(delButton);
    ((QLayout*)box[1])->addWidget(cancelButton);
    //add e-mails in program button / кнопка добавления адресов
    addMultipleButton = new_button(tr("Add many receivers"));
    connect(addMultipleButton,SIGNAL(clicked()),this,SLOT(add_mult_slot()));
    ((QLayout*)box[0])->addWidget(addMultipleButton);
    /////////////////////////////////////

    vbox = new QVBoxLayout();
    for (int i=0; i<count; i++)
    {
        ((QBoxLayout*)vbox)->addLayout(box[i]);
    }

    this->setLayout(vbox);

    this->show();
}

ReceiverWindow::~ReceiverWindow()
{
}

//создание модели списка
QObject* ReceiverWindow::create_and_fill_model()
{
    Data* data = ((MainWindow*)mainWindow)->data;
    cols = 4;
    rows = data->recvEmail.size();
    model = new QStandardItemModel(rows, cols);
    for (int nRow = 0; nRow < rows; ++nRow)
    {
        std::string* str = &data->recvEmail[nRow];
        Receiver* rec = &data->recvData[nRow];
        ((QStandardItemModel*)model)->setData(
                ((QStandardItemModel*)model)->index(nRow, 0), rec->name.c_str());
        ((QStandardItemModel*)model)->setData(
                ((QStandardItemModel*)model)->index(nRow, 1), str->c_str());
        ((QStandardItemModel*)model)->setData(
                ((QStandardItemModel*)model)->index(nRow, 2), rec->male);
        ((QStandardItemModel*)model)->setData(
                ((QStandardItemModel*)model)->index(nRow, 3), rec->enabled);
    }
    QStringList qList;
    for (int i=0; i<(int)(sizeof(titles)/sizeof(titles[0])); i++)
    {
        qList << titles[i];
    }
    ((QStandardItemModel*)model)->setHorizontalHeaderLabels(qList);

    return model;
}

#include "Delegate.h"
QWidget* ReceiverWindow::create_and_fill_store()
{
    QWidget* treeView = new QTreeView();
    CheckDelegate* check = new CheckDelegate();
    ((QTreeView*)treeView)->setItemDelegateForColumn(2,check);
    ((QTreeView*)treeView)->setItemDelegateForColumn(3,check);
    ((QTreeView*)treeView)->setModel((QStandardItemModel*)model);

    return treeView;
}

void ReceiverWindow::clear()
{
    ((QStandardItemModel*)model)->removeRows(0,rows);
}

void ReceiverWindow::ok_slot()
{
    Data* data = ((MainWindow*)mainWindow)->data;
    //очистка
    data->recvEmail.clear();
    data->recvData.clear();

    for (int nRow = 0; nRow < rows; ++nRow)
    {
        int count=0;
        Receiver rec;

        QModelIndex qIndex = ((QStandardItemModel*)model)->index(nRow, count++);
        QVariant qData = ((QStandardItemModel*)model)->data(qIndex);
        rec.name = qData.toString().toStdString();

        qIndex = ((QStandardItemModel*)model)->index(nRow, count++);
        qData = ((QStandardItemModel*)model)->data(qIndex);
        data->recvEmail.push_back(qData.toString().toStdString());

        qIndex = ((QStandardItemModel*)model)->index(nRow, count++);
        qData = ((QStandardItemModel*)model)->data(qIndex);
        rec.male = qData.toBool();

        qIndex = ((QStandardItemModel*)model)->index(nRow, count++);
        qData = ((QStandardItemModel*)model)->data(qIndex);
        rec.enabled = qData.toBool();

        data->recvData.push_back(rec);
    }

    emit close();

    std::string text;

    //вставка информации одной строкой в виджет главного окна
    text = data->get_message_about_receiver();
    /////////////////////////////////////////////////////////
    ((QLineEdit*)((MainWindow*)mainWindow)->receiverEdit)->setText(text.c_str());
}

void ReceiverWindow::add_slot()
{
    new AddReceiverWindow(this);
}

void ReceiverWindow::delete_slot()
{
    int row = ((QTreeView*)store)->selectionModel()->currentIndex().row();

    ((QStandardItemModel*)model)->removeRow(row);
    rows--;
}
///////////////////
void ReceiverWindow::closeEvent(QCloseEvent* event)
{
    event->accept();
}

void ReceiverWindow::add_mult_slot()
{
    new AddMultReceiverWindow(this);
}

void ReceiverWindow::add(std::string e)
{
    QString str = QString::fromStdString(e);
    add(str,str);
}

void ReceiverWindow::add(QString n, QString e)
{
    int nRow = rows;

    ((QStandardItemModel*)model)->insertRow(rows++);

    //std::string templat[] = {"Имя", "Почка"};
    int nCol=0;

    ((QStandardItemModel*)model)->setData(
            ((QStandardItemModel*)model)->index(nRow, nCol), n);
    nCol++;

    ((QStandardItemModel*)model)->setData(
            ((QStandardItemModel*)model)->index(nRow, nCol), e);
    nCol++;

    for (; nCol<cols; nCol++)
    {
        ((QStandardItemModel*)model)->setData(
                ((QStandardItemModel*)model)->index(nRow, nCol), 1);
    }
}
