#ifndef ADD_RECEIVER_WINDOW_H
#define ADD_RECEIVER_WINDOW_H
#include "BaseWindow.h"
//окно для добавления элемента в список получателей
class AddReceiverWindow : BaseWindow
{
    Q_OBJECT

    QWidget *window;
    static const int count = 3;
    QLayout *box[count];
    QWidget* nameLabel;
    QWidget* senderLabel;
    /////////////////////////////////
    QWidget* okButton;
    QWidget* cancelButton;
    QLayout* vbox;
public:
    QWidget* nameEdit;
    QWidget* senderEdit;
    AddReceiverWindow(QWidget*);
    ~AddReceiverWindow();
private slots:
    void ok_slot();
};
#endif
