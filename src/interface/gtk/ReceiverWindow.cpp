#include "ReceiverWindow.h"
#include "Receiver.h"
#include "../func.h"
#include "gui.h"
#include "Data.h"
ReceiverWindow::ReceiverWindow()
{
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Выбор адресов получателей");
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	/////////////////////////////////
	box[0] = gtk_hbox_new(0,5);
	box[1] = gtk_hbox_new(0,15);
	/////////////////////////////////
	model = create_and_fill_model();
	store = GTK_WIDGET(create_and_fill_store());
	/////////////////////////////////
	addButton = new_button("Добавить",receiver_add_signal,model);
	delButton = new_button("Удалить",del_signal,store);
	/////////////////////////////////
	okButton = new_button("_Готово",receiver_ok_signal,model,1);
	cancelButton = new_button("_Отмена",receiver_cancel_signal,model,1);
	/////////////////////////////////
	GTK_WIDGET_SET_FLAGS (okButton, GTK_CAN_DEFAULT);
	gtk_widget_grab_focus(okButton);
	/////////////////////////////////
	gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(receiver_destroy_signal), NULL);
	/////////////////////////////////////
	gtk_box_pack_start(GTK_BOX(box[0]),store,1,1,5);
	gtk_box_pack_start(GTK_BOX(box[1]),okButton,1,1,5);
	gtk_box_pack_start(GTK_BOX(box[1]),addButton,1,1,5);
	gtk_box_pack_start(GTK_BOX(box[1]),delButton,1,1,5);
	gtk_box_pack_start(GTK_BOX(box[1]),cancelButton,1,1,5);
	/////////////////////////////////////
	GtkWidget *vbox = gtk_vbox_new(0,5);
	for (int i=0; i<count; i++)
	{
		gtk_box_pack_start(GTK_BOX(vbox),box[i],1,0,5);
	}
	
	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	gtk_widget_show_all(window);
}

ReceiverWindow::~ReceiverWindow()
{
}

//создание модели списка
GtkTreeModel* ReceiverWindow::create_and_fill_model()
{
	GtkTreeStore *treestore;
	GtkTreeIter toplevel;

	//должны быть 2 столбца строковые и 2 с выбором 2-х вариантов
	treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_BOOLEAN, NULL);

	//добавление в модель каждого элемента
	for (int i=0; i<data->recvEmail.size(); i++)
	{
		std::string* str = &data->recvEmail[i];
		Receiver* rec = &data->recvData[i];
		gtk_tree_store_append(treestore, &toplevel, NULL);
		gtk_tree_store_set(treestore, &toplevel,
			COL_NAME, rec->name.c_str(),
			COL_ADRESS, str->c_str(),
			COL_MALE, rec->male,
			COL_ENABLED, rec->enabled,
			-1);
	}

	return GTK_TREE_MODEL(treestore);
}

GtkTreeView* ReceiverWindow::create_and_fill_store()
{
	GtkTreeView* store = GTK_TREE_VIEW(gtk_tree_view_new());
	
	int i=0;
	GtkTreeViewColumn* column;
	GtkCellRenderer *renderer;
	for (;i<sizeof(titles)/sizeof(titles[0]); i++)
	{
		if (i<2)
		{
			//не очень понятный код для создания редактируемой ячейки с изменяемым размером и подсоединения сигнала его изменения к celled_signal
			renderer = gtk_cell_renderer_text_new ();
			g_object_set (renderer,"editable", TRUE, NULL);
			g_signal_connect (renderer, "edited", G_CALLBACK (celled_signal), model);
			g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (i));
			int col_offset = gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (store), -1, titles[i], renderer, "text", i, NULL);
			column = gtk_tree_view_get_column (GTK_TREE_VIEW (store), col_offset - 1);
			gtk_tree_view_column_set_resizable(column, 1);
		}
		else
		{
			//не очень понятный код для создания ячейки с выбором одного из двух и изменяемым размером и подсоединения сигнала его изменения к receiver_toggle_signal
			renderer = gtk_cell_renderer_toggle_new ();
			g_object_set (renderer, "xalign", 0.0, NULL);
			g_object_set_data (G_OBJECT (renderer), "column", (gint *)i);

			g_signal_connect (renderer, "toggled", G_CALLBACK (receiver_toggle_signal), model);

			int col_offset = gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (store), -1, titles[i], renderer, "active", i, NULL);

			column = gtk_tree_view_get_column (GTK_TREE_VIEW (store), col_offset - 1);
			
			gtk_tree_view_column_set_resizable(column, 1);
			gtk_tree_view_column_set_fixed_width (column, 50);
			gtk_tree_view_column_set_clickable (column, TRUE);
		}
	}
	
	gtk_tree_view_set_model(store, GTK_TREE_MODEL(model));
	
	return store;
}

void ReceiverWindow::clear()
{
	GtkTreeIter iter;
	bool valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(model), &iter);
	while (valid)
	{
		//удаление элемента и увеличение итератора
		valid = gtk_tree_store_remove (GTK_TREE_STORE (model), &iter);
	}
}
