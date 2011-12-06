#include "causeeffectlabelwidget.h"
#include "ui_causeeffectlabelwidget.h"
#include "gameobject.h"
#include <QPainter>

CauseEffectLabelWidget::CauseEffectLabelWidget(Component *component, QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CauseEffectLabelWidget)
{
    ui->setupUi(this);
    m_hover = false;
    m_selected = false;
    m_linkedToSelectedAsCause = false;
    m_linkedToSelectedAsEffect = false;

    m_manager = component->getParentObject()->getCauseEffectManager();
    m_componentID = component->getID();
    m_name = name;

    m_defsAsCause = m_manager->getDefinitionsWithLinkedEffects(m_componentID, m_name);
    m_defsAsEffect = m_manager->getDefinitionsWithLinkedCauses(m_componentID, m_name);

    m_selectionID = -1;
    m_selectionName = "";

    ui->tb_Name->setText(component->getTag() + "::" + m_name);

    this->setNeutralAppearance();
    connect(ui->tb_Name, SIGNAL(clicked()),
            this, SLOT(buttonClicked()));

//    m_palette.setBrush(QPalette::Highlight, QColor(255, 40, 40, 100));
//    m_palette.setBrush(QPalette::HighlightedText, QColor(0, 0, 0));
//    this->setPalette(m_palette);
}

CauseEffectLabelWidget::~CauseEffectLabelWidget()
{
    delete ui;
}

//void CauseEffectLabelWidget::paintEvent(QPaintEvent *event)
//{
//    Q_UNUSED(event)

//    if (m_hover)
//    {
//        QPainter painter(this);
//        painter.setBrush(QColor(255, 255, 255, 25));

//        if (m_linkedToSelectedAsCause || m_linkedToSelectedAsEffect || m_selected)
//            painter.setPen(Qt::red);
//        else
//            painter.setPen(Qt::green);

//        painter.drawRect(ui->tb_Name->rect().x() - 1 + ui->tb_Name->x(),
//                         ui->tb_Name->rect().y() - 1 + ui->tb_Name->y(),
//                         ui->tb_Name->rect().width() + 1,
//                         ui->tb_Name->rect().height() + 1);
//    }
//}

//void CauseEffectLabelWidget::mouseMoveEvent(QMouseEvent *event)
//{
//    QRect buttonRect = QRect(ui->tb_Name->rect().x() + ui->tb_Name->x(),
//                        ui->tb_Name->rect().y()+ ui->tb_Name->y(),
//                        ui->tb_Name->rect().width(),
//                        ui->tb_Name->rect().height());

//    qDebug() << event->pos();

//    if (buttonRect.contains(event->pos()))
//        m_hover = true;
//    else
//        m_hover = false;
//    this->update();
//}

//void CauseEffectLabelWidget::leaveEvent(QEvent *event)
//{
//    m_hover = false;
//    this->update();
//}

void CauseEffectLabelWidget::newSelection(int componentID, QString name)
{
    m_selectionID = componentID;
    m_selectionName = name;

    if (m_selectionID == m_componentID && m_selectionName == m_name)
    {
        return;
    }

    //setup new selection links
    m_defsAsCause = m_manager->getDefinitionsWithLinkedEffects(m_componentID, m_name);
    m_defsAsEffect = m_manager->getDefinitionsWithLinkedCauses(m_componentID, m_name);

    foreach(CauseEffectManager::CauseEffectDefinition def, m_defsAsCause)
    {
        if (def.effectID == m_selectionID && def.effectName == m_selectionName)
        {
            m_linkedToSelectedAsCause = true;
            setLinkedAsCauseAppearance();
            return;
        }
    }
    foreach(CauseEffectManager::CauseEffectDefinition def, m_defsAsEffect)
    {
        if (def.causeID == m_selectionID && def.causeName == m_selectionName)
        {
            m_linkedToSelectedAsEffect = true;
            setLinkedAsEffectAppearance();
            return;
        }
    }

    this->update();
}

void CauseEffectLabelWidget::deselection(int componentID, QString name)
{
    m_selectionID = -1;
    m_selectionName = "";

    //undo old selection links
    if (m_linkedToSelectedAsCause)
    {
        m_linkedToSelectedAsCause = false;
        setNeutralAppearance();
    }
    else if (m_linkedToSelectedAsEffect)
    {
        m_linkedToSelectedAsEffect = false;
        setNeutralAppearance();
    }

    this->update();
}

void CauseEffectLabelWidget::buttonClicked()
{
    if (m_selectionID == -1 && m_selectionName == "")
    {
        m_selected = true;
        setSelectedAppearance();
        emit selected(m_componentID, m_name);
    }
    else if (m_selected)
    {
        m_selected = false;
        setNeutralAppearance();
        emit deselected(m_componentID, m_name);
    }
    else if (m_linkedToSelectedAsCause)
    {
        m_manager->removeCauseEffectDefinition(m_componentID, m_name, m_selectionID, m_selectionName);

        m_linkedToSelectedAsCause = false;
        setNeutralAppearance();
    }
    else if (m_linkedToSelectedAsEffect)
    {
        m_manager->removeCauseEffectDefinition(m_selectionID, m_selectionName, m_componentID, m_name);

        m_linkedToSelectedAsEffect = false;
        setNeutralAppearance();
    }
    else
    {
        if (m_name.startsWith("effect") && m_selectionName.startsWith("effect"))
        {
            qDebug() << "Cannot connect an effect to an effect";
            return;
        }
        else if ((m_name.startsWith("cause") || m_name.startsWith("effect")) && m_selectionName.startsWith("cause"))
        {
            //the selection will cause this
            if (m_manager->addCauseEffectDefinition(m_selectionID, m_selectionName, m_componentID, m_name))
            {
                m_defsAsEffect = m_manager->getDefinitionsWithLinkedCauses(m_componentID, m_name);
                m_linkedToSelectedAsEffect = true;
                setLinkedAsEffectAppearance();
            }
        }
        else if (m_name.startsWith("cause") && m_selectionName.startsWith("effect"))
        {
            //this will cause the selected effect
            if (m_manager->addCauseEffectDefinition(m_componentID, m_name, m_selectionID, m_selectionName))
            {
                m_defsAsCause = m_manager->getDefinitionsWithLinkedEffects(m_componentID, m_name);
                m_linkedToSelectedAsCause = true;
                setLinkedAsCauseAppearance();
            }
        }
        else
        {
            setNeutralAppearance();
        }
    }


    this->update();
}

void CauseEffectLabelWidget::setNeutralAppearance()
{
    ui->tb_Name->setChecked(false);
    ui->tb_Name->setPalette(QPalette(QColor(180, 200, 255)));
}

void CauseEffectLabelWidget::setSelectedAppearance()
{
    ui->tb_Name->setChecked(true);
    ui->tb_Name->setPalette(QPalette(QColor(250, 100, 100)));
}

void CauseEffectLabelWidget::setLinkedAsCauseAppearance()
{
    ui->tb_Name->setChecked(true);
    ui->tb_Name->setPalette(QPalette(QColor(240, 240, 100)));
}

void CauseEffectLabelWidget::setLinkedAsEffectAppearance()
{
    ui->tb_Name->setChecked(true);
    ui->tb_Name->setPalette(QPalette(QColor(100, 250, 100)));
}
