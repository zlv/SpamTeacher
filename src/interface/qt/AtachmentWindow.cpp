#include "MainWindow.h"
#include "AtachmentWindow.h"
#include "../func.h"
#include "../../Data.h"
#include <QTreeView>
//строки
QString titles2[1];
AtachmentWindow::AtachmentWindow(QWidget* w) : BaseWindow()
{
    //////////////////////////////////
    titles2[0] = tr("File");
    mainWindow = w;
    this->setWindowTitle(tr("Select file atachments"));
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
    /////////////////////////////////////

    vbox = new QVBoxLayout();
    for (int i=0; i<count; i++)
    {
        ((QBoxLayout*)vbox)->addLayout(box[i]);
    }

    this->setLayout(vbox);

    this->show();
}

AtachmentWindow::~AtachmentWindow()
{
}
//создание модели списка
QObject* AtachmentWindow::create_and_fill_model()
{
    Data* data = ((MainWindow*)mainWindow)->data;
    cols = 1;
    rows = data->fileatach.size();
    model = new QStandardItemModel(rows, cols);
    for (int nRow = 0; nRow < rows; ++nRow)
    {
        std::string* str = &data->fileatach[nRow];
        QString file = str->c_str();
        ((QStandardItemModel*)model)->setData(
                ((QStandardItemModel*)model)->index(nRow, 0), file);
    }
    QStringList qList;
    for (int i=0; i<(int)(sizeof(titles2)/sizeof(titles2[0])); i++)
    {
        qList << titles2[i];
    }
    ((QStandardItemModel*)model)->setHorizontalHeaderLabels(qList);

    return model;
}

#include "Delegate.h"
QWidget* AtachmentWindow::create_and_fill_store()
{
    QWidget* treeView = new QTreeView();
    ((QTreeView*)treeView)->setModel((QStandardItemModel*)model);

    return treeView;
}

void AtachmentWindow::clear()
{
    ((QStandardItemModel*)model)->removeRows(0,rows);
}

void AtachmentWindow::ok_slot()
{
    Data* data = ((MainWindow*)mainWindow)->data;
    //очистка
    data->fileatach.clear();

    for (int nRow = 0; nRow < rows; ++nRow)
    {
        QModelIndex qIndex = ((QStandardItemModel*)model)->index(nRow, 0);
        QVariant qData = ((QStandardItemModel*)model)->data(qIndex);
        std::string str = qData.toString().toStdString();

                data->fileatach.push_back(str);
    }

    std::string text;

    //вставка информации одной строкой в виджет главного окна
    text = data->get_message_about_atachment();
    /////////////////////////////////////////////////////////
    ((QLineEdit*)((MainWindow*)mainWindow)->atachEdit)->setText(text.c_str());

    emit close();
}

void AtachmentWindow::add_slot()
{
    int nRow = rows;
    QString qStr = QFileDialog::getOpenFileName(this,tr("Select file"),"");
    ((QStandardItemModel*)model)->insertRow(rows++);
    ((QStandardItemModel*)model)->setData(
            ((QStandardItemModel*)model)->index(nRow, 0), qStr);
}

void AtachmentWindow::delete_slot()
{
    int row = ((QTreeView*)store)->selectionModel()->currentIndex().row();

    ((QStandardItemModel*)model)->removeRow(row);
    rows--;
}
///////////////////
void AtachmentWindow::closeEvent(QCloseEvent* event)
{
    event->accept();
}
