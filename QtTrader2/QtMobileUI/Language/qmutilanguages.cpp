#include "qmutilanguages.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QtXml/QDomDocument>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QMessageBox>
#include <QDebug>

QMutiLanguages::QMutiLanguages(const QString& langConfigXml, int languageId, QObject *parent) : QObject(parent)
{
    if(!QFile::exists(langConfigXml))
    {
        showErrorMsg("No Exist configuration file of languages");
        return ;
    }
    m_langConfigXmlFileDir = langConfigXml;
    m_configLangId = languageId;
    if(!loadMutiLanguages(parent))
        return ;
    useConfigLanguageAsCurrentLanguage();
}
QMutiLanguages& QMutiLanguages::operator=(const QMutiLanguages& lang)
{
    if(&lang==this)
        return *this;
    this->m_configMap = lang.m_configMap;
    this->m_resourceMap = lang.m_resourceMap;
    this->m_currentLanguage = lang.m_currentLanguage;
    this->m_configLanguage = lang.m_configLanguage;
    this->m_defaultLanguage = lang.m_defaultLanguage;
    this->m_currentResource = lang.m_currentResource;
    this->m_languageList = lang.m_languageList;
    this->m_langConfigXmlFileDir = lang.m_langConfigXmlFileDir;
    return *this;
}
QMutiLanguages::~QMutiLanguages()
{
    qDebug() << "destruct---------------- QMutiLanguages::~QMutiLanguages()";
}

void QMutiLanguages::reLoadMutiLanguages(const QString& langConfigXml)
{
    m_langConfigXmlFileDir = langConfigXml;
    this->readLanguageConfigXml();
    this->useConfigLanguageAsCurrentLanguage();
}

//public;
void QMutiLanguages::useConfigLanguageAsCurrentLanguage()
{
    if( (!m_configLanguage.isEmpty()) && (m_currentLanguage==m_configLanguage) )	//防止重复调用;
        return ;

    if(!contains(this->m_configLangId))
    {
        //ShowErrorMsg("No Exist the Language version same with OS !");
        return ;
    }
    Language lang = m_configMap[this->m_configLangId];
    m_configLanguage = lang;
    changeCurrentLanguage(lang);
}

void QMutiLanguages::changeCurrentLanguage(int langId)
{
    changeCurrentLanguage(m_configMap[langId]);
}
void QMutiLanguages::changeCurrentLanguage(const QString& languageName)
{
    int langId = 0x0409;
    for(int i=0;i<m_languageList.count();i++)
    {
        if(languageName==m_languageList[i].m_name)
        {
            langId = m_languageList[i].m_langId;
            break;
        }

    }
    changeCurrentLanguage(m_configMap[langId]);
}
void QMutiLanguages::changeCurrentLanguage(const Language& language)
{
    if(changeLanguages(language))
        m_currentLanguage = language;
}
Language QMutiLanguages::getCurrentLanguage()
{
    return  m_currentLanguage;
}
Language QMutiLanguages::getConfigLanguage()
{
    return  m_configLanguage;
}
Language QMutiLanguages::getDefaultLanguage()
{
    return  m_defaultLanguage;
}

Language QMutiLanguages::getLanguage(int langId)
{
    return 	m_configMap[langId];
}

