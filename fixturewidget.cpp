#include "fixturewidget.h"
#include "ui_fixturewidget.h"
#include "fixtureitemdelegate.h"

#include <QFileSystemModel>
#include <QCoreApplication>
#include <QDir>

QString safeJoinPath(QString path1, QString path2)
{
    return QDir::cleanPath(path1 + QDir::separator() + path2);
}

FixtureWidget::FixtureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FixtureWidget)
{
    ui->setupUi(this);

    QFileSystemModel *model = new QFileSystemModel(this);

    QString path = safeJoinPath(QCoreApplication::applicationDirPath(), "fixture-profiles");
    model->setRootPath(path);

    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(path));

    for (int i=1; i<model->columnCount(); i++) {
        ui->treeView->hideColumn(i);
    }

    auto index = model->index(safeJoinPath(model->rootPath(), "manufacturers.json"));
    ui->treeView->setRowHidden(index.row(), index.parent(), true);

    ui->treeView->setItemDelegate(new FixtureItemDelegate(this));
}

FixtureWidget::~FixtureWidget()
{
    delete ui;
}
