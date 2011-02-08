#include "AtachmentWindow.h"
#include "../func.h"
#include "gui.h"
#include "../../Data.h"
//содержание
enum
{
    COL_NAME,
    NUM_COLS
};
//строки
const char titles[][30+1] = {"Название"};
//содержание
const int collist[NUM_COLS] = {COL_NAME};
AtachmentWindow::AtachmentWindow(GtkWidget* w)
{
    mainWindow = w;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Выбор файлов вложений");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    /////////////////////////////////
    box[0] = gtk_hbox_new(0,5);
    box[1] = gtk_hbox_new(0,15);
    /////////////////////////////////
    model = create_and_fill_model();
    store = GTK_WIDGET(create_and_fill_store());
    /////////////////////////////////
    addButton = new_button("Добавить",atach_add_signal,model);
    delButton = new_button("Удалить",del_signal,store);
    /////////////////////////////////
    okButton = new_button("Готово",atach_ok_signal,model);
    cancelButton = new_button("Отмена",atach_cancel_signal,model);
    gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(atach_destroy_signal), NULL);
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

AtachmentWindow::~AtachmentWindow()
{
}

//создание модели списка
GtkTreeModel* AtachmentWindow::create_and_fill_model()
{
    GtkTreeStore *treestore;
    GtkTreeIter toplevel;

    //должны быть 1 столбец
    treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, NULL);

    //добавление в модель каждого элемента
    for (int i=0; i<data->fileatach.size(); i++)
    {
        std::string* str = &data->fileatach[i];
        gtk_tree_store_append(treestore, &toplevel, NULL);
        gtk_tree_store_set(treestore, &toplevel,
            COL_NAME, str->c_str(),
            -1);
    }

    return GTK_TREE_MODEL(treestore);
}

GtkTreeView* AtachmentWindow::create_and_fill_store()
{
    GtkTreeView* store = GTK_TREE_VIEW(gtk_tree_view_new());
    
    int i=0;
    GtkTreeViewColumn* column;
    GtkCellRenderer *renderer;
    for (;i<sizeof(titles)/sizeof(titles[0]); i++)
    {
        //if (i<2)
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
    }
    
    gtk_tree_view_set_model(store, GTK_TREE_MODEL(model));
    
    return store;
}

void AtachmentWindow::clear()
{
    GtkTreeIter iter;
    bool valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(model), &iter);
    while (valid)
    {
        //удаление элемента и увеличение итератора
        valid = gtk_tree_store_remove (GTK_TREE_STORE (model), &iter);
    }
}

//сигналы для вложений
//добавить получателя
void atach_add_signal(GtkWidget* b, gpointer data)
{    
    GtkWidget * file_chooser_dialog;
    file_chooser_dialog = gtk_file_chooser_dialog_new("Выбор файла вложения", NULL, GTK_FILE_CHOOSER_ACTION_OPEN,  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
    if (gtk_dialog_run(GTK_DIALOG (file_chooser_dialog)) == GTK_RESPONSE_ACCEPT)
    {
        GtkWidget * toplevel;
        char *filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_dialog));
        GtkTreeIter iter;
        GtkTreeModel *model = (GtkTreeModel *)atachWindow->model;
        gtk_tree_store_append (GTK_TREE_STORE (model), &iter, NULL);
        gtk_tree_store_set (GTK_TREE_STORE (model), &iter, collist[0], filename, -1);
        
        g_free (filename);
    }
    gtk_widget_destroy (file_chooser_dialog);
}

void atach_ok_signal(GtkWidget* b, gpointer m)
{
    Data* data = ((MainWindow*)mainWindow)->data;
    //очистка
    data->fileatach.clear();
    
    //итератор для списка
    GtkTreeIter toplevel;
    //информация
    gchar *name;
    
    //получение итератора первого элемента списка
    bool valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(m), &toplevel);
    
    int count=0;
    
    //пока есть элементы
    while (valid)
    {
        //получение информации
        gtk_tree_model_get(GTK_TREE_MODEL(m), &toplevel,
                           COL_NAME, &name,
                           -1);
                           //увеличение итератора
                           valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(m), &toplevel);
                           //запись информации в recvEmail
                           data->fileatach.push_back(name);
                           
                           count++;
    }
    gtk_widget_destroy(atachWindow->window);
    
    std::string text;
    
    //вставка информации одной строкой в виджет главного окна
    text = data->get_message_about_atachment();
    gtk_entry_set_text(GTK_ENTRY(((MainWindow*)mainWindow)->atachEdit),text.c_str());
}
void atach_cancel_signal(GtkWidget* b, gpointer data)
{
    gtk_widget_destroy(atachWindow->window);
}

void atach_destroy_signal(GtkWidget* b, gpointer data)
{
    delete atachWindow;
    atachWindow = NULL;
}
