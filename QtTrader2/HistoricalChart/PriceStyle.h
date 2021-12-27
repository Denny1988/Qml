#pragma once
#include <QRect>
#include <QVariant>
#include <QColor>
#include "Common.h"
class QPainter;
namespace HistoricalChartSpace{
	class PricePanel;
	class PriceStyle
	{
	public:
		PriceStyle(PricePanel* parent) :m_pricePanel(parent), m_type(enPriceUnknown){}
		virtual void drawPrice(QPainter* painter, QRectF& rect, double barDistance) = 0;
		enPriceType getPriceType() const { return m_type; }
	protected:
		PricePanel*		 m_pricePanel;
        enPriceType		 m_type;	// Reset in subclass
	};

	class PriceBarStyle : public PriceStyle
	{
	public:
		PriceBarStyle(PricePanel* parent);
		void drawPrice(QPainter* painter, QRectF& rect, double barDistance);
	};

	class PriceLineStyle : public PriceStyle
	{
	public:
		PriceLineStyle(PricePanel* parent);
		void drawPrice(QPainter* painter, QRectF& rect, double barDistance);
	};

	class PriceCandlestickStyle : public PriceStyle
	{
	public:
		PriceCandlestickStyle(PricePanel* parent);
		void drawPrice(QPainter* painter, QRectF& rect, double barDistance);
	};

}
