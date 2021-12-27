#pragma  once
#include "historicalchart_global.h"
#include <QVariant>
#include <QVector>
#include <QColor>
#include <vector>
#include <QDebug>
namespace HistoricalChartSpace
{
	class Constants
	{
	public:
		static const QString LogFile;
		
		static const int BidLessThan;
		static const int BidGreaterThan;
		static const int AskLessthan;
		static const int AskGreaterThan;
	};

	enum PriceAlertState
	{
		Pending = 0,
		Hit = 1,
		Processed = 2,
		Expired = 3
	};

	enum FrequencyType
	{
		Second = 1,
		Minute = 2,
		Hour = 3,
		Day = 4,
		Week = 5,
		Month = 6,
		Year = 7,
	};

	enum enSeriesType
	{
		enSeriesType_OPen,
		enSeriesType_Close,
		enSeriesType_High,
		enSeriesType_Low,
		enSeriesType_Volume,
		enSeriesType_Unknown
	};

    // Items that cannot be changed by the user
	enum enMarginSpace
	{
        enMarginXCoordinate,						// The height occupied by the x-axis
        enMarginYCoordinate,						// The width of the Y coordinate axis
        enMarginGridXStandardSpace,					// Standard distance of grid X
        enMarginGridYStandardSpace,					// Standard distance of grid Y
        enMarginOutGridStandardSpace,				// The length of the protruding grid to mark (the convex point in front of the text on the X, Y axis)
        enMarginXTextHSpace,						// The height of the X-axis text
        enMarginXTextWSpace,						// The width of the X axis text
        enMarginYTextHSpace,						// Y axis text height
        enMarginYTextWSpace,						// Y axis text width
        enMarginPriceBarStandardDistance,			// Standard distance of price
        enMarginPriceVolumeSpaceScale,				// Proportion of trading lots
        enMarginValueToBondSpaceScale,				// The ratio of the distance between the data and the boundary
        enMarginCandlestickSpaceScale,				// The ratio of the allocated space occupied by a single candlestick
        enMarginPriceBarLength,						// Price bar graph bar raised length
        enMarginMouseMoveScale,						// Mouse movement ratio
        enMarginSplitHight,							// The height of the separator between panels
        enMarginYcoodinateTinyShift,				// Small deviation of Y coordinate
        enMarginSpaceTextToCoordinate,				// The distance between text and coordinates
        enMarginHalfSpaceLineSelect,				// Half of the selected line
        enMarginTextInfoToLeft,						// The distance between the text display information and the left border
        enMarginTextInfoToTop,						// The distance between the text display information and the upper boundary
        enMarginIndicatorTextInfoW,					// The width of the indicator text display information
        enMarginIndicatorTextInfoH,					// The height of the indicator text display information
        enMarginPriceTextInfoH,						// The height of the price text display information
        enMarginPriceTextInfoW,						// The width of the price text display information
        enMarginRealTimePriceStandardSpace,			// Real-time price standard interval
        enMarginRealTimeCustomCrossCursorMargin,	// Real-time price crosshair cursor interval
        enMarginRealTimeCustomCrossCursorMarginLen,	// The length of the real-time price cross cursor
        enMarginDrawingObjectNodeDia,					// The diameter of the line object node
        enMarginAdjustBound,						// Boundary adaptation
        enMarginDrawingObjectToBound,					// The distance between the line object and the boundary (when the mouse is dragged)
		enMarginUnknown
	};

    // User-configurable items
	enum enParameterConfig
	{
        // Public property start
        enParameterCommonBegin,
        // Common attributes, shared by all members
		enBoolShowGrid,
		enBoolShiftData,
		enBoolAutoScroll,
		enBoolShowLableInfo,

