#ifndef BUSINESS_MODEL_SETTING_INSTALMENTSETTINGDETAIL_H
#define BUSINESS_MODEL_SETTING_INSTALMENTSETTINGDETAIL_H
//#include <quuid.h>
#include "Common/Enum/InstalmentEnum.h"
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include "traderbusiness_global.h"
#include <QDebug>

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class TRADERBUSINESS_EXPORT InstalmentSettingDetail
			{
			private:
				QUuid  _instalmentPolicyId;
				bool _isActive;
				int _period;
				QDecNumber _minDownPayment;
				QDecNumber _maxDownPayment;
				QDecNumber _interestRate;			
				Common::Enum::ContractTerminateType::ContractTerminateType _contractTerminateType;
				QDecNumber _contractTerminateFee;
                Common::Enum::AdministrationFeeBaseType::AdministrationFeeBaseType _administrationFeeBaseType;
                QDecNumber _administratioFee;
				int _downPaymentBasis;
                Common::Enum::Frequence::Frequence _frequence;

			public:
				void update(const QDomNode& xmlInstalmentPolicyDetail)
				{
					QDomNamedNodeMap map = xmlInstalmentPolicyDetail.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "Period")
							this->_period= value.toInt();
						else if(name == "IsActive")
							this->_isActive = Common::toBool(value);
						else if(name == "Frequence")
							this->_frequence= static_cast<Common::Enum::Frequence::Frequence>(value.toInt());
						else if(name == "MinDownPayment")
							this->_minDownPayment= value.toDouble();
						else if(name == "MaxDownPayment")
							this->_maxDownPayment= value.toDouble();
						else if(name == "InterestRate")
							this->_interestRate= value.toDouble();
						else if(name == "ContractTerminateType")
							this->_contractTerminateType= static_cast<Common::Enum::ContractTerminateType::ContractTerminateType>(value.toInt());
						else if(name == "ContractTerminateFee")
							this->_contractTerminateFee= value.toDouble();
                        else if(name == "AdministrationFeeBase")
                            this->_administrationFeeBaseType= static_cast<Common::Enum::AdministrationFeeBaseType::AdministrationFeeBaseType>(value.toInt());
                        else if(name == "AdministratioFee")
                            this->_administratioFee= value.toDouble();
						else if(name == "DownPaymentBasis")                            
							this->_downPaymentBasis= value.toInt();
					}
				}

                QSharedPointer<InstalmentSettingDetail> copy()
                {
                    QSharedPointer<InstalmentSettingDetail> newObject(new InstalmentSettingDetail);
                    newObject->_instalmentPolicyId = _instalmentPolicyId;
                    newObject->_isActive = _isActive;
                    newObject->_period = _period;
                    newObject->_minDownPayment = _minDownPayment;
                    newObject->_maxDownPayment = _maxDownPayment;
                    newObject->_interestRate = _interestRate;
                    newObject->_contractTerminateType = _contractTerminateType;
                    newObject->_contractTerminateFee = _contractTerminateFee;
                    newObject->_administrationFeeBaseType = _administrationFeeBaseType;
                    newObject->_contractTerminateFee = _contractTerminateFee;
                    newObject->_downPaymentBasis = _downPaymentBasis;
                    newObject->_frequence = _frequence;
                    return newObject;
                }


			public:

				QUuid getInstalmentPolicyId() const
				{
				return _instalmentPolicyId;
				}
				
				bool getIsActive() const
				{
					return _isActive;
				}				

				int getPeriod() const
				{
					return _period;
				}

				QDecNumber getMinDownPayment() const
				{
					return _minDownPayment;
				}

				QDecNumber getMaxDownPayment() const
				{
					return _maxDownPayment;
				}

				QDecNumber getInterestRate() const
				{
					return _interestRate;
				}

				Common::Enum::Frequence::Frequence getFrequence() const
				{
					return _frequence;
				}

				Common::Enum::ContractTerminateType::ContractTerminateType getContractTerminateType() const
				{
					return _contractTerminateType;
				}

				QDecNumber getContractTerminateFee() const
				{
					return _contractTerminateFee;
				}

                Common::Enum::AdministrationFeeBaseType::AdministrationFeeBaseType getAdministrationFeeBaseType() const
                {
                    return _administrationFeeBaseType;
                }

                QDecNumber getAdministratioFee() const
                {
                    return _administratioFee;
                }

				int getDownPaymentBasis() const
				{
					return _downPaymentBasis;
				}


				void setContractTerminateFee(const QDecNumber& value)
				{
					_contractTerminateFee = value;
				}

				void setContractTerminateType(const Common::Enum::ContractTerminateType::ContractTerminateType& value)
				{
					_contractTerminateType = value;
				}

                void setAdministratioFee(QDecNumber value)
                {
                    _administratioFee = value;
                }

                void setAdministrationFeeBaseType(const Common::Enum::AdministrationFeeBaseType::AdministrationFeeBaseType& value)
                {
                    _administrationFeeBaseType = value;
                }


				void setDownPaymentBasis(const int& value)
				{
					_downPaymentBasis = value;
				}

				void setFrequence(const Common::Enum::Frequence::Frequence& value)
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





			};
		}
	}
}
#endif
