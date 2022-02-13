#include "fixturewidget.h"
#include "ui_fixturewidget.h"

FixtureWidget::FixtureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FixtureWidget)
{
    ui->setupUi(this);
}

FixtureWidget::~FixtureWidget()
{
    delete ui;
}
