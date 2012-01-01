#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include <QWidget>
#include "../property.h"

class EditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EditWidget(Property *prop, QWidget *parent = 0);

    virtual QVariant getValue() = 0;
    virtual void setValue(QVariant value) = 0;

protected:
    Property::ValueHelperDialog propertyValueHelperDialog() {return m_property->getValueHelperDialog();}

private:
    Property* m_property;

signals:

public slots:
    void writeProperty() {m_property->setValue(getValue());}
    void readProperty() {setValue(m_property->getValue());}
};

#endif // EDITWIDGET_H
