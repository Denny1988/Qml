#include "IndicatorCore.h"
#include "SC.h"
#include "SR.h"
#include "BollingerBands.h"
#include "MovingAverages.h"
#include "VIDYA.h"
#include "ParabolicSAR.h"
#include "StandardDeviation.h"
#include "AroonOscillator.h"
#include "TrueRange.h"
#include "CommodityChannelIndex.h"
#include "ChandeMomentumOscillator.h"
#include "MACD.h"
#include "MomentumOscillator.h"
#include "RelativeStrengthIndex.h"
#include "StochasticOscillator.h"
#include "UltimateOscillator.h"
#include "WilliamsR.h"
#include "Aroon.h"
#include "LinearRegressionForecast.h"
#include "LinearRegressionIntercept.h"
#include "LinearRegressionRSquared.h"
#include "LinearRegressionSlope.h"
#include "Median.h"
#include "TRIX.h"
#include "TypicalPrice.h"
#include "AccumulativeSwingIndex.h"
#include "ChaikinMoneyFlow.h"
#include "ChaikinVolatility.h"
#include "DetrendedPriceOscillator.h"
#include "FractalChaosBands.h"
#include "FractalChaosOscillator.h"
#include "HighLowBands.h"
#include "HighMinusLow.h"
#include "MassIndex.h"
#include "MoneyFlowIndex.h"
#include "MovingAverageEnvelope.h"
#include "PerformanceIndex.h"
#include "PriceOscillator.h"
#include "PriceROC.h"
#include "RainbowOscillator.h"
#include "SwingIndex.h"
#include "VerticalHorizontalFilter.h"
#include "WeightedClose.h"
#include "WellesWilderSmoothing.h"
#include "WilliamsAccumulationDistribution.h"
#include "EaseOfMovement.h"
#include "NegativeVolumeIndex.h"
#include "PositiveVolumeIndex.h"
#include "OnBalanceVolume.h"
#include "PriceVolumeTrend.h"
#include "TradeVolumeIndex.h"
#include "VolumeOscillator.h"
#include "VolumeROC.h"

namespace HistoricalChartSpace
{
	IndicatorSettings::IndicatorSettings()
		: m_plotNum(0),
		m_dependency(enCPDUnknown),
		m_maType(LocalMAType_Unknown),
		m_isInPriceChart(false),
		m_decimalPlaces(3)
	{
	}

	void IndicatorSettings::updateTitle()
	{
		m_title = IndicatorCore::getIndicatorName(m_type);

		QHashIterator<QString, int> itInt(m_hashInputIntData);
		while (itInt.hasNext())
		{
			itInt.next();
			m_title += "(" + QString::number(itInt.value()) + ")";
		}
		QHashIterator<QString, double> itDouble(m_hashInputDoubleData);
		while (itDouble.hasNext())
		{
			itDouble.next();
			m_title += "(" + QString::number(itDouble.value()) + ")";
		}
	}

	bool IndicatorSettings::isDataValid() const
	{
		return true;
	}

