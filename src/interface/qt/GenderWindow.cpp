#include "GenderWindow.h"
#include "MainWindow.h"
#include "../func.h"
#include "BaseWindow.h"
GenderWindow::GenderWindow(QWidget* w) : BaseWindow(), window(w)
{
    //////////////////////////////////
    this->setWindowTitle(tr("Template settings"));
    /////////////////////////////////
    maleEdit = new_edit(tr("mr"));
    maleLabel = new_label(tr("male"));
    femaleEdit = new_edit(tr("mrs"));
    femaleLabel = new_label(tr("female"));
    /////////////////////////////////
    okButton = new_button(tr("&Ok"));
    cancelButton = new_button(tr("&Cancel"));
    /////////////////////////////////////
    connect(okButton,SIGNAL(clicked()),this,SLOT(ok_slot()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    /////////////////////////////////////
    box[0] = new QHBoxLayout();
    box[1] = new QHBoxLayout();
    box[2] = new QHBoxLayout();
    /////////////////////////////////////
    ((QLayout*)box[0])->addWidget(maleLabel);
    ((QLayout*)box[0])->addWidget(maleEdit);
    ((QLayout*)box[1])->addWidget(femaleLabel);
    ((QLayout*)box[1])->addWidget(femaleEdit);
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
GenderWindow::~GenderWindow()
{
}
////////////////////////////////////
void GenderWindow::ok_slot()
{
    //получение строк из виджетов
    std::string sMale = ((QLineEdit*)maleEdit)->text().toStdString();
    std::string sFemale = ((QLineEdit*)femaleEdit)->text().toStdString();
    //удаление окна
    emit close();

    //получение текста из шаблона genderCode
    std::string text = get_gender_message(sMale,sFemale,genderCode);

    ((QTextEdit*)((MainWindow*)window)->messageEdit)->insertPlainText(text.c_str());
}
//получение информации о выборе текста в зависимости от пола из шаблона t
std::string GenderWindow::get_gender_message(std::string m,std::string f,std::string t)
{
    std::string r = t;
    int pos;

    pos = r.find("%ms")+2;
    r.replace(pos,1,m);
    pos = r.find("%fs")+2;
    r.replace(pos,1,f);
    return r;
}
