#include "propertyeditwidget.h"
#include "ui_propertyeditwidget.h"
#include "gamefiledialog.h"


PropertyEditWidget::PropertyEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PropertyEditWidget)
{
    ui->setupUi(this);

    m_lineEdit = 0;
    m_doubleSpinBox = 0;
    m_spinBox = 0;
    m_checkBox = 0;
    m_toolButton = new QToolButton();
    m_toolButton->setText("...");
    connect(m_toolButton, SIGNAL(clicked()), this, SLOT(getStringFromFile()));
}

PropertyEditWidget::~PropertyEditWidget()
{
    if (m_toolButton)
        m_toolButton->deleteLater();
    delete ui;
}

bool PropertyEditWidget::setProperty(QMetaProperty property, QObject* object)
{
    if (m_lineEdit)
        m_lineEdit->deleteLater();
    if (m_doubleSpinBox)
        m_doubleSpinBox->deleteLater();
    if (m_spinBox)
        m_spinBox->deleteLater();
    if (m_checkBox)
        m_checkBox->deleteLater();

    m_fileExtensions.clear();
    m_fileDirectory.clear();

    m_property = property;
    m_object = object;

    QVariant value = property.read(object);

    switch (value.type())
    {
    case QVariant::String:
        m_lineEdit = new QLineEdit(value.toString(), this);
        ui->horizontalLayout->addWidget(m_lineEdit);
        if (QString(property.name()).contains("pixmap"))
        {
            m_fileExtensions << "*.png" << "*.bmp";
            m_fileDirectory = "pics";
            ui->horizontalLayout->addWidget(m_toolButton);
        }
        break;
    case QVariant::Double:
        m_doubleSpinBox = new QDoubleSpinBox(this);
        m_doubleSpinBox->setMaximum(999999999);
        m_doubleSpinBox->setMinimum(-999999999);
        m_doubleSpinBox->setValue(value.toDouble());
        ui->horizontalLayout->addWidget(m_doubleSpinBox);
        break;
    case QVariant::Int:
        m_spinBox = new QSpinBox(this);
        m_spinBox->setMaximum(999999999);
        m_spinBox->setMinimum(-999999999);
        m_spinBox->setValue(value.toInt());
        ui->horizontalLayout->addWidget(m_doubleSpinBox);
        break;
    case QVariant::Bool:
        m_checkBox = new QCheckBox(this);
        m_checkBox->setChecked(value.toBool());
        ui->horizontalLayout->addWidget(m_checkBox);
        break;
    default:
        return false;
    }

    return true;
}

bool PropertyEditWidget::writeProperty()
{
    QVariant value;

    if (m_lineEdit)
        value.setValue(m_lineEdit->text());
    else if (m_doubleSpinBox)
        value.setValue(m_doubleSpinBox->value());
    else if (m_spinBox)
        value.setValue(m_spinBox->value());
    else if (m_checkBox)
        value.setValue(m_checkBox->isChecked());
    else
        return false;

    return m_property.write(m_object, value);
}

void PropertyEditWidget::getStringFromFile()
{
    GameFileDialog *dlg = new GameFileDialog(this);
    dlg->setFileExtensions(m_fileExtensions);
    dlg->setSubdirectory(m_fileDirectory);
    dlg->setAccept("Select");

    if (dlg->exec())
    {
        if (m_lineEdit)
            m_lineEdit->setText(dlg->getFileName());
    }

    delete dlg;
}