    QString IndicatorCore::getIndicatorType(enIndicatorType type)
    {
        QString str;
        switch (type)
        {
        case HistoricalChartSpace::enIndicatorAroon:
            str = "Aroon";
            break;
        case HistoricalChartSpace::enIndicatorMACD:
            str = "MACD";
            break;
        case HistoricalChartSpace::enIndicatorStochasticOscillator:
            str = "StochasticOscillator";
            break;
        case HistoricalChartSpace::enIndicatorAroonOscillator:
            str = "AroonOscillator";
            break;
        case HistoricalChartSpace::enIndicatorBollingerBands:
            str = "BollingerBands";
            break;
        case HistoricalChartSpace::enIndicatorChandeMomentumOscillator:
            str = "ChandeMomentumOscillator";
            break;
        case HistoricalChartSpace::enIndicatorCommodityChannelIndex:
            str = "CommodityChannelIndex";
            break;
        case HistoricalChartSpace::enIndicatorMovingAverages:
            str = "MovingAverages";
            break;
        case HistoricalChartSpace::enIndicatorVIDYA:
            str = "VIDYA";
            break;
        case HistoricalChartSpace::enIndicatorLinearRegressionForecast:
            str = "LinearRegressionForecast";
            break;
        case HistoricalChartSpace::enIndicatorLinearRegressionIntercept:
            str = "LinearRegressionIntercept";
            break;
        case HistoricalChartSpace::enIndicatorLinearRegressionRSquared:
            str = "LinearRegressionRSquared";
            break;
        case HistoricalChartSpace::enIndicatorLinearRegressionSlope:
            str = "LinearRegressionSlope";
            break;
        case HistoricalChartSpace::enIndicatorMedian:
            str = "Median";
            break;
        case HistoricalChartSpace::enIndicatorMomentumOscillator:
            str = "MomentumOscillator";
            break;
        case HistoricalChartSpace::enIndicatorParabolicSAR:
            str = "ParabolicSAR";
            break;
        case HistoricalChartSpace::enIndicatorRelativeStrengthIndex:
            str = "RelativeStrengthIndex";
            break;
        case HistoricalChartSpace::enIndicatorStandardDeviation:
            str = "StandardDeviation";
            break;
        case HistoricalChartSpace::enIndicatorTRIX:
            str = "TRIX";
            break;
        case HistoricalChartSpace::enIndicatorTrueRange:
            str = "TrueRange";
            break;
        case HistoricalChartSpace::enIndicatorTypicalPrice:
            str = "TypicalPrice";
            break;
        case HistoricalChartSpace::enIndicatorUltimateOscillator:
            str = "UltimateOscillator";
            break;
        case HistoricalChartSpace::enIndicatorWilliamsR:
            str = "WilliamsR";
            break;
        case HistoricalChartSpace::enIndicatorAccumulativeSwingIndex:
            str = "AccumulativeSwingIndex";
            break;
        case HistoricalChartSpace::enIndicatorChaikinMoneyFlow:
            str = "ChaikinMoneyFlow";
            break;
        case HistoricalChartSpace::enIndicatorChaikinVolatility:
            str = "ChaikinVolatility";
            break;
        case HistoricalChartSpace::enIndicatorDetrendedPriceOscillator:
            str = "DetrendedPriceOscillator";
            break;
        case HistoricalChartSpace::enIndicatorFractalChaosBands:
            str = "FractalChaosBands";
            break;
        case HistoricalChartSpace::enIndicatorFractalChaosOscillator:
            str = "FractalChaosOscillator";
            break;
        case HistoricalChartSpace::enIndicatorHighLowBands:
            str = "HighLowBands";
            break;
        case HistoricalChartSpace::enIndicatorHighMinusLow:
            str = "HighMinusLow";
            break;
        case HistoricalChartSpace::enIndicatorMassIndex:
            str = "MassIndex";
            break;
        case HistoricalChartSpace::enIndicatorMoneyFlowIndex:
            str = "MoneyFlowIndex";
            break;
        case HistoricalChartSpace::enIndicatorMovingAverageEnvelope:
            str = "MovingAverageEnvelope";
            break;
        case HistoricalChartSpace::enIndicatorPerformanceIndex:
            str = "PerformanceIndex";
            break;
        case HistoricalChartSpace::enIndicatorPriceOscillator:
            str = "PriceOscillator";
            break;
        case HistoricalChartSpace::enIndicatorPriceROC:
            str = "PriceROC";
            break;
        case HistoricalChartSpace::enIndicatorRainbowOscillator:
            str = "RainbowOscillator";
            break;
        case HistoricalChartSpace::enIndicatorSwingIndex:
            str = "SwingIndex";
            break;
        case HistoricalChartSpace::enIndicatorVerticalHorizontalFilter:
            str = "VerticalHorizontalFilter";
            break;
        case HistoricalChartSpace::enIndicatorWeightedClose:
            str = "WeightedClose";
            break;
        case HistoricalChartSpace::enIndicatorWellesWilderSmoothing:
            str = "WellesWilderSmoothing";
            break;
        case HistoricalChartSpace::enIndicatorWilliamsAccumulationDistribution:
            str = "WilliamsAccumulationDistribution";
            break;
        case HistoricalChartSpace::enIndicatorEaseOfMovement:
            str = "EaseOfMovement";
            break;
        case HistoricalChartSpace::enIndicatorNegativeVolumeIndex:
            str = "NegativeVolumeIndex";
            break;
        case HistoricalChartSpace::enIndicatorPositiveVolumeIndex:
            str = "PositiveVolumeIndex";
            break;
        case HistoricalChartSpace::enIndicatorOnBalanceVolume:
            str = "OnBalanceVolume";
            break;
        case HistoricalChartSpace::enIndicatorPriceVolumeTrend:
            str = "PriceVolumeTrend";
            break;
        case HistoricalChartSpace::enIndicatorTradeVolumeIndex:
            str = "TradeVolumeIndex";
            break;
        case HistoricalChartSpace::enIndicatorVolumeOscillator:
            str = "VolumeOscillator";
            break;
        case HistoricalChartSpace::enIndicatorVolumeROC:
            str = "VolumeROC";
            break;
        case HistoricalChartSpace::enIndicatorEnd:
            break;
        default:
            break;
        }
        return str;
    }

