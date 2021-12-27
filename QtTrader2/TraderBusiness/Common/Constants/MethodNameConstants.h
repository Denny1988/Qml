#ifndef METHODNAMECONSTANS_H
#define METHODNAMECONSTANS_H

#include <QString>

namespace Common 
{
	namespace Constants
	{
		class MethodNameConstants
		{
		public:
			static const QString Login;
			static const QString Logout;
			static const QString ActivateAccount;
			static const QString RecoverPasswordDatas;
            static const QString ResetPassword;
			static const QString GetInitData;
			static const QString GetSettingData;
			static const QString GetTradingData;
			static const QString Place;
			static const QString MultipleClose;
			static const QString Quote;
			static const QString AsyncGetChartData2ForTrader;		
			static const QString AsyncGetChartData2ForCppTrader;
			static const QString GetDeliveryAddresses;
			static const QString ApplyDelivery;
			static const QString InstalmentPayoff;
			static const QString ChangeMarginPin;
			static const QString ModifyTelephoneIdentificationCode;
			static const QString GetRecoverPasswordData;
			static const QString CancelOrder;
			static const QString GetAccountsForSetting;
			static const QString GetAccountForMarketingGroup;
			static const QString UpdateAccountsSetting;
			static const QString GetInstrumentForSetting;
			static const QString UpdateInstrumentsSetting;
			static const QString GetQuotePolicies;
			static const QString ChangeLeverage;
			static const QString GetAccountBanksApproved;
			static const QString GetAccountBankReferenceData;
			static const QString AdditionalClient;
			static const QString MarginApply;
			static const QString MarginAgent;
			static const QString CallMarginExtension;
			static const QString PaymentInstruction;
			static const QString PaymentInstructionInternal;
			static const QString PaymentInstructionCash;
			static const QString VerifyMarginPin;
			static const QString Recover;
			static const QString GetChartCount;
			static const QString GetChartVersion;			
			static const QString GetReportUrl;
            static const QString GetOrderConfirmationReportUrl;
			static const QString OrderQuery;
			static const QString DeleteMessage;
			static const QString GetNewsContents;
			static const QString UpdatePriceAlerts;
			static const QString UpdatePriceAlertState;
			static const QString GetLogData;
			static const QString GetServerDateTime;
			static const QString SaveIsSptReconfirm;
			static const QString GetChartFirstTradeDay;
			static const QString GetFixPeriodBeginTimes;
			static const QString VerifyTransaction;
			static const QString GetBORealTimeData;
			static const QString SetClickTradeLog;
			static const QString ResetAlertLevel;
			static const QString SignAgreement;
			static const QString ProvisionalLogin;
			static const QString ProvisionalPlace;
			static const QString ProvisionalLogout;
			static const QString GetTransactionXml;
			static const QString GetWatchWorlds;
			static const QString GetVerificationCode;
			static const QString UpdateMessageState;
            static const QString ApplyOfficialFixing;
            static const QString BindingWeChat;
            static const QString UnBindingWeChat;
            static const QString ClickNewsLog;
            static const QString ChangeSingleChannel;
            static const QString GetPaymentParameter;
            static const QString TraderDepositRequest;
            static const QString CreateTokenByWFBOServer;
            static const QString CreateIexCrmAccessToken;
            static const QString CallClientRegistration;
            static const QString UpdateEmailSMSVerificationDate;

		};
	}
}
#endif


