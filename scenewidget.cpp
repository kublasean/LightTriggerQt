#include "scenewidget.h"
#include "ui_scenewidget.h"

SceneWidget::SceneWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SceneWidget)
{
    ui->setupUi(this);
}

SceneWidget::~SceneWidget()
{
    delete ui;
}
