#ifndef BUSINESS_COMPUTINGMANAGER_H
#define BUSINESS_COMPUTINGMANAGER_H
#include <QDomNode>
#include <QSharedPointer>
#include "Business/Model/Computing/ComputingAccount.h"
#include "Business/Model/Computing/ComputingInstrument.h"
#include "Business/Model/Computing/ComputingOrder.h"
#include "Business/Model/Computing/OverridedQuotation.h"
#include "Business/Model/InitialQuotation.h"
#include "traderbusiness_global.h"

using namespace Business::Model::Computing;

namespace Business
{
	class SettingManager;
	class QuotationManager;
	class TRADERBUSINESS_EXPORT ComputingManager
	{		
	private:
		QMap<QUuid, QSharedPointer<ComputingAccount>> _computingAccounts;
		QMap<QUuid, AccountMapping> _orderMapping;
		QSharedPointer<SettingManager> _settingManager;
		QSharedPointer<QuotationManager> _quotationManager;

		static QMutex  m_Mutex;

	public:
		ComputingManager(QSharedPointer<SettingManager>& settingManager, QSharedPointer<QuotationManager>& quotationManager);
		QMap<QString, QSharedPointer<OverridedQuotation>> _computingOverridedQuotations;//QString = accountId + InstrumentId

	public:
		QMap<QUuid, QSharedPointer<AccountComputingResult>> getAccountComputingResults(QList<QUuid> accountIds);
		QSharedPointer<AccountComputingResult> getAccountComputingResult(QUuid accountId);
		QMap<QUuid, QSharedPointer<OrderComputingResult>> getOrderComputingResults(QList<QUuid> orderIds);
		QSharedPointer<OrderComputingResult> getOrAddOrderComputingResult(QSharedPointer<Model::Trading::TradingOrder> order);
		QSharedPointer<OverridedQuotationComputingResult> getOverridedQuotationComputingResult(QUuid accountId, QUuid instrumentId);
		QSharedPointer<OverridedQuotationComputingResult> getOverridedQuotationComputingResultByQuotePolicyId(QUuid quotePolicyId, QUuid instrumentId);
		QSharedPointer<OverridedQuotationComputingResult> getOverridedQuotationComputingResultByDefaultQuotePolicy(QUuid instrumentId);
		bool hasOverridedQuotationComputingResultByQuotePolicyId(QUuid quotePolicyId, QUuid instrumentId);

		//QMap<QUuid,QUuid>
		void updateOrAddComputingModel(QList<QSharedPointer<Model::Trading::TradingOrder>> orderList);
		void InitializeQuotation(QList<QSharedPointer<Model::InitialQuotation>> initialQuotationList);
		void computeAll();
		void computeAll(QList<QUuid> instrumentIds);
		void computePart(QList<QUuid> accountIds, bool isLoad = false);
		void computePart(QUuid accountId);
		void computeQuotation(QList<QUuid> instrumentIds);
		void computeQuotationOrAddComputingQuotetation(QSharedPointer<Model::InitialQuotation> quotation);
		void addComputingModel(QSharedPointer<Model::Trading::TradingOrder> tradingOrder);
		void deleteComputingModel(QSharedPointer<Model::Trading::TradingOrder> tradingOrder);

	private:
		bool isComputForQuotation(QUuid accountId);
		void addComputingQuotetation(QString key, QSharedPointer<Model::InitialQuotation> ouotation);

	};

}
#endif
