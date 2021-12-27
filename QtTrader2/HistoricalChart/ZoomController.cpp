#include "ZoomController.h"
#include "historicalchart.h"
#include "PricePanel.h"
#include "ToolBar.h"

namespace HistoricalChartSpace
{
	double ZoomController::getBarDistance(ZoomLevel zoomLevel)
	{
		return Helper::GetMarginSpaceValue(enMarginPriceBarStandardDistance) * zoomLevel;
	}

	ZoomController::ZoomController(HistoricalChart& view, Config& config, QGraphicsRectItem& mainPainterPanel)
		: m_view(view), m_config(config), m_mainPainterPanel(mainPainterPanel)
	{
		double barDistance = m_config.getCommonSettings().m_barDistance;
		double minBarDistance = ZoomController::getBarDistance(ZoomLevel::zoom_minimal);
		double maxBarDistance = ZoomController::getBarDistance(ZoomLevel::zoom_maximal);
		if (barDistance < minBarDistance)
		{
			barDistance = minBarDistance;
		}
		else if (barDistance > maxBarDistance)
		{
			barDistance = maxBarDistance;
		}

		this->updateBarDistance(barDistance);
	}

	ZoomController::~ZoomController()
	{
	}

	void ZoomController::zoom(bool isZoomIn)
	{
		if (this->isSmallestLevel() && !isZoomIn || this->isLargestLevel() && isZoomIn)
		{
			return;
		}
		const double rate = 0.2;
		double zoomRate = 1 + (isZoomIn ? rate : -rate);
		double barDistance = m_view.m_baseData.getBarDistance() * zoomRate;
		this->updateBarDistance(barDistance);
		m_view.updateChart(true);
		this->setToolbarState();
	}

	// called from commonProperties Setting Dialog:
	void ZoomController::setZoomLevel(ZoomLevel zoomLevel)
	{
		if (!m_view.isInFixPeriodMode())
		{
			m_zoomRangeStack.clear();
		}

		double barDistance = ZoomController::getBarDistance(zoomLevel);
		this->updateBarDistance(barDistance);
		m_view.updateChart(true);
	}

	void ZoomController::zoomAll()
	{
		m_zoomRangeStack.clear();
		this->zoomRagne(0, m_view.m_baseData.size() - 1);
	}

	void ZoomController::zoomRagne(int beginIndex, int endIndex)
	{
		BarRange range = { beginIndex , endIndex };

		// if current range changed, push real current range first:
		if (!m_zoomRangeStack.empty())
		{
			BarRange prevRange = m_zoomRangeStack.back();
			BarRange realRange = { m_view.m_baseData.getBeginIndex(), m_view.m_baseData.getEndIndex() };
			if (prevRange != realRange)
			{
				m_zoomRangeStack.push_back(realRange);
			}
		}

		m_zoomRangeStack.push_back(range);
		this->internalZoomRagne(beginIndex, endIndex);
	}
	void ZoomController::zoomRagneForUnlimited(int beginIndex, int endIndex)
	{
		if (m_zoomRangeStack.size() == 0)
		{
			// only need first range data for get same behavior as fixedPreriod
			BarRange prevRange = { m_view.m_baseData.getBeginIndex(), m_view.m_baseData.getEndIndex() };
			m_zoomRangeStack.push_back(prevRange);
		}
		this->zoomRagne(beginIndex, endIndex);
	}
	void ZoomController::zoomCurrentRagne()
	{
		if (!m_zoomRangeStack.empty())
		{
			BarRange range = m_zoomRangeStack.back();
			this->internalZoomRagne(range.BeginIndex, range.EndIndex);
		}
	}
	
	bool ZoomController::zoomPrevRagne()
	{
		if (!m_zoomRangeStack.empty())
		{
			BarRange range = m_zoomRangeStack.back();
			BarRange realRange = { m_view.m_baseData.getBeginIndex(), m_view.m_baseData.getEndIndex() };
			if (range != realRange)
			{
				this->internalZoomRagne(range.BeginIndex, range.EndIndex);
				return true;
			}
			else
			{
				m_zoomRangeStack.pop_back();
				if (!m_zoomRangeStack.empty())
				{
					BarRange range = m_zoomRangeStack.back();
					this->internalZoomRagne(range.BeginIndex, range.EndIndex);
					return true;
				}
			}
		}
		return false;
	}

	//bool ZoomController::isOnZoomStackHead()
	//{
	//	return m_zoomRangeStack.size() <= 1;
	//}

	bool ZoomController::isSmallestLevel()
	{
		double minBarDistance = ZoomController::getBarDistance(ZoomLevel::zoom_minimal);
		return m_view.m_baseData.getBarDistance() <= minBarDistance;
	}

	bool ZoomController::isLargestLevel()
	{
		double maxBarDistance = ZoomController::getBarDistance(ZoomLevel::zoom_maximal);
		return m_view.m_baseData.getBarDistance() >= maxBarDistance;
	}

	void ZoomController::internalZoomRagne(int beginIndex, int endIndex)
	{
		m_view.m_baseData.setBeginIndex(beginIndex);
		m_view.m_baseData.setEndIndex(endIndex);

		double barDistance = m_mainPainterPanel.boundingRect().width() / (endIndex - beginIndex);
		this->updateBarDistance(barDistance);
		m_view.m_pricePanel->refresh();
	}

	void ZoomController::resetZoomLevels()
	{
		m_zoomRangeStack.clear();
        double barDistance = 0.0;
#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
        barDistance = ZoomController::getBarDistance(ZoomLevel::zoom_minimal);
#else
        barDistance = ZoomController::getBarDistance(ZoomLevel::zoom_small);
#endif
		this->updateBarDistance(barDistance);
		m_view.updateChart(true);
	}

	void ZoomController::resetZoomHistory()
	{
		m_zoomRangeStack.clear();
	}

	void ZoomController::updateBarDistance(double barDistance)
	{
		CommonSettings& settings = m_config.getCommonSettings();
		settings.m_barDistance = barDistance;
		m_view.m_baseData.setBarDistance(barDistance);
		this->setToolbarState();
	}

	void ZoomController::setToolbarState()
	{
		bool isMinimalState = m_zoomRangeStack.size() <= 1 && this->isSmallestLevel();  // m_zoomRangeStack.back() 为当前正在显示的Range
		bool isMaximalState = this->isLargestLevel();
		m_view.m_toolbar->setZoomState(isMinimalState, isMaximalState);
	}

}
