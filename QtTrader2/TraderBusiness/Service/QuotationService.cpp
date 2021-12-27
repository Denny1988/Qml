#include "QuotationService.h"
#include "Common/Enum/StatusEnum.h"
#include "Common/Constants/MethodNameConstants.h"
#include "Common/Util/Convert_Common.h"
#include "Common/LogHelper.h"
#include "Common/CustomerSetting.h"

using namespace Communication;

namespace Service
{
	QuotationService::QuotationService(QSharedPointer<Communication::SslChannel> channel)
	{
		this->_channel = channel;
        //QObject::connect(this->_channel.data(), SIGNAL(quotationReceived(const QString)), this, SIGNAL(quotationReceived(const QString)), Qt::QueuedConnection);
        QObject::connect(this->_channel.data(), SIGNAL(quotationReceived(const QString)), this, SLOT(quotationReceivedSlot(const QString)), Qt::QueuedConnection);
	}

    void QuotationService::quotationReceivedSlot(const QString quotations)
    {
        //qDebug() << "QuotationService::quotationReceivedSlot ThreadId:" << QThread::currentThreadId();
        emit quotationReceived(quotations);
    }

	///------------------Request Method---------------------
    QSharedPointer<Common::Result::InvokeResult> QuotationService::loginQuotationService(QString userID, const QStringList & instrumentAndQuotePolicyIDs, bool isGuest)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<  "QuotationService::loginQuotationService userID:" << userID
                                                                <<" isGuest:" << isGuest
                                                                << " watchword" <<  this->_channel->getWatchword()
                                                                << " instrumentAndQuotePolicyIDs:" << instrumentAndQuotePolicyIDs;
		QVector<Common::Parameter> parameters;
		parameters.append(Common::Parameter("userID", QVariant(userID)));
		parameters.append(Common::Parameter("clientId", QVariant(this->_channel->getClientId())));
		parameters.append(Common::Parameter("watchword", QVariant(this->_channel->getWatchword())));
		parameters.append(Common::Parameter("instrumentAndQuotePolicyIDs", QVariant(QJsonArray::fromStringList(instrumentAndQuotePolicyIDs))));
        parameters.append(Common::Parameter("isGuestIdentity", QVariant(isGuest)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::Login, parameters, 20);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}
			

    QSharedPointer<Common::Result::InvokeResult> QuotationService::logout(bool isGuest)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<  "QuotationService::logout isGuest:"<< isGuest;
		QVector<Common::Parameter> parameters;
		parameters.append(Common::Parameter("clientId", QVariant(this->_channel->getClientId())));
        parameters.append(Common::Parameter("isGuestIdentity", QVariant(isGuest)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::Logout, parameters, 2);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

	///------------------Notify Method----------------------

}
#include "moc_QuotationService.cpp"
