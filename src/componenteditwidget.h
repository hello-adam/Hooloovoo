#ifndef COMPONENTEDITWIDGET_H
#define COMPONENTEDITWIDGET_H

#include <QWidget>
#include <QToolButton>
#include "component.h"
#include "propertyeditwidget.h"

namespace Ui {
    class ComponentEditWidget;
}

class ComponentEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ComponentEditWidget(QWidget *parent = 0);
    ~ComponentEditWidget();

    bool setComponent(Component* component);

private:
    Ui::ComponentEditWidget *ui;
    Component* m_component;
    QList<PropertyEditWidget*> m_propertyEditWidgets;

signals:
    void componentRemoved();

public slots:
    void removeComponent();
    void saveChanges();
};

#endif // COMPONENTEDITWIDGET_H
