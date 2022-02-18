#ifndef FIXTUREWIDGET_H
#define FIXTUREWIDGET_H

#include "fixturedetailwidget.h"

#include <QWidget>

namespace Ui {
class FixtureWidget;
}

class FixtureWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FixtureWidget(QWidget *parent = nullptr);
    ~FixtureWidget();

private slots:
    void currentPathChanged(const QModelIndex &newPath, const QModelIndex &oldPath);
    void showFixtureErrorMessage(const QString &msg);

private:
    Ui::FixtureWidget *ui;
    FixtureDetailWidget *detailWidget;
};

#endif // FIXTUREWIDGET_H
