#ifndef GAMEOBJECTEDITDIALOG_H
#define GAMEOBJECTEDITDIALOG_H

#include <QDialog>
#include "gameobject.h"
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
    QList<PropertyEditWidget*> m_editWidgets;
    GameObject* m_object;

    QWidget* getObjectEditWidget(QObject* object, QSet<QString> editProperties);

private slots:
    void addComponent();
    void removeComponent();
};

#endif // GAMEOBJECTEDITDIALOG_H
