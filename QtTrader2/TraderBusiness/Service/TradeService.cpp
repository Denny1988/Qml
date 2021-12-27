#include "TradeService.h"

#include "Common/Enum/StatusEnum.h"
#include "Common/Constants/MethodNameConstants.h"
#include "Common/Util/Convert_Common.h"
#include "Common/LogHelper.h"
#include "Common/Constants/LogKeyConstants.h"
#include "Common/CustomerSetting.h"
#include "Common/LoginSetting.h"
#include "Common/TraderBaseInfo.h"

using namespace Communication;

namespace Service
{
	TradeService::TradeService(QSharedPointer<Communication::SslChannel> channel)
	{
		this->_channel = channel;
        QObject::connect(this->_channel.data(), SIGNAL(notifyReceived(const QDomElement&)), this, SLOT(notifyReceivedSlot(const QDomElement&)));
		timeout = 10;
	}

    void TradeService::notifyReceivedSlot(const QDomElement& xmlNotify)
    {
        qDebug() << "TradeService::notifyReceivedSlot ThreadId:" << QThread::currentThreadId();
        emit notifyReceived(xmlNotify);
    }

	///------------------Request Method---------------------
    QSharedPointer<Common::Result::LoginResult> TradeService::login(QString loginID, QString password, QString language, QString validPathName, QString descriptionInfo, bool isMobileTrader, bool isGuest, QString versionName, QString deviceName)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::login "
                                                              << " loginID:"<< loginID
                                                              << " language:"<< language
                                                              << " validPathName:"<< validPathName
                                                              << " isMobileTrader:"<< isMobileTrader
                                                              << " isGuest:"<< isGuest
                                                              << " versionName:" << versionName
                                                              << " deviceName:" << deviceName;
		QVector<Common::Parameter> parameters;
		parameters.append(Common::Parameter("loginID",QVariant(loginID)));
		parameters.append(Common::Parameter("password",QVariant(password)));
		parameters.append(Common::Parameter("language",QVariant(language)));
        parameters.append(Common::Parameter("appType",QVariant(isMobileTrader ? Common::Constants::RequestConstants::MobileAppType : Common::Constants::RequestConstants::AppType)));
		parameters.append(Common::Parameter("validPathName",QVariant(validPathName)));	
        parameters.append(Common::Parameter("descriptionInfo",QVariant(descriptionInfo)));
		parameters.append(Common::Parameter("containsMilSInQuotationTime",QVariant(Common::AppSetting::getInstance()->getContainsMilSInQuotationTime())));
		if(Common::LoginSetting::getInstance()->getUseTraderQuotationServer())
            parameters.append(Common::Parameter("sendQuotationInSsl", QVariant(false)));
        parameters.append(Common::Parameter("isGuestIdentity", QVariant(isGuest)));
        parameters.append(Common::Parameter("versionName", QVariant(versionName)));
        parameters.append(Common::Parameter("deviceName", QVariant(deviceName)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::Login, parameters, 60);
		return QSharedPointer<Common::Result::LoginResult>(new Common::Result::LoginResult(result));
	}

    QSharedPointer<Common::Result::LoginResult> TradeService::loginByWeChat(QString loginID, QString password, QString language, QString validPathName, QString templateCode)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::loginByWeChat "
                                                              << " loginID:"<< loginID
                                                              << " language:"<< language
                                                              << " validPathName:"<< validPathName
                                                              << " templateCode:"<< templateCode;
        QVector<Common::Parameter> parameters;

