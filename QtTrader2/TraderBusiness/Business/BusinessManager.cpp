#include "BusinessManager.h"
#include "Service/ServiceManager.h"
#include <math.h>
#include "Common/ThreadManager.h"
#include "Common/LogHelper.h"
#include "Common/CustomerSetting.h"
#include "Common/LoginSetting.h"
#include "Common/SignalForwarder.h"
#include "Common/AppSetting.h"
#include "BusinessHelper.h"
#include "Common/TimerManager.h"
#include "Common/StatusManager.h"
#include "Common/Enum/TransactionErrorEnum.h"
#include <QMetaEnum>
//#include <QMutexLocker>

namespace Business
{
	BusinessManager* BusinessManager::m_Instance = Q_NULLPTR;

	QMutex BusinessManager::m_Mutex(QMutex::Recursive);
	BusinessManager* BusinessManager::getInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new BusinessManager;
        }
		return m_Instance;
	}

	BusinessManager::BusinessManager()
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "BusinessManager::BusinessManager start: MainThreadId:" << QThread::currentThreadId();
        qDebug()<< "BusinessManager::BusinessManager1111: MainThreadId:" << QThread::currentThreadId();
        this->moveToThread(Common::ThreadManager::getInstance()->getBusinessThread());
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "BusinessManager::BusinessManager moveToThread: BusinessThread:" << Common::ThreadManager::getInstance()->getBusinessThread()->currentThreadId();
		QSharedPointer<SettingManager>  newSettingManager(new SettingManager);
		this->_settingManager = newSettingManager;
		QSharedPointer<InitializeManager>  newInitializeManager(new InitializeManager);
		this->_initializeManager = newInitializeManager;
		QSharedPointer<QuotationManager>  newQuotationManager(new QuotationManager);
		this->_quotationManager = newQuotationManager;
		QSharedPointer<ComputingManager>  newComputingManager(new ComputingManager(this->_settingManager, this->_quotationManager));
		this->_computingManager = newComputingManager;
		QSharedPointer<TradingManager>  newTradingManager(new TradingManager(this->_settingManager, this->_computingManager, this->_initializeManager));
		this->_tradingManager = newTradingManager;
		QSharedPointer<ServerSourceManager>  newServerSourceManager(new ServerSourceManager(this->_settingManager, this->_initializeManager));
		this->_serverSourceManager = newServerSourceManager;
		BusinessHelper::getInstance()->setSettingManager(this->_settingManager);
		this->_isNeedLoadTradingData = false;
		this->_isNeedLoadMultiTradingData = false;
		this->_failedGetTradingDataCount = 0;
        this->_sleepStatus = false;
        this->_noData = true;

		this->_updateBOCountdownTimer = new QTimer(this);
		connect(this->_updateBOCountdownTimer, SIGNAL(timeout()), this, SLOT(updateBOCountdownSlot()));
		this->_updateBOCountdownTimer->start(1000);
        this->_updateBOCountdownTimer->moveToThread(Common::ThreadManager::getInstance()->getBusinessThread());

		this->_transactionExpireInspectorTimer = new QTimer(this);
		connect(this->_transactionExpireInspectorTimer, SIGNAL(timeout()), this, SLOT(transactionExpireInspectorSlot()));
		this->_transactionExpireInspectorTimer->start(3000);
        this->_transactionExpireInspectorTimer->moveToThread(Common::ThreadManager::getInstance()->getBusinessThread());

	}

	void BusinessManager::setIsRefreshStatus(bool isRefresh)
	{
		Common::TraderBaseInfo::getInstance()->setIsRefreshSystem(isRefresh);
	}

	void BusinessManager::loginQuotationServer(QStringList instrumentAndQuotePolicyIDs)
	{		
        this->_loginQuotationServerResult = Service::ServiceManager::getInstance()->getQuotationService()->loginQuotationService(
                    Common::TraderBaseInfo::getInstance()->getUserId().toString().mid(1, 36), instrumentAndQuotePolicyIDs,
                    Common::TraderBaseInfo::getInstance()->getLoginGuest());
		connect(this->_loginQuotationServerResult.data(), SIGNAL(completed()), this, SLOT(loginQuotationServerCompleted()));
	}

	void BusinessManager::resetAlertLevel(const QStringList& accountIds, const QString& accountCodes)
	{
		Service::ServiceManager::getInstance()->getTradeService()->resetAlertLevel(accountIds, accountCodes);
	}

	void BusinessManager::addRequestQuote(QString id)
	{
		if (!this->_requestQuoteMap.contains(id))
			this->_requestQuoteMap.append(id);
	}

	void BusinessManager::removeRequestQuote(QString id)
	{
		this->_requestQuoteMap.removeOne(id);
	}

    void BusinessManager::setisSleepStatus(bool sleepStatus)
    {
        if(this->_sleepStatus != sleepStatus && !sleepStatus)
            this->resume();
        this->_sleepStatus = sleepStatus;
    }

    bool BusinessManager::isNoData()
    {
        return this->_noData;
    }

    void BusinessManager::refreshTradingData(QList<QUuid> transactionIds, QList<QUuid> orderIds)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "Start refreshTradingData transactionIds:" << transactionIds
                                                                << " orderIds:" << orderIds;
        QStringList accountIds;
        accountIds.append(Common::TraderBaseInfo::getInstance()->getCurrentAccountId().toString().mid(1, 36));
        qDebug() << "Start refreshTradingData accountIds:"<< accountIds  << " transactionIds:" << transactionIds << " orderIds:" << orderIds;
        this->_tradingManager->TransactionIds = transactionIds;
        this->_tradingManager->OrderIds = orderIds;
        this->_refreshTradingDataXmlResult = Service::ServiceManager::getInstance()->getTradeService()->getTradingData(accountIds);
        connect(this->_refreshTradingDataXmlResult.data(), SIGNAL(completed()), this, SLOT(refreshTradingDataResultCompletedSlot()));
    }

	//---------------------SLOT---------------------------
	void BusinessManager::getInitData()
    {
        this->_sleepStatus = false;
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "BusinessManager::getInitData start";
        qDebug() <<  "BusinessManager::getInitData " << QThread::currentThreadId();
		this->_initDataXmlResult = Service::ServiceManager::getInstance()->getTradeService()->getInitData();
        connect(this->_initDataXmlResult.data(), SIGNAL(completed()), this, SLOT(initDataResultCompleted()));
	}

	void BusinessManager::refreshInitData()
    {
        this->_sleepStatus = false;
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::refreshInitData start";
		this->_initDataXmlResult = Service::ServiceManager::getInstance()->getTradeService()->getInitData();
		connect(this->_initDataXmlResult.data(), SIGNAL(completed()), this, SLOT(initDataResultCompleted()));
	}

	void BusinessManager::initDataResultCompleted()
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "initDataResultCompleted start: ThreadId:" << QThread::currentThreadId();
        qDebug() <<  "BusinessManager::initDataResultCompleted start ThreadId:" << QThread::currentThreadId();
		disconnect(this->_initDataXmlResult.data(), SIGNAL(completed()), this, SLOT(initDataResultCompleted()));
		if (this->_initDataXmlResult->hasError())
        {
            QLOG_ERROR(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::initDataResultCompleted error: " << this->_initDataXmlResult->error();
            Common::StatusManager::getInstance()->setIsInitFailStatus(true);
		}
		else
        {
            qDebug() <<  "BusinessManager::initDataResultCompleted init start";
			QDomDocument doc;
			doc.setContent(this->_initDataXmlResult->getByteArrayResult());
			QDomElement contentXml = doc.documentElement().toElement();
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "initDataResultCompleted: updateInitializeData start";
			this->_initializeManager->updateInitializeData(contentXml, this->_settingManager);
			Common::TraderBaseInfo::getInstance()->setLogoutByNoOperation(this->_settingManager->getSystemSetting()->getTraderIdleTimeout());
			Common::TraderBaseInfo::getInstance()->setMaxPasswordReserveCount(this->_settingManager->getSystemSetting()->getMaxPasswordReserveCount());
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "initDataResultCompleted: getNeedSelectAccount start";
			bool needSelectAccount = this->_settingManager->getSystemSetting()->getNeedSelectAccount();
			auto accounts = this->_initializeManager->getAccountsBySortAccountCode();
			Common::TraderBaseInfo::getInstance()->setHideSelectALLQCheckBox(accounts.count() == 1 ? true : false);
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "initDataResultCompleted: accountCount:" << accounts.count();
			if (accounts.count() == 0 && !needSelectAccount)
			{
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "initDataResultCompleted: sendCustomerNoAccountSignal";
				Common::StatusManager::getInstance()->setIsCustomerNoAccountStatus(true);
				Common::SignalForwarder::getInstance()->sendCustomerNoAccountSignal();
			}
			else
			{
				if (needSelectAccount)
				{
                    QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "initDataResultCompleted: sendNeedSelectAccountSignal";
					Common::StatusManager::getInstance()->setIsNeedSelectAccountStatus(true);
					Common::SignalForwarder::getInstance()->sendNeedSelectAccountSignal();
				}
				else
				{
					if (this->_initializeManager->getInstruments().count() == 0)
					{
                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "initDataResultCompleted: sendNeedSelectInstrumentSignal";
						Common::StatusManager::getInstance()->setIsNeedSelectInstrumentStatus(true);
						Common::SignalForwarder::getInstance()->sendNeedSelectInstrumentSignal();
					}
					else
                    {
                        qDebug() <<  "BusinessManager::initDataResultCompleted 33";

						QList<QUuid> totalAccountIds;
						QList<QUuid> checkAccountIds;
						foreach(auto account, accounts)
						{
							if (Common::CustomerSetting::getInstance()->getCheckAccountStatus(account->_id))
								checkAccountIds.append(account->_id);
							totalAccountIds.append(account->_id);
						}
						Common::TraderBaseInfo::getInstance()->setTotalAccounts(totalAccountIds, checkAccountIds);
						auto checkAccounts = Common::TraderBaseInfo::getInstance()->getCheckedAccounts();
                        QString accountIdStr = Common::CustomerSetting::getInstance()->getValue("AccountUI/CurrentAccount");
						if (accountIdStr != "" && checkAccounts.contains(QUuid(accountIdStr)))
						{
							Common::TraderBaseInfo::getInstance()->setCurrentAccountId(QUuid(accountIdStr));
						}
						else
						{
							Common::TraderBaseInfo::getInstance()->setCurrentAccountId(checkAccounts.at(0));
						}
                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "initDataResultCompleted updateInitializeData start";
						this->_quotationManager->updateInitializeData(contentXml);
						Common::StatusManager::getInstance()->setInitDataStatus(true);
						if (Common::TraderBaseInfo::getInstance()->getIsRefreshSystem())
                        {
                            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "initDataResultCompleted: Refresh";
							this->startGetSettingAndTradingDataSlot();
                            this->setIsRefreshStatus(false);                            
						}
                        qDebug() <<  "BusinessManager::initDataResultCompleted end";
                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "initDataResultCompleted: this->getInitDataResultCompleted() end";
					}
				}
			}
        }
	}

	void BusinessManager::isGetSettingDataOrTraderData()
	{
		bool isInitDataCompleted = Common::StatusManager::getInstance()->isInitDataCompleted();
		if (isInitDataCompleted)
		{
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "isGetSettingDataOrTraderData: start this->startGetSettingAndTradingDataSlot()";
			this->startGetSettingAndTradingDataSlot();
		}
		else
		{
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "isGetSettingDataOrTraderData: connect this->startGetSettingAndTradingDataSlot()";
			connect(Common::StatusManager::getInstance(), SIGNAL(initializeDataCompletedSignal()), this, SLOT(startGetSettingAndTradingDataSlot()), Qt::QueuedConnection);
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "isGetSettingDataOrTraderData: connect startGetSettingAndTradingDataSlot end";
		}
	}

	void BusinessManager::startGetSettingAndTradingDataSlot()
    {
		disconnect(Common::StatusManager::getInstance(), SIGNAL(initializeDataCompletedSignal()), this, SLOT(startGetSettingAndTradingDataSlot()));
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "startGetSettingAndTradingDataSlot: start";
		QStringList accountIds;
		auto accounts = this->_initializeManager->getAccounts();
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "startGetSettingAndTradingDataSlot: account count:" << QString::number(accounts.count());
		this->_loadAccountCount = 50;
		if (accounts.count() > this->_loadAccountCount)
        {
			this->_unLoadTradingAccountIds = Common::TraderBaseInfo::getInstance()->getTotalAccounts();
			this->_isCompleteInitTradingData = false;			
			this->_isNeedLoadMultiTradingData = true;
			Common::AppSetting::getInstance()->setIsNeedLoadMultiTradingData(true);
			this->getSettingData();
		}
		else
        {
			this->_isCompleteInitTradingData = true;
			this->_isNeedLoadMultiTradingData = false;
			Common::AppSetting::getInstance()->setIsNeedLoadMultiTradingData(false);
			for (int i = 0; i < accounts.length(); i++)
			{
				accountIds.append(accounts.at(i)->_id.toString().mid(1, 36));
			}
			this->getSettingAndTradingData(accountIds);
		}
	}

	void BusinessManager::getSettingAndTradingData(const QStringList &  accountIds)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "getSettingAndTradingData Start";
		this->getSettingData();
        this->getTradingData(accountIds);
	}

	void BusinessManager::settingDataResultCompleted()
    {
		disconnect(this->_settingDataXmlResult.data(), SIGNAL(completed()), this, SLOT(settingDataResultCompleted()));
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "BusinessManager::settingDataResultCompleted start: ThreadId:" << QThread::currentThreadId();
        qDebug() << "settingDataResultCompleted start: ThreadId:" << QThread::currentThreadId();
		if (this->_settingDataXmlResult->hasError())
		{
			QLOG_ERROR(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted faild: " << this->_settingDataXmlResult->error();
			Common::StatusManager::getInstance()->setIsInitSettingDataFailStatus(true);
		}
		else
        {            
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted initSettingSourceData start";
			this->_serverSourceManager->initSettingSourceData(this->_settingDataXmlResult->getByteArrayResult());
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted initSettingSourceData end";

			if(Common::LoginSetting::getInstance()->getUseTraderQuotationServer() && Service::ServiceManager::getInstance()->isUseTraderQuotationServer())
				this->loginQuotationServer(this->_serverSourceManager->getInstrumentAndQuotePolicyIDs());
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted loginQuotationServer end";
			this->_settingDataXmlResult->clearByteArray();
			this->_settingManager->setQuotePolicyAccountMappingAndAccountExpireNotify(true);
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted setQuotePolicyAccountMappingAndAccountExpireNotify end";
			this->_computingManager->InitializeQuotation(this->_quotationManager->getOverridedQuotations());            
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted InitializeQuotation end";

			int physicalCount = this->_initializeManager->getPhysicalCount();
			auto systemSetting = this->_settingManager->getSystemSetting();
			if (physicalCount == 0)
			{
				systemSetting->setIsShowPhysical(false);
				systemSetting->setIsShowOpenOrderAndSummary(true);
			}
			else if (physicalCount == this->_initializeManager->getInstruments().count())
			{
				systemSetting->setIsShowOpenOrderAndSummary(false);
				systemSetting->setIsShowPhysical(true);
			}
			if (this->_settingManager->getAccountCount() > 0)
			{
				if (!this->_settingManager->setCurrentAccountAndQuotePolicyId())
				{
					Common::StatusManager::getInstance()->setIsInitSettingDataFailStatus(true);
                    QLOG_ERROR( Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted setCurrentAccountAndQuotePolicyId failed";
					return;
				}

				Common::StatusManager::getInstance()->setInitSettingDataStatus(true);

				int totalInstrumentCount = this->_initializeManager->getInstrumentCount();
				int boInstrumentCount = this->_initializeManager->getBOInstrumentCount();
				int notBOInstrumentCount = this->_initializeManager->getNotBOInstrumentCount();
				bool onlyExistsBOInstrument = (totalInstrumentCount == boInstrumentCount) && notBOInstrumentCount == 0;
				if (onlyExistsBOInstrument)
				{
					Common::SignalForwarder::getInstance()->sendIsShowFunctionModelMenuTypeSignal(FunctionModelMenuType::QuotationCard, false);
					Common::SignalForwarder::getInstance()->sendIsShowFunctionModelMenuTypeSignal(FunctionModelMenuType::QuotationList, false);
				}
				if (boInstrumentCount == 0)
					Common::SignalForwarder::getInstance()->sendIsShowFunctionModelMenuTypeSignal(FunctionModelMenuType::BOQuotationCard, false);
				Common::TraderBaseInfo::getInstance()->setOnlyExistsBOInstrument(onlyExistsBOInstrument);
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted setOnlyExistsBOInstrument end";

				if (this->_isNeedLoadTradingData)
				{
                    QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted::tradingDataResultCompleted start";
					bool isOk = this->_tradingManager->initTradingData(this->_tradingDataXmlResult->getByteArrayResult());					
					this->_tradingDataXmlResult->clearByteArray();
                    this->_noData = false;
                    QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted::initTradingData end";
					if (isOk)
					{
						this->_tradingManager->startSetInitData();
                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted::startSetInitData end";
						Common::StatusManager::getInstance()->setIsInitTradingDataFailStatus(false);
						Common::StatusManager::getInstance()->setInitTradingDataStatus(true);
						this->startComputing();
                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted startComputing end";
						this->calculateInstrumentSummarySlot();
                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted calculateInstrumentSummarySlot end";
						this->_tradingManager->refreshOpenOrderButtonIsEnable();
                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted refreshOpenOrderButtonIsEnable end";
						Common::TraderBaseInfo::getInstance()->setInitFailCount(0);
						connect(Common::SignalForwarder::getInstance(), SIGNAL(refreshPhysicalButtonIsEnableSignal()), this, SLOT(refreshPhysicalButtonIsEnableSlot()));
						connect(Common::SignalForwarder::getInstance(), SIGNAL(computeAllSignal()), this, SLOT(computeAllSlot()));
						connect(Common::SignalForwarder::getInstance(), SIGNAL(computeAccountSignal(QUuid)), this, SLOT(computeAccountSlot(QUuid)));
						connect(Common::SignalForwarder::getInstance(), SIGNAL(computeQuotationSignal(QList<QUuid>)), this, SLOT(computeQuotationSlot(QList<QUuid>)));
						connect(Common::SignalForwarder::getInstance(), SIGNAL(computeAllSignal(QList<QUuid>)), this, SLOT(computeAllSlot(QList<QUuid>)));
						connect(Common::SignalForwarder::getInstance(), SIGNAL(changeEmployeeAccountListCurrencySignal(QUuid, QUuid)), this, SLOT(changeEmployeeAccountListCurrencySlot(QUuid, QUuid)));
						connect(Common::StatusManager::getInstance(), SIGNAL(showAccountOrdersSignal(const QString&, bool)), this, SLOT(refreshOpenOrderButtonIsEnableByAccountSlot(const QString&, bool)), Qt::QueuedConnection);
						
					}
					else
					{
                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::tradingDataResult No Data!";
					}
				}
				if (this->_isNeedLoadMultiTradingData)
				{
                    QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted loadMultiAccountTradingData start";
					this->loadMultiAccountTradingData();
                    QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted loadMultiAccountTradingData end";
				}
				this->_isNeedLoadMultiTradingData = false;

                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted send IsShowSytemMenu Signal";
				Common::SignalForwarder::getInstance()->sendIsShowSytemMenuSignal(Common::Enum::MainSytemMenu::ChangeLeverage, this->_settingManager->getSystemSetting()->getEnableModifyLeverage());
				Common::SignalForwarder::getInstance()->sendIsShowSytemMenuSignal(Common::Enum::MainSytemMenu::SptReconfirm, this->_settingManager->isShowSptReconfirm());
                Common::SignalForwarder::getInstance()->sendIsShowSytemMenuSignal(Common::Enum::MainSytemMenu::UserCenter,
                                                                                  this->_settingManager->getSystemSetting()->getIsIexCrmUser() &&
                                                                                  this->_settingManager->getSystemSetting()->getIexCrmUserCenterUrl().length() > 0);
                bool enableOwnerRegistration = false;
                bool isShowBankAccount = false;
                auto userId = Common::TraderBaseInfo::getInstance()->getUserId();
                auto accounts = Business::BusinessManager::getInstance()->getSettingManager()->getAccounts();
                foreach (QSharedPointer<Setting::BusinessAccount> account, accounts) {
                    if(account->getCustomerId() == userId || account->getAgentId() == userId)
                    {
                        enableOwnerRegistration = account->getMarginSetting()->getEnableOwnerRegistration();
                        if(enableOwnerRegistration) break;
                    }
                }
                foreach (QSharedPointer<Setting::BusinessAccount> account, accounts) {
                    auto marginSetting = account->getMarginSetting();
                    bool isAgent = false;
                    if (!account->getAgentId().isNull())
                    {
                        isAgent = (account->getAgentId() == userId && account->getCustomerId()!= userId);
                    }
                    if(!systemSetting->getIsEmployee() && !isAgent)
                    {
                        if(marginSetting->getEnablePI() && account->getCustomerId() == userId)
                        {
                            isShowBankAccount = true;
                            break;
                        }
                    }
                    else if (systemSetting->getIsEmployee())
                    {
                        if (marginSetting->getEnablePI() && marginSetting->getAllowSalesPayment() && account->getSalesCode() == Common::TraderBaseInfo::getInstance()->getUserName())
                        {
                            isShowBankAccount = true;
                            break;
                        }
                    }
                }
                qDebug() << " sendIsShowSytemMenuSignal: OwnerRegistration:" << (!systemSetting->getIsEmployee() && enableOwnerRegistration);
                qDebug() << " sendIsShowSytemMenuSignal: isShowBankAccount:" << isShowBankAccount;
                Common::SignalForwarder::getInstance()->sendIsShowSytemMenuSignal(Common::Enum::MainSytemMenu::OwnerRegistration,
                                                                                   !systemSetting->getIsEmployee() && enableOwnerRegistration);
                Common::SignalForwarder::getInstance()->sendIsShowSytemMenuSignal(Common::Enum::MainSytemMenu::BankAccount, isShowBankAccount);

                connect(Service::ServiceManager::getInstance()->getTradeService().data(), SIGNAL(notifyReceived(const QDomElement&)), this, SLOT(notifyReceivedSlot(const QDomElement&)));
                connect(Service::ServiceManager::getInstance()->getQuotationService().data(), SIGNAL(quotationReceived(const QString)), this, SLOT(quotationReceivedSlot(const QString)));
				Common::SignalForwarder::getInstance()->sendIsShowSystemTitleBarSignal(MainSytemMenu::Margin, this->_settingManager->isShowMagin());				
                bool showChangepassword = false;
                if(Business::BusinessManager::getInstance()->getSettingManager()->isAllowChangePassword())
                    showChangepassword = true;
                QList<QSharedPointer<Common::Struct::SettingAccount>> changePhoneVerificatioSettingAccount = Business::BusinessManager::getInstance()->getSettingManager()->getAccountsByModifyTelephoneIdentificationCode();
                if(changePhoneVerificatioSettingAccount.count()>0)
                      showChangepassword = true;
                QList<QSharedPointer<Common::Struct::SettingAccount>> changeMarginSettingAccount = Business::BusinessManager::getInstance()->getSettingManager()->getAccountsByMarginPin();
                if(changeMarginSettingAccount.count()>0)
                      showChangepassword = true;
                Common::SignalForwarder::getInstance()->sendIsShowSytemMenuSignal(Common::Enum::MainSytemMenu::ChangePassword, showChangepassword);
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted end";
			}
            else
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResult No Data!";
        }
	}

	void BusinessManager::tradingDataResultCompleted()
    {
		disconnect(this->_tradingDataXmlResult.data(), SIGNAL(completed()), this, SLOT(tradingDataResultCompleted()));
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "BusinessManager::tradingDataResultCompleted start:ThreadId:" << QThread::currentThreadId();
        qDebug() << "tradingDataResultCompleted start:ThreadId:" << QThread::currentThreadId();
		if (this->_tradingDataXmlResult->hasError())
		{
            QLOG_ERROR(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::BusinessManager::tradingDataResultCompleted faild: " << this->_tradingDataXmlResult->error();
			this->_failedGetTradingDataCount++;
			if (this->_failedGetTradingDataCount > Common::AppSetting::getInstance()->getReGetTradingDataMaxCount())
				Common::StatusManager::getInstance()->setIsInitTradingDataFailStatus(true);
			else
			{
				this->_tradingDataXmlResult = Service::ServiceManager::getInstance()->getTradeService()->getTradingData(this->_tradingAccountIds);
				connect(this->_tradingDataXmlResult.data(), SIGNAL(completed()), this, SLOT(tradingDataResultCompleted()));
			}
		}
		else
        {
			this->_failedGetTradingDataCount = 0;
			if (this->_isCompleteInitTradingData)
			{
				if (Common::StatusManager::getInstance()->isSettingDataCompleted())
				{
                    QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::tradingDataResultCompleted initTradingData start..";
					bool isOk = this->_tradingManager->initTradingData(this->_tradingDataXmlResult->getByteArrayResult());
                    QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::tradingDataResultCompleted initTradingData end..";
					this->_tradingDataXmlResult->clearByteArray();
                    this->_noData = false;
					if (isOk)
					{
						this->_tradingManager->startSetInitData();
                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::settingDataResultCompleted::startSetInitData end";
						Common::StatusManager::getInstance()->setIsInitTradingDataFailStatus(false);
						Common::StatusManager::getInstance()->setInitTradingDataStatus(true);

                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::tradingDataResultCompleted startComputing start.";
						this->startComputing();
                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::tradingDataResultCompleted startComputing end..";
						this->calculateInstrumentSummarySlot();
                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::tradingDataResultCompleted calculateInstrumentSummarySlot end..";
						this->_tradingManager->refreshOpenOrderButtonIsEnable();
                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::tradingDataResultCompleted refreshOpenOrderButtonIsEnable end..";
						Common::TraderBaseInfo::getInstance()->setInitFailCount(0);
						connect(Common::SignalForwarder::getInstance(), SIGNAL(refreshPhysicalButtonIsEnableSignal()), this, SLOT(refreshPhysicalButtonIsEnableSlot()));
						connect(Common::SignalForwarder::getInstance(), SIGNAL(computeAllSignal()), this, SLOT(computeAllSlot()));
						connect(Common::SignalForwarder::getInstance(), SIGNAL(computeAccountSignal(QUuid)), this, SLOT(computeAccountSlot(QUuid)));
						connect(Common::SignalForwarder::getInstance(), SIGNAL(computeQuotationSignal(QList<QUuid>)), this, SLOT(computeQuotationSlot(QList<QUuid>)));
						connect(Common::SignalForwarder::getInstance(), SIGNAL(computeAllSignal(QList<QUuid>)), this, SLOT(computeAllSlot(QList<QUuid>)));
						connect(Common::SignalForwarder::getInstance(), SIGNAL(changeEmployeeAccountListCurrencySignal(QUuid, QUuid)), this, SLOT(changeEmployeeAccountListCurrencySlot(QUuid, QUuid)));
						connect(Common::StatusManager::getInstance(), SIGNAL(showAccountOrdersSignal(const QString&, bool)), this, SLOT(refreshOpenOrderButtonIsEnableByAccountSlot(const QString&, bool)), Qt::QueuedConnection);
					}
					else
					{
                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::tradingDataResult No Data!";
					}
				}
				else
                    this->_isNeedLoadTradingData = true;
			}
			else
			{
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::tradingDataResultCompleted initMultiAccountTradingData  start..";
				this->_tradingManager->initTradingData(this->_tradingDataXmlResult->getByteArrayResult());
				this->_tradingDataXmlResult->clearByteArray();
				this->loadMultiAccountTradingData();
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::tradingDataResultCompleted initMultiAccountTradingData end..";
			}
		}
		for (int i = 0; i < this->_currentLoadAccountIds.count(); i++)
		{
			this->_unLoadTradingAccountIds.removeOne(this->_currentLoadAccountIds[i]);
		}
		this->_currentLoadAccountIds.clear();
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::tradingDataResultCompleted end";
	}

	void BusinessManager::notifyReceivedSlot(const QDomElement& xmlNotify)
	{
		if (Common::TraderBaseInfo::getInstance()->getIsRefreshSystem()) return;
		QString name = xmlNotify.nodeName();
        qDebug() << "BusinessManager::notifyReceivedSlot start " << name << " ThreadId:" << QThread::currentThreadId();
        QLOG_INFO(Common::Constants::LogKeyConstants::Business)<<"BusinessManager::notifyReceivedSlot start name:" << name<< " ThreadId:" << QThread::currentThreadId();
		if (name == "SettingSource")
		{
            QMutexLocker locker(&BusinessManager::m_Mutex);
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot updateSettingSourceData start";
			this->_serverSourceManager->updateSettingSourceData(xmlNotify);
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot updateSettingSourceData end";
            if (Common::TraderBaseInfo::getInstance()->getIsRefreshSystem() || this->_sleepStatus) return;
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot serverSourceManager calculate start";
			this->_serverSourceManager->calculate();
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot serverSourceManager calculate end";
			this->_tradingManager->updateBussnessAccountInstrumentByUpdateCommand();
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot serverSourceManager updateBussnessAccountInstrumentByUpdateCommand end";
			Common::SignalForwarder::getInstance()->sendUpdateCommandSignal();
			if (Common::StatusManager::getInstance()->isComputingCompleted())
            {
                this->computeQuotationSlot(this->_initializeManager->getInstrumentKeyIds());
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot computingManager computeAll start";
				this->_computingManager->computeAll();
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot tradingManager computeAllMarketingGroupFund start";
                this->_tradingManager->computeAllMarketingGroupFund();
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot tradingManager computeAllMarketingGroupFund end";
            }
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot refreshPhysicalButtonIsEnableSlot start";
			this->refreshPhysicalButtonIsEnableSlot();
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot tradingManager refreshOpenOrderButtonIsEnable start";
            this->_tradingManager->refreshOpenOrderButtonIsEnable();
		}
		if (name == "SettingUpdate")
		{
            QMutexLocker locker(&BusinessManager::m_Mutex);
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot updateSettingData start";
			this->_settingManager->updateSettingData(xmlNotify);
            if (Common::TraderBaseInfo::getInstance()->getIsRefreshSystem() || this->_sleepStatus) return;
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot serverSourceManager calculate start";
			this->_serverSourceManager->calculate();
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot tradingManager updateBussnessAccountInstrumentByUpdateCommand start";
			this->_tradingManager->updateBussnessAccountInstrumentByUpdateCommand();
			Common::SignalForwarder::getInstance()->sendUpdateCommandSignal();
			if (Common::StatusManager::getInstance()->isComputingCompleted())
			{
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot computingManager computeAll start";
				this->_computingManager->computeAll();
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot tradingManager computeAllMarketingGroupFund start";
				this->_tradingManager->computeAllMarketingGroupFund();
			}
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot refreshPhysicalButtonIsEnableSlot start";
			this->refreshPhysicalButtonIsEnableSlot();
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot refreshOpenOrderButtonIsEnable start";
			this->_tradingManager->refreshOpenOrderButtonIsEnable();
		}
		else if (name == "Account")
		{
			if (Common::StatusManager::getInstance()->isComputingCompleted())
			{
				QMutexLocker locker(&BusinessManager::m_Mutex);
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot updateTradingDataByNotifyReceived start";
				this->_tradingManager->updateTradingDataByNotifyReceived(xmlNotify);
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot updateTradingDataByNotifyReceived end";
			}
		}
		else if (name == "Messages" || name == "Newses")
		{
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot updateChatOrNews start";
			this->_settingManager->updateChatOrNews(xmlNotify);
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot updateChatOrNews end";
		}
		else if (name == "KickoutCommand")
		{
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot KickoutCommand start";
			Common::SignalForwarder::getInstance()->sendKickoutSignal();
		}
		else if (name == "PriceAlerts")
		{
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot updatePriceAlerts start";
			this->_settingManager->updatePriceAlerts(xmlNotify);
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot updatePriceAlerts end";
		}
		else if (name == "Log")
		{
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot Log start";
			QUuid id = QUuid(xmlNotify.toElement().attribute("id"));
            if (!this->_logResults.contains(id) && !id.isNull())
			{
				QSharedPointer<Common::Result::XmlDataResult> getLogResult = Service::ServiceManager::getInstance()->getTradeService()->getLogData(id.toString().mid(1, 36),
					Common::TraderBaseInfo::getInstance()->getLanguage());
				this->_logResults.insert(id, getLogResult);
				connect(getLogResult.data(), SIGNAL(completed()), this, SLOT(getLogResultCompletedSlot()));
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot C++ Log end";
			}
            QUuid qtAppId = QUuid(xmlNotify.toElement().attribute("QtAppId"));
            if (!this->_logResults.contains(qtAppId) && !qtAppId.isNull())
            {
                QSharedPointer<Common::Result::XmlDataResult> getLogResult = Service::ServiceManager::getInstance()->getTradeService()->getLogData(qtAppId.toString().mid(1, 36),
                    Common::TraderBaseInfo::getInstance()->getLanguage());
                this->_logResults.insert(qtAppId, getLogResult);
                connect(getLogResult.data(), SIGNAL(completed()), this, SLOT(getQtAppLogResultCompletedSlot()));
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot mobile Log end";
            }
		}
		else if (name == "RefreshCommand")
		{
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot RefreshCommand start";
			this->setIsRefreshStatus(true);
			Common::SignalForwarder::getInstance()->sendRefreshSystemSignal();
		}
		else if (name == "QuoteAnswer")
		{
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot QuoteAnswer start";
			QString instrumentId = "";
			QString ask = "";
			QString bid = "";
			QString answerLot = "";
			QString quoteLot = "";
			QDomNamedNodeMap map = xmlNotify.attributes();
			for (int i = 0; i < map.count(); i++)
			{
				QDomNode qDomNode = map.item(i);
				QString name = qDomNode.nodeName();
				QString value = Common::fix(qDomNode.nodeValue());
				if (name == "Ask")
					ask = value;
				else if (name == "Bid")
					bid = value;
				else if (name == "QuoteLot")
					answerLot = value;
				else if (name == "AnswerLot")
					quoteLot = value;
				else if (name == "InstrumentId")
					instrumentId = value;
			}
			if (ask != "" && bid != "" && answerLot != "" && quoteLot != "")
			{
				if (instrumentId == "" || this->_requestQuoteMap.contains(instrumentId))
				{
					if (instrumentId != "") this->_requestQuoteMap.removeOne(instrumentId);
					Common::SignalForwarder::getInstance()->sendQuoteCommandSignal(ask, bid, answerLot, quoteLot);					
				}
			}
		}
		else if (name == "InstrumentDailyCloseQuotations")
		{
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot updateDailyCloseQuotation start";
			this->_quotationManager->updateDailyCloseQuotation(xmlNotify, this->_settingManager);
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot updateDailyCloseQuotation end";
		}
		else if (name == "ChartQuotationCommands")
		{
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot ChartQuotationCommands start";
			Common::SignalForwarder::getInstance()->sendChartQuotationsSignal(xmlNotify);
		}
        else if (name == "RefreshForQueryCommand")
        {
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::notifyReceivedSlot RefreshForQueryCommand start";
//<RefreshForQueryCommand UpdateAction=\"1\"><Accounts><Account Id=\"3ce86e06-7c08-421a-92fb-5047cca3e468\" Code=\"DTest002\" Name=\"DTest002\" /></Accounts></RefreshForQueryCommand>
            //UpdateAction::UpdateAction updateAction = static_cast<UpdateAction::UpdateAction>(xmlSettingData.toElement().attribute("UpdateAction").toInt());
            QString accountCode = "";
            QString accountName = "";
            QDomNodeList childlist = xmlNotify.childNodes();
            for (int i = 0; i < childlist.count(); i++)
            {
                QDomNode itemQDomNode = childlist.item(i);
                QString name = itemQDomNode.nodeName();
                if (name == "Accounts")
                {
                    QDomNodeList itemChildlist = itemQDomNode.childNodes();
                    for (int j = 0; j < itemChildlist.count(); j++)
                    {
                        QDomNode itemChildQDomNode = itemChildlist.item(j);
                        QString itemName = itemChildQDomNode.nodeName();
                        if (itemName == "Account")
                        {
                            QDomNamedNodeMap map = itemChildQDomNode.attributes();
                            for (int k = 0; k < map.count(); k++)
                            {
                                QDomNode qDomNode = map.item(k);
                                QString itemChildName = qDomNode.nodeName();
                                QString itemChildValue = qDomNode.nodeValue();
                                if(itemChildName == "Code")
                                    accountCode = itemChildValue;
                                else if(itemChildName == "Name")
                                    accountName = itemChildValue;
                            }
                        }
                    }
                }
            }

            if(accountCode.length() > 0)
                Common::SignalForwarder::getInstance()->sendRefreshSystemByQueryForAddAccountSignal(accountCode, accountName);
        }
	}

	void BusinessManager::quotationReceivedSlot(const QString quotations)
	{
		if (Common::TraderBaseInfo::getInstance()->getIsRefreshSystem()) return;        
        //qDebug()<< "BusinessManager::quotationReceivedSlot:"<< quotations  << " ThreadId:" << QThread::currentThreadId();
		QList<QUuid> instrumentIds = this->_quotationManager->quotationReceived(quotations);
        if (Common::StatusManager::getInstance()->isComputingCompleted() && !this->_sleepStatus)
		{
            //QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::quotationReceivedSlot 1111111111111111start ThreadId:" << QThread::currentThreadId();
            //optimization
            //When calculating, judge whether it is related to the price. If there is, the price will be recalculated. If it does not matter, the account information will not be recalculated, only the OverrideQuotation will be regenerated
            //Currently calculate all
			this->_computingManager->computeAll(instrumentIds);
			this->_tradingManager->computeAllMarketingGroupFund();
			//emit quotation sign to Chart
			this->_tradingManager->refreshPhysicalDataByInstruments(instrumentIds);
			foreach(QUuid instrumentId, instrumentIds)
			{
				this->_tradingManager->calculateInstrumentSummaryByInstrument(instrumentId);
			}
			Common::SignalForwarder::getInstance()->sendQuotationChangeSignal();
            //QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::quotationReceivedSlot 1111111111111111end";
		}
	}

	void BusinessManager::calculateInstrumentSummarySlot()
	{
		if (Common::StatusManager::getInstance()->isTradingDataCompleted())
		{
			this->_tradingManager->calculateInstrumentSummary();
			Common::SignalForwarder::getInstance()->sendCalculateInstrumentSummarySignal();
		}
	}

	void BusinessManager::addPlaceResult(QSharedPointer<Common::Result::PlaceResult> placeResult)
	{
		this->_placeResults.insert(placeResult->getId(), placeResult);
		connect(placeResult.data(), SIGNAL(placeCompleted(QUuid)), this, SLOT(placeResultCompletedSlot(QUuid)));
	}

	void BusinessManager::placeResultCompletedSlot(QUuid id)
	{
		disconnect(sender(), SIGNAL(placeCompleted(QUuid)), this, SLOT(placeResultCompletedSlot(QUuid)));
		QSharedPointer<Common::Result::PlaceResult> placeResult = this->_placeResults.value(id);
		if (placeResult->getTransactionError() == "OK" || placeResult->getTransactionError() == "Action_ShouldAutoFill")
		{
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  " BusinessManager::placeResultCompletedSlot successful id:" << id;
		}
		else
		{
			this->_tradingManager->updateOrderInfo(id, Phase::Canceled, placeResult->getTransactionError());
            QLOG_ERROR(Common::Constants::LogKeyConstants::Business) <<  " BusinessManager::placeResultCompletedSlot:"<< placeResult->getTransactionError();
		}
		this->_placeResults.remove(id);
	}

	void BusinessManager::getLogResultCompletedSlot()
	{		
		Common::Result::XmlDataResult* senderResult = (Common::Result::XmlDataResult*)sender();
		QUuid id = senderResult->getUuid();
		if (this->_logResults.contains(id))
		{
			auto getLogResult = this->_logResults[id];
			if (senderResult->hasError())
			{
				QLOG_ERROR(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::getLogResultCompletedSlot: " << getLogResult->error();
			}
			else
			{
                qDebug() << "BusinessManager::getLogResultCompletedSlot id:" << id;
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  " BusinessManager::getLogResultCompletedSlot id:" << id;
				QDomElement qDomElement = getLogResult->getXmlResult();
                if(!this->_settingManager->isContainLog(id, qDomElement.childNodes().at(0), false))
                {
                    this->_settingManager->addLog(id, qDomElement.childNodes().at(0));
                    Common::SignalForwarder::getInstance()->sendAddLogSignal(id);
                }
			}
			this->_placeResults.remove(id);
		}
		disconnect(sender(), SIGNAL(completed()), this, SLOT(getLogResultCompletedSlot()));
	}

    void BusinessManager::getQtAppLogResultCompletedSlot()
    {
        Common::Result::XmlDataResult* senderResult = (Common::Result::XmlDataResult*)sender();
        QUuid id = senderResult->getUuid();
        if (this->_logResults.contains(id))
        {
            auto getLogResult = this->_logResults[id];
            if (senderResult->hasError())
            {
                QLOG_ERROR(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::getLogResultCompletedSlot: " << getLogResult->error();
            }
            else
            {
                qDebug() << "BusinessManager::getQtAppLogResultCompletedSlot id:" << id;
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) << " BusinessManager::getQtAppLogResultCompletedSlot id:" << id;
                QDomElement qDomElement = getLogResult->getXmlResult();
                if(!this->_settingManager->isContainLog(id, qDomElement.childNodes().at(0), true))
                {
                    this->_settingManager->addQtAppLog(id, qDomElement.childNodes().at(0));
                    Common::SignalForwarder::getInstance()->sendAddQtAppLogSignal(id);
                }
            }
            this->_placeResults.remove(id);
        }
        disconnect(sender(), SIGNAL(completed()), this, SLOT(getQtAppLogResultCompletedSlot()));
    }

	void BusinessManager::refreshPhysicalButtonIsEnableSlot()
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << " BusinessManager::refreshPhysicalButtonIsEnableSlot start";
		this->_tradingManager->refreshPhysicalButtonIsEnable();
	}

    void BusinessManager::refreshOpenOrderButtonIsEnableSlot()
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << " BusinessManager::refreshOpenOrderButtonIsEnableSlot start";
		this->_tradingManager->refreshOpenOrderButtonIsEnable();
	}

    void BusinessManager::refreshOpenOrderButtonIsEnableByAccountSlot(const QString & accountCode, bool isShow)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << " BusinessManager::refreshOpenOrderButtonIsEnableByAccountSlot start accountCode:" << accountCode << " isShow:" << isShow;
		this->_tradingManager->refreshOpenOrderButtonIsEnableByAccount(accountCode, isShow);
	}

	void BusinessManager::computeAllSlot()
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << " BusinessManager::computeAllSlot start";
		this->_computingManager->computeAll();
		this->_tradingManager->computeAllMarketingGroupFund();
	}

	void BusinessManager::computeAccountSlot(QUuid accountId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << " BusinessManager::computeAccountSlot start accountId:" << accountId;
		this->_computingManager->computePart(accountId);
		auto settingAccount = this->_settingManager->getAccount(accountId);
        this->_tradingManager->computeMarketingGroupFund(settingAccount->getSalesId().toString()+settingAccount->getCurrencyId().toString());
	}

	void BusinessManager::computeQuotationSlot(QList<QUuid> instrumentIds)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << " BusinessManager::computeQuotationSlot start";
		this->_computingManager->computeQuotation(instrumentIds);
	}

	void BusinessManager::computeAllSlot(QList<QUuid> instrumentIds)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << " BusinessManager::computeAllSlot start";
		this->_computingManager->computeAll(instrumentIds);
		this->_tradingManager->computeAllMarketingGroupFund();
	}

	void BusinessManager::updateBOCountdownSlot()
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << " BusinessManager::updateBOCountdownSlot start";
		if (Common::TraderBaseInfo::getInstance()->getIsRefreshSystem() || this->_tradingManager.isNull()) return;
		this->_tradingManager->updateBOCountdown();
	}

    void BusinessManager::loginQuotationServerCompleted()
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << " BusinessManager::loginQuotationServerCompleted start";
		disconnect(this->_loginQuotationServerResult.data(), SIGNAL(completed()), this, SLOT(loginQuotationServerCompleted()));
		if (this->_loginQuotationServerResult->hasError())
		{
			Common::SignalForwarder::getInstance()->sendLoginQuotationServerFaildSignal();
		}
		else
		{
			Service::ServiceManager::getInstance()->startQuotationHeartBeat();
		}
	}

	void BusinessManager::changeEmployeeAccountListCurrencySlot(QUuid salesId, QUuid currencyId)
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << " BusinessManager::changeEmployeeAccountListCurrencySlot start salesId:" << salesId << " currencyId:" << currencyId;
		this->_tradingManager->changeMarketingGroupFund(salesId, currencyId);
	}

	void BusinessManager::transactionExpireInspectorSlot()
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << " BusinessManager::transactionExpireInspectorSlot start";
		if (Common::TraderBaseInfo::getInstance()->getIsRefreshSystem() || this->_tradingManager.isNull()) return;
		QList<QUuid> transactionsMayExpried = this->_tradingManager->getTransactionsMayExpried();
		if (transactionsMayExpried.count() > 0)
		{
			QStringList transactionIds;
			foreach(QUuid transactionId, transactionsMayExpried)
			{
				transactionIds.append(transactionId.toString().mid(1, 36));
			}
			this->_verifyTransactionXmlResult = Service::ServiceManager::getInstance()->getTradeService()->verifyTransaction(transactionIds);
			connect(this->_verifyTransactionXmlResult.data(), SIGNAL(completed()), this, SLOT(verifyTransactionResultCompleted()));
		}
	}

	void BusinessManager::verifyTransactionResultCompleted()
	{
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << " BusinessManager::verifyTransactionResultCompleted start";
		disconnect(this->_verifyTransactionXmlResult.data(), SIGNAL(completed()), this, SLOT(verifyTransactionResultCompleted()));
		if (this->_verifyTransactionXmlResult->hasError())
		{
			QLOG_ERROR(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::verifyTransactionResultCompleted: " << this->_verifyTransactionXmlResult->error();
		}
		else
		{
            //QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "verifyTransactionResultCompleted:ThreadId:" << QThread::currentThreadId();
			QList<QUuid> tranIds;
			QDomNode verifyTransactionsQDomNode = this->_verifyTransactionXmlResult->getXmlResult().childNodes().at(0);
			QString name = verifyTransactionsQDomNode.nodeName();
			if (name == "VerifyTransactions")
			{
				QDomNodeList verifyTransactionChildlist = verifyTransactionsQDomNode.childNodes();
				for (int i = 0; i < verifyTransactionChildlist.count(); i++)
				{
					QUuid id = QUuid(verifyTransactionChildlist.at(i).toElement().attribute("Id"));
					tranIds.append(id);
				}
			}
			if (tranIds.count() > 0)
			{
				this->_tradingManager->cancelExpired(tranIds);
			}
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) << " BusinessManager::verifyTransactionResultCompleted end";
		}
	}

    void BusinessManager::changeSingleChannelSlot()
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << " BusinessManager::changeSingleChannelSlot start";
        if(Common::LoginSetting::getInstance()->getUseTraderQuotationServer())
        {
            qint64 initCompletedTime = this->_initCompletedTime.toMSecsSinceEpoch();
            qint64 lastQuotationReceivedTime = this->_quotationManager->getLastQuotationReceivedTime().toMSecsSinceEpoch();

            int interval = (initCompletedTime - lastQuotationReceivedTime);
            qDebug() << " initCompletedTime:" << initCompletedTime
                     << " lastQuotationReceivedTime:" << lastQuotationReceivedTime
                     << " interval:" << interval;
            if (interval == 0)
            {
                if(Service::ServiceManager::getInstance()->isUseTraderQuotationServer())
                {
                    disconnect(Service::ServiceManager::getInstance()->getQuotationService().data(), SIGNAL(quotationReceived(const QString)), this, SLOT(quotationReceivedSlot(const QString)));
                    Service::ServiceManager::getInstance()->getQuotationService()->logout();
                    Service::ServiceManager::getInstance()->stopQuotationServer();
                    if(Service::ServiceManager::getInstance()->channelIsConnected())
                        Service::ServiceManager::getInstance()->getTradeService()->changeSingleChannel();
                    connect(Service::ServiceManager::getInstance()->getQuotationService().data(), SIGNAL(quotationReceived(const QString)), this, SLOT(quotationReceivedSlot(const QString)));
                }
            }
        }
    }

    void BusinessManager::refreshTradingDataResultCompletedSlot()
    {
        disconnect(this->_refreshTradingDataXmlResult.data(), SIGNAL(completed()), this, SLOT(refreshTradingDataResultCompletedSlot()));
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "BusinessManager::refreshTradingDataResultCompletedSlot start";
        bool isSuccess = false;
        if (this->_refreshTradingDataXmlResult->hasError())
        {
            QLOG_ERROR(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::BusinessManager::refreshTradingDataResultCompletedSlot faild: " << this->_refreshTradingDataXmlResult->error();
            qDebug() <<  "BusinessManager::BusinessManager::refreshTradingDataResultCompletedSlot faild: " << this->_refreshTradingDataXmlResult->error();
        }
        else
        {
            QMutexLocker locker(&BusinessManager::m_Mutex);
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::refreshTradingDataResultCompletedSlot initTradingData start..";
            bool isOk = this->_tradingManager->refreshTradingData(this->_refreshTradingDataXmlResult->getByteArrayResult());
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::refreshTradingDataResultCompletedSlot initTradingData end..";
            qDebug() <<  "BusinessManager::BusinessManager::refreshTradingDataResultCompletedSlot isOk: " << isOk;
            if (isOk)
            {
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::refreshTradingDataResultCompletedSlot computeAllSlot start.";
                this->computeAllSlot();
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::refreshTradingDataResultCompletedSlot computeAllSlot end..";
                isSuccess = true;
                qDebug() <<  "BusinessManager::BusinessManager::refreshTradingDataResultCompletedSlot computeAllSlot";
            }
            else
            {
                QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::refreshTradingDataResultCompletedSlot No Data!";
            }
        }
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "BusinessManager::refreshTradingDataResultCompletedSlot end";
        Common::SignalForwarder::getInstance()->sendRefreshTradingDataStatusSignal(isSuccess);
    }


	//---------------------private method---------------------------    
    void BusinessManager::getSettingData()
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "Start getSettingData: ThreadId:" << QThread::currentThreadId();
        this->_settingDataXmlResult = Service::ServiceManager::getInstance()->getTradeService()->getSettingData();
        connect(this->_settingDataXmlResult.data(), SIGNAL(completed()), this, SLOT(settingDataResultCompleted()));
    }

    void BusinessManager::getTradingData(const QStringList &  accountIds)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "Start getTradingData: ThreadId:" << QThread::currentThreadId();
        this->_tradingDataXmlResult = Service::ServiceManager::getInstance()->getTradeService()->getTradingData(accountIds);
        this->_tradingAccountIds = accountIds;
        connect(this->_tradingDataXmlResult.data(), SIGNAL(completed()), this, SLOT(tradingDataResultCompleted()));
    }

	void BusinessManager::startComputing()
    {
		if (Common::StatusManager::getInstance()->isSettingDataCompleted() && Common::StatusManager::getInstance()->isTradingDataCompleted())
		{
			this->_computingManager->updateOrAddComputingModel(this->_tradingManager->getOpenOrders());
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "startComputing initPhysical start.";
			this->initPhysical();
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "startComputing initPhysical end.";
			if(Common::TraderBaseInfo::getInstance()->getIsEmployee())
				this->_tradingManager->computeAllMarketingGroupFund();
            Common::StatusManager::getInstance()->setComputingStatus(true);
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "startComputing startComputing end.";

            this->_initCompletedTime = QDateTime::currentDateTime();
            this->_quotationManager->setLastQuotationReceivedTime(this->_initCompletedTime);
            int noQuotationChangeSingleChannelTime = Common::AppSetting::getInstance()->getNoQuotationChangeSingleChannelTime();
            QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "startComputing startComputing noQuotationChangeSingleChannelTime:" << noQuotationChangeSingleChannelTime;
            QTimer::singleShot(noQuotationChangeSingleChannelTime*1000, this, SLOT(changeSingleChannelSlot()));
		}
		Common::SignalForwarder::getInstance()->sendStopProcessBarSignal();
	}

	void BusinessManager::initPhysical()
	{
		this->_tradingManager->refreshInventory();
		this->_tradingManager->refreshPendingInventory();
		this->_tradingManager->refreshShortSell();
	}

    void BusinessManager::loadMultiAccountTradingData()
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "loadMultiAccountTradingData start";
        qDebug() << "BusinessManager::loadMultiAccountTradingData ThreadId:" << QThread::currentThreadId();
        if (this->_unLoadTradingAccountIds.count() <= this->_loadAccountCount)
        {
            this->_isCompleteInitTradingData = true;//Set this state to true in the last stroke, it will initialize the remaining calculations and other methods
        }
        QStringList accountIds;
        int loadCount = this->_unLoadTradingAccountIds.count() <= this->_loadAccountCount ? this->_unLoadTradingAccountIds.count() : this->_loadAccountCount;
        for (int i = 0; i < loadCount; i++)
        {
            this->_currentLoadAccountIds.append(this->_unLoadTradingAccountIds.at(i));
            accountIds.append(this->_unLoadTradingAccountIds.at(i).toString().mid(1, 36));
        }
        this->getTradingData(accountIds);
        QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "loadMultiAccountTradingData end";
    }

    void BusinessManager::resume()
    {
        if (Common::StatusManager::getInstance()->isComputingCompleted())
        {
            this->_serverSourceManager->calculate();
            this->_tradingManager->updateBussnessAccountInstrumentByUpdateCommand();
            Common::SignalForwarder::getInstance()->sendUpdateCommandSignal();
            if (Common::StatusManager::getInstance()->isComputingCompleted())
            {
                this->_computingManager->computeAll();
                this->_tradingManager->computeAllMarketingGroupFund();
            }
            this->refreshPhysicalButtonIsEnableSlot();
            this->_tradingManager->refreshOpenOrderButtonIsEnable();


            this->_computingManager->computeAll();
            this->_tradingManager->computeAllMarketingGroupFund();
            //emit quotation sign to Chart
            this->_tradingManager->refreshPhysicalData();
            this->_tradingManager->calculateInstrumentSummary();
            Common::SignalForwarder::getInstance()->sendQuotationChangeSignal();
        }
    }


	//---------------------public method---------------------------
	QSharedPointer<SettingManager> BusinessManager::getSettingManager()
	{
		//QMutexLocker locker(&this->_mutex);
		return this->_settingManager;
	}

	QSharedPointer<InitializeManager> BusinessManager::getInitializeManager()
	{
		//QMutexLocker locker(&this->_mutex);
		return this->_initializeManager;
	}

	QSharedPointer<TradingManager> BusinessManager::getTradingManager()
	{
		//QMutexLocker locker(&this->_mutex);
		return this->_tradingManager;
	}

	QSharedPointer<ComputingManager> BusinessManager::getComputingManager()
	{
		//QMutexLocker locker(&this->_mutex);
		return this->_computingManager;
	}

	QSharedPointer<QuotationManager> BusinessManager::getQuotationManager()
	{
		//QMutexLocker locker(&this->_mutex);
		return this->_quotationManager;
	}

	QSharedPointer<ServerSourceManager> Business::BusinessManager::getServerSourceManager()
	{
		return this->_serverSourceManager;
	}

	void BusinessManager::clearData()
	{
        QMutexLocker locker(&BusinessManager::m_Mutex);

        qDebug() << "BusinessManager::clearData()";
        disconnect(Service::ServiceManager::getInstance()->getTradeService().data(), SIGNAL(notifyReceived(const QDomElement&)), this, SLOT(notifyReceivedSlot(const QDomElement&)));
        disconnect(Service::ServiceManager::getInstance()->getQuotationService().data(), SIGNAL(quotationReceived(const QString)), this, SLOT(quotationReceivedSlot(const QString)));
		disconnect(Common::StatusManager::getInstance(), SIGNAL(initializeDataCompletedSignal()), this, SLOT(startGetSettingAndTradingDataSlot()));
		disconnect(Common::SignalForwarder::getInstance(), SIGNAL(refreshPhysicalButtonIsEnableSignal()), this, SLOT(refreshPhysicalButtonIsEnableSlot()));
		disconnect(Common::SignalForwarder::getInstance(), SIGNAL(computeAllSignal()), this, SLOT(computeAllSlot()));
		disconnect(Common::SignalForwarder::getInstance(), SIGNAL(computeAccountSignal(QUuid)), this, SLOT(computeAccountSlot(QUuid)));
		disconnect(Common::SignalForwarder::getInstance(), SIGNAL(computeQuotationSignal(QList<QUuid>)), this, SLOT(computeQuotationSlot(QList<QUuid>)));
		disconnect(Common::SignalForwarder::getInstance(), SIGNAL(computeAllSignal(QList<QUuid>)), this, SLOT(computeAllSlot(QList<QUuid>)));
		disconnect(Common::StatusManager::getInstance(), SIGNAL(showAccountOrdersSignal(const QString&, bool)), this, SLOT(refreshOpenOrderButtonIsEnableByAccountSlot(const QString&, bool)));

		this->_placeResults.clear();

		QSharedPointer<QuotationManager>  newQuotationManager(new QuotationManager);
		this->_quotationManager = newQuotationManager;
		QSharedPointer<SettingManager>  newSettingManager(new SettingManager);
		this->_settingManager = newSettingManager;
		QSharedPointer<InitializeManager>  newInitializeManager(new InitializeManager);
		this->_initializeManager = newInitializeManager;
		QSharedPointer<ComputingManager>  newComputingManager(new ComputingManager(this->_settingManager, this->_quotationManager));
		this->_computingManager = newComputingManager;
		QSharedPointer<TradingManager>  newTradingManager(new TradingManager(this->_settingManager, this->_computingManager, this->_initializeManager));
		this->_tradingManager = newTradingManager;
		QSharedPointer<ServerSourceManager>  newServerSourceManager(new ServerSourceManager(this->_settingManager, this->_initializeManager));
		this->_serverSourceManager = newServerSourceManager;
		BusinessHelper::getInstance()->setSettingManager(this->_settingManager);
		this->_isNeedLoadTradingData = false;
        this->_noData = true;
	}

	//////---------------------//////
	/*QMap<QUuid, QSharedPointer<OverridedQuotationComputingResult>> BusinessManager::getOverridedQuotations(QUuid accountId)
	{
	QList<QUuid> instrumentIds = this->_settingManager->getAccount(accountId)->getInstrumentIds();
	return this->_computingManager->getOverridedQuotationComputingResult(accountId,instrumentIds);
	}*/
}

#include "moc_BusinessManager.cpp"
