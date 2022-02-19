#include "fixturedetailwidget.h"
#include "ui_fixturedetailwidget.h"

FixtureDetailWidget::FixtureDetailWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FixtureDetailWidget)
{
    ui->setupUi(this);

    dimensionsLineEdit = createLineEdit();
    weightLineEdit = createLineEdit();
    powerLineEdit = createLineEdit();
    connectorLineEdit = createLineEdit();

    ui->formLayout->addRow(tr("Dimensions"), dimensionsLineEdit);
    ui->formLayout->addRow(tr("Weight"), weightLineEdit);
    ui->formLayout->addRow(tr("Power"), powerLineEdit);
    ui->formLayout->addRow(tr("Connector"), connectorLineEdit);

    setDisabled(true);

    ui->groupBox->setTitle(tr("Fixture"));
}

FixtureDetailWidget::~FixtureDetailWidget()
{
    delete ui;
}

QLineEdit *FixtureDetailWidget::createLineEdit()
{
    QLineEdit *field = new QLineEdit(this);
    field->setReadOnly(true);
    return field;
}

void FixtureDetailWidget::setDetail(QLineEdit *field, const QString &value)
{
    field->setText(value);
}

void FixtureDetailWidget::setDetails(const FixtureDetails &details)
{
    ui->groupBox->setTitle(details.name);

    setDetail(dimensionsLineEdit, details.dimensions);
    setDetail(weightLineEdit, details.weight);
    setDetail(powerLineEdit, details.power);
    setDetail(connectorLineEdit, details.connector);

    setEnabled(true);

    ui->listWidget->clear();
    for (auto it = details.availableChannels.constBegin(); it != details.availableChannels.constEnd(); it++) {
        ui->listWidget->addItem(it.value().name);
    }
}
