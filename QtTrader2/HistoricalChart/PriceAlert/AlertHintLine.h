#pragma once
#include <QGraphicsLineItem>
#include <QColor>
#include <QPen>
#include <QLineF>

namespace HistoricalChartSpace
{
	class PricePanel;
	class AlertHintLine : public QGraphicsLineItem
	{
	public:
		AlertHintLine(PricePanel* pricePanel, const QPen& pen, int lineLength);
		~AlertHintLine();

		void moveTo(double y);

	protected:
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

	private:
		int m_lineLength;
		const QPen& m_pen;
		QLineF m_line;
		PricePanel* m_pricePanel;
	};
}