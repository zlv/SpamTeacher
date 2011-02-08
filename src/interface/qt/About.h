#ifndef ABOUT_H
#define ABOUT_H
#include "BaseWindow.h"
class About : BaseWindow
{
    Q_OBJECT

    QWidget *window;
    QLayout* vbox;
    //поле, в котором размещается содержимое README, field that contains text from README
    QWidget *view;
    /////////////////////////////////
public:
    About();
    ~About();
};
#endif
