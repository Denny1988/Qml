#ifndef BUSINESS_MODEL_SERVER_PHYSICALPAYMENTDISCOUNT_H
#define BUSINESS_MODEL_SERVER_PHYSICALPAYMENTDISCOUNT_H
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include "Common/Enum/PhysicalEnum.h"
#include <QUuid>
#include <QXmlStreamReader>

using namespace Common::Enum;
namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class PhysicalPaymentDiscount
			{
			private:
				AllowedPaymentForm::AllowedPaymentFormFlags _allowedPaymentForm;
				QString _code;
				DiscountBasis::DiscountBasis _discountBasis;
				QUuid _id;
				DiscountOption::DiscountOption _option;


			public:
				AllowedPaymentForm::AllowedPaymentFormFlags getAllowedPaymentForm() const
				{
					return _allowedPaymentForm;
				}

				QString getCode() const
				{
					return _code;
				}

				DiscountBasis::DiscountBasis getDiscountBasis() const
				{
					return _discountBasis;
				}

				QUuid getId() const
				{
					return _id;
				}

				DiscountOption::DiscountOption getOption() const
				{
					return _option;
				}


				void setAllowedPaymentForm(const AllowedPaymentForm::AllowedPaymentFormFlags& value)
				{
					_allowedPaymentForm = value;
				}

				void setCode(const QString& value)
				{
					_code = value;
				}

				void setDiscountBasis(const DiscountBasis::DiscountBasis& value)
				{
					_discountBasis = value;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setOption(const DiscountOption::DiscountOption& value)
				{
					_option = value;
				}

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_allowedPaymentForm = static_cast<AllowedPaymentForm::AllowedPaymentFormFlags>(attributes.value("AllowedPaymentForm").toInt());
					this->_code = attributes.value("Code").toString();
					this->_discountBasis = static_cast<DiscountBasis::DiscountBasis>(attributes.value("DiscountBasis").toInt());
					this->_id = QUuid(attributes.value("Id").toString());
					this->_option = static_cast<DiscountOption::DiscountOption>(attributes.value("Option").toInt());
				}

				void update(const QDomNode& xmlNode)
				{
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if (name == "AllowedPaymentForm")
							this->_allowedPaymentForm = static_cast<AllowedPaymentForm::AllowedPaymentFormFlags>(value.toInt());
						else if (name == "Code")
							this->_code = value;
						else if (name == "DiscountBasis")
							this->_discountBasis = static_cast<DiscountBasis::DiscountBasis>(value.toInt());
						else if (name == "Id")
							this->_id = QUuid(value);
						else if (name == "Option")
							this->_option = static_cast<DiscountOption::DiscountOption>(value.toInt());
					}
				}



			};
		}
	}
}
#endif

