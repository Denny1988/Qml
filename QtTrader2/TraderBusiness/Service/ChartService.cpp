#include "ChartService.h"

#include "Common/Enum/StatusEnum.h"
#include "Common/Constants/MethodNameConstants.h"
#include "Common/Util/Convert_Common.h"
#include "Common/LogHelper.h"
#include "Common/CustomerSetting.h"

using namespace Communication;

namespace Service
{
	ChartService::ChartService(QSharedPointer<Communication::SslChannel> channel)
	{
		this->_channel = channel;
		timeout = 10;
	}
	
	///------------------Request Method---------------------
	QSharedPointer<Common::Result::ChartXmlDataResult> ChartService::getChartData(QString instrumentId, QString fromDateTime, QString toDateTime, QString dataCycleParameter)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<  "ChartService::getChartData"
                                                                << " instrumentId:" << instrumentId
                                                                << " fromDateTime:" << fromDateTime
                                                                << " toDateTime:" << toDateTime
                                                                <<" dataCycleParameter:" << dataCycleParameter;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("accountID",QVariant(Common::CustomerSetting::getInstance()->getValue("AccountUI/CurrentAccount"))));
		parameters.append(Common::Parameter("instrumentID",QVariant(instrumentId)));
		parameters.append(Common::Parameter("from",QVariant(fromDateTime)));
		parameters.append(Common::Parameter("to",QVariant(toDateTime)));
		parameters.append(Common::Parameter("dataCycleParameter",QVariant(dataCycleParameter)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::AsyncGetChartData2ForTrader, parameters, timeout);
		return QSharedPointer<Common::Result::ChartXmlDataResult>(new Common::Result::ChartXmlDataResult(result));
	}

	QSharedPointer<Common::Result::ChartXmlDataResult> ChartService::getNewChartData(QString instrumentId, QString quotePolicyId, QString fromDateTime, QString toDateTime, QString dataCycleParameter, int requestType, int rangeType /* = 0 */)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<  "ChartService::getNewChartData"
                                                                 << " instrumentId:" << instrumentId
                                                                 << " quotePolicyId:" << quotePolicyId
                                                                 << " fromDateTime:" << fromDateTime
                                                                 << " toDateTime:" << toDateTime
                                                                 <<" dataCycleParameter:" << dataCycleParameter
                                                                 <<" requestType:" << requestType
                                                                 <<" rangeType:" << rangeType;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("accountID",QVariant(Common::CustomerSetting::getInstance()->getValue("AccountUI/CurrentAccount"))));
		parameters.append(Common::Parameter("instrumentID",QVariant(instrumentId)));
		parameters.append(Common::Parameter("quotePolicyID",QVariant(quotePolicyId)));
		parameters.append(Common::Parameter("from",QVariant(fromDateTime)));
		parameters.append(Common::Parameter("to",QVariant(toDateTime)));
		parameters.append(Common::Parameter("dataCycleParameter",QVariant(dataCycleParameter)));
		parameters.append(Common::Parameter("requestType", QVariant(requestType)));
		parameters.append(Common::Parameter("rangeType", QVariant(rangeType)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::AsyncGetChartData2ForCppTrader, parameters, timeout * 30);
		return QSharedPointer<Common::Result::ChartXmlDataResult>(new Common::Result::ChartXmlDataResult(result, requestType));
	}

	QSharedPointer<Common::Result::ChartXmlDataResult> ChartService::getQuotePolicies(QString instrumentId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<  "ChartService::getQuotePolicies instrumentId:" << instrumentId;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("instrumentId",QVariant(instrumentId)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetQuotePolicies, parameters, timeout);
		return QSharedPointer<Common::Result::ChartXmlDataResult>(new Common::Result::ChartXmlDataResult(result));
	}

	QSharedPointer<Common::Result::ChartCountResult> ChartService::getChartCount(QString instrumentId, QString quotePolicyId, QString dataCycleParameter)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) << "ChartService::getChartCount instrumentId:" << instrumentId
                                                               << " quotePolicyId:" << quotePolicyId
                                                               << " dataCycleParameter:" << dataCycleParameter;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("instrumentID",QVariant(instrumentId)));
		parameters.append(Common::Parameter("quotePolicyID",QVariant(quotePolicyId)));
		parameters.append(Common::Parameter("dataCycleParameter",QVariant(dataCycleParameter)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetChartCount, parameters, timeout);
		return QSharedPointer<Common::Result::ChartCountResult>(new Common::Result::ChartCountResult(result));
	}

	QSharedPointer<Common::Result::ChartVersionResult> ChartService::getChartVersion(QString instrumentId, QString quotePolicyId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<  "ChartService::getChartVersion instrumentId:" << instrumentId
                                                               << " quotePolicyId:" << quotePolicyId ;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("instrumentID",QVariant(instrumentId)));
		parameters.append(Common::Parameter("quotePolicyID",QVariant(quotePolicyId)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetChartVersion, parameters, timeout);
		return QSharedPointer<Common::Result::ChartVersionResult>(new Common::Result::ChartVersionResult(result));
	}

	QSharedPointer<Common::Result::ChartFirstTradeDayResult> ChartService::GetChartFirstTradeDay(int rangeType)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<  "ChartService::GetChartFirstTradeDay rangeType:" << rangeType;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("rangeType",QVariant(rangeType)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetChartFirstTradeDay, parameters, timeout);
		return QSharedPointer<Common::Result::ChartFirstTradeDayResult>(new Common::Result::ChartFirstTradeDayResult(result));
	}

	QSharedPointer<Common::Result::ChartXmlDataResult> ChartService::getBORealTimeData(QString instrumentId, QString quotePolicyId, int dataCount)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<  "ChartService::getBORealTimeData instrumentId:" << instrumentId << " quotePolicyId:" <<quotePolicyId << " dataCount:" <<dataCount;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("instrumentID",QVariant(instrumentId)));
		parameters.append(Common::Parameter("quotePolicyID",QVariant(quotePolicyId)));
		parameters.append(Common::Parameter("dataCount",QVariant(dataCount)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetBORealTimeData, parameters, timeout);
		return QSharedPointer<Common::Result::ChartXmlDataResult>(new Common::Result::ChartXmlDataResult(result));
	}

	QSharedPointer<Common::Result::FixPeriodBeginTimeResult> ChartService::getFixPeriodBeginTimes(QString instrumentId)
	{
		QLOG_INFO(Common::Constants::LogKeyConstants::Service) << "ChartService::getFixPeriodBeginTimes, instrumentId:" << instrumentId;
		QVector<Common::Parameter> parameters;
		parameters.append(Common::Parameter("instrumentId", QVariant(instrumentId)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetFixPeriodBeginTimes, parameters, timeout);
		return QSharedPointer<Common::Result::FixPeriodBeginTimeResult>(new Common::Result::FixPeriodBeginTimeResult(result));
	}

	///------------------Notify Method----------------------

}
#include "moc_ChartService.cpp"
