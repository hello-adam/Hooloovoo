#ifndef PROPERTYEDITWIDGET_H
#define PROPERTYEDITWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QComboBox>
#include <QSharedPointer>
#include <QToolButton>
#include <QMetaProperty>
#include <QVariant>

namespace Ui {
    class PropertyEditWidget;
}

class PropertyEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyEditWidget(QWidget *parent = 0);
    ~PropertyEditWidget();

    bool setProperty(QMetaProperty property, QObject* object);
    QVariant getValue();

private:
    Ui::PropertyEditWidget *ui;
    QMetaProperty m_property;
    QObject* m_object;

    QWidget* m_editWidget;
    QToolButton* m_toolButton;
    QToolButton* m_removeButton;

    QStringList m_fileExtensions;
    QString m_fileDirectory;

private slots:
    void getStringFromFile();
    void removeItemFromList();
    void colorDialog();

public slots:
    void writeProperty();
};

#endif // PROPERTYEDITWIDGET_H
