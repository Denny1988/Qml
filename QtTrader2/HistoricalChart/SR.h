#pragma once
#include <QString>
#include <QJsonObject>

namespace HistoricalChartSpace
{
	class SR
	{
	public:
        static QString setLanguage(const QString& language, QString languagePath = "");
		static QString get(const QString& key);

	private:
		SR();
		~SR();

		QString internalGetString(const QString& key);
        void switchLanguage(const QString& language, QString languagePath);

	private:
		QJsonObject m_jsonObject;

	private:
		static SR _instance;
	};
}
