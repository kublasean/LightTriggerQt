#include "fixtureitemdelegate.h"
#include <QPainter>
#include <QDebug>
#include <QFileSystemModel>
#include <QApplication>

FixtureItemDelegate::FixtureItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void FixtureItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{

    const QFileSystemModel *model = qobject_cast<const QFileSystemModel *>(index.model());

    // Don't change how directories are painted
    if (model->isDir(index)) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    opt.text = model->fileInfo(index).baseName();               // No extension
    opt.features &= ~(QStyleOptionViewItem::HasDecoration);     // No Icon

    const QWidget *widget = option.widget;
    widget->style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
}
