#ifndef SERVICE_CHARTSERVICE_H
#define SERVICE_CHARTSERVICE_H
#include <QObject>
#include "Common/Result/Result.h"
#include "Common/Result/PlaceResult.h"
#include "Common/Result/ChartXmlDataResult.h"
#include "Common/Result/ChartCountResult.h"
#include "traderbusiness_global.h"

namespace Service
{
	class TRADERBUSINESS_EXPORT ChartService: public QObject
	{		
		Q_OBJECT
		public:
			ChartService(QSharedPointer<Communication::SslChannel> channel);
			QSharedPointer<Common::Result::ChartXmlDataResult> getChartData(QString instrumentId, QString fromDateTime, QString toDateTime, QString dataCycleParameter);
			QSharedPointer<Common::Result::ChartXmlDataResult> getNewChartData(QString instrumentId, QString quotePolicyId, QString fromDateTime, QString toDateTime, QString dataCycleParameter, int requestType, int rangeType = 0);
			QSharedPointer<Common::Result::ChartXmlDataResult> getQuotePolicies(QString instrumentId);
			QSharedPointer<Common::Result::ChartCountResult> getChartCount(QString instrumentId, QString quotePolicyId, QString dataCycleParameter);
			QSharedPointer<Common::Result::ChartVersionResult> getChartVersion(QString instrumentId, QString quotePolicyId);
			QSharedPointer<Common::Result::ChartFirstTradeDayResult> GetChartFirstTradeDay(int rangeType);
			QSharedPointer<Common::Result::ChartXmlDataResult> getBORealTimeData(QString instrumentId, QString quotePolicyId, int dataCount);
			QSharedPointer<Common::Result::FixPeriodBeginTimeResult> getFixPeriodBeginTimes(QString instrumentId);

		private:
			QSharedPointer<Communication::SslChannel> _channel;
			int timeout;
	};	
}
#endif	   //CHARTSERVICE_H
