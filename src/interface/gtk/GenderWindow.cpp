#include "GenderWindow.h"
#include "../func.h"
GenderWindow::GenderWindow()
{
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Выбор шаблона");
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	/////////////////////////////////
	box[0] = gtk_hbox_new(0,5);
	box[1] = gtk_hbox_new(0,5);
	box[2] = gtk_hbox_new(0,15);
	/////////////////////////////////
	maleEdit = new_edit("ый",NULL);
	maleLabel = new_label("муж.",NULL);
	femaleEdit = new_edit("ая",NULL);
	femaleLabel = new_label("жен.",NULL);
	/////////////////////////////////
	okButton = new_button("Готово",gender_ok_signal);
	cancelButton = new_button("Отмена",gender_cancel_signal);
	gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(gender_destroy_signal), NULL);
	/////////////////////////////////////
	gtk_box_pack_start(GTK_BOX(box[0]),maleLabel,0,0,5);
	gtk_box_pack_start(GTK_BOX(box[0]),maleEdit,0,0,5);
	gtk_box_pack_start(GTK_BOX(box[1]),femaleLabel,0,0,5);
	gtk_box_pack_start(GTK_BOX(box[1]),femaleEdit,0,0,5);
	gtk_box_pack_start(GTK_BOX(box[2]),okButton,1,1,5);
	gtk_box_pack_start(GTK_BOX(box[2]),cancelButton,1,1,5);
	/////////////////////////////////////
	GtkWidget *vbox = gtk_vbox_new(0,5);
	for (int i=0; i<count; i++)
	{
		gtk_box_pack_start(GTK_BOX(vbox),box[i],1,0,5);
	}
	
	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	gtk_widget_show_all(window);
}
////////////////////////////////////
GenderWindow::~GenderWindow()
{
}
