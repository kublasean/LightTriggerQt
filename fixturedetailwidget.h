#ifndef FIXTUREDETAILWIDGET_H
#define FIXTUREDETAILWIDGET_H

#include "models/fixturedetails.h"

#include <QWidget>
#include <QLineEdit>


namespace Ui {
class FixtureDetailWidget;
}

class FixtureDetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FixtureDetailWidget(QWidget *parent = nullptr);
    ~FixtureDetailWidget();

public slots:
    void setDetails(const FixtureDetails &details);

private slots:
    void modeChanged(int modeIndex);

private:
    QLineEdit *createLineEdit();
    void setDetail(QLineEdit *field, const QString &value);

    Ui::FixtureDetailWidget *ui;

    QLineEdit *dimensionsLineEdit, *weightLineEdit, *powerLineEdit, *connectorLineEdit;

    FixtureDetails cachedDetails;
};

#endif // FIXTUREDETAILWIDGET_H
