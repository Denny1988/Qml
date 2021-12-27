#ifndef _CONVERT_COMMON_H
#define _CONVERT_COMMON_H
#include <iostream>
#include <sstream>
#include <QString>
#include <QVector>
#include <qdatetime.h>
#include <QDomDocument>
#include "Common/Constants/DateTimeConstants.h"
#include "Common/Struct/SettingStruct.h"
#include <iomanip>
#include <QLocale>
#include "QDecNumber.h"
#include <QRegExp>
#include <QtMath>

using namespace Common::Struct;
//#include "..\Exception\Convert_Exception.h"
namespace Common
{
	static const int _MaxScale = 13;
    const double _PRECISION = 0.000001;

	static double round(double value)
	{
		return value < 0.0 ? std::ceil(value - 0.5): std::floor(value + 0.5);
	}

	static double round(double rawValue, int precision)
	{
		if(precision > _MaxScale) precision = 2;
		std::stringstream s;
		s << std::setprecision(precision) << std::setiosflags(std::ios_base::fixed) << rawValue;
		s >> rawValue;
		return rawValue;
	}

	static QString roundToString2(double value, int precision)
	{
		if(precision > _MaxScale) precision = 2;
        if(precision > 0 && value != 0.0)
        {
            if(value > 0.0)
                value = value + 0.00000001;//�е�ֵΪʵ��Ϊ7.775��ת��double���;ͱ��7.7749999999999999 ���Բ�ȫ�����������������ȷ��7.78����ʵ��ȴ��7.77
            else
                value = value - 0.00000001;
        }
		return QString::number(value, '.', precision);	
	}

	static QString roundToString(double value, int precision, bool isUseThousandSeparators = false)
	{	
		if(precision > _MaxScale) precision = 2;
		if(precision < 0) precision = 0;
        if(precision > 0 && value != 0.0)
        {
            if(value > 0.0)
                value = value + 0.00000001;//�е�ֵΪʵ��Ϊ7.775��ת��double���;ͱ��7.7749999999999999 ���Բ�ȫ�����������������ȷ��7.78����ʵ��ȴ��7.77
            else
                value = value - 0.00000001;
        }
        if(isUseThousandSeparators)
		{
			bool isZero = round(value, precision) == 0.0;
			/*Qt5.7 not support
			const QLocale & cLocale = QLocale::c();
			QString newValue = cLocale.toString(value, 'f', precision);*/
			QString newValue = QString("%L1").arg(value, 0, 'f', precision);
			if(newValue.contains("-") && isZero)//���磺value=-0.002��precision=2�� ���ʱ��ͻ���ʾ-0.00�����������û��Ҫ��ʾ����
				return newValue.mid(1);
			else
				return newValue;
		}
		else
		{
			QString newValue = QString::number(value, '.', precision);		
			if(newValue.contains("-") && newValue.toDouble() == 0.0)//���磺value=-0.002��precision=2�� ���ʱ��ͻ���ʾ-0.00�����������û��Ҫ��ʾ����
				return newValue.mid(1);
			else
				return newValue;
		}
	}

    static QString removeExcesszeros(QString value)
    {
        if(!value.contains(".")) return value;
        QString newValue = "";
        for(int i = value.count() -1; i >0; i--)
        {
            if(value[i]=='.')
            {
                newValue = value.left(i);
                break;
            }
            else if(value[i]!='0')
            {
                newValue = value.left(i+1);
                break;
            }
        }
        return newValue;
    }

    static QString roundToString3(double value, int precision, bool isUseThousandSeparators = false)
    {
        QString newValue = roundToString(value, precision, isUseThousandSeparators);
        return removeExcesszeros(newValue);
    }

	static QDecNumber roundToDecimal(double value, int precision)
	{
		if(precision > _MaxScale) precision = 2;
		if(value == 0.0) return 0.0;
		return QDecNumber::fromQString(roundToString2(value, precision));
	}

	static QDecNumber roundToDecimal(QDecNumber value, int precision)
	{
		if(value == 0.0) return 0.0;
		return QDecNumber::fromQString(roundToString2(value.toDouble(), precision));
	}

	static QString roundToPercentageString(double value, int precision)
	{
		if(value == 0.0) return "";
		return QString::number(value*100, '.', precision) + "%";
	}

	static double stringToDouble(QString value)
	{
		bool ok;
		double d;

		QLocale c(QLocale::C);
		d = c.toDouble(value, &ok );
		return d;
	}

    static int stringToInt(QString value)
    {
        if(value.contains('.'))
        {
            int position = value.indexOf (".",0);
            QString newStr = value.left(position);
            return newStr.toInt();
        }
        else
            return value.toInt();
    }

