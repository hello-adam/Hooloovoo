#include "propertyeditwidget.h"
#include "ui_propertyeditwidget.h"
#include "gamefiledialog.h"
#include <QDebug>
#include <QListWidget>
#include <QPalette>
#include <QColorDialog>
#include "editwidgets/pointedit.h"
#include "editwidgets/triggerlineedit.h"
#include "component.h"

PropertyEditWidget::PropertyEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PropertyEditWidget)
{
    ui->setupUi(this);

    m_object = 0;
    m_editWidget = 0;
    m_toolButton = new QToolButton();
    m_toolButton->setText("...");

    m_removeButton = new QToolButton();
    m_removeButton->setText("Remove");
    connect(m_removeButton, SIGNAL(clicked()), this, SLOT(removeItemFromList()));
}

PropertyEditWidget::~PropertyEditWidget()
{
    if (m_editWidget)
        delete m_editWidget;
    if (m_toolButton)
        delete m_toolButton;
    delete ui;
}

bool PropertyEditWidget::setProperty(QMetaProperty property, QObject* object)
{
    if (m_editWidget)
        delete m_editWidget;

    m_fileExtensions.clear();
    m_fileDirectory.clear();

    m_property = property;
    m_object = object;

    QVariant value = property.read(object);

    if (property.isEnumType())
    {
        QComboBox *comboBox = new QComboBox(this);
        m_editWidget = comboBox;
        QMetaEnum enumerator = property.enumerator();
        for (int i=0; i<enumerator.keyCount(); i++)
        {
            comboBox->addItem(enumerator.key(i));
        }
        int index = comboBox->findText(enumerator.valueToKey(*reinterpret_cast<const int *>(value.constData())));
        if (index == -1)
        {
            qDebug() << "Error reading enum";
        }
        comboBox->setCurrentIndex(index);
        ui->horizontalLayout->addWidget(comboBox);
        return true;
    }
    else if (value.type() == QVariant::String)
    {
        if (QString(property.name()).endsWith("trigger", Qt::CaseInsensitive))
        {
            TriggerLineEdit *triggerEdit;
            triggerEdit = new TriggerLineEdit();
            triggerEdit->setTriggerText(value.toString());
            m_editWidget = triggerEdit;
            ui->horizontalLayout->addWidget(triggerEdit);

            return true;
        }
        else if ((QString(property.name()).startsWith("triggerTo", Qt::CaseInsensitive)))
        {
            TriggerLineEdit *triggerEdit;
            triggerEdit = new TriggerLineEdit();
            triggerEdit->setTriggerText(value.toString());
            triggerEdit->setAdvancedEditButton(false);
            m_editWidget = triggerEdit;
            ui->horizontalLayout->addWidget(triggerEdit);

            return true;
        }
        else
        {
            QLineEdit *lineEdit = new QLineEdit(value.toString(), this);
            m_editWidget = lineEdit;
            ui->horizontalLayout->addWidget(lineEdit);
            if (QString(property.name()).contains("pixmap", Qt::CaseInsensitive))
            {
                m_fileExtensions << "*.png" << "*.bmp";
                m_fileDirectory = "pics";
                ui->horizontalLayout->addWidget(m_toolButton);

                connect(m_toolButton, SIGNAL(clicked()), this, SLOT(getStringFromFile()));
            }
            else if (QString(property.name()).contains("object", Qt::CaseInsensitive))
            {
                m_fileExtensions << "*.gameobject";
                m_fileDirectory = "objects";
                ui->horizontalLayout->addWidget(m_toolButton);

                connect(m_toolButton, SIGNAL(clicked()), this, SLOT(getStringFromFile()));
            }
            return true;
        }
    }
    else if (value.type() == QVariant::Double)
    {
        QDoubleSpinBox *doubleSpinBox = new QDoubleSpinBox(this);
        m_editWidget = doubleSpinBox;
        doubleSpinBox->setMaximum(999999999);
        doubleSpinBox->setMinimum(-999999999);
        doubleSpinBox->setValue(value.toDouble());
        ui->horizontalLayout->addWidget(doubleSpinBox);
        return true;
    }
    else if (value.type() == QVariant::Int)
    {
        QSpinBox *spinBox = new QSpinBox(this);
        m_editWidget = spinBox;
        spinBox->setMaximum(999999999);
        spinBox->setMinimum(-999999999);
        spinBox->setValue(value.toInt());
        ui->horizontalLayout->addWidget(spinBox);
        return true;
    }
    else if (value.type() == QVariant::Bool)
    {
        QCheckBox *checkBox = new QCheckBox(this);
        m_editWidget = checkBox;
        checkBox->setChecked(value.toBool());
        ui->horizontalLayout->addWidget(checkBox);
        return true;
    }
    else if (value.type() == QVariant::StringList)
    {
        QListWidget *listWidget = new QListWidget(this);
        m_editWidget = listWidget;
        listWidget->addItems(value.toStringList());
        ui->verticalLayout->insertWidget(0,listWidget);
        if (QString(property.name()).contains("pixmap", Qt::CaseInsensitive))
        {
            m_fileExtensions << "*.png" << "*.bmp";
            m_fileDirectory = "pics";
            ui->horizontalLayout->addWidget(m_toolButton);
            m_toolButton->setText("Add");
            ui->horizontalLayout->addWidget(m_removeButton);

            connect(m_toolButton, SIGNAL(clicked()), this, SLOT(getStringFromFile()));
        }
        return true;
    }
    else if (value.type() == QVariant::Color)
    {
        QColor color = value.value<QColor>();
        m_toolButton->setPalette(QPalette(color));
        m_editWidget = 0;
        ui->horizontalLayout->addWidget(m_toolButton);
        m_toolButton->setText("Select Color");
        connect(m_toolButton, SIGNAL(clicked()), this, SLOT(colorDialog()));
    }
    else if (value.type() == QVariant::PointF)
    {
        PointEdit *pointEdit = new PointEdit(value.toPointF(), this);
        m_editWidget = pointEdit;
        ui->horizontalLayout->addWidget(pointEdit);
        return true;
    }

    return false;
}

