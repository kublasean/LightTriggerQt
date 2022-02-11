#include "navtoolbar.h"
#include <QActionGroup>
#include <QDebug>

NavToolBar::NavToolBar(QWidget *parent)
    : QToolBar(parent)
{
    // Toolbar styling
    setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextUnderIcon);

    QActionGroup *navGroup = new QActionGroup(this);

    QAction *projectMode = new QAction(QIcon(":/assets/nav-project/baseline_app_registration_black_24dp.png"), tr("Project"), navGroup);
    projectMode->setData(AppMode::Project);
    projectMode->setCheckable(true);
    projectMode->setChecked(true);

    QAction *scenesMode = new QAction(QIcon(":/assets/nav-scenes/baseline_batch_prediction_black_24dp.png"), tr("Scenes"), navGroup);
    scenesMode->setCheckable(true);
    scenesMode->setData(AppMode::Scenes);

    QAction *triggersMode = new QAction(QIcon(":/assets/nav-triggers/baseline_piano_black_24dp.png"), tr("Triggers"), navGroup);
    triggersMode->setCheckable(true);
    triggersMode->setData(AppMode::Triggers);

    connect(navGroup, &QActionGroup::triggered, this, &NavToolBar::navSelection);
    addActions(navGroup->actions());
}

void NavToolBar::navSelection(QAction *action)
{
    AppMode mode = qvariant_cast<AppMode>(action->data());
}
