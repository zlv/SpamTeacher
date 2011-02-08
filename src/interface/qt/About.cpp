#include "About.h"
#include <fstream>
#include <string>
const int MAX=200;
const int SUPERMAX=100000;
#ifndef _WIN32
    char readme[][MAX+1] = {"README","/usr/local/share/MassDelivery/doc/README"};
#else
    char readme[][MAX+1] = {"Readme.txt"};
#endif
About::About() : BaseWindow()
{
    //////////////////////////////////
    this->setWindowTitle(tr("About"));
    /////////////////////////////////
    std::string text;
    std::ifstream file;
    int size = sizeof(readme)/sizeof(readme[0]);
    for (int i=0; i<size; i++)
    {
        file.open(readme[i]);
        if (file.good()) break;
    }
    char buf[SUPERMAX+1];
    file.getline(buf,SUPERMAX,'\n');
    text=buf;
    while (file.good())
    {
        file.getline(buf,SUPERMAX,'\n');
        if (file.eof()) break;
        text+=std::string("\n")+buf;
    }
    file.close();
    
    view = new QTextEdit;
    ((QTextEdit*)view)->setPlainText(text.c_str());
    /////////////////////////////////////
    vbox = new QVBoxLayout();
    vbox->addWidget(view);

    this->setLayout(vbox);

    this->show();
}
////////////////////////////////////
About::~About()
{
}
