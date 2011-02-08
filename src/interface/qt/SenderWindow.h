#ifndef SENDER_WINDOW_H
#define SENDER_WINDOW_H
#include "BaseWindow.h"
//настройка отправителей
class SenderWindow : public BaseWindow
{
    Q_OBJECT

    QWidget* mainWindow;
    ////////////////////////////////
    static const int count = 5;
    QWidget* senderLabel;
    QWidget* serverLabel;
    QWidget* nameLabel;
    QWidget* userLabel;
    QWidget* passLabel;
    /////////////////////////////////
    QWidget* okButton;
    QWidget* cancelButton;
    QLayout* vbox;
    QLayout* box[count];
public:
    ///////////////
    QWidget* window;
    /////////////////////
    QWidget* senderEdit;
    QWidget* serverEdit;
    QWidget* nameEdit;
    QWidget* userEdit;
    QWidget* passEdit;
    SenderWindow(QWidget*);
    ~SenderWindow();
    void clear();
private slots:
    //"Готово"
    void ok_slot();
public:
    void closeEvent(QCloseEvent*);
};
#endif
