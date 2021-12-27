#ifndef MARGINENUM_H
#define MARGINENUM_H

namespace Common
{
	namespace Enum
	{
		namespace AccountBankEditType
		{
			enum AccountBankEditType
			{
				Insert,
				Modify
			};
		}

		namespace MarginApproveResult
		{
			enum MarginApproveResult
			{
				NotApproved,
				Successed,
				NoEnoughBalaceToPay,
				Failed,
                IPSNotReady,
                RunErrorFailed
			};
		}

        namespace PaymentMethod
        {
            enum PaymentMethod
            {
                WireTransferChannel = 1,
                CryptocurrencyTransfer = 2,
                QRCode = 3
            };
        }
	}
}
#endif
