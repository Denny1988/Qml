#ifndef SERVICE_QUOTATIONSERVICE_H
#define SERVICE_QUOTATIONSERVICE_H
#include <QObject>
#include "Common/Result/Result.h"
#include "Common/Result/PlaceResult.h"
#include "traderbusiness_global.h"

namespace Service
{
	class TRADERBUSINESS_EXPORT QuotationService: public QObject
	{
		Q_OBJECT
		
		public:
			QuotationService(QSharedPointer<Communication::SslChannel> channel);
            QSharedPointer<Common::Result::InvokeResult> loginQuotationService(QString userID, const QStringList & instrumentAndQuotePolicyIDs, bool isGuest=false);
            QSharedPointer<Common::Result::InvokeResult> logout(bool isGuest=false);

		private:
			QSharedPointer<Communication::SslChannel> _channel;

		signals:
			void quotationReceived(const QString quotations);

        public slots:
            void quotationReceivedSlot(const QString quotations);

	};
	
}
#endif	   //QUOTATIONSERVICE_H
