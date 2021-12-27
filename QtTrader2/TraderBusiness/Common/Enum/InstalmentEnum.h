#ifndef INSTALMENTENUM_H
#define INSTALMENTENUM_H
namespace Common
{
	namespace Enum
	{
		namespace Frequence
		{
			enum Frequence
			{
				//偿还频率
				//-1：不定期，0:月，1:季度，2:双周, 3:年
				Occasional = -1,
				Month = 0,
				Quarterly = 1,
				Fortnight = 2,
				Year = 3
			};
		}

		namespace AllowCloseInstalment
		{
			enum AllowCloseInstalment
			{
				//提前还款
				//0,不容許；1，任何情況下容許； 2，不存在欠款(逾期未还)下容許; 3,允许提前还款(此项是给backoffice用的)
				DisAllow = 0,
				AllowAll = 1,
				AllowWhenNoOutstandingInstalment = 2,
				AllowPrepayment =3
			};
		}

		namespace AdvancePaymentOption
		{
			enum AdvancePaymentOption
			{
				//提前还款
				//0,不容許；1，任何情況下容許； 2，允许分期(此项是给backoffice用的); 3,允许预付提前还款
				DisallowAll = 0,
				AllowAll = 1,
				AllowInstalment = 2,
				AllowPrepayment = 3
			};
		}

		namespace AdministrationFeeBaseType
		{
			enum AdministrationFeeBaseType
			{
				//手续费
				//0:Fixed Amount(数量*比例）, 1:%(价值*比例), 2:LumpSum（固定金额)
				FixedAmount = 0,
				Percentage = 1,
				LumpSum = 2
			};
		}

		namespace ContractTerminateType
		{
			enum ContractTerminateType
			{
				//終止合約扣费類別
				//0:还款金额比例; 1:  扣一期; 2:Fixed Amount(手数*金额）；3:LumpSum（固定金额)，
				RepaymentRatio = 0,
				DeductOneInstalment = 1,
				FixedAmount = 2,
				LumpSum = 3
			};
		}

		namespace PhysicalInstalmentType
		{
			enum PhysicalInstalmentType
			{
				//还款类型
				//0:全款 1:等额本金 2：等额本息
				None = 0,
				EqualPrincipal = 1,
				EqualInstalment = 2
			};
		}

		namespace RecalculateRateType
		{
			enum RecalculateRateType
			{
				//利息重算类型
				//1:下月重算 2:下一年重算 
                None = 0,
				RecalucateNextMonth = 1,
				RecalucateNextYear = 2,
			};
		}

		namespace PhysicalType
		{
			enum PhysicalType
			{
				//实货类型
				//0:全款 1:分期 2：预付
				FullPayment = 0,
				Instalment = 1,
				PrePayment = 2,
			};
		}


	}
}
#endif
