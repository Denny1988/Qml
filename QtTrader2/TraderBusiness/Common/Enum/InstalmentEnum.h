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
				//����Ƶ��
				//-1�������ڣ�0:�£�1:���ȣ�2:˫��, 3:��
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
				//��ǰ����
				//0,�����S��1���κ���r�����S�� 2��������Ƿ��(����δ��)�����S; 3,������ǰ����(�����Ǹ�backoffice�õ�)
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
				//��ǰ����
				//0,�����S��1���κ���r�����S�� 2���������(�����Ǹ�backoffice�õ�); 3,����Ԥ����ǰ����
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
				//������
				//0:Fixed Amount(����*������, 1:%(��ֵ*����), 2:LumpSum���̶����)
				FixedAmount = 0,
				Percentage = 1,
				LumpSum = 2
			};
		}

		namespace ContractTerminateType
		{
			enum ContractTerminateType
			{
				//�Kֹ�ϼs�۷�e
				//0:���������; 1:  ��һ��; 2:Fixed Amount(����*����3:LumpSum���̶����)��
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
				//��������
				//0:ȫ�� 1:�ȶ�� 2���ȶϢ
				None = 0,
				EqualPrincipal = 1,
				EqualInstalment = 2
			};
		}

		namespace RecalculateRateType
		{
			enum RecalculateRateType
			{
				//��Ϣ��������
				//1:�������� 2:��һ������ 
                None = 0,
				RecalucateNextMonth = 1,
				RecalucateNextYear = 2,
			};
		}

		namespace PhysicalType
		{
			enum PhysicalType
			{
				//ʵ������
				//0:ȫ�� 1:���� 2��Ԥ��
				FullPayment = 0,
				Instalment = 1,
				PrePayment = 2,
			};
		}


	}
}
#endif
