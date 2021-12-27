#ifndef BUSINESS_MODEL_SERVER_DELIVERYSPECIFICATIONDETAIL_H
#define BUSINESS_MODEL_SERVER_DELIVERYSPECIFICATIONDETAIL_H
#include <QUuid>
#include <QXmlStreamReader>

namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class DeliverySpecificationDetail
			{
			private:
				QString _deliveryCharge;
				QUuid _deliverySpecificationId;
				QString _size;
                QString _ENG;
                QString _CHS;
                QString _CHT;
                QString _JPN;
                QString _VN;
                QString _MAL;
                QString _premium;


			public:
				QString getDeliveryCharge() const
				{
					return _deliveryCharge;
				}

				QUuid getDeliverySpecificationId() const
				{
					return _deliverySpecificationId;
				}

				QString getSize() const
				{
					return _size;
				}

                QString getENG() const
                {
                    return _ENG;
                }

                QString getCHS() const
                {
                    return _CHS;
                }

                QString getCHT() const
                {
                    return _CHT;
                }

                QString getJPN() const
                {
                    return _JPN;
                }

                QString getVN() const
                {
                    return _VN;
                }

                QString getMAL() const
                {
                    return _MAL;
                }

                QString getPremium() const
                {
                    return _premium;
                }



				void setDeliveryCharge(const QString& value)
				{
					_deliveryCharge = value;
				}

				void setDeliverySpecificationId(const QUuid& value)
				{
					_deliverySpecificationId = value;
				}

				void setSize(const QString& value)
				{
					_size = value;
				}

                void setENG(const QString& value)
                {
                    _ENG = value;
                }

                void setCHS(const QString& value)
                {
                    _CHS = value;
                }

                void setCHT(const QString& value)
                {
                    _CHT = value;
                }

                void setJPN(const QString& value)
                {
                    _JPN = value;
                }

                void setVN(const QString& value)
                {
                    _VN = value;
                }

                void setMAL(const QString& value)
                {
                    _MAL = value;
                }

                void setPremium(const QString& value)
                {
                    _premium = value;
                }



			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_deliveryCharge = attributes.value("DeliveryCharge").toString();
					this->_deliverySpecificationId = QUuid(attributes.value("DeliverySpecificationId").toString());
					this->_size = attributes.value("Size").toString();                    
                    this->_ENG = attributes.value("ENG").toString();
                    this->_CHS = attributes.value("CHS").toString();
                    this->_CHT = attributes.value("CHT").toString();
                    this->_JPN = attributes.value("JPN").toString();
                    this->_VN = attributes.value("VN").toString();
                    this->_MAL = attributes.value("MAL").toString();
                    this->_premium = attributes.value("Premium").toString();
                    if(this->_premium.length()==0)
                        this->_premium = "0";
				}

				void update(const QDomNode& xmlNode)
				{
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if (name == "DeliveryCharge")
							this->_deliveryCharge = value;
						else if (name == "DeliverySpecificationId")
							this->_deliverySpecificationId = QUuid(value);
						else if (name == "Size")
							this->_size = value;
                        else if (name == "ENG")
                            this->_ENG = value;
                        else if (name == "CHS")
                            this->_CHS = value;
                        else if (name == "CHT")
                            this->_CHT = value;
                        else if (name == "JPN")
                            this->_JPN = value;
                        else if (name == "VN")
                            this->_VN = value;
                        else if (name == "MAL")
                            this->_MAL = value;
                        else if (name == "Premium")
                            this->_premium = value;
					}
				}


			};
		}
	}
}
#endif

