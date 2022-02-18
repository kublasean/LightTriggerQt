#ifndef FIXTUREDETAILWIDGET_H
#define FIXTUREDETAILWIDGET_H

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
    void setDetails(const QString &dim,
                    const QString &weight,
                    const QString &power,
                    const QString &conn);

private:
    QLineEdit *createLineEdit();
    void setDetail(QLineEdit *field, const QString &value);

    Ui::FixtureDetailWidget *ui;

    QLineEdit *dimensionsLineEdit, *weightLineEdit, *powerLineEdit, *connectorLineEdit;
};

#endif // FIXTUREDETAILWIDGET_H