        parameters.append(Common::Parameter("loginID",QVariant(loginID)));
        parameters.append(Common::Parameter("password",QVariant(password)));
        parameters.append(Common::Parameter("language",QVariant(language)));
        parameters.append(Common::Parameter("appType",QVariant(Common::Constants::RequestConstants::AppType)));
        parameters.append(Common::Parameter("validPathName",QVariant(validPathName)));
        parameters.append(Common::Parameter("containsMilSInQuotationTime",QVariant(Common::AppSetting::getInstance()->getContainsMilSInQuotationTime())));
        if(Common::LoginSetting::getInstance()->getUseTraderQuotationServer())
            parameters.append(Common::Parameter("sendQuotationInSsl", QVariant(false)));
        parameters.append(Common::Parameter("loginType",QVariant(QString("WeChat"))));
        parameters.append(Common::Parameter("templateCode",QVariant(templateCode)));

        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::Login, parameters, 30);
        return QSharedPointer<Common::Result::LoginResult>(new Common::Result::LoginResult(result));
    }

    QSharedPointer<Common::Result::InvokeResult> TradeService::logout(bool isGuest)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::logout isGuest:" << isGuest;
		QVector<Common::Parameter> parameters;
        parameters.append(Common::Parameter("isGuestIdentity", QVariant(isGuest)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::Logout, parameters, 2);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

	QSharedPointer<Common::Result::InvokeResult> TradeService::activateAccount(QString loginID, QString oldPassword, QString newPassword, QString recoverPasswordDataXml)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::activateAccount loginID:"<<loginID;
		QVector<Common::Parameter> parameters;
		parameters.append(Common::Parameter("loginID",QVariant(loginID)));
		parameters.append(Common::Parameter("oldPassword",QVariant(oldPassword)));
		parameters.append(Common::Parameter("newPassword",QVariant(newPassword)));
		parameters.append(Common::Parameter("recoverPasswordDatas",QVariant(recoverPasswordDataXml)));	
		parameters.append(Common::Parameter("maxPasswordReserveCount", QVariant(Common::TraderBaseInfo::getInstance()->getMaxPasswordReserveCount())));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::ActivateAccount, parameters, timeout);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

    QSharedPointer<Common::Result::InvokeResult> TradeService::resetPassword(QString userOpenId, QString authType, QString newPassword)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::resetPassword userOpenId:" <<userOpenId << " authType:" <<authType;
        QVector<Common::Parameter> parameters;
        parameters.append(Common::Parameter("userOpenId",QVariant(userOpenId)));
        parameters.append(Common::Parameter("authType",QVariant(authType)));
        parameters.append(Common::Parameter("newPassword",QVariant(newPassword)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::ResetPassword, parameters, timeout);
        return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
    }

	QSharedPointer<Common::Result::XmlDataResult> TradeService::getInitData()
	{
		QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getInitData";
		QVector<Common::Parameter> parameters;		
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetInitData, parameters, 60, true);		
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
	}

	QSharedPointer<Common::Result::XmlDataResult> TradeService::getSettingData()
	{
		QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getSettingData";
		QVector<Common::Parameter> parameters;		
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetSettingData, parameters, 120, true);
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
	}

	QSharedPointer<Common::Result::XmlDataResult> TradeService::getTradingData(const QStringList & accountIds)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getTradingData accountIds:" << accountIds;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("ids",QVariant(QJsonArray::fromStringList(accountIds))));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetTradingData, parameters, 120, true);
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
	}

	QSharedPointer<Common::Result::PlaceResult> TradeService::beginPlace(QString tranXmlString, QUuid id)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::beginPlace id:" << id << " tranXmlString:" << tranXmlString;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("transactionXml",QVariant(tranXmlString)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::Place, parameters, 60);
		return QSharedPointer<Common::Result::PlaceResult>(new Common::Result::PlaceResult(result, id));
	}

	QSharedPointer<Common::Result::InvokeResult> TradeService::multipleClose(const QStringList &  orderIds)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::multipleClose orderIds:" << orderIds;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("orderIDs",QVariant(QJsonArray::fromStringList(orderIds))));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::MultipleClose, parameters, timeout);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

	QSharedPointer<Common::Result::QuoteResult> TradeService::requestQuote(QString instrumentId, double quoteLot, int status, int setPriceMaxMovePips, int lifeTime)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::requestQuote instrumentId:" << instrumentId
                                                              << " quoteLot:" << quoteLot
                                                              << " status:" << status
                                                              << " setPriceMaxMovePips:" << setPriceMaxMovePips
                                                              << " lifeTime:" << lifeTime;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("instrumentID",instrumentId));
		parameters.append(Common::Parameter("quoteLot",QVariant(quoteLot)));
		parameters.append(Common::Parameter("bsStatus",QVariant(status)));
		parameters.append(Common::Parameter("setPriceMaxMovePips",QVariant(setPriceMaxMovePips)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::Quote, parameters, lifeTime);
		return QSharedPointer<Common::Result::QuoteResult>(new Common::Result::QuoteResult(result));
	}

	QSharedPointer<Common::Result::XmlDataResult> TradeService::getDeliveryAddresses(QString deliveryPointGroupId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getDeliveryAddresses deliveryPointGroupId:" << deliveryPointGroupId;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("deliveryPointGroupId", deliveryPointGroupId));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetDeliveryAddresses, parameters, timeout);
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
	}


	QSharedPointer<Common::Result::PlaceResult> TradeService::applyDelivery(QString deliveryXmlString)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::applyDelivery deliveryXmlString:" << deliveryXmlString;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("deliveryRequire", deliveryXmlString));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::ApplyDelivery, parameters, 60);
		return QSharedPointer<Common::Result::PlaceResult>(new Common::Result::PlaceResult(result));

	}

	QSharedPointer<Common::Result::PlaceResult> TradeService::instalmentPayoff(QString accountId, QString currencyId, QString sumSourcePaymentAmount, QString sumSourceTerminateFee, QString terminateXml)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::instalmentPayoff accountId:" << accountId
                                                              << " currencyId:" <<currencyId
                                                              << " sumSourceTerminateFee:" <<sumSourceTerminateFee
                                                              << " terminateXml:" << terminateXml;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("accountID", accountId));
		parameters.append(Common::Parameter("currencyID", currencyId));
		parameters.append(Common::Parameter("sumSourcePaymentAmount", sumSourcePaymentAmount));
		parameters.append(Common::Parameter("sumSourceTerminateFee", sumSourceTerminateFee));
		parameters.append(Common::Parameter("instalmentXml", ""));
		parameters.append(Common::Parameter("terminateXml", terminateXml));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::InstalmentPayoff, parameters, timeout);
		return QSharedPointer<Common::Result::PlaceResult>(new Common::Result::PlaceResult(result));
	}

	QSharedPointer<Common::Result::InvokeResult> TradeService::changeMarginPin(QString accountId, QString oldPassword, QString newPassword)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::changeMarginPin accountId:" << accountId;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("accountID", accountId));
		parameters.append(Common::Parameter("oldPassword", oldPassword));
		parameters.append(Common::Parameter("newPassword", newPassword));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::ChangeMarginPin, parameters, timeout);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

	QSharedPointer<Common::Result::InvokeResult> TradeService::modifyTelephoneIdentificationCode(QString accountId, QString oldCode, QString newCode, bool isReset)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::modifyTelephoneIdentificationCode accountId:" << accountId << " isReset:" << isReset;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("accountID", accountId));
		parameters.append(Common::Parameter("oldCode", oldCode));
		parameters.append(Common::Parameter("newCode", newCode));
		parameters.append(Common::Parameter("isReset", isReset));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::ModifyTelephoneIdentificationCode, parameters, timeout);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}


	QSharedPointer<Common::Result::InvokeResult> TradeService::recoverPasswordDatas(QString recoverPasswordDataXml)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::recoverPasswordDatas recoverPasswordDataXml:" << recoverPasswordDataXml;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("recoverPasswordDatas", recoverPasswordDataXml));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::RecoverPasswordDatas, parameters, timeout);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

	QSharedPointer<Common::Result::XmlDataResult> TradeService::getRecoverPasswordDatas(QString userId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getRecoverPasswordDatas userId:" << userId;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("userId",userId));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetRecoverPasswordData, parameters, timeout);
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
	}
	
	QSharedPointer<Common::Result::PlaceResult> TradeService::cancelOrder(QString accountId, QString transactionId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::cancelOrder accountId:" << accountId << " transactionId:" << transactionId;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("transactionID", transactionId));
		parameters.append(Common::Parameter("accountID", accountId));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::CancelOrder, parameters, timeout);
		return QSharedPointer<Common::Result::PlaceResult>(new Common::Result::PlaceResult(result));
	}

	QSharedPointer<Common::Result::XmlDataResult> TradeService::getAccountsForSetting()
	{
		QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getAccountsForSetting";
		QVector<Common::Parameter> parameters;
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetAccountsForSetting, parameters, timeout);
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
	}

	QSharedPointer<Common::Result::XmlDataResult> TradeService::getMarketingGroupForSetting()
	{
		QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getMarketingGroupForSetting";
		QVector<Common::Parameter> parameters;
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetAccountForMarketingGroup, parameters, timeout);
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
	}

	QSharedPointer<Common::Result::XmlDataResult> TradeService::updateAccountsSetting(const QStringList & accountIDs)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::UpdateAccountsSetting accountIDs:" << accountIDs;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("accountIDs", QVariant(QJsonArray::fromStringList(accountIDs))));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::UpdateAccountsSetting, parameters, 120);
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
	}

	QSharedPointer<Common::Result::XmlDataResult> TradeService::updateInstrumentsSetting(const QStringList &  instrumentIDs)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::updateInstrumentsSetting instrumentIDs:" << instrumentIDs;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("instrumentIDs", QVariant(QJsonArray::fromStringList(instrumentIDs))));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::UpdateInstrumentsSetting, parameters, 120);
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
	}

	QSharedPointer<Common::Result::XmlDataResult> TradeService::getInstrumentForSetting()
	{
		QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getInstrumentForSetting";
		QVector<Common::Parameter> parameters;
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetInstrumentForSetting, parameters, timeout);
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
	}

	QSharedPointer<Common::Result::ChartXmlDataResult> TradeService::getQuotePolicies(QString instrumentId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getQuotePolicies instrumentId:" << instrumentId;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("instrumentId",QVariant(instrumentId)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetQuotePolicies, parameters, timeout);
		return QSharedPointer<Common::Result::ChartXmlDataResult>(new Common::Result::ChartXmlDataResult(result));
	}

	QSharedPointer<Common::Result::InvokeResult> TradeService::changeLeverage(QString accountId, QString leverage)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::changeLeverage accountId:" << accountId << " leverage:" << leverage;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("accountID",QVariant(accountId)));
		parameters.append(Common::Parameter("leverage",QVariant(leverage)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::ChangeLeverage, parameters, timeout);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

    QSharedPointer<Common::Result::XmlDataResult> TradeService::getAccountBanksApproved(QString accountId, QString language, bool isSendBankCardPhoto)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getAccountBanksApproved accountId:" << accountId << " language:"<< language << " isSendBankCardPhoto:" << isSendBankCardPhoto;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("accountID",QVariant(accountId)));
		parameters.append(Common::Parameter("language",QVariant(language)));
        parameters.append(Common::Parameter("isSendBankCardPhoto",QVariant(isSendBankCardPhoto)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetAccountBanksApproved, parameters, timeout);
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result, QUuid(accountId)));
	}

    QSharedPointer<Common::Result::XmlDataResult> TradeService::getAccountBankReferenceData(QString countryId, QString language, QString customerId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getAccountBankReferenceData countryId:" << countryId << " language:" << language<< " customerId:" << customerId;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("countryID",QVariant(countryId)));
		parameters.append(Common::Parameter("language",QVariant(language)));
        parameters.append(Common::Parameter("customerId",QVariant(customerId)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetAccountBankReferenceData, parameters, timeout);
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
	}

	QSharedPointer<Common::Result::AddMarginResult> TradeService::additionalClient(QString email, QString receive, QString organizationName, QString customerName, 
				QString reportDate, QString accountCode, QString correspondingAddress, QString registratedEmailAddress, 
				QString tel, QString mobile, QString fax, QString fillName1,
                QString ICNo1, QString fillName2, QString ICNo2, QString fillName3, QString ICNo3, QString accountId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::AdditionalClient email:" << email
                                                              << " receive:" <<receive
                                                              << " organizationName:" << organizationName
                                                              << " customerName:" << customerName
                                                              << " reportDate:" << reportDate
                                                              << " accountCode:" << accountCode
                                                              << " correspondingAddress:" << correspondingAddress
                                                              << " registratedEmailAddress:" << registratedEmailAddress
                                                              << " tel:" << tel
                                                              << " mobile:" << mobile
                                                              << " fax:" << fax
                                                              << " fillName1:" << fillName1
                                                              << " ICNo1:" << ICNo1
                                                              << " fillName2:" << fillName2
                                                              << " ICNo2:" << ICNo2
                                                              << " fillName3:" << fillName3
                                                              << " ICNo3:" << ICNo3
                                                              << " accountId:" << accountId;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("email",QVariant(email)));
		parameters.append(Common::Parameter("receive",QVariant(receive)));
		parameters.append(Common::Parameter("organizationName",QVariant(organizationName)));
		parameters.append(Common::Parameter("customerName",QVariant(customerName)));
		parameters.append(Common::Parameter("reportDate",QVariant(reportDate)));
		parameters.append(Common::Parameter("accountCode",QVariant(accountCode)));
		parameters.append(Common::Parameter("correspondingAddress",QVariant(correspondingAddress)));
		parameters.append(Common::Parameter("registratedEmailAddress",QVariant(registratedEmailAddress)));
		parameters.append(Common::Parameter("tel",QVariant(tel)));
		parameters.append(Common::Parameter("mobile",QVariant(mobile)));
		parameters.append(Common::Parameter("fax",QVariant(fax)));
		parameters.append(Common::Parameter("fillName1",QVariant(fillName1)));
		parameters.append(Common::Parameter("ICNo1",QVariant(ICNo1)));
		parameters.append(Common::Parameter("fillName2",QVariant(fillName2)));
		parameters.append(Common::Parameter("ICNo2",QVariant(ICNo2)));
		parameters.append(Common::Parameter("fillName3",QVariant(fillName3)));
		parameters.append(Common::Parameter("ICNo3",QVariant(ICNo3)));
        parameters.append(Common::Parameter("accountId", QVariant(accountId)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::AdditionalClient, parameters, timeout);
		return QSharedPointer<Common::Result::AddMarginResult>(new Common::Result::AddMarginResult(result));
	}

	QSharedPointer<Common::Result::AddMarginResult> TradeService::marginApply(QString id, QString accountBankApprovedId, QString accountID, QString countryID, 
		QString bankID, QString bankName, QString accountBankNo, QString accountBankType, 
		QString accountOpener, QString accountBankProp, QString accountBankBCID, QString accountBankBCName,
        QString idType, QString idNo, QString bankProvinceID, QString bankCityID, QString bankAddress, QString swiftCode, QString applicationType, QString accountId, QString bankCardPhotoId, QString language)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::apply id:" << id
                                                                << " accountBankApprovedId:" << accountBankApprovedId
                                                                << " accountID:" << accountID
                                                                << " countryID:" << countryID
                                                                << " bankID:" << bankID
                                                                << " bankName:" << bankName
                                                                << " accountBankNo:" << accountBankNo
                                                                << " accountBankType:" << accountBankType
                                                                << " accountOpener:" << accountOpener
                                                                << " accountBankProp:" << accountBankProp
                                                                << " accountBankBCID:" << accountBankBCID
                                                                << " accountBankBCName:" << accountBankBCName
                                                                << " idType:" << idType
                                                                << " idNo:" << idNo
                                                                << " bankProvinceID:" << bankProvinceID
                                                                << " bankCityID:" << bankCityID
                                                                << " bankAddress:" << bankAddress
                                                                << " swiftCode:" << swiftCode
                                                                << " applicationType:" << applicationType
                                                                << " accountId:" << accountId
                                                                << " bankCardPhotoId:" << bankCardPhotoId
                                                                << " language:" << language;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("id",QVariant(id)));
		parameters.append(Common::Parameter("accountBankApprovedId",QVariant(accountBankApprovedId)));
		parameters.append(Common::Parameter("accountID",QVariant(accountID)));
		parameters.append(Common::Parameter("countryID",QVariant(countryID)));
		parameters.append(Common::Parameter("bankID",QVariant(bankID)));
		parameters.append(Common::Parameter("bankName",QVariant(bankName)));
		parameters.append(Common::Parameter("accountBankNo",QVariant(accountBankNo)));
		parameters.append(Common::Parameter("accountBankType",QVariant(accountBankType)));
		parameters.append(Common::Parameter("accountOpener",QVariant(accountOpener)));
		parameters.append(Common::Parameter("accountBankProp",QVariant(accountBankProp)));
		parameters.append(Common::Parameter("accountBankBCID",QVariant(accountBankBCID)));
		parameters.append(Common::Parameter("accountBankBCName",QVariant(accountBankBCName)));
		parameters.append(Common::Parameter("idType",QVariant(idType)));
		parameters.append(Common::Parameter("idNo",QVariant(idNo)));
		parameters.append(Common::Parameter("bankProvinceID",QVariant(bankProvinceID)));
		parameters.append(Common::Parameter("bankCityID",QVariant(bankCityID)));
		parameters.append(Common::Parameter("bankAddress",QVariant(bankAddress)));
		parameters.append(Common::Parameter("swiftCode",QVariant(swiftCode)));
		parameters.append(Common::Parameter("applicationType",QVariant(applicationType)));
        parameters.append(Common::Parameter("accountId", QVariant(accountId)));
        parameters.append(Common::Parameter("bankCardPhotoId", QVariant(bankCardPhotoId)));
        parameters.append(Common::Parameter("language", QVariant(language)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::MarginApply, parameters, timeout);
		return QSharedPointer<Common::Result::AddMarginResult>(new Common::Result::AddMarginResult(result));
	}

	QSharedPointer<Common::Result::AddMarginResult> TradeService::marginAgent(QString email, QString receive, QString organizationName, QString customerName, 
		QString reportDate, QString accountCode, QString previousAgentCode, QString previousAgentName, 
        QString newAgentCode, QString newAgentName, QString newAgentICNo, QString dateReply, QString accountId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::Agent email:" << email
                                                               << " receive:" << receive
                                                               << " organizationName:" << organizationName
                                                               << " customerName:" << customerName
                                                               << " reportDate:" << reportDate
                                                               << " accountCode:" << accountCode
                                                               << " previousAgentCode:" << previousAgentCode
                                                               << " previousAgentName:" << previousAgentName
                                                               << " newAgentCode:" << newAgentCode
                                                               << " newAgentName:" << newAgentName
                                                               << " newAgentICNo:" << newAgentICNo
                                                               << " dateReply:" << dateReply
                                                               << " accountId:" << accountId;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("email",QVariant(email)));
		parameters.append(Common::Parameter("receive",QVariant(receive)));
		parameters.append(Common::Parameter("organizationName",QVariant(organizationName)));
		parameters.append(Common::Parameter("customerName",QVariant(customerName)));
		parameters.append(Common::Parameter("reportDate",QVariant(reportDate)));
		parameters.append(Common::Parameter("accountCode",QVariant(accountCode)));
		parameters.append(Common::Parameter("previousAgentCode",QVariant(previousAgentCode)));
		parameters.append(Common::Parameter("previousAgentName",QVariant(previousAgentName)));
		parameters.append(Common::Parameter("newAgentCode",QVariant(newAgentCode)));
		parameters.append(Common::Parameter("newAgentName",QVariant(newAgentName)));
		parameters.append(Common::Parameter("newAgentICNo",QVariant(newAgentICNo)));
		parameters.append(Common::Parameter("dateReply",QVariant(dateReply)));
        parameters.append(Common::Parameter("accountId", QVariant(accountId)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::MarginAgent, parameters, timeout);
		return QSharedPointer<Common::Result::AddMarginResult>(new Common::Result::AddMarginResult(result));
	}

	QSharedPointer<Common::Result::AddMarginResult> TradeService::callMarginExtension(QString email, QString receive, QString organizationName, QString customerName, 
        QString reportDate, QString accountCode, QString currency, QString currencyValue, QString dueDate, QString accountId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::CallMarginExtension email:"<<email
                                                              << " receive:" << receive
                                                              << " organizationName:" << organizationName
                                                              << " customerName:" << customerName
                                                              << " accountCode:" << accountCode
                                                              << " reportDate:" << reportDate
                                                              << " currency:" << currency
                                                              << " currencyValue:" << currencyValue
                                                              << " dueDate:" << dueDate
                                                              << " accountId:" << accountId;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("email",QVariant(email)));
		parameters.append(Common::Parameter("receive",QVariant(receive)));
		parameters.append(Common::Parameter("organizationName",QVariant(organizationName)));
		parameters.append(Common::Parameter("customerName",QVariant(customerName)));
		parameters.append(Common::Parameter("reportDate",QVariant(reportDate)));
		parameters.append(Common::Parameter("accountCode",QVariant(accountCode)));
		parameters.append(Common::Parameter("currency",QVariant(currency)));
		parameters.append(Common::Parameter("currencyValue",QVariant(currencyValue)));
		parameters.append(Common::Parameter("dueDate",QVariant(dueDate)));
        parameters.append(Common::Parameter("accountId", QVariant(accountId)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::CallMarginExtension, parameters, timeout);
		return QSharedPointer<Common::Result::AddMarginResult>(new Common::Result::AddMarginResult(result));
	}

	QSharedPointer<Common::Result::AddMarginResult> TradeService::paymentInstruction(QString email, QString receive, QString organizationName, QString customerName, 
		QString reportDate, QString accountCode, QString currency, QString currencyValue, QString beneficiaryName,
        QString bankAccount, QString bankerName,QString bankerAddress,QString swiftCode,QString remarks,QString thisisClien, QString accountBankApprovedId, QString traderLanguage, QString accountId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::PaymentInstruction email:"<<email
                                                              << " receive:" << receive
                                                              << " organizationName:" << organizationName
                                                              << " customerName:" << customerName
                                                              << " reportDate:" << reportDate
                                                              << " accountCode:" << accountCode
                                                              << " currency:" << currency
                                                              << " currencyValue:" << currencyValue
                                                              << " beneficiaryName:" << beneficiaryName
                                                              << " bankAccount:" << bankAccount
                                                              << " bankerName:" << bankerName
                                                              << " bankAccount:" << bankerAddress
                                                              << " bankerName:" << swiftCode
                                                              << " bankAccount:" << remarks
                                                              << " bankerName:" << thisisClien
                                                              << " accountBankApprovedId:" << accountBankApprovedId
                                                              << " traderLanguage:" << traderLanguage
                                                              << " accountId:" << accountId;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("email",QVariant(email)));
		parameters.append(Common::Parameter("receive",QVariant(receive)));
		parameters.append(Common::Parameter("organizationName",QVariant(organizationName)));
		parameters.append(Common::Parameter("customerName",QVariant(customerName)));
		parameters.append(Common::Parameter("reportDate",QVariant(reportDate)));
		parameters.append(Common::Parameter("accountCode",QVariant(accountCode)));
		parameters.append(Common::Parameter("currency",QVariant(currency)));
		parameters.append(Common::Parameter("currencyValue",QVariant(currencyValue)));
		parameters.append(Common::Parameter("beneficiaryName",QVariant(beneficiaryName)));
		parameters.append(Common::Parameter("bankAccount",QVariant(bankAccount)));
		parameters.append(Common::Parameter("bankerName",QVariant(bankerName)));
		parameters.append(Common::Parameter("bankerAddress",QVariant(bankerAddress)));
		parameters.append(Common::Parameter("swiftCode",QVariant(swiftCode)));
		parameters.append(Common::Parameter("remarks",QVariant(remarks)));
		parameters.append(Common::Parameter("thisisClien",QVariant(thisisClien)));
		parameters.append(Common::Parameter("accountBankApprovedId", QVariant(accountBankApprovedId)));
        parameters.append(Common::Parameter("traderLanguage", QVariant(traderLanguage)));
        parameters.append(Common::Parameter("accountId", QVariant(accountId)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::PaymentInstruction, parameters, 120);
		return QSharedPointer<Common::Result::AddMarginResult>(new Common::Result::AddMarginResult(result));
	}

	QSharedPointer<Common::Result::AddMarginResult> TradeService::paymentInstructionInternal(QString email,QString organizationName, QString customerName, 
		QString reportDate, QString accountCode, QString currency, QString amount, QString beneficiaryAccount,
        QString beneficiaryAccountOwner, QString email2, QString traderLanguage, QString accountId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::PaymentInstructionInternal email:" << email
                                                              << " organizationName:" << organizationName
                                                              << " customerName:" << customerName
                                                              << " reportDate:" << reportDate
                                                              << " accountCode:" << accountCode
                                                              << " currency:" << currency
                                                              << " amount:" << amount
                                                              << " beneficiaryAccount:" << beneficiaryAccount
                                                              << " beneficiaryAccountOwner:" << beneficiaryAccountOwner
                                                              << " email2:" << email2
                                                              << " traderLanguage:" << traderLanguage
                                                              << " accountId:" << accountId;

		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("email",QVariant(email)));
		parameters.append(Common::Parameter("organizationName",QVariant(organizationName)));
		parameters.append(Common::Parameter("customerName",QVariant(customerName)));
		parameters.append(Common::Parameter("reportDate",QVariant(reportDate)));
		parameters.append(Common::Parameter("accountCode",QVariant(accountCode)));
		parameters.append(Common::Parameter("currency",QVariant(currency)));
		parameters.append(Common::Parameter("amount",QVariant(amount)));
		parameters.append(Common::Parameter("beneficiaryAccount",QVariant(beneficiaryAccount)));
		parameters.append(Common::Parameter("beneficiaryAccountOwner",QVariant(beneficiaryAccountOwner)));
		parameters.append(Common::Parameter("email2",QVariant(email2)));
        parameters.append(Common::Parameter("traderLanguage", QVariant(traderLanguage)));
        parameters.append(Common::Parameter("accountId", QVariant(accountId)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::PaymentInstructionInternal, parameters, 120);
		return QSharedPointer<Common::Result::AddMarginResult>(new Common::Result::AddMarginResult(result));
	}
		
	QSharedPointer<Common::Result::AddMarginResult> TradeService::paymentInstructionCash(QString email,QString organizationName, QString customerName, 
		QString reportDate, QString accountCode, QString currency, QString amount, QString beneficiaryName,
        QString beneficiaryAddress, QString traderLanguage, QString accountId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::PaymentInstructionCash email:" << email
                                                              << " organizationName:" << organizationName
                                                              << " customerName:" << customerName
                                                              << " reportDate:" << reportDate
                                                              << " accountCode:" << accountCode
                                                              << " currency:" << currency
                                                              << " amount:" << amount
                                                              << " beneficiaryName:" << beneficiaryName
                                                              << " beneficiaryAddress:" << beneficiaryAddress
                                                              << " traderLanguage:" << traderLanguage
                                                              << " accountId:" << accountId;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("email",QVariant(email)));
		parameters.append(Common::Parameter("organizationName",QVariant(organizationName)));
		parameters.append(Common::Parameter("customerName",QVariant(customerName)));
		parameters.append(Common::Parameter("reportDate",QVariant(reportDate)));
		parameters.append(Common::Parameter("accountCode",QVariant(accountCode)));
		parameters.append(Common::Parameter("currency",QVariant(currency)));
		parameters.append(Common::Parameter("amount",QVariant(amount)));
		parameters.append(Common::Parameter("beneficiaryName",QVariant(beneficiaryName)));
		parameters.append(Common::Parameter("beneficiaryAddress",QVariant(beneficiaryAddress)));
        parameters.append(Common::Parameter("traderLanguage", QVariant(traderLanguage)));
        parameters.append(Common::Parameter("accountId", QVariant(accountId)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::PaymentInstructionCash, parameters, 120);
		return QSharedPointer<Common::Result::AddMarginResult>(new Common::Result::AddMarginResult(result));
	}

	QSharedPointer<Common::Result::InvokeResult> TradeService::verifyMarginPin(QString accountId, QString password)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::VerifyMarginPin accountId:" << accountId;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("accountID",QVariant(accountId)));
		parameters.append(Common::Parameter("password",QVariant(password)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::VerifyMarginPin, parameters, timeout);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

	QSharedPointer<Common::Result::XmlDataResult> TradeService::orderQuery(QString selectedAccountId, QString selectedInstrumentId, int lastDays, int orderQueryType)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::orderQuery selectedAccountId:" << selectedAccountId
                                                               << " selectedInstrumentId:" <<selectedInstrumentId
                                                               << " lastDays:" << lastDays
                                                               << " orderQueryType:" << orderQueryType;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("accountID",QVariant(selectedAccountId)));
		parameters.append(Common::Parameter("instrumentID",QVariant(selectedInstrumentId)));
		parameters.append(Common::Parameter("lastDays",QVariant(lastDays)));
		parameters.append(Common::Parameter("orderType",QVariant(orderQueryType)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::OrderQuery, parameters, 120);
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
	}

	QSharedPointer<Common::Result::InvokeResult> TradeService::deleteMessage(QString id)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::deleteMessage id:" << id;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("id",QVariant(id)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::DeleteMessage, parameters, timeout);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}
			
	QSharedPointer<Common::Result::InvokeResult> TradeService::getNewsContents(QString id)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getNewsContents id:" << id;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("newsID",QVariant(id)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetNewsContents, parameters, timeout);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

	QSharedPointer<Common::Result::UpdatePriceAlertResult> TradeService::updatePriceAlerts(QUuid id, QString priceAlertRequestXml)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::updatePriceAlerts id:" << id << " priceAlertRequestXml:" << priceAlertRequestXml;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("priceAlertRequestXml",QVariant(priceAlertRequestXml)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::UpdatePriceAlerts, parameters, timeout);
		return QSharedPointer<Common::Result::UpdatePriceAlertResult>(new Common::Result::UpdatePriceAlertResult(result, id));
	}

    QSharedPointer<Common::Result::InvokeResult> TradeService::updatePriceAlertState(const QStringList& ids)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::updatePriceAlertState ids:" << ids;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("ids", QVariant(QJsonArray::fromStringList(ids))));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::UpdatePriceAlertState, parameters, timeout);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

	QSharedPointer<Common::Result::XmlDataResult> TradeService::getLogData(QString id, QString language)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getLogData id:"<< id << " language:" << language;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("id",QVariant(id)));
		parameters.append(Common::Parameter("language",QVariant(language)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetLogData, parameters, timeout);
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result, QUuid(id)));
	}

	QSharedPointer<Common::Result::InvokeResult> TradeService::saveIsSptReconfirm(bool isSptReconfirm)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::SaveIsSptReconfirm isSptReconfirm:" << isSptReconfirm;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("isSptReconfirm", QVariant(isSptReconfirm)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::SaveIsSptReconfirm, parameters, timeout);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

	QSharedPointer<Common::Result::XmlDataResult> TradeService::verifyTransaction(const QStringList &  transactionIds)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::verifyTransaction transactionIds:" << transactionIds;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("transactionIDs",QVariant(QJsonArray::fromStringList(transactionIds))));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::VerifyTransaction, parameters, timeout);
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
	}

	QSharedPointer<Common::Result::InvokeResult> TradeService::setClickTradeLog(QString instrumentId,  int disclaimerOption)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::SetClickTradeLog instrumentId:" << instrumentId << " disclaimerOption:" << disclaimerOption;
		QVector<Common::Parameter> parameters;		
		parameters.append(Common::Parameter("instrumentID", QVariant(instrumentId)));
		parameters.append(Common::Parameter("disclaimerOption", QVariant(disclaimerOption)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::SetClickTradeLog, parameters, timeout);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

	QSharedPointer<Common::Result::InvokeResult> TradeService::getServerDateTime()
	{
		QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getServerDateTime";
		QVector<Common::Parameter> parameters;
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetServerDateTime, parameters, timeout);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

	QSharedPointer<Common::Result::InvokeResult> TradeService::resetAlertLevel(const QStringList & accountIds, const QString& accountCodes)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::resetAlertLevel accountCodes:" << accountCodes << " accountIds:" << accountIds;
		QVector<Common::Parameter> parameters;
		parameters.append(Common::Parameter("ids", QVariant(QJsonArray::fromStringList(accountIds))));
		parameters.append(Common::Parameter("accountCodes", QVariant(accountCodes)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::ResetAlertLevel, parameters, timeout);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

	QSharedPointer<Common::Result::InvokeResult> TradeService::signAgreement(const QStringList & accountIds, const int & counterPartyId, const QString& instrumentId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::signAgreement counterPartyId:" << counterPartyId << " instrumentId:" << instrumentId << " accountIds:" << accountIds;
		QVector<Common::Parameter> parameters;
		parameters.append(Common::Parameter("accountIds", QVariant(QJsonArray::fromStringList(accountIds))));
		parameters.append(Common::Parameter("counterPartyId", QVariant(counterPartyId)));
		parameters.append(Common::Parameter("instrumentId", QVariant(instrumentId)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::SignAgreement, parameters, timeout);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

	QSharedPointer<Common::Result::XmlDataResult> TradeService::getTransactionXml(QString tranXmlString)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getTransactionXml tranXmlString:" << tranXmlString;
		QVector<Common::Parameter> parameters;
		parameters.append(Common::Parameter("transactionXml", QVariant(tranXmlString)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetTransactionXml, parameters, timeout);
		return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
	}

	QSharedPointer<Common::Result::InvokeResult> TradeService::getWatchWorlds(QString clientId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getWatchWorlds clientId:" << clientId;
		QVector<Common::Parameter> parameters;
		parameters.append(Common::Parameter("count", QVariant(Common::AppSetting::getInstance()->getWatchWorlds())));
		parameters.append(Common::Parameter("clientId", QVariant(clientId)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetWatchWorlds, parameters, 10);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}
	QSharedPointer<Common::Result::GetVerificationCodeResult> TradeService::getVerificationCode(QString language, QString mobilePhone)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getVerificationCode language:" << language << " mobilePhone:" << mobilePhone;
		QVector<Common::Parameter> parameters;
		parameters.append(Common::Parameter("language", QVariant(language)));
		parameters.append(Common::Parameter("mobilePhone", QVariant(mobilePhone)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetVerificationCode, parameters, timeout);
		return QSharedPointer<Common::Result::GetVerificationCodeResult>(new Common::Result::GetVerificationCodeResult(result));
	}

	QSharedPointer<Common::Result::InvokeResult> TradeService::updateMessageState(QString messageId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::updateMessageState messageId:" << messageId;
		QVector<Common::Parameter> parameters;
		parameters.append(Common::Parameter("messageId", QVariant(messageId)));
		QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::UpdateMessageState, parameters, 10);
		return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
	}

    QSharedPointer<Common::Result::PlaceResult> TradeService::applyOfficialFixing(QString accountId, QString orderId, QString settleDate)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::applyOfficialFixing accountId:" << accountId << " orderId:" << orderId << " settleDate:" << settleDate;
        QVector<Common::Parameter> parameters;
        parameters.append(Common::Parameter("accountId",QVariant(accountId)));
        parameters.append(Common::Parameter("orderId",QVariant(orderId)));
        parameters.append(Common::Parameter("settleDate",QVariant(settleDate)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::ApplyOfficialFixing, parameters, 60);
        return QSharedPointer<Common::Result::PlaceResult>(new Common::Result::PlaceResult(result));
    }

    QSharedPointer<Common::Result::InvokeResult> TradeService::bindingWeChat(QString appOpenId, QString accessCode)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::BindingWeChat appOpenId:" << appOpenId << " accessCode:" << accessCode;
        QVector<Common::Parameter> parameters;
        parameters.append(Common::Parameter("appOpenId", QVariant(appOpenId)));
        parameters.append(Common::Parameter("accessCode", QVariant(accessCode)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::BindingWeChat, parameters, 30);
        return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
    }

    QSharedPointer<Common::Result::InvokeResult> TradeService::unBindingWeChat(QString appOpenId, QString userOpenId)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::UnBindingWeChat appOpenId:" << appOpenId << " userOpenId:" << userOpenId;
        QVector<Common::Parameter> parameters;
        parameters.append(Common::Parameter("appOpenId", QVariant(appOpenId)));
        parameters.append(Common::Parameter("userOpenId", QVariant(userOpenId)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::UnBindingWeChat, parameters, 30);
        return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
    }

    QSharedPointer<Common::Result::InvokeResult> TradeService::clickNewsLog(QString loginId)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::clickNewsLog loginId:" << loginId;
        QVector<Common::Parameter> parameters;
        parameters.append(Common::Parameter("LoginName", QVariant(loginId)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::ClickNewsLog, parameters, 30);
        return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));

    }

    QSharedPointer<Common::Result::InvokeResult> TradeService::changeSingleChannel()
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::changeSingleChannel";
        QVector<Common::Parameter> parameters;
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::ChangeSingleChannel, parameters, 30);
        return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
    }

    QSharedPointer<Common::Result::XmlDataResult> TradeService::getPaymentParameter(QString organizationIds, QString language, QString userId)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::getInitData organizationIds:" << organizationIds << "language:" <<language << " userId:"<<userId;
        QVector<Common::Parameter> parameters;
        parameters.append(Common::Parameter("organizationIds", QVariant(organizationIds)));
        parameters.append(Common::Parameter("language", QVariant(language)));
        parameters.append(Common::Parameter("userId", QVariant(userId)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::GetPaymentParameter, parameters, 120, true);
        return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
    }

    QSharedPointer<Common::Result::AddMarginResult> TradeService::traderDepositRequest(QString onlineTransferParameterId, QString traderLanguage, QString payVoucherId,
                                                                         QString payCurrency, QString payAmount, QString targetCurrency, QString targetAmount, QString account, QString accountId)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::traderDepositRequest onlineTransferParameterId:" << onlineTransferParameterId
                                                               << " traderLanguage:" <<traderLanguage
                                                               << " payVoucherId:" <<payVoucherId
                                                               << " payCurrency:" <<payCurrency
                                                               << " payAmount:" << payAmount
                                                               << " targetCurrency:" << targetCurrency
                                                               << " targetAmount:" << targetAmount
                                                               << " account:" << account
                                                               << " accountId:" << accountId ;
        QVector<Common::Parameter> parameters;
        parameters.append(Common::Parameter("onlineTransferParameterId", QVariant(onlineTransferParameterId)));
        parameters.append(Common::Parameter("traderLanguage", QVariant(traderLanguage)));
        parameters.append(Common::Parameter("payVoucherId", QVariant(payVoucherId)));
        parameters.append(Common::Parameter("payCurrency", QVariant(payCurrency)));
        parameters.append(Common::Parameter("payAmount", QVariant(payAmount)));
        parameters.append(Common::Parameter("targetCurrency", QVariant(targetCurrency)));
        parameters.append(Common::Parameter("targetAmount", QVariant(targetAmount)));
        parameters.append(Common::Parameter("account", QVariant(account)));
        parameters.append(Common::Parameter("accountId", QVariant(accountId)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::TraderDepositRequest, parameters, 120);
        return QSharedPointer<Common::Result::AddMarginResult>(new Common::Result::AddMarginResult(result));
    }

    QSharedPointer<Common::Result::InvokeResult> TradeService::createTokenByWFBOServer(QString userCode, QString language, QString prodQtys)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::createTokenByWFBOServer userCode:"
                                                              << userCode << " language:" << language << " prodQtys:" << prodQtys;
        QVector<Common::Parameter> parameters;
        parameters.append(Common::Parameter("userCode", QVariant(userCode)));
        parameters.append(Common::Parameter("language", QVariant(language)));
        parameters.append(Common::Parameter("prodQtys", QVariant(prodQtys)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::CreateTokenByWFBOServer, parameters, 120);
        return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));

    }

    QSharedPointer<Common::Result::XmlDataResult> TradeService::CreateIexCrmAccessToken(QString loginName, int validMinutes)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::CreateIexCrmAccessToken loginName:" << loginName << " validMinutes:"<<validMinutes;
        QVector<Common::Parameter> parameters;
        parameters.append(Common::Parameter("loginName", QVariant(loginName)));
        parameters.append(Common::Parameter("validMinutes", QVariant(validMinutes)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::CreateIexCrmAccessToken, parameters, 120);
        return QSharedPointer<Common::Result::XmlDataResult>(new Common::Result::XmlDataResult(result));
    }

    QSharedPointer<Common::Result::AddMarginResult> TradeService::callClientRegistration(
            QString language, QString reportDate, QString organizationName, QString customerName, QString accountId, QString accountCode, QString email, QString tel,
            QString cPIDCardNO, QString contactPerson, QString cPAddress, QString cPNationality, QString postcode,
            int documentType, QString birthDate, int countryId, QString iDCardPictureFileId, QString iDCardPicture2FileId)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::callClientRegistration email:" << email
                                                              << " language:" <<language
                                                              << " organizationName:" << organizationName
                                                              << " customerName:" << customerName
                                                              << " reportDate:" << reportDate
                                                              << " accountCode:" << accountCode
                                                              << " cPIDCardNO:" << cPIDCardNO
                                                              << " contactPerson:" << contactPerson
                                                              << " tel:" << tel
                                                              << " cPAddress:" << cPAddress
                                                              << " cPNationality:" << cPNationality
                                                              << " postcode:" << postcode
                                                              << " documentType:" << documentType
                                                              << " birthDate:" << birthDate
                                                              << " countryId:" << countryId
                                                              << " iDCardPictureFileId:" << iDCardPictureFileId
                                                              << " iDCardPicture2FileId:" << iDCardPicture2FileId
                                                              << " accountId:" << accountId;
        QVector<Common::Parameter> parameters;
        parameters.append(Common::Parameter("language",QVariant(language)));
        parameters.append(Common::Parameter("reportDate",QVariant(reportDate)));
        parameters.append(Common::Parameter("organizationName",QVariant(organizationName)));
        parameters.append(Common::Parameter("customerName",QVariant(customerName)));
        parameters.append(Common::Parameter("accountId", QVariant(accountId)));
        parameters.append(Common::Parameter("accountCode",QVariant(accountCode)));
        parameters.append(Common::Parameter("email",QVariant(email)));
        parameters.append(Common::Parameter("tel",QVariant(tel)));
        parameters.append(Common::Parameter("cPIDCardNO",QVariant(cPIDCardNO)));
        parameters.append(Common::Parameter("contactPerson",QVariant(contactPerson)));
        parameters.append(Common::Parameter("cPAddress",QVariant(cPAddress)));
        parameters.append(Common::Parameter("cPNationality",QVariant(cPNationality)));
        parameters.append(Common::Parameter("postcode",QVariant(postcode)));
        parameters.append(Common::Parameter("documentType",QVariant(documentType)));
        parameters.append(Common::Parameter("birthDate",QVariant(birthDate)));
        parameters.append(Common::Parameter("countryId",QVariant(countryId)));
        parameters.append(Common::Parameter("iDCardPictureFileId",QVariant(iDCardPictureFileId)));
        parameters.append(Common::Parameter("iDCardPicture2FileId",QVariant(iDCardPicture2FileId)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::CallClientRegistration, parameters, timeout);
        return QSharedPointer<Common::Result::AddMarginResult>(new Common::Result::AddMarginResult(result));
    }

    QSharedPointer<Common::Result::InvokeResult> TradeService::updateEmailSMSVerificationDate(QString LoginName,
                                                                    QString UniquelyIdentifies,
                                                                    QString EmailSMSVerificationStartDate)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<"TradeService::updateEmailSMSVerificationDate LoginName:"
                                                              << LoginName << " UniquelyIdentifies:" << UniquelyIdentifies
                                                              << " EmailSMSVerificationStartDate:" << EmailSMSVerificationStartDate;
        QVector<Common::Parameter> parameters;
        parameters.append(Common::Parameter("loginName", QVariant(LoginName)));
        parameters.append(Common::Parameter("iniquelyIdentifies", QVariant(UniquelyIdentifies)));
        parameters.append(Common::Parameter("emailSMSVerificationStartDate", QVariant(EmailSMSVerificationStartDate)));
        QSharedPointer<AsyncResult> result = this->_channel->asyncInvoke(Common::Constants::MethodNameConstants::UpdateEmailSMSVerificationDate, parameters, 120);
        return QSharedPointer<Common::Result::InvokeResult>(new Common::Result::InvokeResult(result));
    }
	///------------------Notify Method----------------------

}
#include "moc_TradeService.cpp"
