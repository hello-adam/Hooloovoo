#ifndef PROPERTY_H
#define PROPERTY_H

#include <QObject>
#include <QtXml>

class Property : public QObject
{
    Q_OBJECT
public:

    enum ValueHelperDialog
    {
        NoHelperDialog, GameObjectDialog, AudioDialog, PictureDialog
    };

    explicit Property(QObject *propertyHolder, QString propertyName, ValueHelperDialog helper = NoHelperDialog);

    virtual QDomElement serialize();
    virtual void deserialize(const QDomElement &prop);

    virtual QVariant getValue();
    virtual void setValue(QVariant value);

    ValueHelperDialog getValueHelperDialog() {return m_helper;}
    void setValueHelperDialog(ValueHelperDialog helper) {m_helper = helper;}

    virtual QString getDisplayName();
    virtual QString getDisplayValue();
    virtual QWidget* createEditorWidget();

    bool isComponent() {return !m_propertyHolder;}

    QObject* getHolder() {return m_propertyHolder;}
    QMetaProperty getProperty() {return m_propertyHolder->metaObject()->property(m_propertyHolder->metaObject()->indexOfProperty(qPrintable(m_propertyName)));}
    QString getName() {return m_propertyName;}

private:
    QObject *m_propertyHolder;
    QString m_propertyName;

    ValueHelperDialog m_helper;

    QVariant m_value;

signals:

public slots:

};

#endif // PROPERTY_H
