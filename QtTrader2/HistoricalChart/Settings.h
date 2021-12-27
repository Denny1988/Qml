#pragma  once
#include "Common.h"
#include <QVector>
#include <QColor>
#include <qnamespace.h>

namespace HistoricalChartSpace
{
    // Note: When modifying the struct, you must pay attention to modifying the Config, because you want to achieve persistence
	struct CommonSettings
	{
		bool	  m_isShowGrid;
		bool      m_isAutoScroll;
		bool	  m_isShiftData;
		bool	  m_isShowLabelInfo;
		bool      m_isShowCrosshair;
		bool      m_isShowQuickPlacer;
		bool      m_isShowExecutedOrder;
		bool      m_isShowPendingOrder;
		QColor	  m_background;
		QColor	  m_foreground;
		QColor	  m_gridColor;
        QColor	  m_productCodeColor;
		ZoomLevel m_zoomLevel;
		double    m_barDistance;
		CommonSettings();
	};

	struct PriceSettings
	{
		enPriceType		m_drawType;
		bool			m_isShowBidPriceLine;
		bool			m_isShowAskPriceLine;
		bool			m_isShowVolume;
		QColor			m_barUpColor;
		QColor			m_barDownColor;
		QColor			m_bullCandleColor;
		QColor			m_bearCandleColor;
		QColor			m_crossStarColor;
		QColor			m_lineChartColor;
		QColor			m_volumesColor;
		QColor			m_bidLineColor;
		QColor			m_askLineColor;
		QColor			m_openOrderLineColor;
		QColor			m_pendingOrderLineColor;
		PriceSettings();
	};	

    // Real-time data display settings
	struct HISTORICALCHART_EXPORT RealTimePriceSettings
	{
		bool m_bDrawGrid;
		bool m_bDrawAsk;
		bool m_bDrawbid;
		bool m_bDrawCrosshair;
		bool m_isShowXaxis;
		QColor m_colorAsk;
		QColor m_colorBid;
		QColor m_colorBackground;
		QColor m_coordinateBackground;
		QColor m_colorForeground;
		QColor m_colorGrid;
		QColor m_colorCrosshair;

		RealTimePriceSettings();
	};

    // Real-time data display settings
	struct HISTORICALCHART_EXPORT BORealTimePriceSettings
	{
		bool m_bDrawGrid;
		bool m_bDrawBarStroke;
		bool m_bDrawCrosshair;
		bool m_isShowXaxis;
		bool m_isShowBOText;
		QColor m_colorBarStroke;
		QColor m_colorBackground;
		QColor m_coordinateBackground;
		QColor m_colorBOText;
		QColor m_colorForeground;
		QColor m_colorGrid;
		QColor m_colorCrosshair;
		QColor m_colorBullish;
		QColor m_colorBearish;

		BORealTimePriceSettings();
	};

	// DrawingObject settings
	struct DrawingObjectSettings
	{
		enDrawingObjectType		m_type;
		bool					m_bSelectAble;
		bool					m_bAsBackground;
		QString					m_name;
		QString					m_describe;
		QColor					m_colorLine;
		double					m_lfLineWidth;
		int						m_intLineType;
		QVector<double>			m_vecDate;
		QVector<double>			m_vecValue;
		int						m_intFlagsToShow;
		int						m_intRayFlags;
		int						m_intFillShape;

        double m_angle;        // Rotation angle
        QString m_fontFamily;  // Font
		int m_fontSize;        // font pointSize

        bool					m_bhasLevels;  // Is there Levels
		QMap<double, QString>	m_mapLevels;
		QColor					m_levelsColorLine;
		double					m_levelsLineWidth;
		int						m_levelsLineType;
        int						m_panelIndex;	// Window index, used for DrawingObjectListDialog.
		DrawingObjectSettings(enDrawingObjectType type = enDrawingObjectTypeUnknown);
	};
}
