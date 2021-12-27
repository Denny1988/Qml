#include "Settings.h"
#include "Helper.h"
#include "SC.h"
namespace HistoricalChartSpace
{
	CommonSettings::CommonSettings()
	{
		m_isAutoScroll = Helper::GetDefaultConfig(enBoolAutoScroll).toBool();
		m_isShiftData = Helper::GetDefaultConfig(enBoolShiftData).toBool();
		m_isShowGrid = Helper::GetDefaultConfig(enBoolShowGrid).toBool();
		m_isShowLabelInfo = Helper::GetDefaultConfig(enBoolShowLableInfo).toBool();
		m_background = Helper::GetDefaultConfig(enColorBackground).value<QColor>();
		m_foreground = Helper::GetDefaultConfig(enColorForeground).value<QColor>();
		m_gridColor = Helper::GetDefaultConfig(enColorGrid).value<QColor>();
		m_zoomLevel = zoom_normal;
		m_barDistance = Helper::GetMarginSpaceValue(enMarginPriceBarStandardDistance) * zoom_normal;
	}

	PriceSettings::PriceSettings()
	{
		m_drawType = (enPriceType)Helper::GetDefaultConfig(enIntPriceDrawType).toInt();
		m_isShowBidPriceLine = Helper::GetDefaultConfig(enBoolPriceShowBidLine).toBool();
		m_isShowAskPriceLine = Helper::GetDefaultConfig(enBoolPriceShowAskLine).toBool();
		m_isShowVolume = Helper::GetDefaultConfig(enBoolPriceShowVolume).toBool();
		m_barUpColor = Helper::GetDefaultConfig(enColorPriceBarUp).value<QColor>();
		m_barDownColor = Helper::GetDefaultConfig(enColorPriceBarDown).value<QColor>();
		m_bullCandleColor = Helper::GetDefaultConfig(enColorPriceBullCandle).value<QColor>();
		m_bearCandleColor = Helper::GetDefaultConfig(enColorPriceBearCandle).value<QColor>();
		m_crossStarColor = Helper::GetDefaultConfig(enColorPriceLineChart).value<QColor>();
		m_lineChartColor = QColor(Qt::gray);;
		m_volumesColor = Helper::GetDefaultConfig(enColorPriceVolume).value<QColor>();
		m_bidLineColor = QColor(Qt::darkGreen); //Helper::GetDefaultConfig(enColorBidLine).value<QColor>(); According to requiement of Denny.
		m_askLineColor = Helper::GetDefaultConfig(enColorAskLine).value<QColor>();
		m_openOrderLineColor = QColor(Qt::darkYellow);
		m_pendingOrderLineColor = QColor(Qt::darkGreen);
	}

	RealTimePriceSettings::RealTimePriceSettings()
	{
		m_bDrawGrid = true;
		m_bDrawCrosshair = true;
		m_bDrawAsk = true;
		m_bDrawbid = true;
		m_isShowXaxis = true;

		m_colorGrid = Qt::lightGray;
		m_colorBackground = m_coordinateBackground = Qt::black;
		m_colorForeground = Qt::white;
		m_colorBid = Qt::red;
		m_colorAsk = Qt::green;
		m_colorCrosshair = Qt::lightGray;
	}
	
	BORealTimePriceSettings::BORealTimePriceSettings()
	{
		m_bDrawGrid = true;
		m_bDrawCrosshair = true;
		m_bDrawBarStroke = true;
		m_isShowBOText = true;
		m_isShowXaxis = true;

		m_colorGrid = Qt::lightGray;
		m_colorBackground = m_coordinateBackground = Qt::black;
		m_colorForeground = Qt::white;
		m_colorBarStroke = Qt::green;
		m_colorCrosshair = Qt::lightGray;
		m_colorBOText = QColor("#ffa500");
		m_colorBullish = Qt::green;
		m_colorBearish = Qt::red;
	}

    // Initialization of DrawingObject
	DrawingObjectSettings::DrawingObjectSettings(enDrawingObjectType type) :m_type(type)
	{
		// base Settings
		m_bSelectAble = true;
		m_bAsBackground = false;
		m_colorLine = Qt::red;
		m_intLineType = Qt::SolidLine;
		m_lfLineWidth = 1.0;
		m_intFlagsToShow = enshowInTime_all;
		m_intRayFlags = RayShow_noRay;
		m_intFillShape = brushToFillShape_noBrush;
		m_bhasLevels = false;
		m_levelsColorLine = Qt::yellow;
		m_levelsLineType = Qt::SolidLine;
		m_levelsLineWidth = 1.0;
		//m_index = Common::GetNewObjectNumber(m_type);

		switch (m_type)
		{
		case HistoricalChartSpace::enDrawingObjectHorizontal:
		{
			m_vecValue << 0.0;
		}
		break;
		case HistoricalChartSpace::enDrawingObjectVertical:
		{
			m_intRayFlags = RayShow_Single;
			m_vecDate << 0.0;
		}
		break;
		case HistoricalChartSpace::enDrawingObjectTrend:
		{
			m_intRayFlags = RayShow_Right | RayShow_Left;
			double data = 0.0;
			m_vecDate << data << data;
			m_vecValue << data << data;
		}
		break;
		case HistoricalChartSpace::enDrawingObjectEllipse:
		{
			m_intFillShape = brushToFillShape_NeedBrush;
			double data = 0.0;
			m_vecValue << data << data << data;
			m_vecDate << data << data << data;
		}
		break;
		case HistoricalChartSpace::enDrawingObjectFibo:
		{
			m_bhasLevels = true;
			m_intLineType = Qt::DotLine;
			m_mapLevels = Helper::getDefaultLevels(enDrawingObjectFibo);
			double data = 0.0;
			m_vecDate << data << data;
			m_vecValue << data << data;
		}
		break;
		case HistoricalChartSpace::enDrawingObjectRect:
		{
			m_intFillShape = brushToFillShape_NeedBrush;
			double data = 0.0;
			m_vecDate << data << data;
			m_vecValue << data << data;
		}
		break;
		case HistoricalChartSpace::enDrawingObjectTriangle:
		{
			m_intFillShape = brushToFillShape_NeedBrush;
			double data = 0.0;
			m_vecValue << data << data << data;
			m_vecDate << data << data << data;
		}
		break;
		case HistoricalChartSpace::enDrawingObjectText:
		{
			m_angle = 0;
			m_fontFamily = "Arial";
			m_fontSize = 9;
			m_describe = "Text";
			m_vecValue << 0.0;
			m_vecDate << 0.0;
		}
		break;
		case HistoricalChartSpace::enDrawingObjectChannel:
		{
			m_intRayFlags = RayShow_Right | RayShow_Left;
			double data = 0.0;
            // Control point data
			m_vecDate << data << data << data;
			m_vecValue << data << data << data;
		}
		break;
		case HistoricalChartSpace::enDrawingObjectTypeUnknown:
			break;
		default:
			break;
		}

		m_name = Helper::getDrawingObjBaseStrInfo(m_type);
		//if(m_index != 0)
		//{
		//	m_strLineName += QString::number(m_index + 1);
		//}
	}

}
