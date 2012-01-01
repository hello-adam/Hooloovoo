#ifndef GAMEOBJECTEDITWIDGET_H
#define GAMEOBJECTEDITWIDGET_H

#include <QWidget>
#include "gameobject.h"
#include "propertytreeview.h"

namespace Ui {
    class GameObjectEditWidget;
}

class GameObjectEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameObjectEditWidget(QWidget *parent = 0);
    ~GameObjectEditWidget();

private:
    Ui::GameObjectEditWidget *ui;
    PropertyTreeView* m_propertyTreeView;

    GameObject* m_gameObject;

    int m_selectedComponentRow;

    void setupComboBoxAndButtons();

public slots:
    void setGameObject(GameObject* gameObject);

private slots:
    void checkRemoveSelection(QModelIndex selection);
    void checkAddSelection(QString componentName);

    void removeSelectedComponent();
    void addSelectedComponent();
};

#endif // GAMEOBJECTEDITWIDGET_H
