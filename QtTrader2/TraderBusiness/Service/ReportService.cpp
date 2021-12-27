#include "ReportService.h"
#include "Common/Enum/StatusEnum.h"
#include "Common/Constants/MethodNameConstants.h"
#include "Common/Util/Convert_Common.h"
#include "Common/LogHelper.h"
#include "Common/CustomerSetting.h"

using namespace Communication;

namespace Service
{
	ReportService::ReportService(QSharedPointer<Communication::SslChannel> channel)
	{
		this->_channel = channel;
	}

	
	///------------------Request Method---------------------
	
	///------------------public Method----------------------
	QSharedPointer<Common::Result::GetReportUrlResult> ReportService::getReportUrl(QString companyName, QString reportType, const QStringList &  accountIds, 
				QString tradeDayBegin, QString tradeDayEnd, QString language, QString userId, QString deliveryRequestId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<  "ReportService::getReportUrl companyName:" << companyName
                                                               << " reportType:" << reportType
                                                               << " accountIds:" << accountIds
                                                               << " tradeDayBegin:" << tradeDayBegin
                                                               << " tradeDayEnd:" << tradeDayEnd
                                                               << " language:" << language
                                                               << " userId:" << userId
                                                               << " deliveryRequestId:" << deliveryRequestId;
		QVector<Common::Parameter> parameters;
		parameters.append(Common::Parameter("companyName",QVariant(companyName)));
		parameters.append(Common::Parameter("reportType",QVariant(reportType)));
		parameters.append(Common::Parameter("accountIds",QVariant(QJsonArray::fromStringList(accountIds))));
		parameters.append(Common::Parameter("tradeDayBegin",QVariant(tradeDayBegin)));
		parameters.append(Common::Parameter("tradeDayEnd",QVariant(tradeDayEnd)));		
		parameters.append(Common::Parameter("language",QVariant(language)));		
		parameters.append(Common::Parameter("userId",QVariant(userId)));		
		parameters.append(Common::Parameter("deliveryRequestId",QVariant(deliveryRequestId)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetReportUrl, parameters, 60*10);
		return QSharedPointer<Common::Result::GetReportUrlResult>(new Common::Result::GetReportUrlResult(result));
	}


    QSharedPointer<Common::Result::GetReportUrlResult> ReportService::GetOrderConfirmationReportUrl(QString companyName, QString language, const QStringList &  orderIds)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<  "ReportService::GetOrderConfirmationReportUrl companyName:" << companyName
                                                               << " language:" << language
                                                               << " orderIds:" << orderIds;
        QVector<Common::Parameter> parameters;

        parameters.append(Common::Parameter("companyName",QVariant(companyName)));
        parameters.append(Common::Parameter("language",QVariant(language)));
        parameters.append(Common::Parameter("orderIds",QVariant(QJsonArray::fromStringList(orderIds))));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetOrderConfirmationReportUrl, parameters, 60*10);
        return QSharedPointer<Common::Result::GetReportUrlResult>(new Common::Result::GetReportUrlResult(result));
    }
}
#include "moc_ReportService.cpp"
