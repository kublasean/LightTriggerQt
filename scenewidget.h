#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QWidget>

namespace Ui {
class SceneWidget;
}

class SceneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SceneWidget(QWidget *parent = nullptr);
    ~SceneWidget();

private:
    Ui::SceneWidget *ui;
};

#endif // SCENEWIDGET_H