	QString IndicatorCore::getIndicatorName(enIndicatorType type)
	{
        QString str = getIndicatorType(type);
		return SR::get(str);
	}

	Indicator* IndicatorCore::CreateIndicator(BaseData& baseData, IndicatorSettings& settings, bool needInitialize, Calculator* calculator)
	{
		Indicator* pIndicator = nullptr;
		switch (settings.m_type)
		{
		case enIndicatorBollingerBands:
			pIndicator = new BollingerBands(settings, baseData, needInitialize);
			break;
		case enIndicatorMovingAverages:
			pIndicator = new MovingAverages(settings, baseData, needInitialize);
			break;
		case enIndicatorVIDYA:
			pIndicator = new VIDYA(settings, baseData, needInitialize);
			break;
		case enIndicatorParabolicSAR:
			pIndicator = new ParabolicSAR(settings, baseData, needInitialize);
			break;
		case enIndicatorStandardDeviation:
			pIndicator = new StandardDeviation(settings, baseData, needInitialize);
			break;
		case enIndicatorAroonOscillator:
			pIndicator = new AroonOscillator(settings, baseData, needInitialize);
			break;
		case enIndicatorTrueRange:
			pIndicator = new TrueRange(settings, baseData, needInitialize);
			break;
		case enIndicatorCommodityChannelIndex:
			pIndicator = new CommodityChannelIndex(settings, baseData, needInitialize);
			break;
		case enIndicatorChandeMomentumOscillator:
			pIndicator = new ChandeMomentumOscillator(settings, baseData, needInitialize);
			break;
		case enIndicatorMACD:
			pIndicator = new MACD(settings, baseData, needInitialize);
			break;
		case enIndicatorMomentumOscillator:
			pIndicator = new MomentumOscillator(settings, baseData, needInitialize);
			break;
		case enIndicatorRelativeStrengthIndex:
			pIndicator = new RelativeStrengthIndex(settings, baseData, needInitialize);
			break;
		case enIndicatorStochasticOscillator:
			pIndicator = new StochasticOscillator(settings, baseData, needInitialize);
			break;
		case enIndicatorUltimateOscillator:
			pIndicator = new UltimateOscillator(settings, baseData, needInitialize);
			break;
		case enIndicatorWilliamsR:
			pIndicator = new WilliamsR(settings, baseData, needInitialize);
			break;
		case enIndicatorAroon:
			pIndicator = new Aroon(settings, baseData, needInitialize);
			break;
		case enIndicatorLinearRegressionForecast:
			pIndicator = new LinearRegressionForecast(settings, baseData, needInitialize);
			break;
		case enIndicatorLinearRegressionIntercept:
			pIndicator = new LinearRegressionIntercept(settings, baseData, needInitialize);
			break;
		case enIndicatorLinearRegressionRSquared:
			pIndicator = new LinearRegressionRSquared(settings, baseData, needInitialize);
			break;
		case enIndicatorLinearRegressionSlope:
			pIndicator = new LinearRegressionSlope(settings, baseData, needInitialize);
			break;
		case enIndicatorMedian:
			pIndicator = new Median(settings, baseData, needInitialize);
			break;
		case enIndicatorTRIX:
			pIndicator = new TRIX(settings, baseData, needInitialize);
			break;
		case enIndicatorTypicalPrice:
			pIndicator = new TypicalPrice(settings, baseData, needInitialize);
			break;
		case enIndicatorAccumulativeSwingIndex:
			pIndicator = new AccumulativeSwingIndex(settings, baseData, needInitialize);
			break;
		case enIndicatorChaikinMoneyFlow:
			pIndicator = new ChaikinMoneyFlow(settings, baseData, needInitialize);
			break;
		case enIndicatorChaikinVolatility:
			pIndicator = new ChaikinVolatility(settings, baseData, needInitialize);
			break;
		case enIndicatorDetrendedPriceOscillator:
			pIndicator = new DetrendedPriceOscillator(settings, baseData, needInitialize);
			break;
		case enIndicatorFractalChaosBands:
			pIndicator = new FractalChaosBands(settings, baseData, needInitialize);
			break;
		case enIndicatorFractalChaosOscillator:
			pIndicator = new FractalChaosOscillator(settings, baseData, needInitialize);
			break;
		case enIndicatorHighLowBands:
			pIndicator = new HighLowBands(settings, baseData, needInitialize);
			break;
		case enIndicatorHighMinusLow:
			pIndicator = new HighMinusLow(settings, baseData, needInitialize);
			break;
		case enIndicatorMassIndex:
			pIndicator = new MassIndex(settings, baseData, needInitialize);
			break;
		case enIndicatorMoneyFlowIndex:
			pIndicator = new MoneyFlowIndex(settings, baseData, needInitialize);
			break;
		case enIndicatorMovingAverageEnvelope:
			pIndicator = new MovingAverageEnvelope(settings, baseData, needInitialize);
			break;
		case enIndicatorPerformanceIndex:
			pIndicator = new PerformanceIndex(settings, baseData, needInitialize);
			break;
		case enIndicatorPriceOscillator:
			pIndicator = new PriceOscillator(settings, baseData, needInitialize);
			break;
		case enIndicatorPriceROC:
			pIndicator = new PriceROC(settings, baseData, needInitialize);
			break;
		case enIndicatorRainbowOscillator:
			pIndicator = new RainbowOscillator(settings, baseData, needInitialize);
			break;
		case enIndicatorSwingIndex:
			pIndicator = new SwingIndex(settings, baseData, needInitialize);
			break;
		case enIndicatorVerticalHorizontalFilter:
			pIndicator = new VerticalHorizontalFilter(settings, baseData, needInitialize);
			break;
		case enIndicatorWeightedClose:
			pIndicator = new WeightedClose(settings, baseData, needInitialize);
			break;
		case enIndicatorWellesWilderSmoothing:
			pIndicator = new WellesWilderSmoothing(settings, baseData, needInitialize);
			break;
		case enIndicatorWilliamsAccumulationDistribution:
			pIndicator = new WilliamsAccumulationDistribution(settings, baseData, needInitialize);
			break;
		case enIndicatorEaseOfMovement:
			pIndicator = new EaseOfMovement(settings, baseData, needInitialize);
			break;
		case enIndicatorNegativeVolumeIndex:
			pIndicator = new NegativeVolumeIndex(settings, baseData, needInitialize);
			break;
		case enIndicatorOnBalanceVolume:
			pIndicator = new OnBalanceVolume(settings, baseData, needInitialize);
			break;
		case enIndicatorPositiveVolumeIndex:
			pIndicator = new PositiveVolumeIndex(settings, baseData, needInitialize);
			break;
		case enIndicatorPriceVolumeTrend:
			pIndicator = new PriceVolumeTrend(settings, baseData, needInitialize);
			break;
		case enIndicatorTradeVolumeIndex:
			pIndicator = new TradeVolumeIndex(settings, baseData, needInitialize);
			break;
		case enIndicatorVolumeOscillator:
			pIndicator = new VolumeOscillator(settings, baseData, needInitialize);
			break;
		case enIndicatorVolumeROC:
			pIndicator = new VolumeROC(settings, baseData, needInitialize);
			break;
		case enIndicatorEnd:
			break;
		default:
			break;
		}

		pIndicator->setCalculator(calculator);
		if (baseData.size() > 0)
		{
			calculator->beginCalculateAll(pIndicator);
		}
		return pIndicator;
	}

