#ifndef BUSINESS_MODEL_SERVER_CUSTOMER_H
#define BUSINESS_MODEL_SERVER_CUSTOMER_H
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include "Common/Enum/AccountOrderTypeEnum.h"
#include <QUuid>
#include "Common/Constants/DateTimeConstants.h"
#include "Common/SignalForwarder.h"
#include <QXmlStreamReader>

using namespace Common::Enum;
using namespace Common::Constants;
namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class Customer
			{
			public:
				Customer()
				{
					this->_dealingPolicyId = QUuid();
				}

			private:
				int _dQOrderOutTime;
				QUuid _dealingPolicyId;
				bool _disallowTrade;
				bool _enableFastTrade;
				QUuid _id;
                QString _code;
				bool _isCalculateFloat;
				bool _isDisplayLedger;
				bool _isEmployee;
				bool _isNoShowAccountStatus;
				bool _isSptReconfirm;
				AccountOrderType::AccountOrderType _multiAccountsOrderType;
				bool _needSelectAccount;
				QUuid _privateQuotePolicyId;
				QUuid _publicQuotePolicyId;
				bool _showLog;
				AccountOrderType::AccountOrderType _singleAccountOrderType;
				int   _displayAlert;
                bool _isIexCrmUser;
                bool _isUploadClientLog;

			public:
				int getDQOrderOutTime() const
				{
					return _dQOrderOutTime;
				}

				QUuid getDealingPolicyId() const
				{
					return _dealingPolicyId;
				}

				bool getDisallowTrade() const
				{
					return _disallowTrade;
				}

				bool getEnableFastTrade() const
				{
					return _enableFastTrade;
				}

				QUuid getId() const
				{
					return _id;
				}

                QString getCode() const
                {
                    return _code;
                }

				bool getIsCalculateFloat() const
				{
					return _isCalculateFloat;
				}

				bool getIsDisplayLedger() const
				{
					return _isDisplayLedger;
				}

				bool getIsEmployee() const
				{
					return _isEmployee;
				}

				bool getIsNoShowAccountStatus() const
				{
					return _isNoShowAccountStatus;
				}

				bool getIsSptReconfirm() const
				{
					return _isSptReconfirm;
				}
				
				AccountOrderType::AccountOrderType getMultiAccountsOrderType() const
				{
					return _multiAccountsOrderType;
				}

				bool getNeedSelectAccount() const
				{
					return _needSelectAccount;
				}

				QUuid getPrivateQuotePolicyId() const
				{
					return _privateQuotePolicyId;
				}

				QUuid getPublicQuotePolicyId() const
				{
					return _publicQuotePolicyId;
				}

				bool getShowLog() const
				{
					return _showLog;
				}

				AccountOrderType::AccountOrderType getSingleAccountOrderType() const
				{
					return _singleAccountOrderType;
				}

				int getDisplayAlert() const
				{
					return _displayAlert;
				}

                bool getIsIexCrmUser() const
                {
                    return _isIexCrmUser;
                }

                bool getIsUploadClientLog() const
                {
                    return _isUploadClientLog;
                }



				void setDQOrderOutTime(const int& value)
				{
					_dQOrderOutTime = value;
				}

				void setDealingPolicyId(const QUuid& value)
				{
					_dealingPolicyId = value;
				}

				void setDisallowTrade(const bool& value)
				{
					_disallowTrade = value;
				}

				void setEnableFastTrade(const bool& value)
				{
					_enableFastTrade = value;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

                void setCode(const QString& value)
                {
                    _code = value;
                }

				void setIsCalculateFloat(const bool& value)
				{
					_isCalculateFloat = value;
				}

				void setIsDisplayLedger(const bool& value)
				{
					_isDisplayLedger = value;
				}

				void setIsEmployee(const bool& value)
				{
					_isEmployee = value;
				}

				void setIsNoShowAccountStatus(const bool& value)
				{
					_isNoShowAccountStatus = value;
				}

				void setIsSptReconfirm(const bool& value)
				{
					_isSptReconfirm = value;
				}
				
				void setMultiAccountsOrderType(const AccountOrderType::AccountOrderType& value)
				{
					_multiAccountsOrderType = value;
				}

				void setNeedSelectAccount(const bool& value)
				{
					_needSelectAccount = value;
				}

				void setPrivateQuotePolicyId(const QUuid& value)
				{
					_privateQuotePolicyId = value;
				}

				void setPublicQuotePolicyId(const QUuid& value)
				{
					_publicQuotePolicyId = value;
				}

				void setShowLog(const bool& value)
				{
					_showLog = value;
				}

				void setSingleAccountOrderType(const AccountOrderType::AccountOrderType& value)
				{
					_singleAccountOrderType = value;
				}

				void setDisplayAlert(const int& value)
				{
					_displayAlert = value;
				}

                void setIsIexCrmUser(const bool& value)
                {
                    _isIexCrmUser = value;
                }

                void setIsUploadClientLog(const bool& value)
                {
                    _isUploadClientLog = value;
                }



				public:
					void update(QXmlStreamReader& xmlStreamReader)
					{
						QXmlStreamAttributes attributes = xmlStreamReader.attributes();
						this->_dQOrderOutTime = attributes.value("DQOrderOutTime").toInt();
						this->_dealingPolicyId = attributes.value("DealingPolicyId").toString() != "" ? QUuid(attributes.value("DealingPolicyId").toString()) : QUuid();
						this->_disallowTrade = Common::toBool(attributes.value("DisallowTrade").toString());
						this->_enableFastTrade = Common::toBool(attributes.value("EnableFastTrade").toString());
						this->_id = QUuid(attributes.value("Id").toString());
                        this->_code = attributes.hasAttribute("Code") ? attributes.value("Code").toString() : "";
						this->_isCalculateFloat = Common::toBool(attributes.value("IsCalculateFloat").toString());
						this->_isDisplayLedger = Common::toBool(attributes.value("IsDisplayLedger").toString());
						this->_isEmployee = Common::toBool(attributes.value("IsEmployee").toString());
						this->_isNoShowAccountStatus = Common::toBool(attributes.value("IsNoShowAccountStatus").toString());
						this->_isSptReconfirm = Common::toBool(attributes.value("IsSptReconfirm").toString());
						this->_multiAccountsOrderType = static_cast<AccountOrderType::AccountOrderType>(attributes.value("MultiAccountsOrderType").toInt());
						this->_needSelectAccount = Common::toBool(attributes.value("NeedSelectAccount").toString());
						this->_privateQuotePolicyId = QUuid(attributes.value("PrivateQuotePolicyId").toString());
						this->_publicQuotePolicyId = QUuid(attributes.value("PublicQuotePolicyId").toString());
						this->_showLog = Common::toBool(attributes.value("ShowLog").toString());
						this->_singleAccountOrderType = static_cast<AccountOrderType::AccountOrderType>(attributes.value("SingleAccountOrderType").toInt());
						this->_displayAlert = attributes.value("DisplayAlert").toInt();
                        this->_isIexCrmUser = Common::toBool(attributes.value("IsIexCrmUser").toString());
                        this->_isUploadClientLog = attributes.hasAttribute("IsUploadClientLog") ?
                                Common::toBool(attributes.value("IsUploadClientLog").toString()) : false;
					}

					void update(const QDomNode& xmlNode)
					{
						QDomNamedNodeMap map = xmlNode.attributes();
						for (int i = 0; i < map.count(); i++)
						{
							QDomNode qDomNode = map.item(i);
							QString name = qDomNode.nodeName();
							QString value = qDomNode.nodeValue();
							if (name == "DQOrderOutTime")
								this->_dQOrderOutTime = value.toInt();
							else if (name == "DealingPolicyId")
								this->_dealingPolicyId = value!="" ?  QUuid(value) : QUuid();
							else if (name == "DisallowTrade")
								this->_disallowTrade = Common::toBool(value);
							else if (name == "EnableFastTrade")
								this->_enableFastTrade = Common::toBool(value);
							else if (name == "Id")
								this->_id = QUuid(value);
                            else if (name == "Code")
                                this->_code = value;
							else if (name == "IsCalculateFloat")
								this->_isCalculateFloat = Common::toBool(value);
							else if (name == "IsDisplayLedger")
								this->_isDisplayLedger = Common::toBool(value);
							else if (name == "IsEmployee")
								this->_isEmployee = Common::toBool(value);
							else if (name == "IsNoShowAccountStatus")
								this->_isNoShowAccountStatus = Common::toBool(value);
							else if (name == "IsSptReconfirm")
								this->_isSptReconfirm = Common::toBool(value);
							else if (name == "MultiAccountsOrderType")
								this->_multiAccountsOrderType = static_cast<AccountOrderType::AccountOrderType>(value.toInt());
							else if (name == "NeedSelectAccount")
								this->_needSelectAccount = Common::toBool(value);
							else if (name == "PrivateQuotePolicyId")
							{
								QUuid id = QUuid(value);
								if (this->_privateQuotePolicyId != id)
								{
									this->_privateQuotePolicyId = id;
									Common::SignalForwarder::getInstance()->sendRefreshSystemSignal();
									break;
								}
							}
							else if (name == "PublicQuotePolicyId")
								this->_publicQuotePolicyId = QUuid(value);
							else if (name == "ShowLog")
								this->_showLog = Common::toBool(value);
							else if (name == "SingleAccountOrderType")
								this->_singleAccountOrderType = static_cast<AccountOrderType::AccountOrderType>(value.toInt());
							else if (name == "DisplayAlert")
								this->_displayAlert = value.toInt();
                            else if (name == "IsIexCrmUser")
                                this->_isIexCrmUser = Common::toBool(value);
                            else if (name == "IsUploadClientLog")
                                this->_isUploadClientLog = Common::toBool(value);

						}
					}
			};
		}
	}
}
#endif

