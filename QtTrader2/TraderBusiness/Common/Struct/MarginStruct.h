#ifndef MARGINSTRUCT_H
#define MARGINSTRUCT_H
#include <QUuid>
#include <qdatetime.h>
#include <QSharedPointer>
#include "QDecNumber.h"
#include <QByteArray>
#include <QMap>
#include "Common/Enum/MarginEnum.h"

using namespace Common::Enum;

namespace Common
{
	namespace Struct
	{
        struct OnlineTransferParameter
		{
            QUuid  _id;
            PaymentMethod::PaymentMethod  _payType;
            QString  _merchantAccountId;
            int  _showOrder;
            double  _minPayAmount;
            double _maxPayAmount;
            QUuid _payCurrencyId;
            QString  _payCurrencyCode;
            QString  _payCurrencyName;
            QUuid _payTargetCurrencyId;
            QString  _payTargetCurrencyCode;
            QString  _payTargetCurrencyName;
            QString _editTargetAmountPercent;
            int _marginRequestAutoCancelTimeM;
            QString _payAddress;
            QByteArray _QRCode;
            QString _title;
            QString _name;
            QString _accountName;
            QString _bankName;
            QString _accountNumber;
            QString _bankAddress;
            QString _bankSwiftCode;
            QString _depositPin;
		};
	}
}
#endif
