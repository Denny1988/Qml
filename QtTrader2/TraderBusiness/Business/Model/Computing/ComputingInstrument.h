#ifndef BUSINESS_MODEL_COMPUTING_COMPUTINGINSTRUMENT_H
#define BUSINESS_MODEL_COMPUTING_COMPUTINGINSTRUMENT_H
#include <QUuid>
#include <QMap>
#include <QSharedPointer>
#include "ComputingOrder.h"
#include "Business/Model/Setting/BusinessInstrument.h"

namespace Business
{
	namespace Model
	{
		namespace Computing
		{
			class ComputingInstrument : public DemandComputing
			{
			public:
				QUuid _accountId;
				QUuid _instrumentId;

			private:
				bool shouldUseDayNecessary(QDateTime baseTime, bool useNightNecessaryWhenBreak);
				QDecNumber calculateNecessary(QSharedPointer<Model::Setting::BusinessInstrument> settingInstrument, QDecNumber buyMarginSum, QDecNumber sellMarginSum, QDecNumber buySum, QDecNumber sellSum);
				static bool caseInsensitiveLessThan(QSharedPointer<Model::Setting::VolumeNecessaryDetail> v1, QSharedPointer<Model::Setting::VolumeNecessaryDetail> v2);
				QDecNumber calculateVolumeNecessary(QSharedPointer<Model::Setting::BusinessInstrument> settingInstrument, QDecNumber marginRate, QDecNumber defaultMargin, QDecNumber netLot, bool useDayMargin);
				QDecNumber calculatePartialPaymentPhysicalNecessary(QSharedPointer<Model::Setting::BusinessInstrument> settingInstrument, QDecNumber physcialUnPaidBuyMarginSum, QDecNumber physcialUnPaidBuySum, QDecNumber physcialPaidPledgeSellMarginSum, QDecNumber physcialPaidPledgeSellSum);

			protected:
				QSharedPointer<DemandComputingResult> doCompute();
			};
		}
	}
}
#endif
