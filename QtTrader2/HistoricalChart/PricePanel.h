#pragma  once
#include "ChartPanel.h"
#include "Common.h"
#include "PriceStyle.h"
#include "Settings.h"
#include <QSharedPointer>
#include "TradeHelper/OrderLine.h"

namespace HistoricalChartSpace
{
	class SplitPanel;
	class Config;
	class HistoricalChart;
	class PricePanel : public ChartPanel
	{
		Q_OBJECT
	public:
		friend class PriceCandlestickStyle;
		friend class PriceLineStyle;
		friend class PriceBarStyle;
		friend class PriceRealTimePanel;

		PricePanel(HistoricalChart* pView, Config& config, BaseData& baseData);
		~PricePanel();
		
		const PriceSettings& getPriceSettings() const { return m_priceSettings; }
		void applyConfig();
		void moveToEnd();
		void moveToHome();
		void changeStyle();
        int getSpareBar();
        bool moveChart(int bars);
		void adjustYScale(double moveRatio);
		void updateEndIndex();
        virtual void refresh() override;

	protected:
		void drawGrid(QPainter* painter, const QRectF& rect);
		void drawBidAskLine(QPainter* painter);
		void drawLineAndYCoordinate(const QRectF& rect, double left, double y, QPainter *painter, double data);
		void drawVolume(QPainter* painter, QRectF& rect);
		void paintMainPanel(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
		void paintYCoorPanel(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
		int getShiftBarCount();
		void clearData();

	private:
		int	getBarCountCanBeDisplayed() const;

	private:
		const PriceSettings& m_priceSettings;
		double m_adjustYScale;
		QSharedPointer<PriceStyle> m_PriceStyle;
	};
}
