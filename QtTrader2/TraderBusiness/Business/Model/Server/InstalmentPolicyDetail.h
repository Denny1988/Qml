#ifndef BUSINESS_MODEL_SERVER_INSTALMENTPOLICYDETAIL_H
#define BUSINESS_MODEL_SERVER_INSTALMENTPOLICYDETAIL_H
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
			class InstalmentPolicyDetail
			{
			private:
				QDecNumber _contractTerminateFee;
				ContractTerminateType::ContractTerminateType _contractTerminateType;
                QDecNumber _administrationFee;
                AdministrationFeeBaseType::AdministrationFeeBaseType _administrationFeeBaseType;
				int _downPaymentBasis;
				Frequence::Frequence _frequence;
				QUuid _instalmentPolicyId;
				QDecNumber _interestRate;
				bool _isActive;
				QDecNumber _maxDownPayment;
				QDecNumber _minDownPayment;
                int _period;


			public:
				QDecNumber getContractTerminateFee() const
				{
					return _contractTerminateFee;
				}

				ContractTerminateType::ContractTerminateType getContractTerminateType() const
				{
					return _contractTerminateType;
                }

                QDecNumber getAdministrationFee() const
                {
                    return _administrationFee;
                }

                AdministrationFeeBaseType::AdministrationFeeBaseType getAdministrationFeeBaseType() const
                {
                    return _administrationFeeBaseType;
                }

				int getDownPaymentBasis() const
				{
					return _downPaymentBasis;
				}

				Frequence::Frequence getFrequence() const
				{
					return _frequence;
				}

				QUuid getInstalmentPolicyId() const
				{
					return _instalmentPolicyId;
				}

				QDecNumber getInterestRate() const
				{
					return _interestRate;
				}

				bool getIsActive() const
				{
					return _isActive;
				}

				QDecNumber getMaxDownPayment() const
				{
					return _maxDownPayment;
				}

				QDecNumber getMinDownPayment() const
				{
					return _minDownPayment;
				}

				int getPeriod() const
				{
					return _period;
				}
				
				void setContractTerminateFee(const QDecNumber& value)
				{
					_contractTerminateFee = value;
				}

				void setContractTerminateType(const ContractTerminateType::ContractTerminateType& value)
				{
					_contractTerminateType = value;
				}

                void setAdministrationFee(const QDecNumber& value)
                {
                    _administrationFee = value;
                }

                void setAdministrationFeeBaseType(const AdministrationFeeBaseType::AdministrationFeeBaseType& value)
                {
                    _administrationFeeBaseType = value;
                }

				void setDownPaymentBasis(const int& value)
				{
					_downPaymentBasis = value;
				}

				void setFrequence(const Frequence::Frequence& value)
				{
					_frequence = value;
				}

				void setInstalmentPolicyId(const QUuid& value)
				{
					_instalmentPolicyId = value;
				}

				void setInterestRate(const QDecNumber& value)
				{
					_interestRate = value;
				}

				void setIsActive(const bool& value)
				{
					_isActive = value;
				}

				void setMaxDownPayment(const QDecNumber& value)
				{
					_maxDownPayment = value;
				}

				void setMinDownPayment(const QDecNumber& value)
				{
					_minDownPayment = value;
				}

				void setPeriod(const int& value)
				{
					_period = value;
				}

				public:
					void update(QXmlStreamReader& xmlStreamReader)
					{
						QXmlStreamAttributes attributes = xmlStreamReader.attributes();
						this->_contractTerminateFee = QDecNumber::fromQString(attributes.value("ContractTerminateFee").toString());
						this->_contractTerminateType = static_cast<ContractTerminateType::ContractTerminateType>(attributes.value("ContractTerminateType").toInt());
                        this->_administrationFee = QDecNumber::fromQString(attributes.value("AdministrationFee").toString());
                        this->_administrationFeeBaseType = static_cast<AdministrationFeeBaseType::AdministrationFeeBaseType>(attributes.value("AdministrationFeeBase").toInt());
                        this->_downPaymentBasis = attributes.value("DownPaymentBasis").toInt();
						this->_frequence = static_cast<Frequence::Frequence>(attributes.value("Frequence").toInt());
						this->_instalmentPolicyId = QUuid(attributes.value("InstalmentPolicyId").toString());
						this->_interestRate = QDecNumber::fromQString(attributes.value("InterestRate").toString());
						this->_isActive = Common::toBool(attributes.value("IsActive").toString());
						this->_maxDownPayment = QDecNumber::fromQString(attributes.value("MaxDownPayment").toString());
						this->_minDownPayment = QDecNumber::fromQString(attributes.value("MinDownPayment").toString());
						this->_period = attributes.value("Period").toInt();

                        qDebug() << " this->_period:" << this->_period <<  " AdministrationFee:" <<  attributes.value("AdministrationFee").toString();
					}

					void update(const QDomNode& xmlNode)
					{
						QDomNamedNodeMap map = xmlNode.attributes();
						for (int i = 0; i < map.count(); i++)
						{
							QDomNode qDomNode = map.item(i);
							QString name = qDomNode.nodeName();
							QString value = qDomNode.nodeValue();
							if (name == "ContractTerminateFee")
								this->_contractTerminateFee = QDecNumber::fromQString(value);
							else if (name == "ContractTerminateType")
								this->_contractTerminateType = static_cast<ContractTerminateType::ContractTerminateType>(value.toInt());
                            else if (name == "AdministrationFee")
                                this->_administrationFee = QDecNumber::fromQString(value);
                            else if (name == "AdministrationFeeBase")
                                this->_administrationFeeBaseType = static_cast<AdministrationFeeBaseType::AdministrationFeeBaseType>(value.toInt());
							else if (name == "DownPaymentBasis")
								this->_downPaymentBasis = value.toInt();
							else if (name == "Frequence")
								this->_frequence = static_cast<Frequence::Frequence>(value.toInt());
							else if (name == "InstalmentPolicyId")
								this->_instalmentPolicyId = QUuid(value);
							else if (name == "InterestRate")
								this->_interestRate = QDecNumber::fromQString(value);
							else if (name == "IsActive")
								this->_isActive = Common::toBool(value);
							else if (name == "MaxDownPayment")
								this->_maxDownPayment = QDecNumber::fromQString(value);
							else if (name == "MinDownPayment")
								this->_minDownPayment = QDecNumber::fromQString(value);
							else if (name == "Period")
								this->_period = value.toInt();
						}
					}
			};
		}
	}
}
#endif