	static int getDecimal(QString value)
	{
		int position = value.indexOf (".",0);
		if(position == -1) return 0;
		return value.length() -1 -position;
	}

	static int getValidDecimals(QString value)
	{
		//ע��ȥ��0.100��������00����Чλ��
		int position = value.indexOf (".",0);
		if(position == -1) return 0;
		if(value.toDouble() == 0.0) return 0;
		int decimals = value.length() -1 -position;
		int realDecimals = decimals;
		for (int i = 1; i <= decimals; i++)
		{
			if(value.right(i).toInt()==0)
				realDecimals--;
		}
		decimals = realDecimals;
		return decimals;
	}


	static bool isIntegral(double value)
	{
		return ((double)((int)value)) == value;
	}

	static int getDecimalPlaces(double value)
	{
		for (int index = 0; index < 10; index++)
		{
			double newValue = (value * qPow(10, index));
			if (isIntegral(newValue))
			{
				return index;
			}
		}
		return -1;
	}


	static QDecNumber integralMultiple(QDecNumber lot, QDecNumber rate)
	{
		if (rate.isZero()) return lot;
		int multiple = (lot.toDouble() / rate.toDouble());
		return rate * QDecNumber(multiple);
	}

	/*static int decimalToInt(QDecNumber value)
	{
	QString strValue = value.toString();
	if(strValue.contains('.'))
	{

	}
	return strValue.toInt();
	}*/
	/*inline double convertToDouble(const std::string& s)
	{
	std::istringstream i(s);
	double x;
	if(!(i >> x))
	throw  Exception::BadConversion("convertToDouble(\"" + s + "\")");
	return x;
	}

	template<typename T>
	inline std::string toString(T d)
	{
	std::ostringstream o;
	o << d;
	return o.str();
	}*/

	static bool toBool(const QString s)
	{
		return s.toUpper() == "TRUE" ? true : false;
	}

	static QString convertQVector2DToQStringXML(QVector<QVector<QString>> datas)
	{
		QDomDocument doc;
		QDomElement root = doc.documentElement();
		for (int i = 0; i < datas.count(); i++)
		{
			QDomElement parentNote = doc.createElement("Parent");				
			for (int j = 0; j < datas[i].count(); j++)
			{
				QDomElement childNote = doc.createElement("Child");					
				childNote.appendChild(doc.createTextNode(datas[i][j]));
				parentNote.appendChild(childNote);
			}
			root.appendChild(parentNote);
		}
		doc.appendChild(root);
		return doc.toString();
	}

	static QDateTime convertToDateTime(const QString& value)
	{
		//"yyyy-MM-dd HH:mm:ss"
		/*int year = value.mid(0, 4).toInt();  
		int month = value.mid(5, 2).toInt();  
		int day = value.mid(8, 2).toInt(); 
		int hour = value.mid(11, 2).toInt();  
		int minute = value.mid(14, 2).toInt(); 
		int second = value.mid(17, 2).toInt(); */
		int year = value.mid(0, 4).toInt();  
		return QDateTime(QDate(
			year > 5000 ? 5000: year, 
			value.mid(5, 2).toInt(), 
			value.mid(8, 2).toInt()), 
			QTime(
			value.mid(11, 2).toInt(), 
			value.mid(14, 2).toInt(), 
			value.mid(17, 2).toInt()));
	}

	static QLocale::Language getLanguagebyQString(QString languageQString)
	{
		if (languageQString == "English")
		{
			return QLocale::English;
		}
		else if(languageQString == "Chinese")
		{
			return QLocale::Chinese;
		}
		else if(languageQString == "Japanese")
		{
			return QLocale::Japanese;//����
		}
		else if(languageQString == "Hindi")
		{
			return QLocale::Hindi;//ӡ����
		}
		else if(languageQString == "Malay")
		{
			return QLocale::Malay;//������
		}
		else if(languageQString == "Thai")
		{
			return QLocale::Thai;//̩��
		}
		else if(languageQString == "Korean")
		{
			return QLocale::Korean;//���������
		}
		else if(languageQString == "Vietnamese")
		{
			return QLocale::Vietnamese;//Խ����
		}	
		else if(languageQString == "French")
		{
			return QLocale::French;//����
		}
		else if(languageQString == "German")
		{
			return QLocale::German;//����
		}
		else if(languageQString == "Russian")
		{
			return QLocale::Russian;//����
		}
		else if(languageQString == "Italian")
		{
			return QLocale::Italian;//�������
		}
		else if(languageQString == "Burmese")
		{
			return QLocale::Burmese;//�����
		}
		else if(languageQString == "Bengali")
		{
			return QLocale::Bengali;//�ϼ�����
		}	
		else if(languageQString == "Dutch")
		{
			return QLocale::Dutch;//������
		}
		else if(languageQString == "Irish")
		{
			return QLocale::Irish;//��������
		}
		else if(languageQString == "Mongolian")
		{
			return QLocale::Mongolian;//�ɹ���
		}
		else if(languageQString == "Portuguese")
		{
			return QLocale::Portuguese;//��������
		}
		else if(languageQString == "Spanish")
		{
			return QLocale::Spanish;//��������
		} 
		else if(languageQString == "Filipino")
		{
			return QLocale::Filipino;//���ɱ���
		} 		
		else
		{
			return QLocale::C;
		}
	}

