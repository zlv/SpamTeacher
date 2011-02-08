#include "BaseWindow.h"
#include <fstream>
#include <QDesktopWidget>
const int MAX = 200;
const char sIcon[][MAX] = {"icon.xpm", "/usr/local/share/MassDelivery/icon.xpm"};
BaseWindow::BaseWindow()
{
    std::ifstream file;
    int i=0;
    int size = sizeof(sIcon)/sizeof(sIcon[0]);
    for (; i<size; i++)
    {
        file.open(sIcon[i]);
        if (file.good()) break;
    }
    icon = new QIcon(sIcon[i]);
    this->setWindowIcon((*icon));
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());
    //получаем прямоугольник с размерами как у экрана
    QPoint center = rect.center(); //получаем координаты центра экрана
    center.setX(center.x() - (this->width()/2));
    center.setY(center.y() - (this->height()/2));
    this->move(center);
}
