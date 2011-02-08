#ifndef ADDMULTRECEIVERWINDOW_H
#define ADDMULTRECEIVERWINDOW_H
#include "BaseWindow.h"
#include "ReceiverWindow.h"
class AddMultReceiverWindow : public BaseWindow
{
    Q_OBJECT

    static const int count = 2;
    QLayout* box[count];
    QLayout* vbox;
    QWidget* instLabel;
    /////////////////////////////////
    QWidget* okButton;
    QWidget* cancelButton;
public:
    QWidget* window;
    QWidget* edit;
    AddMultReceiverWindow(QWidget*);
    ~AddMultReceiverWindow();
private slots:
    void ok_slot();
private:
    std::string get_gender_message(std::string,std::string,std::string);
};

#endif // ADDMULTRECEIVERWINDOW_H