	static QLocale::Country getCountrybyQString(QString countryQString)
	{
		if (countryQString == "UnitedStates")
		{
			return QLocale::UnitedStates;
		}
		else if(countryQString == "China")
		{
			return QLocale::China;
		}
		else if(countryQString == "Japan")
		{
			return QLocale::Japan;//����
		}
		else if(countryQString == "India")
		{
			return QLocale::India;//ӡ��
		}
		else if(countryQString == "Malay")
		{
			return QLocale::Malaysia;//��������
		}
		else if(countryQString == "Thailand")
		{
			return QLocale::Thailand;//̩��
		}
		else if(countryQString == "SouthKorea")
		{
			return QLocale::SouthKorea;//����
		}
		else if(countryQString == "NorthKorea")
		{
			return QLocale::NorthKorea;//����
		}
		else if(countryQString == "Vietnam")
		{
			return QLocale::Vietnam;//Խ��
		}	
		else if(countryQString == "France")
		{
			return QLocale::France;//����
		}
		else if(countryQString == "Germany")
		{
			return QLocale::Germany;//�¹�
		}
		else if(countryQString == "Russia")
		{
			return QLocale::Russia;//����˹
		}
		else if(countryQString == "Italy")
		{
			return QLocale::Italy;//�����
		}
		else if(countryQString == "Myanmar")
		{
			return QLocale::Myanmar;//���
		}
		else if(countryQString == "Bangladesh")
		{
			return QLocale::Bangladesh;//�ϼ�����
		}	
		else if(countryQString == "Netherlands")
		{
			return QLocale::Netherlands;//����
		}
		else if(countryQString == "Irish")
		{
			return QLocale::Ireland;//������
		}
		else if(countryQString == "Mongolia")
		{
			return QLocale::Mongolia;//�ɹ�
		}
		else if(countryQString == "Portugal")
		{
			return QLocale::Portugal;//������
		}
		else if(countryQString == "Spain")
		{
			return QLocale::Spain;//������
		} 
		else if(countryQString == "Philippines")
		{
			return QLocale::Philippines;//���ɱ�
		} 		
		else
		{
			return QLocale::AnyCountry;
		}
	}

	static bool isValidEmail(QString email, bool allowEmpty)
	{
		if (email!="")
		{
			return email.contains("@") && email.contains(".") && email.indexOf("@") > 0
				&& email.indexOf(".") < email.length() - 1
				&& email.lastIndexOf(".") > email.indexOf("@") + 1;
		}
		else
		{
			return allowEmpty;
		}
	}

	static bool isTelphoneNo(QString telphoneNo)
	{
		if (telphoneNo=="" || telphoneNo.length() < 3) return false;
		if (!telphoneNo[0].isDigit() && telphoneNo[0] != '+') return false;
		if (telphoneNo[0] == '+' && !telphoneNo[1].isDigit()) return false;
		for (int index = 1; index < telphoneNo.length(); index++)
		{
			QChar c = telphoneNo.at(index);
			if (!c.isDigit() && c != '-') return false;
		}
		return true;
	}

	static bool isMobileNo(QString telphoneNo)
	{
		for (int index = 0; index < telphoneNo.length(); index++)
		{
			QChar c = telphoneNo.at(index);
			if (!c.isDigit()) return false;
		}
		return true;
	}

	static bool isFaxNo(QString telphoneNo)
	{
		for (int index = 0; index < telphoneNo.length(); index++)
		{
			QChar c = telphoneNo.at(index);
			if (!c.isDigit() && c != '-') return false;
		}
		return true;
	}

	static QString fix(QString value)
	{
		if (value.isEmpty() || value.isNull()) return "";
		value = value.replace("&lt;", "<");
		value = value.replace("&gt;", ">");
		value = value.replace("&amp;", "&");
		value = value.replace("&apos;", "'");
		value = value.replace("&quot;", "\"");
		return value;
	}

	static bool isNumberOrChar(QString value)
	{
		QRegExp rx("^[0-9a-zA-Z]*$"); 
		int i = rx.indexIn(value);
		if(i <0 ) return false;
		else return true;
	}

