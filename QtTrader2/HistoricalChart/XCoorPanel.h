#pragma  once
#include <QGraphicsRectItem>
#include <vector>
#include <QObject>
#include "Settings.h"
#include "Config.h"
#include "ZoomController.h"

namespace HistoricalChartSpace
{
	class XCoorPanel : public QGraphicsRectItem
	{
	public:
		XCoorPanel(Config& config, ZoomController* &zoomController);
		~XCoorPanel();
		//void updateXCoor(std::vector<double>& vecDate);
		void updateXCoor(BaseData& baseData);

	private:
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
		void drawGrid(QPainter* painter,QRectF& rect);
		void drawXCoor(QPainter* painter,QRectF& rect);

		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;


	private:
		const CommonSettings& m_commonSettings;
		std::vector<double> m_vecDate;
		ZoomController* &m_zoomController;
		QPointF m_mouseStartPos;

	};
}
