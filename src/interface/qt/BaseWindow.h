#ifndef BASEWINDOW_H
#define BASEWINDOW_H
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QLayout>
#include <QCloseEvent>
#include <QStandardItem>
#include <QFileDialog>
class BaseWindow : public QWidget
{
    Q_OBJECT
    QIcon *icon;

public:
    BaseWindow();
};

#endif // BASEWINDOW_H
