#ifndef LOGINRESULT_H
#define LOGINRESULT_H

#include "Common/Util/Convert_Common.h"
#include "InvokeResult.h"
#include "Common/Enum/MarginEnum.h"

namespace Common
{
	namespace Result
	{
		class TRADERBUSINESS_EXPORT LoginResult : public InvokeResult
		{
		public:
			LoginResult(QSharedPointer<Communication::AsyncResult> asyncResult)
				: InvokeResult(asyncResult)
			{
			}
			
			bool getIsActivateAccount()
			{
				/*QString aa = this->asyncResult->jsonResult().value("IsActivateAccount").toString();
				auto aa = this->m_LoginResult->getjsonResult();
				auto bb = aa.take("UserId").toString();
				auto cc = aa.take("Status").toInt();
				auto dd = aa.take("IsActivateAccount").toBool();*/
				auto jsonResult = this->asyncResult->jsonResult();
				return jsonResult.take("IsActivateAccount").toBool();
			}

			QUuid getUserId()
			{
				auto jsonResult = this->asyncResult->jsonResult();
				return QUuid(jsonResult.take("UserId").toString());
			}

			bool getIsEmployee()
			{
				auto jsonResult = this->asyncResult->jsonResult();
				return jsonResult.take("IsEmployee").toBool();
			}
			

			const QJsonObject& getjsonResult()
			{
				return this->asyncResult->jsonResult();
			}

			QString getPathName()
			{
				auto jsonResult = this->asyncResult->jsonResult();
				return jsonResult.take("PathName").toString();
			}

			QString getServerDateTime()
			{
				auto jsonResult = this->asyncResult->jsonResult();
				return jsonResult.take("ServerDateTime").toString();
			}

			QStringList getWatchwords()
			{
				auto jsonResult = this->asyncResult->jsonResult();
				QJsonArray watchwordsArray = jsonResult.take("Watchwords").toArray();
				int size = watchwordsArray.size();
				QStringList watchwords;
				for (int i = 0; i < size; i++)
				{
					auto watchword = watchwordsArray.at(i);
					watchwords.append(watchword.toString());
				}
				return watchwords;
			}

			QString getClientId()
			{
				auto jsonResult = this->asyncResult->jsonResult();
				return jsonResult.take("ClientId").toString();
			}

			bool getEnableSMSVerification()
			{
				auto jsonResult = this->asyncResult->jsonResult();
				return jsonResult.take("EnableSMSVerification").toBool();
			}
			
			QString getVerificationCode()
			{
				auto jsonResult = this->asyncResult->jsonResult();
				return jsonResult.take("VerificationCode").toString();
			}
			
			int getVerificationValidTime()
			{
				auto jsonResult = this->asyncResult->jsonResult();
				return jsonResult.take("VerificationValidTime").toInt();
			}

			QString getMobilePhone()
			{
				auto jsonResult = this->asyncResult->jsonResult();
				return jsonResult.take("MobilePhone").toString();
			}

			bool getIsPasswordExpired()
			{
				auto jsonResult = this->asyncResult->jsonResult();
				return jsonResult.take("IsPasswordExpired").toBool();
			}

			QString getRegexPattern()
			{
				auto jsonResult = this->asyncResult->jsonResult();
				return jsonResult.take("RegexPattern").toString();
			}

			QString getTipInfo()
			{
				auto jsonResult = this->asyncResult->jsonResult();
				return jsonResult.take("TipInfo").toString();
			}

            QString getUserOpenId()
            {
                auto jsonResult = this->asyncResult->jsonResult();
                return jsonResult.take("UserOpenId").toString();
            }

            bool getIsNewRegistId()
            {
                auto jsonResult = this->asyncResult->jsonResult();
                return jsonResult.take("IsNewRegistId").toBool();
            }


		};

		class TRADERBUSINESS_EXPORT XmlDataResult : public InvokeResult
		{
		public:
			XmlDataResult(QSharedPointer<Communication::AsyncResult> asyncResult,const QUuid& uuid = Q_NULLPTR)
				: InvokeResult(asyncResult),
				_uuid(uuid)
			{
			}

			const QDomElement& getXmlResult()
			{
				return this->asyncResult->xmlResult();
			}
			const QUuid& getUuid() const
			{
				return _uuid;
			}
		private:
			QUuid				_uuid;
		};

		class TRADERBUSINESS_EXPORT QuoteResult : public InvokeResult
		{
		public:
			QuoteResult(QSharedPointer<Communication::AsyncResult> asyncResult)
				: InvokeResult(asyncResult)
			{
			}
			
			QString getAsk()
			{
				return this->asyncResult->jsonResult().value("Ask").toString();
			}

