#ifndef FIXTUREWIDGET_H
#define FIXTUREWIDGET_H

#include "fixturedetailwidget.h"
#include "fixturedetailscache.h"

#include <QWidget>

namespace Ui {
class FixtureWidget;
}

class FixtureWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FixtureWidget(FixtureDetailsCache *cache, QWidget *parent = nullptr);
    ~FixtureWidget();

private slots:
    void currentPathChanged(const QModelIndex &newPath, const QModelIndex &oldPath);
    void showFixtureErrorMessage(const QString &msg);

private:
    Ui::FixtureWidget *ui;
    FixtureDetailWidget *detailWidget;
    FixtureDetailsCache *fixtureCache;
    FixtureDetailsReceiver detailsReceiver;
};

#endif // FIXTUREWIDGET_H
