#include "SenderWindow.h"
//функции создания виджетов
#include "../func.h"
//для data и для сигналов
#include "gui.h"
#include "Data.h"
SenderWindow::SenderWindow()
{
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Настройки отправителя");
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	/////////////////////////////////
	box[0] = gtk_hbox_new(0,5);
	box[1] = gtk_hbox_new(0,5);
	box[2] = gtk_hbox_new(0,5);
	box[3] = gtk_hbox_new(0,15);
	/////////////////////////////////
	senderEdit = new_edit(data->senderEmail.c_str(),NULL);
	senderLabel = new_label("Адрес отправителя",NULL);
	///////////////////////////////////////
	serverEdit = new_edit(data->server.c_str(),NULL);
	serverLabel = new_label("Сервер исходящей почты(smtp)",NULL);
	///////////////////////////////////////
	userEdit = new_edit(data->user.c_str(),NULL);
	userLabel = new_label("Имя пользователя",NULL);
	////////////////////////////////
	passEdit = new_edit(data->password.c_str(),NULL);
	passLabel = new_label("Пароль",NULL);
	gtk_entry_set_visibility(GTK_ENTRY(passEdit), FALSE);
	///////////////////////////////////////
	gtk_entry_set_activates_default(GTK_ENTRY(senderEdit),0);
	gtk_entry_set_activates_default(GTK_ENTRY(serverEdit),0);
	gtk_entry_set_activates_default(GTK_ENTRY(userEdit),1);
	gtk_entry_set_activates_default(GTK_ENTRY(passEdit),0);
	/////////////////////////////////
	okButton = new_button("_Готово",sender_ok_signal,NULL,1);
	GTK_WIDGET_SET_FLAGS (okButton, GTK_CAN_DEFAULT);
	gtk_widget_grab_focus(okButton);
	cancelButton = new_button("_Отмена",sender_cancel_signal,NULL,1);
	gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(sender_destroy_signal), NULL);
	/////////////////////////////////////
	gtk_box_pack_start(GTK_BOX(box[0]),senderLabel,0,0,5);
	gtk_box_pack_start(GTK_BOX(box[0]),senderEdit,0,0,5);
	gtk_box_pack_start(GTK_BOX(box[1]),serverLabel,0,0,5);
	gtk_box_pack_start(GTK_BOX(box[1]),serverEdit,0,0,5);
	gtk_box_pack_start(GTK_BOX(box[2]),userLabel,1,0,5);
	gtk_box_pack_start(GTK_BOX(box[2]),userEdit,1,0,5);
	gtk_box_pack_start(GTK_BOX(box[2]),passLabel,1,0,5);
	gtk_box_pack_start(GTK_BOX(box[2]),passEdit,1,0,5);
	gtk_box_pack_start(GTK_BOX(box[3]),okButton,1,1,5);
	gtk_box_pack_start(GTK_BOX(box[3]),cancelButton,1,1,5);
	/////////////////////////////////////
	GtkWidget *vbox = gtk_vbox_new(0,5);
	for (int i=0; i<count; i++)
	{
		gtk_box_pack_start(GTK_BOX(vbox),box[i],1,0,5);
	}
	
	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	gtk_widget_show_all(window);
	///////коментариев не нужно///////////(всё и так понятно)
}

SenderWindow::~SenderWindow()
{
}

#include <iostream>
void SenderWindow::clear()
{
	gtk_entry_set_text(GTK_ENTRY(senderEdit),"");
	gtk_entry_set_text(GTK_ENTRY(serverEdit),"");
	gtk_entry_set_text(GTK_ENTRY(userEdit),"");
	gtk_entry_set_text(GTK_ENTRY(passEdit),"");
}
