#include "Helper.h"
#include "DrawingHorizontal.h"
#include "DrawingVertical.h"
#include "DrawingTrend.h"
#include "DrawingEllipse.h"
#include "Fibbo.h"
#include "DrawingRect.h"
#include "DrawingTriangle.h"
#include "DrawingText.h"
#include "DrawingChannel.h"
#include "SR.h"
#include <qDebug>
#include <QCoreApplication>
#include <QDir>
#include "float.h"
#include <commonhelper.h>

namespace HistoricalChartSpace
{
    double Helper::m_pixelSizeRatio = 1.0;
	QString Helper::getChartProfilePath()
	{
		QString path = QString("%1/Chart").arg(QCoreApplication::applicationDirPath());
		QDir dir(path);
		if (!dir.exists())
		{
			dir.mkpath(path);
		}
		return path;
	}

	BaseDrawing* Helper::CreateDrawingObject(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state)
	{
		BaseDrawing* drawingObject = NULL;
		switch (settings.m_type)
		{
		case HistoricalChartSpace::enDrawingObjectHorizontal:
		{
			drawingObject = new DrawingHorizontal(settings, parent);
		}
		break;
		case HistoricalChartSpace::enDrawingObjectVertical:
		{
			drawingObject = new DrawingVertical(settings, parent);
		}
		break;
		case HistoricalChartSpace::enDrawingObjectTrend:
		{
			drawingObject = new DrawingTrend(settings, parent, state);
		}
		break;
		case HistoricalChartSpace::enDrawingObjectEllipse:
		{
			drawingObject = new DrawingEllipse(settings, parent, state);
		}
		break;
		case HistoricalChartSpace::enDrawingObjectFibo:
		{
			drawingObject = new Fibbo(settings, parent, state);
		}
		break;
		case HistoricalChartSpace::enDrawingObjectRect:
		{
			drawingObject = new DrawingRect(settings, parent, state);
		}
		break;
		case HistoricalChartSpace::enDrawingObjectTriangle:
		{
			drawingObject = new DrawingTriangle(settings, parent, state);
		}
		break;
		case HistoricalChartSpace::enDrawingObjectText:
		{
			drawingObject = new DrawingText(settings, parent, state);
		}
		break;
		case HistoricalChartSpace::enDrawingObjectChannel:
		{
			drawingObject = new DrawingChannel(settings, parent, state);
		}
		break;
		case HistoricalChartSpace::enDrawingObjectTypeUnknown:
			break;
		default:
			break;
		}

		return drawingObject;
	}

