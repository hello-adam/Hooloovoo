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

    QDomElement serialize();
    void deserialize(const QDomElement &prop);

    QVariant getValue();
    void setValue(QVariant value);

    ValueHelperDialog getValueHelperDialog() {return m_helper;}

    QString getDisplayName();
    QString getDisplayValue();
    QWidget* createEditorWidget();

    bool isComponent() {return !m_propertyHolder;}

    QObject* getHolder() {return m_propertyHolder;}
    QMetaProperty getProperty() {return m_propertyHolder->metaObject()->property(m_propertyHolder->metaObject()->indexOfProperty(qPrintable(m_propertyName)));}

private:
    QObject *m_propertyHolder;
    QString m_propertyName;

    ValueHelperDialog m_helper;

    QVariant m_value;

signals:

public slots:

};

#endif // PROPERTY_H
