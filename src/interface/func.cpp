#include "func.h"
#ifdef QT
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#endif
//создание объекта с заголовком и подсоединение к нему сигнала
PWidget new_edit(TString str)
{
#ifdef QT
    PWidget w = new QLineEdit();
    ((QLineEdit*)w)->setText(str);
#endif
#ifdef GTK
    PWidget* w = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(w),str);
#endif
    return w;
}

//а тут ещё подсоединение каких-нибудь данных
PWidget new_button(TString str)
{
#ifdef QT
    return new QPushButton(str);
#endif
#ifdef GTK
    return gtk_button_new_with_mnemonic(str);
#endif
}

//а тут сигнал просто для шаблона
PWidget new_label(TString str)
{
#ifdef QT
    return new QLabel(str);
#endif
#ifdef GTK
    return gtk_label_new(str);
#endif
}
