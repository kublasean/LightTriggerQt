#include "fixturedetailwidget.h"
#include "ui_fixturedetailwidget.h"
#include "models/fixturemimedata.h"

#include <QDrag>
#include <QPoint>
#include <QMouseEvent>
#include <QDebug>
#include <QWindow>

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

void FixtureDetailWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isEnabled()) {
        dragPos = event->pos();
    } else {
        dragPos = QPoint();
    }

    QWidget::mousePressEvent(event);
}

void FixtureDetailWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (dragPos.isNull() || !(event->buttons() & Qt::LeftButton))
        return;

    if ((event->pos() - dragPos).manhattanLength() < QApplication::startDragDistance())
        return;

    qDebug() << "EVENT POS" << event->pos() << "WIDGET POS" << pos();

    ActiveFixture fixture;
    fixture.mode = cachedDetails.modes[ui->comboBox->currentIndex()];
    fixture.detailsAbsolutePath = cachedDetails.absolutePath;
    fixture.id = 1;
    fixture.name = cachedDetails.name;

    FixtureMimeData *data = new FixtureMimeData(fixture);

    //qreal dpr = window()->windowHandle()->devicePixelRatio();
    //QPixmap pixmap(size() * dpr);
    //pixmap.setDevicePixelRatio(dpr);
    //render(&pixmap);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(data);
    //drag->setPixmap(pixmap);
    //drag->setHotSpot(data->hotspot);

    drag->exec(Qt::CopyAction);
    dragPos = QPoint();
}
