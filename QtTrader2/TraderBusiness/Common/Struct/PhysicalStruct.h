#ifndef PHYSICALSTRUCT_H
#define PHYSICALSTRUCT_H
#include <QUuid>
#include <qdatetime.h>
#include <QSharedPointer>
#include "QDecNumber.h"
#include "Common/Enum/InstalmentEnum.h"

using namespace Common::Enum;

namespace Common
{
	namespace Struct
	{
		struct InstalmentDetailItem
		{
			QString  _period;
			QString  _repaymentDueDate;
			QString  _principalInstalment;
			QString  _interestInstalment;
			QString  _instalmentAmount;
			QString  _dateOfActualRepayment;
			QString  _penaltyInterest;
			QString  _status;
			QString  _currencyName;
		};

		struct InstalmentDetail
		{
			Frequence::Frequence  _frequence;
			QString  _noOfInstalment;
			QString  _paidInstalment;
			QString  _unpaidInstalment;
			QString  _noOfLateInstalment;
			QString  _noOfPaidLateInstalment;
			QString  _paidPrincipal;
			QString  _unpaidPrincipalBalance;
			QString  _paidInterest;
			QString  _unpaidInterest;
			QString  _penaltyInterest;
			QString  _contractTerminateFee;
			bool  _isContractTerminateFee;
			QList<QSharedPointer<InstalmentDetailItem>>  _instalmentDetailItems;
		};
	}
}
#endif