	double Helper::GetMarginSpaceValue(enMarginSpace enType)
	{
        double var = -1.0;
        switch (enType)
        {
        case enMarginXCoordinate:
           #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
                var = 20.0;
            #else
                var = 17.0 * Helper::m_pixelSizeRatio;
            #endif
            break;
        case enMarginYCoordinate:
           #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
                var = 65.0;
            #else
                var = 45.0 * Helper::m_pixelSizeRatio;
            #endif
            break;
        case enMarginGridXStandardSpace:
            var = zoom_maximal * Helper::GetMarginSpaceValue(enMarginPriceBarStandardDistance);
            break;
        case enMarginGridYStandardSpace:
           #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
                var = 30.0;
            #else
                var = 20.0 * Helper::m_pixelSizeRatio;
            #endif
            break;
        case enMarginOutGridStandardSpace:
            var = 4.0;
            break;
        case enMarginXTextHSpace:
           #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
                var = 20.0;
            #else
                var = 10.0 * Helper::m_pixelSizeRatio;
            #endif
            break;
        case enMarginXTextWSpace:
           #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
                var = 120.0;
            #else
                var = 200.0 * Helper::m_pixelSizeRatio;
            #endif
            break;
        case enMarginYTextHSpace:
           #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
                var = 20.0;
            #else
                var = 15.0 * Helper::m_pixelSizeRatio;
            #endif
            break;
        case enMarginYTextWSpace:
           #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
                var = 55.0;
            #else
                var = 45.0 * Helper::m_pixelSizeRatio;
            #endif
            break;
        case enMarginPriceBarStandardDistance:
           #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
                var = 1.5;
            #else
                var = 4.0;
            #endif
            break;
        case enMarginPriceVolumeSpaceScale:
			var = 0.05;
			break;
		case enMarginValueToBondSpaceScale:
			var = 0.02;
			break;
			//case enMarginAdjustHOnce:
			//	var = 2.0;
			//	break;
			//case enMarginAdjustHDoubleFirst:
			//	var = 1.0;
			//	break;
			//case enMarginAdjustHDoubleSecond:
			//	var = 3.0;
			//	break;
		case enMarginCandlestickSpaceScale:
			var = 0.75;
			break;
		case enMarginPriceBarLength:
			var = 4.0;
			break;
		case enMarginMouseMoveScale:
			var = 1.0;
			break;
		case enMarginSplitHight:
			var = 3.0;
			break;
		case enMarginYcoodinateTinyShift:
			var = 1.0;
			break;
		case enMarginSpaceTextToCoordinate:
			var = 3.0;
			break;
		case enMarginHalfSpaceLineSelect:
			var = 10.0;
			break;
		case enMarginTextInfoToLeft:
			var = 5.0;
			break;
		case enMarginTextInfoToTop:
			var = 5.0;
			break;
        case enMarginIndicatorTextInfoW:
            var = 300.0;
			break;
		case enMarginIndicatorTextInfoH:
            var = 20.0;
			break;
        case enMarginPriceTextInfoH:
            var = 165.0;
            break;
        case enMarginPriceTextInfoW:
            #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
                 var = 200.0;
             #else
                 var = 200.0 * Helper::m_pixelSizeRatio;;
             #endif
            break;
		case enMarginRealTimePriceStandardSpace:
			var = 2 * Helper::GetMarginSpaceValue(enMarginPriceBarStandardDistance);
			break;
		case enMarginRealTimeCustomCrossCursorMarginLen:
			var = 11.0;
			break;
		case enMarginRealTimeCustomCrossCursorMargin:
			var = 2.0;
			break;
		case enMarginDrawingObjectNodeDia:
			var = 6.0;
			break;
		case enMarginAdjustBound:
			var = 1.0;
			break;
		case enMarginDrawingObjectToBound:
			var = 5.0;
			break;
		case enMarginUnknown:
			break;
		default:
			break;
		}

		return var;
	}


	QVariant Helper::GetDefaultConfig(enParameterConfig enType)
	{
		QVariant var;
		switch (enType)
		{
		case HistoricalChartSpace::enBoolShowGrid:
			var = true;
			break;
		case HistoricalChartSpace::enBoolShiftData:
			var = false;
			break;
		case HistoricalChartSpace::enBoolShowLableInfo:
			var = true;
			break;
		case HistoricalChartSpace::enColorBackground:
			var = QColor(Qt::black);
			break;
		case HistoricalChartSpace::enColorForeground:
			var = QColor(Qt::white);
			break;
		case HistoricalChartSpace::enColorGrid:
			var = QColor(Qt::gray);
			break;
		case HistoricalChartSpace::enColorCoorText:
			var = QColor(Qt::white);
			break;
		case HistoricalChartSpace::enDoubleGlobalScale:
			var = 1.0;
			break;
		case HistoricalChartSpace::enIntPriceDrawType:
			var = enPriceCandlesticks;
			break;
		case HistoricalChartSpace::enBoolPriceShowBidLine:
			var = true;
			break;
		case HistoricalChartSpace::enBoolPriceShowAskLine:
			var = true;
			break;
		case HistoricalChartSpace::enBoolPriceShowVolume:
			var = true;
			break;
		case HistoricalChartSpace::enColorPriceBarUp:
			var = QColor(Qt::green);
			break;
		case HistoricalChartSpace::enColorPriceBarDown:
			var = QColor(Qt::green);
			break;
		case HistoricalChartSpace::enColorPriceBullCandle:
			var = QColor(Qt::green);
			break;
		case HistoricalChartSpace::enColorPriceBearCandle:
			var = QColor(Qt::red);
			break;
		case HistoricalChartSpace::enColorPriceLineChart:
			var = QColor(Qt::green);
			break;
		case HistoricalChartSpace::enColorPriceVolume:
			var = QColor(Qt::green);
			break;
		//case HistoricalChartSpace::enColorBidLine:
		//	var = QColor(Qt::darkGreen);
		//	break;
		case HistoricalChartSpace::enColorAskLine:
			var = QColor(Qt::red);
			break;
		default:
			break;
		}
		return var;
	}

