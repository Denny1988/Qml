#include "SR.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonParseError>
#include <QDebug>
#include "../QtFramework/QsLog.h"
#include "Helper.h"

namespace HistoricalChartSpace
{
	SR SR::_instance;

    QString SR::setLanguage(const QString& language, QString languagePath)
	{
		QString lang;
		if (language == "CHT" || language == "ENG" || language == "CHS")
		{
			lang = language;
		}
		else
		{
			lang = "ENG";
		}
        SR::_instance.switchLanguage(lang, languagePath);
		return lang;
	}

	QString SR::get(const QString& key)
	{
		return SR::_instance.internalGetString(key);
	}

	SR::SR()
	{
	}

	SR::~SR()
	{
	}

    void SR::switchLanguage(const QString& language, QString languagePath)
	{
		// load resource according to language from 
		QString resourceFolder("ChartSR");
		
#ifdef RUNTIME_OS_MACX
		QString fileName = languagePath.length() > 0 ? QString("%1/%2.json").arg(languagePath, language) : QString("%1/%2/%3.json").arg(Helper::getAppDataDirPath()).arg(resourceFolder).arg(language);
#else
		QString fileName = languagePath.length() > 0 ? QString("%1/%2.json").arg(languagePath, language) : QString("%1/%2/%3.json").arg(QCoreApplication::applicationDirPath()).arg(resourceFolder).arg(language);
#endif
		QFile resourceFile(fileName);
		if (resourceFile.exists())
		{
			resourceFile.open(QIODevice::ReadOnly);
			QByteArray rawData = resourceFile.readAll();
			QJsonParseError error;
			QJsonDocument document = QJsonDocument::fromJson(rawData, &error);
			if (document.isObject())
			{
				m_jsonObject = document.object();
			}
			else
			{
				if (error.error != QJsonParseError::NoError)
				{
					QLOG_WARN("Chart") << "SR::loadResource: " << error.errorString();
				}
			}
		}
		else
		{
			QLOG_WARN("Chart") << "SR::switchLanguage: resource file not exists:" << fileName;
		}
	}

	QString SR::internalGetString(const QString& key)
	{
		QString text;
		if (m_jsonObject.isEmpty())
		{
			QLOG_WARN("Chart") << "SR::internalGetString: m_jsonObject not set.";
		}
		else
		{
			if (m_jsonObject.contains(key))
			{
				text = m_jsonObject.value(key).toString();
			}
			else if (key.endsWith(':'))
			{
				QString key2 = key.left(key.length() - 1);
				if (m_jsonObject.contains(key2))
				{
					text = m_jsonObject.value(key2).toString();
					text += ':';
				}
				else
				{
					QLOG_WARN("Chart") << "SR::internalGetString: invalid resource key:" << key2;
				}
			}
			else
			{
				QLOG_WARN("Chart") << "SR::internalGetString: invalid resource key:" << key;
			}
		}
		if (text.isEmpty()) text = key;
		return text;
	}
}
