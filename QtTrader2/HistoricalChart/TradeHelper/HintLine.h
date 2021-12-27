#pragma once
#include <QGraphicsLineItem>
#include <QColor>
#include <QPen>
#include <QLineF>

namespace HistoricalChartSpace
{
	class PricePanel;
	class HintLine : public QGraphicsLineItem
	{
		friend class OrderLine;
	public:
		HintLine(PricePanel* pricePanel, QColor lineColor);
		~HintLine();

		void moveTo(double y, double price, QString& text, bool isStopLoss = false);
		bool isShowOnChart();
		void hide();
	
	protected:
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

	private:
		double m_y;
		double m_price;
		QString m_text;
		bool m_isStopLoss;

		QPen m_pen;
		QLineF m_line;
		PricePanel* m_pricePanel;
	};
}