	static bool compare(QVariant leftData, QVariant rightData, QMetaType::Type type) 
	{  
		if (type == QMetaType::QDateTime) 
		{
			return leftData.toDateTime() < rightData.toDateTime();
		} 
		else if (type == QMetaType::QString) 
		{
			return QString::localeAwareCompare(leftData.toString(), rightData.toString()) < 0;
		} 
		else if (type == QMetaType::Double) 
		{
			return leftData.toDouble() < rightData.toDouble();
		}
		else if (type == QMetaType::Int) 
		{
			return leftData.toInt() < rightData.toInt();
		}
		else 	
			return false;
	} 

	static void fastSortForInstrument(QList<QSharedPointer<SettingInstrument>>& ptr, int begin, int end, Qt::SortOrder order)
	{
		QSharedPointer<SettingInstrument> temp = ptr[begin];//�簴��Sequence�����ظ��ľͰ���ĸ���򣬴�0��ʼ����
		int i = begin + 1, j = end, curPosition = begin;
		bool direction = false;
		while(i <= j)
		{
			if(direction)
			{
				bool compareResult = ptr.at(i)->_sequence == temp->_sequence
					? compare(ptr.at(i)->_code, temp->_code, QMetaType::QString)
					: compare(ptr.at(i)->_sequence, temp->_sequence, QMetaType::Int);			
				if(order == Qt::AscendingOrder ? !compareResult : compareResult)  //�����ǰ����С�ڻ�׼���� ��ô��λ�� �ĵ�ǰλ��
				{
					ptr[curPosition]->_sequence = temp->_sequence;
					ptr.swap(curPosition,i);
					curPosition = i;
					direction = false;
				}
				i++;
			}
			else//�ȴӺ�ǰ�Ƚ�����
			{
				bool compareResult = ptr.at(j)->_sequence == temp->_sequence
					? compare(ptr.at(j)->_code, temp->_code, QMetaType::QString)
					: compare(ptr.at(j)->_sequence, temp->_sequence, QMetaType::Int);	
				if(order == Qt::AscendingOrder ? compareResult : !compareResult)//������һ�����ڻ�׼ ��ô���һ�����ݸ�ֵ����ǰ��׼���ݵ��Ǹ�λ�� ������׼���ݵ�λ��
				{
					ptr[curPosition]->_sequence = temp->_sequence;
					ptr.swap(curPosition,j);
					curPosition = j;
					direction = true;
				}
				j--;
			}
		}     
		temp->_sequence  = curPosition;
		ptr.replace(curPosition, temp);  

		if(curPosition - begin > 1)//ǰ��С�ıȽ�
			fastSortForInstrument(ptr, begin, curPosition - 1, order);
		if(end - curPosition > 1)//�����ıȽ�
			fastSortForInstrument(ptr, curPosition + 1, end, order); 
	}

    static bool isNullOrEmpty(QString value)
	{
		if(value.isEmpty() || value.isNull() || value == "")
			return true;
		return false;
	}

	static QString getHighOrLowFormatString(QString first, QString second, int rearPrice)
	{
		if ((first.isEmpty() || first.isNull() || first.toDouble() == 0.0) && (second.isEmpty() || second.isNull() || second.toDouble() == 0.0)) return "";
		else if (rearPrice == 0 || (second.isEmpty() || second.isNull() || second.toDouble() == 0.0)) return first;
		else
		{
			QString formatString = first;
			formatString.append("/");
			if (rearPrice >= second.length()) formatString.append(second);
			else
			{
                if(first.right(rearPrice) == second.right(rearPrice))
                    rearPrice += 1;
				QString subPrice = second.right(rearPrice);
				if (subPrice.contains("."))
					formatString.append(second.right(rearPrice + 1));
				else
					formatString.append(subPrice);
			}
				
			return formatString;
		}
	}
	
	static bool contains(int mask, int item)
	{
		return (mask & item) == item;
    }

    static double GetReminder(double dDividend, double dDivisor)
    {
        //ȴ����ֵ
        double dNum1 = qFabs(dDividend);
        double dNum2 = qFabs(dDivisor);

        // ����̣�������������
        int iQuotient = (int)(dNum1 / dNum2 + 0.5);
        //��� �� * ����
        double dValue = iQuotient * dNum2;
        //�������
        double dRes = qFabs(dNum1 - dValue);

        return dRes;
    }

    static bool isMultiple(double dDividend, double dDivisor)
    {
        double dReminder = GetReminder(dDividend, dDivisor);
        if(dReminder < _PRECISION)
        {
            return true;
        }
        else
        {
            return false;
        }
    }


}
#endif
