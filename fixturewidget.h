#ifndef FIXTUREWIDGET_H
#define FIXTUREWIDGET_H

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

private:
    Ui::FixtureWidget *ui;
};

#endif // FIXTUREWIDGET_H