		enColorBackground,
		enColorForeground,
		enColorGrid,
		enColorCoorText,
        // Variable public attributes, members have, but they are different
		enDoubleGlobalScale,
        enParameterCommonEnd,
        // End of public property
		///////////////////////////////////////////////////////////////////////////////////////
        // Price attribute starts
        enParameterPriceBegin,
		enIntPriceDrawType,
		enBoolPriceShowBidLine,
		enBoolPriceShowAskLine,
		enBoolPriceShowVolume,

		enColorPriceBarUp,
		enColorPriceBarDown,
		enColorPriceBullCandle,
		enColorPriceBearCandle,
		enColorPriceLineChart,
		enColorPriceVolume,
		enColorBidLine,
		enColorAskLine,

        enParameterPriceEnd,
        // End of price attribute
		///////////////////////////////////////////////////////////////////////////////////////
        enParameterIndicatorBegin,	// Start of indicator properties
        // The default value is different according to the indicator, so the default parameters of the indicator are no longer specified here
        // To view and modify the default parameters of a specific indicator, please modify InitXXXSettings in IndicatorSettings
        enParameterIndicatorEnd, // End of indicator properties

		enParameterConfig_max
	};

	enum enPanelType
	{
		enPanelPrice,
		enPanelIndicator,
		enPanelUnknown
	};

	enum enDrawingObjectType
	{
		enDrawingObjectHorizontal,
		enDrawingObjectVertical,
		enDrawingObjectTrend,
		enDrawingObjectEllipse,
		enDrawingObjectRect,
		enDrawingObjectTriangle,
		enDrawingObjectFibo,
		enDrawingObjectText,
		enDrawingObjectChannel,
		enDrawingObjectTypeUnknown
	};

	enum enPriceType
	{
		enPriceBar,
		enPriceLine,
		enPriceCandlesticks,
		enPriceUnknown
	};
    // About date and time
	enum enDateToShowType
	{
		enShowYear = 0x01,
		enShowMonth = 0x02,
		enShowDay = 0x04,
		enShowTime = 0x08,
		enShowSecond = 0x10,
		enShowDateUnknown
	};
    // Set the scope of the display indicator or line object
	enum showInTime
	{
		enShowInTime_1min = 0x01,
		enshowInTime_2min = 0x02,
		enshowInTime_3min = 0x04,
		enshowInTime_4min = 0x08,
		enshowInTime_5min = 0x10,
		enshowInTime_6min = 0x20,
		enshowInTime_10min = 0x40,
		enshowInTime_12min = 0x80,
		enshowInTime_15min = 0x0100,
		enshowInTime_20min = 0x0200,
		enshowInTime_30min = 0x0400,
        enshowInTime_mins = 0x07FF,		//Show in all minute charts

		enshowInTime_1hour = 0x0800,
		enshowInTime_2hour = 0x1000,
		enshowInTime_3hour = 0x2000,
		enshowInTime_4hour = 0x4000,
		enshowInTime_6hour = 0x8000,
		enshowInTime_8hour = 0x010000,
		enshowInTime_12hour = 0x020000,
        enshowInTime_hours = 0x03F800,	 //Show in all hour charts

		enshowInTime_daily = 0x040000,
		enshowInTime_weekily = 0x080000,
		enShowInTime_monthly = 0x100000,

        enshowInTime_all = 0x1FFFFF,		//Show all the time
		enshowInTime_unknown,
	};

    // Extension cord settings
	enum RayShow
	{
        RayShow_noRay = 0x00,		//No ray option
        RayShow_Left = 0x01,		//Extend options to the left
        RayShow_Right = 0x02,		//Extend options to the right
        RayShow_Single = 0x04,		//Single direction extension (vertical direction) option

