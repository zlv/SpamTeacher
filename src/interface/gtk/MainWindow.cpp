#include "MainWindow.h"
//функции создания виджетов
#include "../func.h"
//для data и для сигналов
#include "gui.h"
#include "Data.h"
//используемый здесь указатель на функцию без аргументов
typedef void (*PFUNC)();
//описание меню
/* Наше меню, массив GtkItemFactoryEntry структур которые определяют каждый пункт меню */
static GtkItemFactoryEntry menu_items[] = {
  { (gchar*)"/_Файл", NULL, NULL, 0, (gchar*)"<Branch>" },
  { (gchar*)"/Файл/О_чистить", (gchar*)"<control>N", (PFUNC)clear_signal, 0, (gchar*)"<Item>" },
  { (gchar*)"/Файл/Настройки _отправителя", (gchar*)"<control>S", (PFUNC)sender_signal, 0, (gchar*)"<Item>" },
  { (gchar*)"/Файл/Настройки _получателя", (gchar*)"<control>R", (PFUNC)receiver_signal, 0, (gchar*)"<Item>" },
  { (gchar*)"/Файл/Настройки _вложений", (gchar*)"<control>E", (PFUNC)atach_signal, 0, (gchar*)"<Item>" },
  { (gchar*)"/Файл/Отправить _сообщение", NULL, (PFUNC)message_signal, 0, (gchar*)"<Item>" },
  { (gchar*)"/Файл/Sep", NULL, NULL, 0, (gchar*)"<Separator>" },
  { (gchar*)"/Файл/В_ыход", (gchar*)"<control>Q", (PFUNC)closeApp, 0, (gchar*)"<Item>" },
  { (gchar*)"/_Справка", NULL, NULL, 0, (gchar*)"<Branch>" },
  //{ (gchar*)"/Справка/_Помощь", (gchar*)"<F1>", NULL, 0, (gchar*)"<Item>" },
  //{ (gchar*)"/Справка/Sep", NULL, NULL, 0, (gchar*)"<Separator>" },
  { (gchar*)"/Справка/_О программе", NULL, (PFUNC)about, 0, (gchar*)"<Item>" }
};
static gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);
//////////////////////////////////
MainWindow::MainWindow()
{
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//SpamTeacher -- рабочее название
	gtk_window_set_title(GTK_WINDOW(window), "SpamTeacher");
	
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_signal_connect(GTK_OBJECT(window), "destroy",
		GTK_SIGNAL_FUNC(closeApp), NULL);

	/////////////////////////////////////
	//установка текста в поля ввода из data
	std::string text;
	text = data->get_message_about_sender();
	senderEdit = new_edit(text.c_str(),NULL);
	text = data->get_message_about_receiver();
	receiverEdit = new_edit(text.c_str(),NULL);
	text = data->get_message_about_atachment();
	atachEdit = new_edit(text.c_str(),NULL);
	/////////////////////////////////////
	gtk_editable_set_editable(GTK_EDITABLE(senderEdit),0);
	gtk_editable_set_editable(GTK_EDITABLE(receiverEdit),0);
	gtk_editable_set_editable(GTK_EDITABLE(atachEdit),0);
	/////////////////////////////////////
	//установка текста в поле для ввода сообщения
	GtkWidget *view;
	GtkTextBuffer *buffer;

	view = gtk_text_view_new ();

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	text = data->message;
	gtk_text_buffer_set_text (buffer, text.c_str(), -1);
	GtkWidget* frame = view;
	frame = gtk_frame_new (NULL);
	gtk_frame_set_label (GTK_FRAME (frame), "Сообщение");
	/////////////////////////////////////////
	GtkWidget* sw = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER(sw), view);
	gtk_container_add(GTK_CONTAINER(frame), sw);
	////////////////////////////////////////
	messageEdit = frame;
	////////////////////////////////////////
	//создание кнопок и подцепление к ним сигналов
	senderButton = new_button("Опции отправителя",sender_signal);
	receiverButton = new_button("Опции получателей",receiver_signal);
	atachButton = new_button("Опции вложений(файлов)",atach_signal);
	messageButton = new_button("Отправить сообщение",message_signal);
	////////////////////////////////////////
	//создание кнопок для настроек
	GtkWidget* settingsButton[setNum];
	settingsButton[0] = new_button("Имя",settings_signal[0]);
	settingsButton[1] = new_button("Пол",settings_signal[1]);
	GtkWidget* messageSettings = gtk_hbox_new(0,1);
	for (int i=0; i<setNum; i++)
	{
		gtk_box_pack_start(GTK_BOX(messageSettings),settingsButton[i],0,0,5);
	}
	GtkWidget* label;
	label = new_label("Тема сообщения:",NULL);
	themeEdit = new_edit(data->theme.c_str(),NULL);
	gtk_box_pack_end(GTK_BOX(messageSettings),themeEdit,0,0,5);
	gtk_box_pack_end(GTK_BOX(messageSettings),label,0,0,5);
	/////////////////////////////////////
	//упаковка всего
	box[0] = gtk_hbox_new(1,5);
	box[1] = gtk_hbox_new(1,5);
	box[2] = gtk_hbox_new(1,5);
	box[3] = gtk_vbox_new(0,3);
	/////////////////////////////////////
	gtk_box_pack_start(GTK_BOX(box[0]),senderEdit,1,0,5);
	gtk_box_pack_start(GTK_BOX(box[0]),senderButton,1,0,5);
	gtk_box_pack_start(GTK_BOX(box[1]),receiverEdit,1,0,5);
	gtk_box_pack_start(GTK_BOX(box[1]),receiverButton,1,0,5);
	gtk_box_pack_start(GTK_BOX(box[2]),atachEdit,1,0,5);
	gtk_box_pack_start(GTK_BOX(box[2]),atachButton,1,0,5);
	gtk_box_pack_start(GTK_BOX(box[3]),messageSettings,0,0,1);
	gtk_box_pack_start(GTK_BOX(box[3]),messageEdit,1,1,1);
	gtk_box_pack_start(GTK_BOX(box[3]),messageButton,0,0,1);
	//для удобства обращения к виджету
	messageEdit = view;
	/////////////////////////////////////
	vbox = gtk_vbox_new(0,5);
	/////////////////////////////////////
	GtkWidget* menubar;
	
	menubar = get_menubar_menu(window);
	gtk_box_pack_start(GTK_BOX(vbox), menubar, 0, 1, 0);
	
	for (int i=0; i<3; i++)
	{
		gtk_box_pack_start(GTK_BOX(vbox),box[i],0,0,5);
	}
	gtk_box_pack_start(GTK_BOX(vbox),box[3],1,1,5);

	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	gtk_widget_show_all(window);
}
/* ugly foreigner source from sample */
GtkWidget* MainWindow::get_menubar_menu(GtkWidget *window)
{
  GtkItemFactory *item_factory;
  GtkAccelGroup *accel_group;
  /* Создаём группу акселераторов (shortcut keys) */
  accel_group = gtk_accel_group_new ();
  /* Создаём ItemFactory (строку меню) */
  item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", accel_group);
  /* Эта функция генерирует пункты меню. Передает item factory,
     номер пункта в массиве, массив непосредственно, а также любые
     данные обратных вызовов для пунктов меню. */
  gtk_item_factory_create_items (item_factory, nmenu_items, menu_items, NULL);
  /* Прикрепляем новую группу акселераторов к окну. */
  gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);
  /* Возвращаем фактически созданную производством пунктов (item factory) панель меню. */
  return gtk_item_factory_get_widget (item_factory, "<main>");
}

void MainWindow::clear()
{
	gtk_entry_set_text(GTK_ENTRY(senderEdit),"");
	gtk_entry_set_text(GTK_ENTRY(receiverEdit),"");
	gtk_entry_set_text(GTK_ENTRY(atachEdit),"");
	//////////////////////////////////
	gtk_entry_set_text(GTK_ENTRY(themeEdit),"");
	
	GtkTextBuffer *buffer;
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (messageEdit));

	gtk_text_buffer_set_text (buffer, "", -1);
}
