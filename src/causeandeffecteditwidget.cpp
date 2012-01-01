#include "causeandeffecteditwidget.h"
#include "ui_causeandeffecteditwidget.h"

CauseAndEffectEditWidget::CauseAndEffectEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CauseAndEffectEditWidget)
{
    ui->setupUi(this);
}

CauseAndEffectEditWidget::~CauseAndEffectEditWidget()
{
    delete ui;

    qDeleteAll(m_causeWidgets);
    m_causeWidgets.clear();

    qDeleteAll(m_effectWidgets);
    m_effectWidgets.clear();
}

void CauseAndEffectEditWidget::addCause(Component *component, QString cause)
{
    m_causeWidgets.push_back(new CauseEffectLabelWidget(component, cause));
    ui->causeLayout->addWidget(m_causeWidgets.back());

    connect(m_causeWidgets.back(), SIGNAL(selected(int,QString)),
            this, SIGNAL(selection(int,QString)));
    connect(m_causeWidgets.back(), SIGNAL(deselected(int,QString)),
            this, SIGNAL(deselection(int,QString)));
    connect(this, SIGNAL(selection(int,QString)),
            m_causeWidgets.back(), SLOT(newSelection(int,QString)));
    connect(this, SIGNAL(deselection(int,QString)),
            m_causeWidgets.back(), SLOT(deselection(int,QString)));
}

void CauseAndEffectEditWidget::addEffect(Component *component, QString effect)
{
    m_effectWidgets.push_back(new CauseEffectLabelWidget(component, effect));
    ui->effectLayout->addWidget(m_effectWidgets.back());

    connect(m_effectWidgets.back(), SIGNAL(selected(int,QString)),
            this, SIGNAL(selection(int,QString)));
    connect(m_effectWidgets.back(), SIGNAL(deselected(int,QString)),
            this, SIGNAL(deselection(int,QString)));
    connect(this, SIGNAL(selection(int,QString)),
            m_effectWidgets.back(), SLOT(newSelection(int,QString)));
    connect(this, SIGNAL(deselection(int,QString)),
            m_effectWidgets.back(), SLOT(deselection(int,QString)));
}

void CauseAndEffectEditWidget::setGameObject(GameObject *gameObject)
{
    m_gameObject = gameObject;

    if (m_gameObject)
    {
        connect(m_gameObject, SIGNAL(componentAdded(Component*)),
                this, SLOT(refresh()));
        connect(m_gameObject, SIGNAL(componentRemoved(Component*)),
                this, SLOT(refresh()));
        connect(m_gameObject, SIGNAL(causeInitiated()),
                this, SLOT(refresh()));
        connect(m_gameObject, SIGNAL(destroyed()),
                this, SLOT(clearObjectReference()));
    }

    refresh();
}

void CauseAndEffectEditWidget::clearObjectReference()
{
    m_gameObject = 0;
    refresh();
}

void CauseAndEffectEditWidget::refresh()
{
    qDeleteAll(m_causeWidgets);
    qDeleteAll(m_effectWidgets);
    m_causeWidgets.clear();
    m_effectWidgets.clear();

    if (!m_gameObject)
        return;

    QList<Component*> components = m_gameObject->getComponents();
    foreach (Component* component, components)
    {
        //add cause and effect data
        QStringList causes = component->getCauseList();
        foreach(QString cause, causes)
        {
            addCause(component, cause);
        }

        QStringList effects = component->getEffectList();
        foreach(QString effect, effects)
        {
            addEffect(component, effect);
        }
    }
}
