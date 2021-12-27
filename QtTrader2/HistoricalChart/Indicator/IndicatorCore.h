#pragma once
#include <QString>
#include <QHash>
#include <QColor>
#include <QVector>

namespace HistoricalChartSpace
{
	// CalculatePriceDependency = CPD
	enum enCPD
	{
		enCPDOpen,
		enCPDClose,
		enCPDHigh,
		enCPDLow,
		enCPDMedian,				//  HL/2
		enCPDTypical,				//  HLC/3
		enCPDWeightedClose,			//  HLCC/4
		emCPDPreviousIndicatorData,
		emCPDFirstIndicatorData,
		enCPDUnknown
	};

	enum enIndicatorDrawType
	{
		enIndicatorDrawLine,
		enIndicatorDrawColumn,
		enIndicatorDrawCurve,
		enIndicatorDrawDot,
		enIndicatorDrawUnknown
	};

	struct IndicatorLineSettings
	{
		QString m_lineName;
		QColor m_lineColor;
		double m_lineWidth;
		Qt::PenStyle m_lineType;
		enIndicatorDrawType m_drawType;
};

#ifdef TA_Lib
    enum LocalMAType	// The MAType here corresponds to TA_MAtype one-to-one
	{
		LocalMAType_SMA = 0,
		LocalMAType_EMA = 1,
		LocalMAType_WMA = 2,
		LocalMAType_DEMA = 3,
		LocalMAType_TEMA = 4,
		LocalMAType_TRIMA = 5,
		LocalMAType_KAMA = 6,
		LocalMAType_MAMA = 7,
		LocalMAType_T3 = 8,
		LocalMAType_Unknown
	};
#else
    enum LocalMAType	// Here there is a one-to-one correspondence between MAType and eMAType (TASDK)
	{
		LocalMAType_SMA = 1,    // indSimpleMovingAverage
		LocalMAType_EMA = 2,    // indExponentialMovingAverage
		LocalMAType_TSMA = 3,   // indTimeSeriesMovingAverage
		LocalMAType_VMA = 4,    // indVariableMovingAverage
		LocalMAType_TRIMA = 5,  // indTriangularMovingAverage
		LocalMAType_WMA = 6,    // indWeightedMovingAverage
        //LocalMAType_VIDYA = 7,	// indVIDYA  One more parameter is required than other moving averages: R2 Scale, so a separate indicator needs to be established.
		LocalMAType_Unknown
	};
#endif

	enum enIndicatorType
	{
		// Trend
		enIndicatorBollingerBands,
		enIndicatorMovingAverageEnvelope,
		enIndicatorMovingAverages,
		enIndicatorParabolicSAR,
		enIndicatorStandardDeviation,
		enIndicatorVIDYA,
		enIndicatorWellesWilderSmoothing,
		enIndicatorTrendEnd,

		// Oscillators
		enIndicatorAroon,
		enIndicatorAroonOscillator,
		enIndicatorChaikinVolatility,
		enIndicatorChandeMomentumOscillator,
		enIndicatorCommodityChannelIndex,
		enIndicatorDetrendedPriceOscillator,
		enIndicatorFractalChaosOscillator,
		enIndicatorMACD,
		enIndicatorMomentumOscillator,
		enIndicatorPriceOscillator,
		enIndicatorRainbowOscillator,
		enIndicatorRelativeStrengthIndex,
		enIndicatorStochasticOscillator,
		enIndicatorTRIX,
		enIndicatorTrueRange,
		enIndicatorUltimateOscillator,
		enIndicatorVerticalHorizontalFilter,
		enIndicatorWilliamsAccumulationDistribution,
		enIndicatorWilliamsR,
		enIndicatorOscillatorsEnd,

		// Volumes
		enIndicatorChaikinMoneyFlow,
		enIndicatorEaseOfMovement,
		enIndicatorMoneyFlowIndex,
		enIndicatorNegativeVolumeIndex,
		enIndicatorOnBalanceVolume,
		enIndicatorPositiveVolumeIndex,
		enIndicatorPriceVolumeTrend,
		enIndicatorTradeVolumeIndex,
		enIndicatorVolumeOscillator,
		enIndicatorVolumeROC,
		enIndicatorVolumesEnd,

		// Others
		enIndicatorAccumulativeSwingIndex,
		enIndicatorFractalChaosBands,
		enIndicatorHighLowBands,
		enIndicatorHighMinusLow,
		enIndicatorLinearRegressionForecast,
		enIndicatorLinearRegressionIntercept,
		enIndicatorLinearRegressionRSquared,
		enIndicatorLinearRegressionSlope,
		enIndicatorMassIndex,
		enIndicatorMedian,
		enIndicatorPerformanceIndex,
		enIndicatorPriceROC,
		enIndicatorSwingIndex,
		enIndicatorTypicalPrice,
		enIndicatorWeightedClose,
		enIndicatorEnd
	};

	class BaseData;
	class Indicator;
	struct IndicatorSettings
	{
		enIndicatorType			m_type;
        enCPD					m_dependency;		// Select dependencies
        LocalMAType				m_maType;			// Choose MA algorithm
		int						m_arrayNum;
		int						m_plotNum;
		bool					m_isInPriceChart;
		QString					m_title;
		QVector<IndicatorLineSettings> m_vectLineSettings;
        QHash<QString, int>		m_hashInputIntData;	   // Input data Int type information
        QHash<QString, double>	m_hashInputDoubleData; // Enter Double type information
		int  m_decimalPlaces;

		IndicatorSettings();

		void updateTitle();
		bool isDataValid() const;
	};

	class Calculator;
	class IndicatorCore
	{
	public:
		static QString getIndicatorName(enIndicatorType type);
        static QString getIndicatorType(enIndicatorType type);
		static Indicator* CreateIndicator(BaseData& baseData, IndicatorSettings& settings, bool needInitialize, Calculator* calculator);
        static QString getIndicatorShorthandName(enIndicatorType type);
        static enIndicatorType getIndicatorTypeByShorthandName(QString shorthandName);

	};
}
