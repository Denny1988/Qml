#pragma once
#include <QLabel>
#include <QPoint>
#include <QMouseEvent>

namespace HistoricalChartSpace
{
	class HistoricalChart;
	class InfoLabel : public QLabel
	{
	public:
		InfoLabel(const QString & text, HistoricalChart* parent);
		~InfoLabel();

		virtual void mousePressEvent(QMouseEvent *) override;
		virtual void mouseMoveEvent(QMouseEvent *) override;
		virtual void mouseReleaseEvent(QMouseEvent *) override;

		void moveToInitialPosition();
		void changeText(const QString& infoText, int lines);

	private:
		QPoint m_startPos;    // mouse pos
		QPoint m_basePoint;   // (left, top) of QWidget
		HistoricalChart* m_pView;
		bool m_isMakeCrosshairHidden;
		bool m_isMovedByUser;
		int m_lines;
	};
}