QVariant PropertyEditWidget::getValue()
{
    QVariant value;

    if (m_toolButton->text() == "Select Color")
        value = m_toolButton->palette().color(QPalette::Button);

    else if (qobject_cast<QLineEdit*>(m_editWidget))
        value.setValue(qobject_cast<QLineEdit*>(m_editWidget)->text());

    else if (qobject_cast<QDoubleSpinBox*>(m_editWidget))
        value.setValue(qobject_cast<QDoubleSpinBox*>(m_editWidget)->value());

    else if (qobject_cast<QSpinBox*>(m_editWidget))
        value.setValue(qobject_cast<QSpinBox*>(m_editWidget)->value());

    else if (qobject_cast<QCheckBox*>(m_editWidget))
        value.setValue(qobject_cast<QCheckBox*>(m_editWidget)->isChecked());

    else if (qobject_cast<PointEdit*>(m_editWidget))
        value.setValue(qobject_cast<PointEdit*>(m_editWidget)->getPoint());

    else if (qobject_cast<TriggerLineEdit*>(m_editWidget))
        value.setValue(qobject_cast<TriggerLineEdit*>(m_editWidget)->getTriggerText());

    else if (qobject_cast<QComboBox*>(m_editWidget))
    {
        QMetaEnum enumerator = m_property.enumerator();
        value.setValue(enumerator.keyToValue(qobject_cast<QComboBox*>(m_editWidget)->currentText().toStdString().c_str()));
    }
    else if (qobject_cast<QListWidget*>(m_editWidget))
    {
        QList<QListWidgetItem*> allItems = qobject_cast<QListWidget*>(m_editWidget)->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
        QStringList allStrings;
        foreach (QListWidgetItem *item, allItems)
        {
            allStrings.push_back(item->text());
        }

        value.setValue(allStrings);
    }
    else
        return QVariant();

    return value;
}

void PropertyEditWidget::writeProperty()
{
    m_property.write(m_object, getValue());
}

void PropertyEditWidget::getStringFromFile()
{
    GameFileDialog *dlg = new GameFileDialog(this);
    dlg->setFileExtensions(m_fileExtensions);
    dlg->setSubdirectory(m_fileDirectory);
    dlg->setAccept("Select");

    if (dlg->exec())
    {
        if (qobject_cast<QLineEdit*>(m_editWidget))
            qobject_cast<QLineEdit*>(m_editWidget)->setText(dlg->getFileName());
        else if (qobject_cast<QListWidget*>(m_editWidget))
            qobject_cast<QListWidget*>(m_editWidget)->addItem(dlg->getFileName());
    }

    delete dlg;
}

void PropertyEditWidget::removeItemFromList()
{
    if (qobject_cast<QListWidget*>(m_editWidget))
    {
        QListWidget* listWidget = qobject_cast<QListWidget*>(m_editWidget);
        qDebug() << "Removing Item: " << listWidget->currentItem()->text();
        delete listWidget->currentItem();
    }
}

void PropertyEditWidget::colorDialog()
{
    QColorDialog dlg;
    dlg.setCurrentColor(m_toolButton->palette().color(QPalette::Button));

    if (dlg.exec())
    {
        m_toolButton->setPalette(QPalette(dlg.selectedColor()));
    }
}
