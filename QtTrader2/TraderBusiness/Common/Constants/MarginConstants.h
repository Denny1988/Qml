#ifndef MARGINCONSTANTS_H
#define MARGINCONSTANTS_H

#include <QString>
#include "traderbusiness_global.h"
namespace Common 
{
	namespace Constants
	{
		class TRADERBUSINESS_EXPORT MarginConstants
		{
		public:
			static const QString OwnerRegistration;
			static const QString AgentRegistration;
			static const QString MarginCallExtension;
			static const QString PaymentInstruction;
			static const QString PaymentInstructionCash;
			static const QString PaymentInstructionTransfer;
			static const QString Deposit;
			static const QString BankAccount;
		};
	}
}
#endif

