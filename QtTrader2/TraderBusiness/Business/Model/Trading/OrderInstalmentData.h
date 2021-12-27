#ifndef BUSINESS_MODEL_TRADING_ORDERINSTALMENTDATA_H
#define BUSINESS_MODEL_TRADING_ORDERINSTALMENTDATA_H
#include <quuid.h>
#include "Common/Enum/PlaceEnum.h"
#include "QDecNumber.h"
#include <QDomNode>
#include <QXmlStreamReader>

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			class OrderInstalmentData
			{
			private:
				QString  _period;
				double  _principal;
				double  _interest;
				double  _debitInterest;
				double  _instalmentAmount;
				QString _paymentDateTimeOnPlan;
				QString _paidDateTime;

			public:
				void update(const QXmlStreamReader& xmlStreamReader)
				{	
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();  
					this->_period = attributes.value("Period").toString();
					this->_principal = attributes.value("Principal").toDouble();
					this->_interest = attributes.value("Interest").toDouble();
					this->_debitInterest = attributes.value("DebitInterest").toDouble();
					this->_paymentDateTimeOnPlan = attributes.value("PaymentDateTimeOnPlan").toString();
					this->_paidDateTime = attributes.value("PaidDateTime").toString();
					this->_instalmentAmount = this->_principal + this->_interest + this->_debitInterest;
				}

				void update(const QDomNode& xmlOrderInstalmentData)
				{					
					QDomNamedNodeMap map = xmlOrderInstalmentData.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "Period")
							this->_period = value;
						else if(name == "Principal")
							this->_principal = value.toDouble();
						else if(name == "Interest")
							this->_interest = value.toDouble();
						else if(name == "DebitInterest")
							this->_debitInterest = value.toDouble();
						else if (name == "PaymentDateTimeOnPlan")
							this->_paymentDateTimeOnPlan = value;
						else if (name == "PaidDateTime")
							this->_paidDateTime = value;
					}
					this->_instalmentAmount = this->_principal + this->_interest + this->_debitInterest;
				}
			public:

				QString getPeriod() const
				{
					return _period;
				}
				double getPrincipal() const
				{
					return _principal;
				}
				double getInterest() const
				{
					return _interest;
				}
				double getDebitInterest() const
				{
					return _debitInterest;
				}
				double getInstalmentAmount() const
				{
					return _instalmentAmount;
				}
				QString getPaymentDateTimeOnPlan() const
				{
					return _paymentDateTimeOnPlan != "" ? _paymentDateTimeOnPlan.left(10) : "";
				}
				QString getPaidDateTime() const
				{
					return _paidDateTime != "" ? _paidDateTime : "";
				}

				

			};
		}
	}
}
#endif
