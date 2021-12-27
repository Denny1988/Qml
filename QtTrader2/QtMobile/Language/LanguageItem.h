#ifndef LANGUAGEITEM_H
#define LANGUAGEITEM_H

#include <QMap>
#include <QList>
#include <QDir>
#include <QDomDocument>
#include <QMessageBox>
#include <qdebug.h>

#include <QApplication>

class LanguageItem
{
public:
    LanguageItem();
    ~LanguageItem();

public:

    bool isEmpty();

private:
    int		m_langId;
    QString m_name;
    QString m_description;
    QString m_dir;
    bool	m_default;
};
#endif	   //LANGUAGEITEM_H
