#ifndef BUSINESS_MODEL_SETTING_INSTALMENTSETTING_H
#define BUSINESS_MODEL_SETTING_INSTALMENTSETTING_H
#include <quuid.h>
#include "Common/Enum/InstalmentEnum.h"
#include "InstalmentSettingDetail.h"
#include <QMap>
#include <QSharedPointer>
#include <QDomNode>
#include "SettingDataBase.h"
using namespace Common::Enum;

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class TRADERBUSINESS_EXPORT InstalmentSetting: public SettingDataBase
			{
			public:
				InstalmentSetting();
				~InstalmentSetting();
			private:
				QUuid  _id;
				QString  _code;
				bool  _allowEqualPrincipal;
				bool  _allowEqualInstalment;
				bool  _allowRecalucateNextMonth;
				bool  _allowRecalucateNextYear;
				Common::Enum::AllowCloseInstalment::AllowCloseInstalment _allowClose;
				Common::Enum::AdvancePaymentOption::AdvancePaymentOption _advancePayment;
				bool  _isDownPayAsFirstPay;

				QMap<Frequence::Frequence, QMap<int,QSharedPointer<InstalmentSettingDetail>>> _instalmentSettingDetails;		
				QSharedPointer<InstalmentSettingDetail> _advancePaymentInstalmentSettingDetail;

			public:
				QSharedPointer<InstalmentSettingDetail>& getOrAddInstalmentSettingDetail(Frequence::Frequence frequence, int period);
				QSharedPointer<InstalmentSettingDetail>& getOrAddAdvancePaymentInstalmentSettingDetail();
				QSharedPointer<InstalmentSettingDetail> getInstalmentSettingDetail(Frequence::Frequence frequence, int period);
				
				void update(const QDomNode& xmlInstalmentSetting);
				bool hasInstalmentSettingOrAdvancePaymentInstalment(bool isAdvancePayment);
				void clearDetail();
                QSharedPointer<InstalmentSetting> copy();


			public:
				QUuid getId() const
				{
					return _id;
				}

				QString getCode() const
				{
					return _code;
				}

				bool getAllowEqualPrincipal() const
				{
					return _allowEqualPrincipal;
				}

				bool getAllowEqualInstalment() const
				{
					return _allowEqualInstalment;
				}

				bool getAllowRecalucateNextMonth() const
				{
					return _allowRecalucateNextMonth;
				}

				bool getAllowRecalucateNextYear() const
				{
					return _allowRecalucateNextYear;
				}

				Common::Enum::AllowCloseInstalment::AllowCloseInstalment getAllowClose() const
				{
					return _allowClose;
				}
								
				Common::Enum::AdvancePaymentOption::AdvancePaymentOption getAdvancePayment() const
				{
					return _advancePayment;
				}

				bool getIsDownPayAsFirstPay() const
				{
					return _isDownPayAsFirstPay;
				}

				QSharedPointer<InstalmentSettingDetail> getAdvancePaymentInstalmentSettingDetail() const
				{
					return _advancePaymentInstalmentSettingDetail;
				}
				
				QMap<Frequence::Frequence, QMap<int,QSharedPointer<InstalmentSettingDetail>>> getInstalmentSettingDetails() const
				{
					return _instalmentSettingDetails;
				}

				QList<QSharedPointer<InstalmentSettingDetail>> getInstalmentSettingDetailsByFrequence(Frequence::Frequence frequence) const
				{
					return _instalmentSettingDetails[frequence].values();
				}



				void setAdvancePayment(const Common::Enum::AdvancePaymentOption::AdvancePaymentOption& value)
				{
					_advancePayment = value;
				}

				void setAllowClose(const Common::Enum::AllowCloseInstalment::AllowCloseInstalment& value)
				{
					_allowClose = value;
				}

				void setAllowEqualInstalment(const bool& value)
				{
					_allowEqualInstalment = value;
				}

				void setAllowEqualPrincipal(const bool& value)
				{
					_allowEqualPrincipal = value;
				}

				void setAllowRecalucateNextMonth(const bool& value)
				{
					_allowRecalucateNextMonth = value;
				}

				void setAllowRecalucateNextYear(const bool& value)
				{
					_allowRecalucateNextYear = value;
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





			};
		}
	}
}
#endif
