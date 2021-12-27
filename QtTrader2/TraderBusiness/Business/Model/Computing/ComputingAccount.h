#ifndef BUSINESS_MODEL_COMPUTING_COMPUTINGACCOUNT_H
#define BUSINESS_MODEL_COMPUTING_COMPUTINGACCOUNT_H
#include <QUuid>
#include <qdatetime.h>
#include "ComputingInstrument.h"
#include <QMap>

namespace Business
{
	namespace Model
	{
		namespace Computing
		{
			struct AccountMapping {
				QUuid accountId;
				QUuid instrumentId;
			};

			class ComputingAccount : public DemandComputing
			{
			public:
				QUuid _accountId;

			protected:
				QSharedPointer<DemandComputingResult> doCompute();
			};
		}
	}
}
#endif
