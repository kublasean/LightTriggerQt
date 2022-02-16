#ifndef FIXTUREITEMDELEGATE_H
#define FIXTUREITEMDELEGATE_H

#include <QStyledItemDelegate>

class FixtureItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit FixtureItemDelegate(QObject *parent = nullptr);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // FIXTUREITEMDELEGATE_H
