#ifndef TRANSACTIONENUM_H
#define TRANSACTIONENUM_H
namespace Common
{
	namespace Enum
	{
		namespace TransactionType
		{
			enum TransactionType
			{
				Single,
				Pair,
				OneCancelOther,
				Mapping,
				MultipleClose,
				Assign = 100,//AssigningOrderID == AssigningOrderID (the id of order been assigned from)
			};
		}

		namespace TransactionSubType
		{
			enum TransactionSubType
			{
				None = 0,
				Amend,  //AssigningOrderID == AmendedOrderId (the id of order been amended)
				IfDone, //AssigningOrderID == IfOrderId (the id of order used as condition)
				Match,
				Assign, //AssigningOrderID == AssigningOrderID (id of the order been assigned from) //NotImplemented
				Mapping,
			};
		}

		
	}
}
#endif
