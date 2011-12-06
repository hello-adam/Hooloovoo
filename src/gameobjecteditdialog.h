#ifndef GAMEOBJECTEDITDIALOG_H
#define GAMEOBJECTEDITDIALOG_H

#include <QDialog>
#include "gameobject.h"
#include "causeandeffecteditwidget.h"
class PropertyEditWidget;
class ComponentEditWidget;

namespace Ui {
    class GameObjectEditDialog;
}

class GameObjectEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameObjectEditDialog(QWidget *parent = 0);
    ~GameObjectEditDialog();

    bool editObject(GameObject* object);

private:
    Ui::GameObjectEditDialog *ui;
    GameObject* m_object;
    QHash<QString, QWidget*> m_componentWidgetLookup;

    QSet<ComponentEditWidget*> m_componentEditWidgets;

    CauseAndEffectEditWidget* m_causeAndEffectWidget;

private slots:
    void addSelectedComponent();
    void addComponentEditWidget(Component* component);
    void removeComponentEditWidget(ComponentEditWidget* editWidget);
    void saveChanges();
    void resetCauseEffectWidget();
};

#endif // GAMEOBJECTEDITDIALOG_H
