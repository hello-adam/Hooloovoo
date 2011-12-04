#ifndef CAUSEANDEFFECTEDITWIDGET_H
#define CAUSEANDEFFECTEDITWIDGET_H

#include <QWidget>
#include "causeeffectlabelwidget.h"

namespace Ui {
    class CauseAndEffectEditWidget;
}

class CauseAndEffectEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CauseAndEffectEditWidget(QWidget *parent = 0);
    ~CauseAndEffectEditWidget();

    void addCause(Component* component, QString cause);
    void addEffect(Component* component, QString effect);

private:
    Ui::CauseAndEffectEditWidget *ui;
    QList<CauseEffectLabelWidget*> m_causeWidgets;
    QList<CauseEffectLabelWidget*> m_effectWidgets;

signals:
    void selection(int ID, QString name);
    void deselection(int ID, QString name);

public slots:

};

#endif // CAUSEANDEFFECTEDITWIDGET_H