	//int Helper::GetMaxSizeOfDataSave()
	//{
	//	return 5000;
	//}

	QString Helper::getShowInTimeText(showInTime type)
	{
		QString str;
		switch (type)
		{
		case HistoricalChartSpace::enShowInTime_1min:
			str = QString("1 %1").arg(SR::get("Minute"));
			break;
		case HistoricalChartSpace::enshowInTime_2min:
			str = QString("2 %1").arg(SR::get("Minutes"));
			break;
		case HistoricalChartSpace::enshowInTime_3min:
			str = QString("3 %1").arg(SR::get("Minutes"));
			break;
		case HistoricalChartSpace::enshowInTime_4min:
			str = QString("4 %1").arg(SR::get("Minutes"));
			break;
		case HistoricalChartSpace::enshowInTime_5min:
			str = QString("5 %1").arg(SR::get("Minutes"));
			break;
		case HistoricalChartSpace::enshowInTime_6min:
			str = QString("6 %1").arg(SR::get("Minutes"));
			break;
		case HistoricalChartSpace::enshowInTime_10min:
			str = QString("10 %1").arg(SR::get("Minutes"));
			break;
		case HistoricalChartSpace::enshowInTime_12min:
			str = QString("12 %1").arg(SR::get("Minutes"));
			break;
		case HistoricalChartSpace::enshowInTime_15min:
			str = QString("15 %1").arg(SR::get("Minutes"));
			break;
		case HistoricalChartSpace::enshowInTime_20min:
			str = QString("20 %1").arg(SR::get("Minutes"));
			break;
		case HistoricalChartSpace::enshowInTime_30min:
			str = QString("30 %1").arg(SR::get("Minutes"));
			break;
		case HistoricalChartSpace::enshowInTime_mins:
			str = SR::get("Minutes");
			break;
		case HistoricalChartSpace::enshowInTime_1hour:
			str = QString("1 %1").arg(SR::get("Hour"));
			break;
		case HistoricalChartSpace::enshowInTime_2hour:
			str = QString("2 %1").arg(SR::get("Hours"));
			break;
		case HistoricalChartSpace::enshowInTime_3hour:
			str = QString("3 %1").arg(SR::get("Hours"));
			break;
		case HistoricalChartSpace::enshowInTime_4hour:
			str = QString("4 %1").arg(SR::get("Hours"));
			break;
		case HistoricalChartSpace::enshowInTime_6hour:
			str = QString("6 %1").arg(SR::get("Hours"));
			break;
		case HistoricalChartSpace::enshowInTime_8hour:
			str = QString("8 %1").arg(SR::get("Hours"));
			break;
		case HistoricalChartSpace::enshowInTime_12hour:
			str = QString("12 %1").arg(SR::get("Hours"));
			break;
		case HistoricalChartSpace::enshowInTime_hours:
			str = SR::get("Hours");
			break;
		case HistoricalChartSpace::enshowInTime_daily:
			str = SR::get("Daily");
			break;
		case HistoricalChartSpace::enshowInTime_weekily:
			str = SR::get("Weekly");
			break;
		case HistoricalChartSpace::enShowInTime_monthly:
			str = SR::get("Monthly");
			break;
		case HistoricalChartSpace::enshowInTime_all:
			break;
		case HistoricalChartSpace::enshowInTime_unknown:
			break;
		default:
			break;
		}
		return str;
	}

	QMap<double, QString> Helper::getDefaultLevels(enDrawingObjectType type)
	{
		QMap<double, QString> map;
		switch (type)
		{
		case HistoricalChartSpace::enDrawingObjectFibo:
			map[0] = "0.0";
			map[0.236] = "23.6";
			map[0.382] = "38.2";
			map[0.5] = "50";
			map[0.618] = "61.8";
			map[1.0] = "100";
			map[1.618] = "161.8";
			map[2.618] = "261.8";
			map[4.236] = "423.6";
			break;
		default:
			break;
		}
		return map;
	}

