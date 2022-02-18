#include "fixturewidget.h"
#include "ui_fixturewidget.h"
#include "fixtureitemdelegate.h"
#include "openfixturefilereaderthread.h"

#include <QFileSystemModel>
#include <QCoreApplication>
#include <QDir>
#include <QItemSelectionModel>
#include <QMessageBox>

QString safeJoinPath(QString path1, QString path2)
{
    return QDir::cleanPath(path1 + QDir::separator() + path2);
}

FixtureWidget::FixtureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FixtureWidget)
{
    ui->setupUi(this);

    // Fixture browser
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

    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &FixtureWidget::currentPathChanged);

    // Fixture detail
    detailWidget = new FixtureDetailWidget();
    ui->verticalLayout->addWidget(detailWidget);
}

FixtureWidget::~FixtureWidget()
{
    delete ui;
}

void FixtureWidget::currentPathChanged(const QModelIndex &newPath, const QModelIndex &)
{
    const QFileSystemModel *model = qobject_cast<const QFileSystemModel *>(newPath.model());
    QString path = model->filePath(newPath);

    if (model->fileInfo(newPath).isDir())
        return;

    OpenFixtureFileReaderThread *parserThread = new OpenFixtureFileReaderThread(model->filePath(newPath), this);
    connect(parserThread, &QThread::finished, parserThread, &QObject::deleteLater);
    connect(parserThread, &OpenFixtureFileReaderThread::parsedDetails, detailWidget, &FixtureDetailWidget::setDetails);
    connect(parserThread, &OpenFixtureFileReaderThread::errorMessage, this, &FixtureWidget::showFixtureErrorMessage);
    parserThread->start();
}

void FixtureWidget::showFixtureErrorMessage(const QString &msg)
{
    QMessageBox::warning(this, tr("Fixture Error"), msg);
}

