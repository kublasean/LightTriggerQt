#ifndef NAVTOOLBAR_H
#define NAVTOOLBAR_H

#include <QToolBar>

class NavToolBar : public QToolBar
{
    Q_OBJECT
public:
    enum AppMode { Project, Scenes, Triggers };

    explicit NavToolBar(QWidget *parent = nullptr);

private slots:
    void navSelection(QAction *selection);

};

Q_DECLARE_METATYPE(NavToolBar::AppMode);



#endif // NAVTOOLBAR_H