	QString Helper::getDrawingObjBaseStrInfo(enDrawingObjectType type)
	{
		QString str;
		switch (type)
		{
		case HistoricalChartSpace::enDrawingObjectHorizontal:
			str = SR::get("Horizontal");
			break;
		case HistoricalChartSpace::enDrawingObjectVertical:
			str = SR::get("Vertical");
			break;
		case HistoricalChartSpace::enDrawingObjectTrend:
			str = SR::get("Trend");
			break;
		case HistoricalChartSpace::enDrawingObjectEllipse:
			str = SR::get("Ellipse");
			break;
		case HistoricalChartSpace::enDrawingObjectRect:
			str = SR::get("Rect");
			break;
		case HistoricalChartSpace::enDrawingObjectTriangle:
			str = SR::get("Triangle");
			break;
		case HistoricalChartSpace::enDrawingObjectFibo:
			str = SR::get("Fibo");
			break;
		case HistoricalChartSpace::enDrawingObjectText:
			str = SR::get("Text");
			break;
		case HistoricalChartSpace::enDrawingObjectChannel:
			str = SR::get("Channel");
			break;
		case HistoricalChartSpace::enDrawingObjectTypeUnknown:
			break;
		default:
			break;
		}
		return str;
	}

	//int Helper::GetKeyShiftValue(keyShiftChart enType)
	//{
	//	int value = 0;
	//	switch (enType)
	//	{
	//	case HistoricalChartSpace::keyShiftChart_left:
	//		value = 1;
	//		break;
	//	case HistoricalChartSpace::keyShiftChart_right:
	//		value = -1;
	//		break;
	//	case HistoricalChartSpace::keyShiftChart_unknown:
	//		break;
	//	default:
	//		break;
	//	}
	//	return value;
	//}

	double Helper::getMinValue(const std::vector<double>* datas, int begin, int end)
	{
		int size = datas->size();
		if (begin < 0 || end < 0 || begin >= size || end >= size)
		{
			return std::numeric_limits<double>::quiet_NaN();
		}

		if (begin > end)
		{
			int temp = begin;
			begin = end;
			end = temp;
		}
		double min = (*datas)[begin];

		for (int i = begin + 1; i < end; ++i)
		{
            if (std::isnan(min))
			{
				min = (*datas)[i];
			}
			else if (min >(*datas)[i])
			{
				min = (*datas)[i];
			}
		}
		return min;
	}

	double Helper::getMaxValue(const std::vector<double>* datas, int begin, int end)
	{
		int size = datas->size();
		if (begin < 0 || end < 0 || begin >= size || end >= size)
		{
			return std::numeric_limits<double>::quiet_NaN();
		}

		if (begin > end)
		{
			int temp = begin;
			begin = end;
			end = temp;
		}
		double max = (*datas)[begin];

		for (int i = begin + 1; i < end; ++i)
		{
            if (std::isnan(max))
			{
				max = (*datas)[i];
			}
			else if (max < (*datas)[i])
			{
				max = (*datas)[i];
			}
		}
		return max;
	}

    void Helper::setMarginXYCoordinate(double marginXCoordinate, double marginYCoordinate)
    {
        HistoricalChartSpace::m_MarginXCoordinate = marginXCoordinate;
        HistoricalChartSpace::m_MarginYCoordinate = marginYCoordinate;
    }

    QString Helper::getAppDataDirPath()
    {

        return QtFramwork::CommonHelper::getAppDataDirPath();
    }

    QString Helper::getChartResPath(QString filepathArgs)
    {
#ifdef RUNTIME_OS_WIN32
        return filepathArgs;
#elif RUNTIME_OS_MACX
        if(filepathArgs.contains(":/"))
        {
            filepathArgs = filepathArgs.replace(":/", "", Qt::CaseSensitive);
        }
        QString appDataDir = getAppDataDirPath();
        QString ret = QString("%1/Resources/ChartImages/%2").arg(appDataDir).arg(filepathArgs);
        return ret;
#endif
    }

}
