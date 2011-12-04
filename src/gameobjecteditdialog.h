#ifndef GAMEOBJECTEDITDIALOG_H
#define GAMEOBJECTEDITDIALOG_H

#include <QDialog>
#include "gameobject.h"
#include "causeandeffecteditwidget.h"
class PropertyEditWidget;

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

    QWidget* getObjectEditWidget(QObject* object, QSet<QString> editProperties);

    CauseAndEffectEditWidget* m_causeAndEffectWidget;

private slots:
    void addSelectedComponent();
};

#endif // GAMEOBJECTEDITDIALOG_H