			QString getBid()
			{
				return this->asyncResult->jsonResult().value("Bid").toString();
			}

			QString getAnswerLot()
			{
				return this->asyncResult->jsonResult().value("AnswerLot").toString();
			}

			QString getQuoteLot()
			{
				return this->asyncResult->jsonResult().value("QuoteLot").toString();
			}
		};

		class TRADERBUSINESS_EXPORT AddMarginResult : public InvokeResult
		{
		public:
			AddMarginResult(QSharedPointer<Communication::AsyncResult> asyncResult)
				: InvokeResult(asyncResult)
			{
			}
			//returnValue : Successed=1   NoEnoughBalaceToPay=-1  NotApproved = 0  Failed=-2
			// "<Result>  <content_result>536,0</content_result></Result>
			QString getReference()
			{
				QDomElement xmlResult = this->asyncResult->xmlResult();
				QString content = xmlResult.text();
				return content.contains(",") ? content.split(',').at(0) : content;
			}

			Common::Enum::MarginApproveResult::MarginApproveResult getApproveResult()
			{
				QDomElement xmlResult = this->asyncResult->xmlResult();
				QString content = xmlResult.text(); 
                if (content == "IPSNotReady")
					return Common::Enum::MarginApproveResult::IPSNotReady;
                if (content.contains(',') && content.split(',').at(0) == "IPSNotReady")
                    return Common::Enum::MarginApproveResult::IPSNotReady;
                if (content.contains(','))
                {
                    QString returnValue = content.split(',').at(1);
                    if(returnValue == "0")
                    {
                        return Common::Enum::MarginApproveResult::NotApproved;
                    }
                    else if(returnValue == "1")
                    {
                        return Common::Enum::MarginApproveResult::Successed;
                    }
                    else if(returnValue == "-1")
                    {
                        return Common::Enum::MarginApproveResult::NoEnoughBalaceToPay;
                    }
                    else
                    {
                        return Common::Enum::MarginApproveResult::Failed;
                    }
                }
                else
                    return Common::Enum::MarginApproveResult::RunErrorFailed;

			}

                        // "<Result>  <content_result>Succeed|KV098</content_result></Result>
                        QString getDepositResult()
                        {
                                QDomElement xmlResult = this->asyncResult->xmlResult();
                                return xmlResult.text();
                        }
                };

		class TRADERBUSINESS_EXPORT GetReportUrlResult : public InvokeResult
		{
		public:
			GetReportUrlResult(QSharedPointer<Communication::AsyncResult> asyncResult)
				: InvokeResult(asyncResult)
			{
			}
			// "<Result>  <content_result>xxxx</content_result></Result>
			QString getReportUrl()
			{
				QDomElement xmlResult = this->asyncResult->xmlResult();
				return xmlResult.text();
			}

		};

		class TRADERBUSINESS_EXPORT ChartVersionResult : public InvokeResult
		{
		public:
			ChartVersionResult(QSharedPointer<Communication::AsyncResult> asyncResult)
				: InvokeResult(asyncResult)
			{
			}
			
			int getChartVersion()
			{
				QDomElement xmlResult = this->asyncResult->xmlResult();
				return xmlResult.text().toInt();
			}

			int	m_callId;
			QUuid m_resultContentKey;
			//ChartProperty m_chartProperty;
		};

		class TRADERBUSINESS_EXPORT ChartFirstTradeDayResult : public InvokeResult
		{
		public:
			ChartFirstTradeDayResult(QSharedPointer<Communication::AsyncResult> asyncResult)
				: InvokeResult(asyncResult)
			{
			}
			
			QString getChartFirstTradeDay()
			{
				QDomElement xmlResult = this->asyncResult->xmlResult();
				return xmlResult.text();
			}

			int	m_callId;
		};

		class TRADERBUSINESS_EXPORT FixPeriodBeginTimeResult : public InvokeResult
		{
		public:
			FixPeriodBeginTimeResult(QSharedPointer<Communication::AsyncResult> asyncResult)
				: InvokeResult(asyncResult)
			{
			}
			
			const QDomElement& getResult()
			{
				return this->asyncResult->xmlResult();
			}

			int	m_callId;
			int m_periodType;
			QUuid m_resultContentKey;
		};

		class TRADERBUSINESS_EXPORT GetVerificationCodeResult : public InvokeResult
		{
		public:
			GetVerificationCodeResult(QSharedPointer<Communication::AsyncResult> asyncResult)
				: InvokeResult(asyncResult)
			{
			}

			QString getResult()
			{
				QDomElement xmlResult = this->asyncResult->xmlResult();
				return xmlResult.text();//VerificationCode|VerificationValidTime
			}

		};

	}
}

#endif	   //LOGINRESULT_H
