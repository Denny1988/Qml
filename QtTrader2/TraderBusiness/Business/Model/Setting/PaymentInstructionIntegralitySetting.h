#ifndef BUSINESS_MODEL_SETTING_PAYMENTINSTRUCTIONINTEGRALITYSETTING_H
#define BUSINESS_MODEL_SETTING_PAYMENTINSTRUCTIONINTEGRALITYSETTING_H
#include <QDomNode>
#include "Common/Util/Convert_Common.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class PaymentInstructionIntegralitySetting
			{
			public:
				PaymentInstructionIntegralitySetting()
				{
					_emailAllowNull = true;
					_amountAllowNull = true;
					_beneficiaryNameAllowNull = true;
					_bankAccountNoAllowNull = true;
					_bankNameAllowNull = true;
					_bankAddressAllowNull = true;
				}
			private:
				bool _emailAllowNull;
				bool _amountAllowNull;
				bool _beneficiaryNameAllowNull;
				bool _bankAccountNoAllowNull;
				bool _bankNameAllowNull;
				bool _bankAddressAllowNull;
			public:
				void update(const QDomNode& xmlNode)
				{
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "EmailAllowNull")
							this->_emailAllowNull= Common::toBool(value);
						else if(name == "AmountAllowNull")
							this->_amountAllowNull= Common::toBool(value);
						else if(name == "BeneficiaryNameAllowNull")
							this->_beneficiaryNameAllowNull= Common::toBool(value);
						else if(name == "BankAccountNoAllowNull")
							this->_bankAccountNoAllowNull= Common::toBool(value);
						else if(name == "BankNameAllowNull")
							this->_bankNameAllowNull= Common::toBool(value);
						else if(name == "BankAddressAllowNull")
							this->_bankAddressAllowNull= Common::toBool(value);
					}
				}

			public:	
				bool getEmailAllowNull() const
				{
					return _emailAllowNull;
				}

				bool getAmountAllowNull() const
				{
					return _amountAllowNull;
				}

				bool getBeneficiaryNameAllowNull() const
				{
					return _beneficiaryNameAllowNull;
				}

				bool getBankAccountNoAllowNull() const
				{
					return _bankAccountNoAllowNull;
				}

				bool getBankNameAllowNull() const
				{
					return _bankNameAllowNull;
				}

				bool getBankAddressAllowNull() const
				{
					return _bankAddressAllowNull;
				}

			};
		}
	}
}
#endif