        RayShow_LeftSelected = 0x10,	 // Extend left selection
        RayShow_RightSelected = 0x20,	 // Extend right to select
        RayShow_SingleSelected = 0x40,  // Single direction selection
		RayShow_UnKnown
	};
    // Magnification ratio setting
	enum ZoomLevel
	{
        zoom_minimal = 0x01,			// Minimum ratio
		zoom_tiny = 0x02,
		zoom_small = 0x04,
        zoom_normal = 0x08,			// Normal value
		zoom_big = 0x10,
		zoom_morebig = 0x20,
        zoom_maximal = 0x40,			// Maximum ratio
		zoom_unknown
	};
    //Color day from setting
	enum BrushToFillShape
	{
        // Is there a fill option
		brushToFillShape_noBrush = 0x00,
		brushToFillShape_NeedBrush = 0x01,
        // Whether the fill option is set
		brushToFillShape_SetBrush = 0x02,
		brushToFillShape_unknown = 0x04
	};

    ////chart keyboard shift offset
	//enum keyShiftChart
	//{
	//	keyShiftChart_left,
	//	keyShiftChart_right,
	//	keyShiftChart_unknown
	//};

	enum PeriodType
	{
		Unlimited = 0,
		Today = 1,
		ThisWeek = 2,
		ThisMonth = 3,
		ThisQuarter = 4,
		ThisYear = 5,
		Day2 = 6,
		Day3 = 7,
		Day4 = 8,
		Day5 = 9,
		Day10 = 10,
		Day15 = 15,
		Day30 = 30,
		Day45 = 45,
		Day60 = 60,
		Day90 = 90,
		Day120 = 120,
		Day180 = 180,
		Month1 = 201,
		Month2 = 202,
		Month3 = 203,
		Month6 = 204,
		Year1 = 305,
		Year2 = 306,
		Year3 = 307,
		Year4 = 308,
		Year5 = 309,
		Year10 = 300,
		Year20 = 301,
	};

	static const int ORIGIN_YEAR = 1583;
	static const int ORIGIN_JULIAN = 2299238;
	static const int monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    static double m_MarginXCoordinate = 60.0;
    static double m_MarginYCoordinate = 120.0;

	struct StructDate{
		short Year;
		short Month;
		short Day;
		short Hour;
		short Minute;
		short Second;
		short Millisecond;
		inline bool operator ==(StructDate other){
			return Year == other.Year && Month == other.Month && Day == other.Day 
				&& Hour == other.Hour && Minute == other.Minute && Second == other.Second && Millisecond == other.Millisecond;
		}
	};

	struct PaintArgs
	{
		double startX;
		double canvasWidth;
		double minValue;
		double minY;
		double scaleY;
		QColor foreground;
	};

	struct HISTORICALCHART_EXPORT PrimHistoricalData
	{
		double date;
		double open;
		double high;
		double low;
		double close;
		double volume;
        //double ask;		//To be compatible with PrimRealTimeData
		PrimHistoricalData(){ memset(this, 0, sizeof(*this)); }
	};

	struct HISTORICALCHART_EXPORT PrimRealTimeData
	{
		double date;
		double ask;
		double bid;
		PrimRealTimeData(){ memset(this, 0, sizeof(*this)); }
	};

	struct HISTORICALCHART_EXPORT PrimBORealTimeData
	{
		double date;
		double barStroke;
		PrimBORealTimeData(){ memset(this, 0, sizeof(*this)); }
	};

	struct HISTORICALCHART_EXPORT Frequency
	{
	public:
		Frequency();
		Frequency(FrequencyType type, int unit = 1);
		QString getName() const;
		double getJDateTimeSpan() const;
		int getTimeSpanInSecond() const;
		FrequencyType getType() const;
		int getUint() const;
		bool operator==(const Frequency& that) const;
		bool operator!=(const Frequency& that) const;
		bool isPloidyOf(Frequency& other, int& multiple) const;
		showInTime getTypicalShowTime() const;
	private:
		FrequencyType m_Type;
		int m_Unit;
	};

	struct HISTORICALCHART_EXPORT ChartProperty
	{
		QString instrumentId;
		QString quotePolicyId;
		Frequency frequency;

		QString getKey() const;
	}; 
}
