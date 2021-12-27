#ifndef QMUTILANGUAGES_H
#define QMUTILANGUAGES_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QVariant>
class  QDomDocument;

typedef struct MyStruct
{
    MyStruct():m_langId(0),m_name(""),m_dir(""),m_default(false){}
    bool isEmpty()
    {
        return m_langId==0;
    }
    bool operator==(const MyStruct& lang) const
    {
        return (this->m_langId==lang.m_langId &&
                this->m_name==lang.m_name &&
                this->m_shortName==lang.m_shortName &&
                this->m_dir==lang.m_dir &&
                this->m_default==lang.m_default &&
                this->m_qtLanguageEnum==lang.m_qtLanguageEnum &&
                this->m_qtCountryEnum==lang.m_qtCountryEnum
                );
    }
    int		m_langId;
    QString m_name;
    QString m_shortName;
    QString m_dir;
    bool	m_default;
    QString m_qtLanguageEnum;
    QString m_qtCountryEnum;

} Language ;

class QMutiLanguages : public QObject
{
    Q_OBJECT

public:
    QMutiLanguages(){}
    QMutiLanguages(const QString& langConfigXml, int languageId, QObject *parent=0);
    QMutiLanguages& operator=(const QMutiLanguages& lang);
    ~QMutiLanguages();
    void reLoadMutiLanguages(const QString& langConfigXml);


public:
    void useConfigLanguageAsCurrentLanguage();			  //使用打包配置的语言版本;
    void changeCurrentLanguage(int);				  //改变当前语言版本(通过language id (LANGID));
    void changeCurrentLanguage(const QString&);		  //改变当前语言版本;
    void changeCurrentLanguage(const Language&);	  //改变当前语言版本;   overload;
    Language getCurrentLanguage();					  //获取当前语言版本;
    Language getConfigLanguage();					  //获取打包配置的语言版本;
    Language getDefaultLanguage();					  //获取默认语言版本;
    Language getLanguage(int);						  //根据langId获取所对应的语言版本;
    bool setDefaultLanguage(int);					  //设置默认语言版本;
    bool setDefaultLanguage(const Language&);		  //设置默认语言版本;   overload;
    bool contains(int);								  //是否存在某种语言版本;
    bool contains(const Language&);					  //是否存在某种语言版本;	overload;
    QList<Language> getLanguageList();				  //获取语言列表;
    Q_INVOKABLE QString Tr(const QString&,const QString& section);//获取语句相应的翻译; overload;
    Q_INVOKABLE QString Tr(const char*,const QString& section);	  //获取语句相应的翻译; overload;
    Q_INVOKABLE QString Tr(const char*,const char* section);	  //获取语句相应的翻译; overload;
    Q_INVOKABLE QString Tr(const QString&,const char* section);	  //获取语句相应的翻译; overload;

private:
    bool loadMutiLanguages(QObject*);
    bool changeLanguages(const Language&);
    bool readLanguageConfigXml();
    bool readConfigXml(const QDomDocument&);
    bool loadResource(const Language&);
    void showErrorMsg(const QString&);
    bool setDefaultSettingToConfigXml(const Language&);

private:
    QMap<int, Language >								m_configMap;
    QMap<int, QMap<QString,QMap<QString,QString> > >	m_resourceMap;
    Language								m_currentLanguage; //当前所使用的语言版本;
    Language								m_configLanguage;      //与打包配置的语言版本;
    Language								m_defaultLanguage; //默认的语言版本;
    QList<Language>							m_languageList;	   //语言列表;
    QMap<QString,QMap<QString,QString> >	m_currentResource; //当前所使用的资源(翻译所依据);
    int		m_configLangId;

private:
    QString									m_langConfigXmlFileDir; //配置文件的路径(文件名);

};



#endif // QMUTILANGUAGES_H
