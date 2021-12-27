#ifndef BUSINESS_MODEL_SERVER_INSTALMENTPOLICY_H
#define BUSINESS_MODEL_SERVER_INSTALMENTPOLICY_H
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include "Common/Enum/InstalmentEnum.h"
#include <QUuid>
#include <QXmlStreamReader>

using namespace Common::Enum;
namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class InstalmentPolicy
			{
			private:
				AdvancePaymentOption::AdvancePaymentOption _advancePayment;
				AllowCloseInstalment::AllowCloseInstalment _allowClose;
				int _allowedInstalmentTypes;
				QString _code;
				QUuid _id;
				bool _isDownPayAsFirstPay;
				int _recalculateRateTypes;


			public:
				AdvancePaymentOption::AdvancePaymentOption getAdvancePayment() const
				{
					return _advancePayment;
				}

				AllowCloseInstalment::AllowCloseInstalment getAllowClose() const
				{
					return _allowClose;
				}

				int getAllowedInstalmentTypes() const
				{
					return _allowedInstalmentTypes;
				}

				QString getCode() const
				{
					return _code;
				}

				QUuid getId() const
				{
					return _id;
				}

				bool getIsDownPayAsFirstPay() const
				{
					return _isDownPayAsFirstPay;
				}

				int getRecalculateRateTypes() const
				{
					return _recalculateRateTypes;
				}

				void setAdvancePayment(const AdvancePaymentOption::AdvancePaymentOption& value)
				{
					_advancePayment = value;
				}

				void setAllowClose(const AllowCloseInstalment::AllowCloseInstalment& value)
				{
					_allowClose = value;
				}

				void setAllowedInstalmentTypes(const int& value)
				{
					_allowedInstalmentTypes = value;
				}

				void setCode(const QString& value)
				{
					_code = value;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setIsDownPayAsFirstPay(const bool& value)
				{
					_isDownPayAsFirstPay = value;
				}

				void setRecalculateRateTypes(const int& value)
				{
					_recalculateRateTypes = value;
				}

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_advancePayment = static_cast<AdvancePaymentOption::AdvancePaymentOption>(attributes.value("AdvancePayment").toInt());
					this->_allowClose = static_cast<AllowCloseInstalment::AllowCloseInstalment>(attributes.value("AllowClose").toInt());
					this->_allowedInstalmentTypes = attributes.value("AllowedInstalmentTypes").toInt();
					this->_code = attributes.value("Code").toString();
					this->_id = QUuid(attributes.value("Id").toString());
					this->_isDownPayAsFirstPay = Common::toBool(attributes.value("IsDownPayAsFirstPay").toString());
					this->_recalculateRateTypes = attributes.value("RecalculateRateTypes").toInt();
				}

				void update(const QDomNode& xmlNode)
				{
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if (name == "AdvancePayment")
							this->_advancePayment = static_cast<AdvancePaymentOption::AdvancePaymentOption>(value.toInt());
						else if (name == "AllowClose")
							this->_allowClose = static_cast<AllowCloseInstalment::AllowCloseInstalment>(value.toInt());
						else if (name == "AllowedInstalmentTypes")
							this->_allowedInstalmentTypes = value.toInt();
						else if (name == "Code")
							this->_code = value;
						else if (name == "Id")
							this->_id = QUuid(value);
						else if (name == "IsDownPayAsFirstPay")
							this->_isDownPayAsFirstPay = Common::toBool(value);
						else if (name == "RecalculateRateTypes")
							this->_recalculateRateTypes = value.toInt();
					}
				}

			};

		}
	}
}
#endif

