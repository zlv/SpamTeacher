#ifndef GENDER_WINDOW_H
#define GENDER_WINDOW_H
#include "BaseWindow.h"
class GenderWindow : public BaseWindow
{
    Q_OBJECT

    static const int count = 3;
    QLayout* box[count];
    QLayout* vbox;
    QWidget* maleLabel;
    QWidget* femaleLabel;
    /////////////////////////////////
    QWidget* okButton;
    QWidget* cancelButton;
public:
    QWidget* window;
    QWidget* maleEdit;
    QWidget* femaleEdit;
    GenderWindow(QWidget*);
    ~GenderWindow();
private slots:
    void ok_slot();
private:
    std::string get_gender_message(std::string,std::string,std::string);
};
#endif