bool QMutiLanguages::setDefaultLanguage(int langId)
{
    if(m_configMap.contains(langId))
        return 	setDefaultLanguage(m_configMap[langId]);
    return false;
}
bool QMutiLanguages::setDefaultLanguage(const Language& lang)
{
    Language language = lang;
    if(language.isEmpty() || language==m_defaultLanguage)
        return false;

    if(setDefaultSettingToConfigXml(lang))
    {
        m_defaultLanguage = language;
        return true;
    }

    return false;
}
bool QMutiLanguages::contains(int langId)
{
    return m_configMap.contains(langId);
}
bool QMutiLanguages::contains(const Language& lang)
{
    return contains(lang.m_langId);
}
QList<Language> QMutiLanguages::getLanguageList()
{
    return m_languageList;
}
QString QMutiLanguages::Tr(const QString& key,const QString& section)
{
    if(!section.isEmpty() && m_currentResource.contains(section) && !key.isEmpty() && m_currentResource[section].contains(key))
        return 	m_currentResource[section][key];
    return key;
}
QString QMutiLanguages::Tr(const char* key,const QString& section)
{
    return  Tr(QString(key),section);
}
QString QMutiLanguages::Tr(const char* key,const char* section)
{
    return  Tr(QString(key),QString(section));
}
QString QMutiLanguages::Tr(const QString& key,const char* section)
{
    return Tr(key,QString(section));
}
//private;
bool QMutiLanguages::loadMutiLanguages(QObject *parent)
{
    if(readLanguageConfigXml())
        return true;
    showErrorMsg("Load file Failed!");
    return false;
}
bool QMutiLanguages::changeLanguages(const Language& lang)
{
    Language language = lang;
    if(language.isEmpty() || !m_resourceMap.contains(language.m_langId))
        return false;
    if(!m_currentResource.isEmpty())
        m_currentResource.clear();
    m_currentResource = m_resourceMap[language.m_langId];
    return true;
}
bool QMutiLanguages::readLanguageConfigXml()
{
    QFile file(m_langConfigXmlFileDir);
    if(!(file.open(QIODevice::ReadOnly)))
    {
        return false;
    }
    QDomDocument domDoc;
    if(!(domDoc.setContent(&file)))
    {
        file.close();
        return false;
    }
    file.close();

    return readConfigXml(domDoc);
}
bool QMutiLanguages::readConfigXml(const QDomDocument& domDoc)
{
    m_resourceMap.clear();
    m_configMap.clear();
    m_languageList.clear();

    QString currentDir = m_langConfigXmlFileDir;
    currentDir = currentDir.left(currentDir.lastIndexOf("/")+1);

    QDomNodeList nodeList = domDoc.elementsByTagName("Languages");
    for(int i=0;i<nodeList.count();i++)
    {
        QDomNode node = nodeList.item(i);
        QDomNodeList itemlist = node.toElement().childNodes();
        for(int j=0;j<itemlist.count();j++)
        {
            QDomNode myNode = itemlist.item(j);
            bool ok;
            Language lang;
            lang.m_langId = myNode.toElement().attribute("langId").toInt(&ok,16);
            lang.m_name = myNode.toElement().attribute("name");
            lang.m_shortName = myNode.toElement().attribute("shortName");
            lang.m_dir = currentDir + myNode.toElement().attribute("dir");
            lang.m_default = (myNode.toElement().attribute("default")=="true") ? true : false;
            if(QFile::exists(lang.m_dir) && loadResource(lang))
            {
                if(lang.m_default)
                {
                    m_defaultLanguage = lang;
                }
                m_languageList.push_back(lang);
                m_configMap.insert(lang.m_langId,lang);
            }
        }
    }
    return true;
}
bool QMutiLanguages::loadResource(const Language& lang)
{
    QFile file(lang.m_dir);
    bool bexist = QFile::exists(lang.m_dir);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    QTextStream stream(&file);
    QString content = stream.readAll();
    QJsonParseError* error=new QJsonParseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8(), error);
    if(jsonDoc.isObject())
    {
        QMap<QString,QMap<QString,QString> >  keyValue;
        QJsonObject root = jsonDoc.object();
        for(auto begin = root.begin(), end= root.end(); begin!=end;++begin)
        {
            QString key = begin.key();
            QJsonObject root2 = begin.value().toObject();
            if(key.isEmpty() || key.isNull())
                continue;
            QMap<QString,QString> vaule;
            for(auto begin2=root2.begin(),end2=root2.end();begin2!=end2;++begin2)
            {
                QString key2 = begin2.key();
                QString value2 = begin2.value().toString();
                if(key2.isEmpty() || key2.isNull())
                    continue;
                vaule.insert(key2,value2);
            }
            keyValue.insert(key,vaule);
        }
        m_resourceMap.insert(lang.m_langId,keyValue);
        return true;
    }
    return false;
}
void QMutiLanguages::showErrorMsg(const QString& msg)
{
    QMessageBox errMsg;
    errMsg.setText(msg);
    errMsg.exec();
}
bool QMutiLanguages::setDefaultSettingToConfigXml(const Language& lang)
{
    QFile readfile(m_langConfigXmlFileDir);
    if(!readfile.open(QIODevice::ReadOnly|QIODevice::Text))
        return false;
    QDomDocument domDoc;
    if(domDoc.setContent(&readfile))
    {
        readfile.close();
        QDomElement root = domDoc.documentElement();  //根元素<configuration>;
        QDomNode subRoot = root.firstChild(); //第一个子元素<Languages>;
        QDomNode child = subRoot.firstChild(); //第一项<Item>;
        while(!child.isNull())
        {
            bool bOk;
            QDomNode oldNode = child;
            QDomElement newItem = child.toElement();
            if(lang.m_langId==newItem.attribute("langId").toInt(&bOk,16))
            {
                newItem.setAttribute("default","true");
            }
            else
            {
                newItem.setAttribute("default","false");
            }
            child = child.nextSibling();
        }
    }
    QFile writefile(m_langConfigXmlFileDir);
    if(!writefile.open(QIODevice::ReadWrite|QIODevice::Text))
        return false;
    QTextStream out(&writefile);
    domDoc.save(out,4);
    writefile.close();

    return true;
}
