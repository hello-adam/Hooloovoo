#ifndef CAUSEEFFECTMANAGER_H
#define CAUSEEFFECTMANAGER_H

#include <QObject>
#include <QtXml>

class GameObject;

class CauseEffectManager : public QObject
{
    Q_OBJECT
public:
    explicit CauseEffectManager(GameObject *parent);

    struct CauseEffectDefinition
    {
        int causeID;
        int effectID;
        QString causeName;
        QString effectName;
    };

    QDomElement serialize();
    bool deserialize(const QDomElement &defs);

    bool addCauseEffectDefinition(int causeID, QString causeName, int effectID, QString effectName);
    bool removeCauseEffectDefinition(int causeID, QString causeName, int effectID, QString effectName);
    void clearDefinitions();

    QList<CauseEffectDefinition> getRelatedDefinitions(int ID, QString name);
    QList<CauseEffectDefinition> getDefinitionsWithLinkedCauses(int ID, QString name);
    QList<CauseEffectDefinition> getDefinitionsWithLinkedEffects(int ID, QString name);

private:
    QList<CauseEffectDefinition> m_definitions;
    GameObject* m_gameObject;

    bool connectCauseEffect(int causeID, QString causeName, int effectID, QString effectName);
    bool disconnectCauseEffect(int causeID, QString causeName, int effectID, QString effectName);

signals:

public slots:

};

#endif // CAUSEEFFECTMANAGER_H
