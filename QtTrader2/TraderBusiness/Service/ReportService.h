#ifndef SERVICE_REPORTSERVICE_H
#define SERVICE_REPORTSERVICE_H
#include <QObject>
#include "Common/Result/Result.h"
#include "traderbusiness_global.h"

namespace Service
{
	class TRADERBUSINESS_EXPORT ReportService: public QObject
	{		
		Q_OBJECT
		public:
			ReportService(QSharedPointer<Communication::SslChannel> channel);

		private:
			QSharedPointer<Communication::SslChannel> _channel;

		public:
			QSharedPointer<Common::Result::GetReportUrlResult> getReportUrl(QString companyName, QString reportType, const QStringList &  accountIds, 
                QString tradeDayBegin, QString tradeDayEnd, QString language, QString userId, QString deliveryRequestId);

            QSharedPointer<Common::Result::GetReportUrlResult> GetOrderConfirmationReportUrl(QString companyName, QString language, const QStringList &  orderIds);

	};
	

}
#endif	   //IREPORTSERVICE_H