    QString IndicatorCore::getIndicatorShorthandName(enIndicatorType type)
    {
        QString str;
        switch (type)
        {
        case HistoricalChartSpace::enIndicatorAroon:
            str = "AN";
            break;
        case HistoricalChartSpace::enIndicatorMACD:
            str = "MACD";
            break;
        case HistoricalChartSpace::enIndicatorStochasticOscillator:
            str = "SKD";
            break;
        case HistoricalChartSpace::enIndicatorAroonOscillator:
            str = "AO";
            break;
        case HistoricalChartSpace::enIndicatorBollingerBands:
            str = "BB";
            break;
        case HistoricalChartSpace::enIndicatorChandeMomentumOscillator:
            str = "CMO";
            break;
        case HistoricalChartSpace::enIndicatorCommodityChannelIndex:
            str = "CCI";
            break;
        case HistoricalChartSpace::enIndicatorMovingAverages:
            str = "MA";
            break;
        case HistoricalChartSpace::enIndicatorVIDYA:
            str = "VIDYA";
            break;
        case HistoricalChartSpace::enIndicatorLinearRegressionForecast:
            str = "LRF";
            break;
        case HistoricalChartSpace::enIndicatorLinearRegressionIntercept:
            str = "LRI";
            break;
        case HistoricalChartSpace::enIndicatorLinearRegressionRSquared:
            str = "LRRS";
            break;
        case HistoricalChartSpace::enIndicatorLinearRegressionSlope:
            str = "LRS";
            break;
        case HistoricalChartSpace::enIndicatorMedian:
            str = "MD";
            break;
        case HistoricalChartSpace::enIndicatorMomentumOscillator:
            str = "MO";
            break;
        case HistoricalChartSpace::enIndicatorParabolicSAR:
            str = "PSAR";
            break;
        case HistoricalChartSpace::enIndicatorRelativeStrengthIndex:
            str = "RSI";
            break;
        case HistoricalChartSpace::enIndicatorStandardDeviation:
            str = "SD";
            break;
        case HistoricalChartSpace::enIndicatorTRIX:
            str = "TRIX";
            break;
        case HistoricalChartSpace::enIndicatorTrueRange:
            str = "TR";
            break;
        case HistoricalChartSpace::enIndicatorTypicalPrice:
            str = "TP";
            break;
        case HistoricalChartSpace::enIndicatorUltimateOscillator:
            str = "UO";
            break;
        case HistoricalChartSpace::enIndicatorWilliamsR:
            str = "W&R";
            break;
        case HistoricalChartSpace::enIndicatorAccumulativeSwingIndex:
            str = "ASI";
            break;
        case HistoricalChartSpace::enIndicatorChaikinMoneyFlow:
            str = "CMF";
            break;
        case HistoricalChartSpace::enIndicatorChaikinVolatility:
            str = "CV";
            break;
        case HistoricalChartSpace::enIndicatorDetrendedPriceOscillator:
            str = "DPO";
            break;
        case HistoricalChartSpace::enIndicatorFractalChaosBands:
            str = "FCB";
            break;
        case HistoricalChartSpace::enIndicatorFractalChaosOscillator:
            str = "FCO";
            break;
        case HistoricalChartSpace::enIndicatorHighLowBands:
            str = "HLB";
            break;
        case HistoricalChartSpace::enIndicatorHighMinusLow:
            str = "HML";
            break;
        case HistoricalChartSpace::enIndicatorMassIndex:
            str = "MI";
            break;
        case HistoricalChartSpace::enIndicatorMoneyFlowIndex:
            str = "MFI";
            break;
        case HistoricalChartSpace::enIndicatorMovingAverageEnvelope:
            str = "MAE";
            break;
        case HistoricalChartSpace::enIndicatorPerformanceIndex:
            str = "PI";
            break;
        case HistoricalChartSpace::enIndicatorPriceOscillator:
            str = "PO";
            break;
        case HistoricalChartSpace::enIndicatorPriceROC:
            str = "PROC";
            break;
        case HistoricalChartSpace::enIndicatorRainbowOscillator:
            str = "RO";
            break;
        case HistoricalChartSpace::enIndicatorSwingIndex:
            str = "SI";
            break;
        case HistoricalChartSpace::enIndicatorVerticalHorizontalFilter:
            str = "VHF";
            break;
        case HistoricalChartSpace::enIndicatorWeightedClose:
            str = "WC";
            break;
        case HistoricalChartSpace::enIndicatorWellesWilderSmoothing:
            str = "WWS";
            break;
        case HistoricalChartSpace::enIndicatorWilliamsAccumulationDistribution:
            str = "WAD";
            break;
        case HistoricalChartSpace::enIndicatorEaseOfMovement:
            str = "EM";
            break;
        case HistoricalChartSpace::enIndicatorNegativeVolumeIndex:
            str = "NVI";
            break;
        case HistoricalChartSpace::enIndicatorPositiveVolumeIndex:
            str = "PVI";
            break;
        case HistoricalChartSpace::enIndicatorOnBalanceVolume:
            str = "OBV";
            break;
        case HistoricalChartSpace::enIndicatorPriceVolumeTrend:
            str = "PVT";
            break;
        case HistoricalChartSpace::enIndicatorTradeVolumeIndex:
            str = "TVI";
            break;
        case HistoricalChartSpace::enIndicatorVolumeOscillator:
            str = "VO";
            break;
        case HistoricalChartSpace::enIndicatorVolumeROC:
            str = "VROC";
            break;
        case HistoricalChartSpace::enIndicatorEnd:
            break;
        default:
            break;
        }
        return str;
    }

