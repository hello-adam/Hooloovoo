#ifndef GAMEOBJECTFILEWIDGET_H
#define GAMEOBJECTFILEWIDGET_H

#include <QWidget>
#include "gamefilemodel.h"

namespace Ui {
    class GameObjectFileWidget;
}

class GameObjectFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameObjectFileWidget(QWidget *parent = 0);
    ~GameObjectFileWidget();

private:
    Ui::GameObjectFileWidget *ui;
    GameFileModel *m_model;

private slots:
};

#endif // GAMEOBJECTFILEWIDGET_H
