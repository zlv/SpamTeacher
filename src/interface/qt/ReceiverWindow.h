#ifndef RECEIVER_WINDOW_H
#define RECEIVER_WINDOW_H
#include "BaseWindow.h"
class ReceiverWindow : public BaseWindow
{
    Q_OBJECT

    static const int count = 2;
    QLayout* vbox;
    QLayout* box[count];
    /////////////////////////////////
    QWidget* addButton;
    QWidget* delButton;
    /////////////////////////////////
    QWidget* okButton;
    QWidget* cancelButton;
    /////////////////////////////////
    //создание модели списка
    QObject* create_and_fill_model();
    //представления списка (должно вызываться после создания модели)
    QWidget* create_and_fill_store();
public:
    //список
    QWidget* store;
    //модель списка
    QObject* model;
    int rows,cols;
    ReceiverWindow(QWidget*);
    ~ReceiverWindow();
    void clear();
private slots:
    //"Готово"
    void ok_slot();
    void add_slot();
    void delete_slot();
public:
    void closeEvent(QCloseEvent*);
private:
    //button for multiple addreses / для того, чтобы много вводить
    QWidget* addMultipleButton;
private slots:
    void add_mult_slot();
public:
    void add(std::string);
    void add(QString, QString);
private:
    ////////////////////////////////
    QWidget* mainWindow;
};
#endif