    enIndicatorType IndicatorCore::getIndicatorTypeByShorthandName(QString shorthandName)
    {
        if(shorthandName == "AN")
            return HistoricalChartSpace::enIndicatorAroon;
        else if(shorthandName == "MACD")
            return HistoricalChartSpace::enIndicatorMACD;
        else if(shorthandName == "SKD")
            return HistoricalChartSpace::enIndicatorStochasticOscillator;
        else if(shorthandName == "AO")
            return HistoricalChartSpace::enIndicatorAroonOscillator;
        else if(shorthandName == "BB")
            return HistoricalChartSpace::enIndicatorBollingerBands;
        else if(shorthandName == "CMO")
            return HistoricalChartSpace::enIndicatorChandeMomentumOscillator;
        else if(shorthandName == "CCI")
            return HistoricalChartSpace::enIndicatorCommodityChannelIndex;
        else if(shorthandName == "MA")
            return HistoricalChartSpace::enIndicatorMovingAverages;
        else if(shorthandName == "VIDYA")
            return HistoricalChartSpace::enIndicatorVIDYA;
        else if(shorthandName == "LRF")
            return HistoricalChartSpace::enIndicatorLinearRegressionForecast;
        else if(shorthandName == "LRI")
            return HistoricalChartSpace::enIndicatorLinearRegressionIntercept;
        else if(shorthandName == "LRRS")
            return HistoricalChartSpace::enIndicatorLinearRegressionRSquared;
        else if(shorthandName == "LRS")
            return HistoricalChartSpace::enIndicatorLinearRegressionSlope;
        else if(shorthandName == "MD")
            return HistoricalChartSpace::enIndicatorMedian;
        else if(shorthandName == "MO")
            return HistoricalChartSpace::enIndicatorMomentumOscillator;
        else if(shorthandName == "PSAR")
            return HistoricalChartSpace::enIndicatorParabolicSAR;
        else if(shorthandName == "RSI")
            return HistoricalChartSpace::enIndicatorRelativeStrengthIndex;
        else if(shorthandName == "SD")
            return HistoricalChartSpace::enIndicatorStandardDeviation;
        else if(shorthandName == "TRIX")
            return HistoricalChartSpace::enIndicatorTRIX;
        else if(shorthandName == "TR")
            return HistoricalChartSpace::enIndicatorTrueRange;
        else if(shorthandName == "TP")
            return HistoricalChartSpace::enIndicatorTypicalPrice;
        else if(shorthandName == "UO")
            return HistoricalChartSpace::enIndicatorUltimateOscillator;
        else if(shorthandName == "W&R")
            return HistoricalChartSpace::enIndicatorWilliamsR;
        else if(shorthandName == "ASI")
            return HistoricalChartSpace::enIndicatorAccumulativeSwingIndex;
        else if(shorthandName == "CMF")
            return HistoricalChartSpace::enIndicatorChaikinMoneyFlow;
        else if(shorthandName == "CV")
            return HistoricalChartSpace::enIndicatorChaikinVolatility;
        else if(shorthandName == "DPO")
            return HistoricalChartSpace::enIndicatorDetrendedPriceOscillator;
        else if(shorthandName == "FCB")
            return HistoricalChartSpace::enIndicatorFractalChaosBands;
        else if(shorthandName == "FCO")
            return HistoricalChartSpace::enIndicatorFractalChaosOscillator;
        else if(shorthandName == "HLB")
            return HistoricalChartSpace::enIndicatorHighLowBands;
        else if(shorthandName == "HML")
            return HistoricalChartSpace::enIndicatorHighMinusLow;
        else if(shorthandName == "MI")
            return HistoricalChartSpace::enIndicatorMassIndex;
        else if(shorthandName == "MFI")
            return HistoricalChartSpace::enIndicatorMoneyFlowIndex;
        else if(shorthandName == "MAE")
            return HistoricalChartSpace::enIndicatorMovingAverageEnvelope;
        else if(shorthandName == "PI")
            return HistoricalChartSpace::enIndicatorPerformanceIndex;
        else if(shorthandName == "PO")
            return HistoricalChartSpace::enIndicatorPriceOscillator;
        else if(shorthandName == "PROC")
            return HistoricalChartSpace::enIndicatorPriceROC;
        else if(shorthandName == "RO")
            return HistoricalChartSpace::enIndicatorRainbowOscillator;
        else if(shorthandName == "SI")
            return HistoricalChartSpace::enIndicatorSwingIndex;
        else if(shorthandName == "VHF")
            return HistoricalChartSpace::enIndicatorVerticalHorizontalFilter;
        else if(shorthandName == "WC")
            return HistoricalChartSpace::enIndicatorWeightedClose;
        else if(shorthandName == "WWS")
            return HistoricalChartSpace::enIndicatorWellesWilderSmoothing;
        else if(shorthandName == "WAD")
            return HistoricalChartSpace::enIndicatorWilliamsAccumulationDistribution;
        else if(shorthandName == "EM")
            return HistoricalChartSpace::enIndicatorEaseOfMovement;
        else if(shorthandName == "NVI")
            return HistoricalChartSpace::enIndicatorNegativeVolumeIndex;
        else if(shorthandName == "PVI")
            return HistoricalChartSpace::enIndicatorPositiveVolumeIndex;
        else if(shorthandName == "OBV")
            return HistoricalChartSpace::enIndicatorOnBalanceVolume;
        else if(shorthandName == "PVT")
            return HistoricalChartSpace::enIndicatorPriceVolumeTrend;
        else if(shorthandName == "TVI")
            return HistoricalChartSpace::enIndicatorTradeVolumeIndex;
        else if(shorthandName == "VO")
            return HistoricalChartSpace::enIndicatorVolumeOscillator;
        else
            return HistoricalChartSpace::enIndicatorVolumeROC;
    }

}
