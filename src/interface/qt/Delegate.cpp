#include <QCheckBox>

#include "Delegate.h"

CheckDelegate::CheckDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *CheckDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QCheckBox *editor = new QCheckBox(parent);

    return editor;
}

void CheckDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    checkBox->setChecked(value);
}

void CheckDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    int value = checkBox->isChecked();

    model->setData(index, value, Qt::EditRole);
}

void CheckDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
