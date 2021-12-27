#ifndef SERVICE_TRADESERIVCE_H
#define SERVICE_TRADESERIVCE_H
#include <QObject>
#include "Common/Result/Result.h"
#include "Common/Result/PlaceResult.h"
#include "Common/Result/ChartXmlDataResult.h"
#include "Common/Result/UpdatePriceAlertResult.h"
#include "traderbusiness_global.h"

namespace Service
{
	class TRADERBUSINESS_EXPORT TradeService: public QObject
	{		
		Q_OBJECT
		public:
			TradeService(QSharedPointer<Communication::SslChannel> channel);			
            QSharedPointer<Common::Result::LoginResult> login(QString loginName, QString password, QString language, QString validPathName, QString descriptionInfo="", bool isMobileTrader = false, bool isGuest=false, QString versionName="", QString deviceName = "");
            QSharedPointer<Common::Result::LoginResult> loginByWeChat(QString loginName, QString password, QString language, QString validPathName, QString templateCode);
            QSharedPointer<Common::Result::InvokeResult> activateAccount(QString loginID, QString oldPassword, QString newPassword, QString recoverPasswordDataXml);			
            QSharedPointer<Common::Result::InvokeResult> logout(bool isGuest=false);
            QSharedPointer<Common::Result::InvokeResult> resetPassword(QString userOpenId, QString authType, QString newPassword);
			QSharedPointer<Common::Result::XmlDataResult> getInitData();
			QSharedPointer<Common::Result::XmlDataResult> getSettingData();
			QSharedPointer<Common::Result::XmlDataResult> getTradingData(const QStringList &  accountIds);
			QSharedPointer<Common::Result::PlaceResult> beginPlace(QString tranXmlString, QUuid id);
			QSharedPointer<Common::Result::InvokeResult> multipleClose(const QStringList &  orderIds);
			QSharedPointer<Common::Result::QuoteResult> requestQuote(QString instrumentId, double quoteLot, int status, int setPriceMaxMovePips, int lifeTime);
			QSharedPointer<Common::Result::XmlDataResult> getDeliveryAddresses(QString deliveryPointGroupId);
			QSharedPointer<Common::Result::PlaceResult> applyDelivery(QString deliveryXmlString);
			QSharedPointer<Common::Result::PlaceResult> instalmentPayoff(QString accountId, QString currencyId, QString sumSourcePaymentAmount, QString sumSourceTerminateFee, QString terminateXml);
			QSharedPointer<Common::Result::InvokeResult> changeMarginPin(QString accountId, QString oldPassword, QString newPassword);
			QSharedPointer<Common::Result::InvokeResult> modifyTelephoneIdentificationCode(QString accountId, QString oldCode, QString newCode, bool isReset);
			QSharedPointer<Common::Result::InvokeResult> recoverPasswordDatas(QString recoverPasswordDataXml);
			QSharedPointer<Common::Result::XmlDataResult> getRecoverPasswordDatas(QString userId);
			QSharedPointer<Common::Result::PlaceResult> cancelOrder(QString accountId, QString transactionId);
			QSharedPointer<Common::Result::XmlDataResult> getAccountsForSetting();
			QSharedPointer<Common::Result::XmlDataResult> getMarketingGroupForSetting();
			QSharedPointer<Common::Result::XmlDataResult> updateAccountsSetting(const QStringList & accountIDs);
			QSharedPointer<Common::Result::XmlDataResult> updateInstrumentsSetting(const QStringList & instrumentIDs);
			QSharedPointer<Common::Result::XmlDataResult> getInstrumentForSetting();
			QSharedPointer<Common::Result::ChartXmlDataResult> getQuotePolicies(QString instrumentId);
			QSharedPointer<Common::Result::InvokeResult> changeLeverage(QString accountId, QString leverage);
            QSharedPointer<Common::Result::XmlDataResult> getAccountBanksApproved(QString accountId, QString language, bool isSendBankCardPhoto = true);
            QSharedPointer<Common::Result::XmlDataResult> getAccountBankReferenceData(QString countryId, QString language, QString customerId="");
			QSharedPointer<Common::Result::AddMarginResult> additionalClient(QString email, QString receive, QString organizationName, QString customerName, 
				QString reportDate, QString accountCode, QString correspondingAddress, QString registratedEmailAddress, 
				QString tel, QString mobile, QString fax, QString fillName1,
                QString ICNo1, QString fillName2, QString ICNo2, QString fillName3, QString ICNo3, QString accountId);
			QSharedPointer<Common::Result::AddMarginResult> marginApply(QString id, QString accountBankApprovedId, QString accountID, QString countryID, 
				QString bankID, QString bankName, QString accountBankNo, QString accountBankType, 
				QString accountOpener, QString accountBankProp, QString accountBankBCID, QString accountBankBCName,
                QString idType, QString idNo, QString bankProvinceID, QString bankCityID, QString bankAddress, QString swiftCode, QString applicationType, QString accountId, QString bankCardPhotoId, QString language="ENG");
			QSharedPointer<Common::Result::AddMarginResult> marginAgent(QString email, QString receive, QString organizationName, QString customerName, 
				QString reportDate, QString accountCode, QString previousAgentCode, QString previousAgentName, 
                QString newAgentCode, QString newAgentName, QString newAgentICNo, QString dateReply, QString accountId);
			QSharedPointer<Common::Result::AddMarginResult> callMarginExtension(QString email, QString receive, QString organizationName, QString customerName, 
                QString reportDate, QString accountCode, QString currency, QString currencyValue, QString dueDate, QString accountId);
			QSharedPointer<Common::Result::AddMarginResult> paymentInstruction(QString email, QString receive, QString organizationName, QString customerName, 
				QString reportDate, QString accountCode, QString currency, QString currencyValue, QString beneficiaryName,
                QString bankAccount, QString bankerName,QString bankerAddress,QString swiftCode,QString remarks,QString thisisClien, QString accountBankApprovedId, QString traderLanguage, QString accountId);
			QSharedPointer<Common::Result::AddMarginResult> paymentInstructionInternal(QString email,QString organizationName, QString customerName, 
				QString reportDate, QString accountCode, QString currency, QString amount, QString beneficiaryAccount,
                QString beneficiaryAccountOwner, QString email2, QString traderLanguage, QString accountId);
			QSharedPointer<Common::Result::AddMarginResult> paymentInstructionCash(QString email,QString organizationName, QString customerName, 
				QString reportDate, QString accountCode, QString currency, QString amount, QString beneficiaryName,
                QString beneficiaryAddress, QString traderLanguage, QString accountId);
			QSharedPointer<Common::Result::InvokeResult> verifyMarginPin(QString accountId, QString password);
			QSharedPointer<Common::Result::XmlDataResult> orderQuery(QString selectedAccountId, QString selectedInstrumentId, int lastDays, int orderQueryType);
			QSharedPointer<Common::Result::InvokeResult> deleteMessage(QString id);
			QSharedPointer<Common::Result::InvokeResult> getNewsContents(QString id);
			QSharedPointer<Common::Result::UpdatePriceAlertResult> updatePriceAlerts(QUuid id, QString priceAlertRequestXml);
			QSharedPointer<Common::Result::InvokeResult> updatePriceAlertState(const QStringList &  ids);
			QSharedPointer<Common::Result::XmlDataResult> getLogData(QString id, QString language);
			QSharedPointer<Common::Result::InvokeResult> saveIsSptReconfirm(bool isSptReconfirm);
			QSharedPointer<Common::Result::XmlDataResult> verifyTransaction(const QStringList &  transactionIds);
			QSharedPointer<Common::Result::InvokeResult> setClickTradeLog(QString instrumentId,  int disclaimerOption);
			QSharedPointer<Common::Result::InvokeResult> getServerDateTime();
			QSharedPointer<Common::Result::InvokeResult> resetAlertLevel(const QStringList& accountIds, const QString& accountCodes);
			QSharedPointer<Common::Result::InvokeResult> signAgreement(const QStringList& accountIds, const int& counterPartyId, const QString& instrumentId);
			QSharedPointer<Common::Result::XmlDataResult> getTransactionXml(QString tranXmlString);
			QSharedPointer<Communication::SslChannel> getChannel() { return _channel; }
			QSharedPointer<Common::Result::InvokeResult> getWatchWorlds(QString clientId);
			QSharedPointer<Common::Result::GetVerificationCodeResult> getVerificationCode(QString language, QString mobilePhone);
			QSharedPointer<Common::Result::InvokeResult> updateMessageState(QString messageId);
            QSharedPointer<Common::Result::PlaceResult> applyOfficialFixing(QString accountId, QString orderId, QString settleDate);
            QSharedPointer<Common::Result::InvokeResult> bindingWeChat(QString appOpenId, QString accessCode);
            QSharedPointer<Common::Result::InvokeResult> unBindingWeChat(QString appOpenId, QString userOpenId);
            QSharedPointer<Common::Result::InvokeResult> clickNewsLog(QString loginId);
            QSharedPointer<Common::Result::InvokeResult> changeSingleChannel();
            QSharedPointer<Common::Result::XmlDataResult> getPaymentParameter(QString organizationIds, QString language, QString userId);
            QSharedPointer<Common::Result::AddMarginResult> traderDepositRequest(QString onlineTransferParameterId, QString traderLanguage, QString payVoucherId,
                                                                                 QString payCurrency, QString payAmount, QString targetCurrency, QString targetAmount, QString account, QString accountId);
            QSharedPointer<Common::Result::InvokeResult> createTokenByWFBOServer(QString userCode, QString language, QString prodQtys);
            QSharedPointer<Common::Result::XmlDataResult> CreateIexCrmAccessToken(QString loginName, int validMinutes);
            QSharedPointer<Common::Result::AddMarginResult> callClientRegistration(
                    QString language, QString reportDate, QString organizationName, QString customerName, QString accountId, QString accountCode, QString email, QString tel,
                    QString cPIDCardNO, QString contactPerson, QString cPAddress, QString cPNationality, QString postcode,
                    int documentType, QString birthDate, int countryId, QString iDCardPictureFileId, QString iDCardPicture2FileId);

            QSharedPointer<Common::Result::InvokeResult> updateEmailSMSVerificationDate(QString LoginName,
                                                                            QString UniquelyIdentifies,
                                                                            QString EmailSMSVerificationStartDate);

		private:
			QSharedPointer<Communication::SslChannel> _channel;
			int timeout;

		signals:
			void notifyReceived(const QDomElement& xmlNotify);

        public slots:
            void notifyReceivedSlot(const QDomElement& xmlNotify);
	};
}
#endif	   //TRADESERIVCE_H
