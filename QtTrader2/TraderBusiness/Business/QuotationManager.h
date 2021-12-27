#ifndef BUSINESS_QUOTATIONMANAGER_H
#define BUSINESS_QUOTATIONMANAGER_H
#include <QDomNode>
#include "Business/Model/InitialQuotation.h"
#include <QMap>
#include <QList>
#include <QSharedPointer>
#include "Business/SettingManager.h"
#include "traderbusiness_global.h"
#include <QDateTime>

namespace Business
{
	enum {ColDelimeter='\t', RowDelimeter='\n'};
	class TRADERBUSINESS_EXPORT QuotationManager
	{
	private:
		QMap<QString,QSharedPointer<Model::InitialQuotation>> _overridedQuotations;
		bool _isWriteLog;
        QDateTime _lastQuotationReceivedTime;

	public:
		QuotationManager();

	public:
		void updateInitializeData(const QDomNode& xmlInitializeData);
		QList<QSharedPointer<Model::InitialQuotation>> getOverridedQuotations();
		QSharedPointer<Model::InitialQuotation> getOverridedQuotation(QUuid quotePolicyId, QUuid instrumentId);
		void setQuotePolicy(QMap<QUuid, QMap<QUuid, QString>> instrumentQuotePolicyMappings);

		QList<QUuid> quotationReceived(const QString quotations);
		std::vector<std::string> parseCommon(const std::string& input,char delimeter);

		void updateDailyCloseQuotation(const QDomNode& xmlData, QSharedPointer<SettingManager> settingManager);
        QDateTime getLastQuotationReceivedTime() { return _lastQuotationReceivedTime; }
        void setLastQuotationReceivedTime(QDateTime value) { _lastQuotationReceivedTime = value; }
	};

}
#endif
