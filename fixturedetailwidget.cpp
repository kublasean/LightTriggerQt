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

    ui->formLayout->insertRow(0,tr("Connector"), connectorLineEdit);
    ui->formLayout->insertRow(0, tr("Power"), powerLineEdit);
    ui->formLayout->insertRow(0, tr("Weight"), weightLineEdit);
    ui->formLayout->insertRow(0, tr("Dimensions"), dimensionsLineEdit);

    setDisabled(true);

    ui->groupBox->setTitle(tr("Fixture"));

    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FixtureDetailWidget::modeChanged);
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
    if (!details.isValid()) {
        return;
    }

    setEnabled(true);

    cachedDetails = details;

    ui->groupBox->setTitle(details.name);

    setDetail(dimensionsLineEdit, details.dimensions);
    setDetail(weightLineEdit, details.weight);
    setDetail(powerLineEdit, details.power);
    setDetail(connectorLineEdit, details.connector);


    ui->comboBox->clear();
    ui->listWidget->clear();

    for (auto it = details.modes.constBegin(); it != details.modes.constEnd(); it++) {
        ui->comboBox->addItem(it->name);
    }
}

void FixtureDetailWidget::modeChanged(int modeIndex)
{
    ui->listWidget->clear();

    if (modeIndex < 0 || modeIndex >= cachedDetails.modes.size()) {
        return;
    }

    for (int i=0; i < cachedDetails.modes[modeIndex].channels.size(); i++) {
        ui->listWidget->addItem(QString::number(i) + ": " + cachedDetails.modes[modeIndex].channels[i].name);
    }
}
