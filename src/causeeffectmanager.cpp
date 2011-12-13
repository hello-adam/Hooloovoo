#include "causeeffectmanager.h"
#include "gameobject.h"

CauseEffectManager::CauseEffectManager(GameObject *parent) :
    QObject(parent)
{
    m_gameObject = parent;
}

QDomElement CauseEffectManager::serialize()
{
    QDomDocument doc;
    QDomElement causeEffect = doc.createElement("causeeffectdefinitions");

    foreach(CauseEffectDefinition def, m_definitions)
    {
        QDomElement defElem = doc.createElement("definition");
        defElem.setAttribute("causeID", def.causeID);
        defElem.setAttribute("causeName", def.causeName);
        defElem.setAttribute("effectID", def.effectID);
        defElem.setAttribute("effectName", def.effectName);

        causeEffect.appendChild(defElem);
    }

    return causeEffect;
}

bool CauseEffectManager::deserialize(const QDomElement &defs)
{
    QDomElement defElem = defs.firstChildElement("definition");
    while (!defElem.isNull())
    {
        CauseEffectDefinition def;
        def.causeID = defElem.attribute("causeID").toInt();
        def.causeName = defElem.attribute("causeName");
        def.effectID = defElem.attribute("effectID").toInt();
        def.effectName = defElem.attribute("effectName");
        m_definitions.push_back(def);

        connectCauseEffect(def.causeID, def.causeName, def.effectID, def.effectName);

        defElem = defElem.nextSiblingElement("definition");
    }

    return true;
}

bool CauseEffectManager::addCauseEffectDefinition(int causeID, QString causeName, int effectID, QString effectName)
{
    CauseEffectDefinition def;

    if (connectCauseEffect(causeID, causeName, effectID, effectName))
    {
        def.causeID = causeID;
        def.causeName = causeName;
        def.effectID = effectID;
        def.effectName = effectName;
        m_definitions.push_back(def);
        return true;
    }

    return false;
}

bool CauseEffectManager::removeCauseEffectDefinition(int causeID, QString causeName, int effectID, QString effectName)
{
    for(int i=0; i<m_definitions.count(); i++)
    {
        CauseEffectDefinition def = m_definitions.at(i);
        if (def.causeID == causeID
                && def.causeName == causeName
                && def.effectID == effectID
                && def.effectName == effectName)
        {
            m_definitions.removeAt(i);

            return disconnectCauseEffect(causeID, causeName, effectID, effectName);
        }
    }

    return false;
}

void CauseEffectManager::clearDefinitions()
{
    foreach (CauseEffectDefinition def, m_definitions)
    {
        disconnectCauseEffect(def.causeID, def.causeName, def.effectID, def.effectName);
    }

    m_definitions.clear();
}

QList<CauseEffectManager::CauseEffectDefinition> CauseEffectManager::getRelatedDefinitions(int ID, QString name)
{
    QList<CauseEffectDefinition> defs;

    defs << getDefinitionsWithLinkedCauses(ID, name);
    defs << getDefinitionsWithLinkedEffects(ID, name);

    return defs;
}

QList<CauseEffectManager::CauseEffectDefinition> CauseEffectManager::getDefinitionsWithLinkedEffects(int ID, QString name)
{
    QList<CauseEffectDefinition> defs;

    foreach (CauseEffectDefinition def, m_definitions)
    {
        if (def.causeID == ID && def.causeName == name)
        {
            defs << def;
        }
    }

    return defs;
}

QList<CauseEffectManager::CauseEffectDefinition> CauseEffectManager::getDefinitionsWithLinkedCauses(int ID, QString name)
{
    QList<CauseEffectDefinition> defs;

    foreach (CauseEffectDefinition def, m_definitions)
    {
        if (def.effectID == ID && def.effectName == name)
        {
            defs << def;
        }
    }

    return defs;
}

bool CauseEffectManager::connectCauseEffect(int causeID, QString causeName, int effectID, QString effectName)
{
    Component* cause;
    if (causeID == 0)
        cause = m_gameObject;
    else
        cause = m_gameObject->getComponentByID(causeID);
    if (!cause)
        return false;
    QMetaObject metaCause = *cause->metaObject();
    int causeSignalID = metaCause.indexOfMethod(causeName.toStdString().c_str());

    Component* effect;
    if (effectID == 0)
        effect = m_gameObject;
    else
        effect = m_gameObject->getComponentByID(effectID);
    if (!effect)
        return false;
    QMetaObject metaEffect = *effect->metaObject();
    int effectSlotID = metaEffect.indexOfMethod(effectName.toStdString().c_str());

    if (causeSignalID < 0 || effectSlotID < 0)
    {
        qDebug() << "signal/slot ids invalid for attempted connect!!";
        return false;
    }

    return QMetaObject::connect(cause, causeSignalID, effect, effectSlotID);
}

bool CauseEffectManager::disconnectCauseEffect(int causeID, QString causeName, int effectID, QString effectName)
{
    Component* cause;
    if (causeID == 0)
        cause = m_gameObject;
    else
        cause = m_gameObject->getComponentByID(causeID);
    if (!cause)
        return false;
    QMetaObject metaCause = *cause->metaObject();
    int causeSignalID = metaCause.indexOfMethod(causeName.toStdString().c_str());

    Component* effect;
    if (effectID == 0)
        effect = m_gameObject;
    else
        effect = m_gameObject->getComponentByID(effectID);
    if (!effect)
        return false;
    QMetaObject metaEffect = *effect->metaObject();
    int effectSlotID = metaEffect.indexOfMethod(effectName.toStdString().c_str());

    if (causeSignalID < 0 || effectSlotID < 0)
    {
        qDebug() << "signal/slot ids invalid for attempted disconnect!!";
        return false;
    }

    return QMetaObject::disconnect(cause, causeSignalID, effect, effectSlotID);
}
