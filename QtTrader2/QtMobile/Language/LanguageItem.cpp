#include "LanguageItem.h"

#include <QMap>
#include <QList>
#include <QDir>
#include <QDomDocument>
#include <QMessageBox>
#include <qdebug.h>

#include <QApplication>


LanguageItem::LanguageItem()
{
}

LanguageItem::~LanguageItem()
{
    qDebug() << "destruct---------------- LanguageItem::~LanguageItem()";
}

bool LanguageItem::isEmpty()
{
    return m_langId==0;
}
