#ifndef BUSINESS_MODEL_SETTING_BOPOLICYSETTING_H
#define BUSINESS_MODEL_SETTING_BOPOLICYSETTING_H
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include "SettingDataBase.h"
#include "Common/Enum/BOEnum.h"
#include <QUuid>

using namespace Common::Enum;
namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class BOPolicyDetailSetting: public SettingDataBase
			{
			private:
				int _frequency;
				QDecNumber  _commissionOpen;
				QDecNumber  _max;
				QDecNumber  _min;
				QDecNumber  _step;
				QDecNumber  _default;
				int  _decimals;				
				QUuid  _BOBetTypeId;
				QDecNumber _odds;
				int _oddsDecimals;
				int  _maxOrderCount;
				QDecNumber  _totalBetLimit;
				BOBetOption::BOBetOption _option;
				int _lastOrderTimeSpan;

			public:
				void update(const QDomNode& xmlMovePipsInfoSetting)
				{
					QDomNamedNodeMap map = xmlMovePipsInfoSetting.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "Frequency")
							this->_frequency= value.toInt();
						else if(name == "CommissionOpen")
							this->_commissionOpen = QDecNumber::fromQString(value);
						else if(name == "Max")
							this->_max = QDecNumber::fromQString(value);
						else if(name == "Min")
							this->_min= QDecNumber::fromQString(value);
						else if(name == "Step")
							this->_step = QDecNumber::fromQString(value);
						else if(name == "Default")
							this->_default = QDecNumber::fromQString(value);
						else if(name == "Decimals")
							this->_decimals = value.toInt();
						else if(name == "BOBetTypeId")
							this->_BOBetTypeId = QUuid(value);
						else if(name == "Odds")
						{
							this->_oddsDecimals =  Common::getValidDecimals(value);
							this->_odds = QDecNumber::fromQString(value);
						}
						else if(name == "MaxOrderCount")
							this->_maxOrderCount = value.toInt();
						else if(name == "TotalBetLimit")
							this->_totalBetLimit= QDecNumber::fromQString(value);
						else if(name == "Option")
							this->_option = static_cast<BOBetOption::BOBetOption>(value.toInt());
						else if(name == "LastOrderTimeSpan")
							this->_lastOrderTimeSpan = value.toInt();
					}
				}

                QSharedPointer<BOPolicyDetailSetting> copy()
                {
                    QSharedPointer<BOPolicyDetailSetting> newObject(new BOPolicyDetailSetting);
                    newObject->_frequency = _frequency;
                    newObject->_commissionOpen = _commissionOpen;
                    newObject->_max = _max;
                    newObject->_min = _min;
                    newObject->_step = _step;
                    newObject->_default = _default;
                    newObject->_decimals = _decimals;
                    newObject->_BOBetTypeId = _BOBetTypeId;
                    newObject->_odds = _odds;
                    newObject->_oddsDecimals = _oddsDecimals;
                    newObject->_maxOrderCount = _maxOrderCount;
                    newObject->_totalBetLimit = _totalBetLimit;
                    newObject->_option = _option;
                    newObject->_lastOrderTimeSpan = _lastOrderTimeSpan;
                    return newObject;
                }

			public:		
				int getFrequency() const
				{
					return _frequency;
				}

				QDecNumber getCommissionOpen() const
				{
					return _commissionOpen;
				}

				QDecNumber getStep() const
				{
					return _step;
				}

				QDecNumber getMin() const
				{
					return _min;
				}

				QDecNumber getMax() const
				{
					return _max;
				}

				QDecNumber getDefault() const
				{
					return _default;
				}

				int getDecimals() const
				{
					return _decimals;
				}

				QUuid getBOBetTypeId() const
				{
					return _BOBetTypeId;
				}

				QDecNumber getOdds() const
				{
					return _odds;
				}

				int getOddsDecimals() const
				{
					return _oddsDecimals;
				}

				int getMaxOrderCount() const
				{
					return _maxOrderCount;
				}

				QDecNumber getTotalBetLimit() const
				{
					return _totalBetLimit;
				}

				BOBetOption::BOBetOption getOption() const
				{
					return _option;
				}

				int getLastOrderTimeSpan() const
				{
					return _lastOrderTimeSpan;
				}



				void setBOBetTypeId(const QUuid& value)
				{
					_BOBetTypeId = value;
				}

				void setCommissionOpen(const QDecNumber& value)
				{
					_commissionOpen = value;
				}

				void setDecimals(const int& value)
				{
					_decimals = value;
				}

				void setDefault(const QDecNumber& value)
				{
					_default = value;
				}

				void setFrequency(const int& value)
				{
					_frequency = value;
				}

				void setLastOrderTimeSpan(const int& value)
				{
					_lastOrderTimeSpan = value;
				}

				void setMax(const QDecNumber& value)
				{
					_max = value;
				}

				void setMaxOrderCount(const int& value)
				{
					_maxOrderCount = value;
				}

				void setMin(const QDecNumber& value)
				{
					_min = value;
				}

				void setOdds(const QDecNumber& value)
				{
					_odds = value;
				}

				void setOddsDecimals(const int& value)
				{
					_oddsDecimals = value;
				}

				void setOption(const BOBetOption::BOBetOption& value)
				{
					_option = value;
				}

				void setStep(const QDecNumber& value)
				{
					_step = value;
				}

				void setTotalBetLimit(const QDecNumber& value)
				{
					_totalBetLimit = value;
				}





			};

			class BOPolicySetting: public SettingDataBase
			{
			private:
				QUuid  _id;
				QString  _code;				
				int  _maxOrderCount;
				QDecNumber  _totalBetLimit;
				QMap<QString, QSharedPointer<BOPolicyDetailSetting>> _BOPolicyDetailSettings;

			public:
				void update(const QDomNode& xmlBOPolicySetting)
				{
					QDomNamedNodeMap map = xmlBOPolicySetting.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "Id")
							this->_id= QUuid(value);
						else if(name == "Code")
							this->_code=  value;
						else if(name == "MaxOrderCount")
							this->_maxOrderCount= value.toInt();
						else if(name == "TotalBetLimit")
							this->_totalBetLimit= QDecNumber::fromQString(value);
					}

					QDomNodeList childlist = xmlBOPolicySetting.childNodes();  
					for (int i = 0; i < childlist.count(); i++)
					{
						QDomNode itemQDomNode = childlist.item(i);
						QString name = itemQDomNode.nodeName();
						if(name == "BOPolicyDetailSetting")
						{
							auto itemNode = itemQDomNode.toElement();
							QString isDelete = itemNode.attribute("IsDelete");
							QString detailKey = itemNode.attribute("DetailKey");
							if(isDelete == "False" || isDelete=="")
							{								
								if(this->_BOPolicyDetailSettings.contains(detailKey))
								{
									this->_BOPolicyDetailSettings[detailKey]->update(itemQDomNode);
								}
								else
								{
                                    QSharedPointer<BOPolicyDetailSetting> boPolicyDetailSetting(new BOPolicyDetailSetting);
                                    boPolicyDetailSetting->update(itemQDomNode);
                                    this->_BOPolicyDetailSettings.insert(detailKey, boPolicyDetailSetting);
								}
							}
							else
							{
								if(this->_BOPolicyDetailSettings.contains(detailKey))
								{
									this->_BOPolicyDetailSettings.remove(detailKey);
								}	

							}						
						}
					}

				}

                QSharedPointer<BOPolicySetting> copy()
                {
                    QSharedPointer<BOPolicySetting> newObject(new BOPolicySetting);
                    newObject->_id = _id;
                    newObject->_code = _code;
                    newObject->_maxOrderCount = _maxOrderCount;
                    newObject->_totalBetLimit = _totalBetLimit;

                    QMap<QString, QSharedPointer<BOPolicyDetailSetting>> copyBOPolicyDetailSettings;
                    QMapIterator<QString, QSharedPointer<BOPolicyDetailSetting>> item(this->_BOPolicyDetailSettings);
                    while (item.hasNext()) {
                        item.next();
                        copyBOPolicyDetailSettings.insert(item.key(), item.value()->copy());
                    }
                    newObject->_BOPolicyDetailSettings = copyBOPolicyDetailSettings;

                    return newObject;
                }


				QSharedPointer<BOPolicyDetailSetting> getBOPolicyDetailSetting(int frequency, QUuid BOBetTypeId)
				{
					QMapIterator<QString, QSharedPointer<BOPolicyDetailSetting>> BOPolicyDetailSettingItem(_BOPolicyDetailSettings);
					while (BOPolicyDetailSettingItem.hasNext())
					{
						BOPolicyDetailSettingItem.next();
						if (BOPolicyDetailSettingItem.value()->getFrequency() == frequency
							&& BOPolicyDetailSettingItem.value()->getBOBetTypeId() == BOBetTypeId)
							return _BOPolicyDetailSettings[BOPolicyDetailSettingItem.key()];
					}
					return _BOPolicyDetailSettings.first();
				}

				QSharedPointer<BOPolicyDetailSetting>& getBOPolicyDetailSetting(QString frequencyKey)
				{
					if (_BOPolicyDetailSettings.contains(frequencyKey))
						return _BOPolicyDetailSettings[frequencyKey];
					return _BOPolicyDetailSettings.first();
				}

				QMap<QString, QSharedPointer<BOPolicyDetailSetting>> getBOPolicyDetailSettings()
				{
					return this->_BOPolicyDetailSettings;
				}

				int getBOPolicyDetailSettingsCount()
				{
					return this->_BOPolicyDetailSettings.count();
				}

				bool isExistBOPolicyDetailSetting(QString frequencyKey)
				{
					return _BOPolicyDetailSettings.contains(frequencyKey);
				}

				void clearDetail()
				{
					_BOPolicyDetailSettings.clear();
				}

				QSharedPointer<BOPolicyDetailSetting>& getOrAddBOPolicyDetail(QString frequencyKey)
				{
					if (!_BOPolicyDetailSettings.contains(frequencyKey))
					{
                        QSharedPointer<BOPolicyDetailSetting> boPolicyDetailSetting(new BOPolicyDetailSetting);
                        this->_BOPolicyDetailSettings.insert(frequencyKey, boPolicyDetailSetting);
					}
					return _BOPolicyDetailSettings[frequencyKey];
				}

                void copyChildItem()
                {
                    QMap<QString, QSharedPointer<BOPolicyDetailSetting>> copyBOPolicyDetailSettings;
                    QMapIterator<QString, QSharedPointer<BOPolicyDetailSetting>> item(this->_BOPolicyDetailSettings);
                    while (item.hasNext()) {
                        item.next();
                        copyBOPolicyDetailSettings.insert(item.key(), QSharedPointer<BOPolicyDetailSetting>(new BOPolicyDetailSetting(*item.value().data())));
                    }
                    this->_BOPolicyDetailSettings = copyBOPolicyDetailSettings;
                }


			public:			
				QUuid getId() const
				{
					return _id;
				}

				QString getCode() const
				{
					return _code;
				}

				int getMaxOrderCount() const
				{
					return _maxOrderCount;
				}

				QDecNumber getTotalBetLimit() const
				{
					return _totalBetLimit;
				}

				



				void setCode(const QString& value)
				{
					_code = value;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setMaxOrderCount(const int& value)
				{
					_maxOrderCount = value;
				}

				void setTotalBetLimit(const QDecNumber& value)
				{
					_totalBetLimit = value;
				}




			};


		}
	}
}
#endif

