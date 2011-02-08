#ifndef PASSWINDOW_H
#define PASSWINDOW_H
#include "BaseWindow.h"
class PassWindow : public BaseWindow
{
    Q_OBJECT

    static const int count = 2;
    QLayout* box[count];
    /////////////////////////////////
    QWidget* okButton;
    QWidget* cancelButton;
    QWidget* label;
public:
    ///////////////
    QWidget* window;
    QWidget* edit;
    PassWindow(QWidget* w,QString mess="");
    ~PassWindow();
private slots:
    void ok_slot();
};
#endif // PASSWINDOW_H
