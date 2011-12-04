#ifndef CAUSEEFFECTLABELWIDGET_H
#define CAUSEEFFECTLABELWIDGET_H

#include <QWidget>
#include "component.h"
#include "causeeffectmanager.h"

namespace Ui {
    class CauseEffectLabelWidget;
}

class CauseEffectLabelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CauseEffectLabelWidget(Component* component, QString name, QWidget *parent = 0);
    ~CauseEffectLabelWidget();

protected:
//    void paintEvent(QPaintEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
//    void leaveEvent(QEvent *event);

private:
    Ui::CauseEffectLabelWidget *ui;
    QPalette m_palette;

    int m_componentID;
    QString m_name;
    CauseEffectManager *m_manager;

    QList<CauseEffectManager::CauseEffectDefinition> m_defsAsCause;
    QList<CauseEffectManager::CauseEffectDefinition> m_defsAsEffect;

    int m_selectionID;
    QString m_selectionName;

    bool m_hover;
    bool m_selected;
    bool m_linkedToSelectedAsCause;
    bool m_linkedToSelectedAsEffect;

    void setNeutralAppearance();
    void setSelectedAppearance();
    void setLinkedAsCauseAppearance();
    void setLinkedAsEffectAppearance();

signals:
    void selected(int componentID, QString name);
    void deselected(int componentID, QString name);

public slots:
    void newSelection(int componentID, QString name);
    void deselection(int componentID, QString name);

    void buttonClicked();
};

#endif // CAUSEEFFECTLABELWIDGET_H
