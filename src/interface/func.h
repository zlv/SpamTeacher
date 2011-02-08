#ifndef FUNC_H
#define FUNC_H
#include "interface.h"
//Функции для создания какого-нибудь виджета
////////////////////////////////////
#ifdef QT
#include <QWidget>
#define PWidget QWidget*
#define TString QString
#else
//#ifdef GTK
#include <gtk/gtk.h>
#define PWidget GtkWidget*
#define TString const char*
//#endif
#endif
PWidget new_edit(TString);
PWidget new_button(TString);
PWidget new_label(TString);
void testsmth();
////////////////////////////////////
#endif
