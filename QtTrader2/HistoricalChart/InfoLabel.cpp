#include "InfoLabel.h"
#include "historicalchart.h"
#include "Crosshair.h"
#include "PricePanel.h"
#include <QFontMetrics>
#include <QDebug>
#include <QSysInfo> 

namespace HistoricalChartSpace
{
	InfoLabel::InfoLabel(const QString & text, HistoricalChart* parent)
		: m_pView(parent), QLabel(text, parent), m_isMakeCrosshairHidden(false), m_isMovedByUser(false), m_lines(0)
	{
#if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
        this->setStyleSheet("background-color:transparent;");
        this->raise();
        qDebug() << "init InfoLabel";
#else
#endif
	}

	InfoLabel::~InfoLabel()
	{
	}

	void InfoLabel::mousePressEvent(QMouseEvent* event)
	{
		m_startPos = event->pos();
		if (m_pView->m_crosshair && m_pView->m_crosshair->isVisible())
		{
			m_pView->m_crosshair->setVisible(false);
			m_isMakeCrosshairHidden = true;
		}
	}

	void InfoLabel::mouseMoveEvent(QMouseEvent* event)
	{
		if (event->buttons() & Qt::LeftButton)
		{
			QPoint targetPoint = m_basePoint + event->pos() - m_startPos;
			//qDebug() << QString("m_originalPoint:[%1,%2],m_startPoint:[%3,%4],event_pos:[%5,%6],targetPoint;[%7,%8]")
			//	.arg(m_originalPoint.x()).arg(m_originalPoint.y()).arg(m_startPoint.x()).arg(m_startPoint.y())
			//	.arg(event->pos().x()).arg(event->pos().y()).arg(targetPoint.x()).arg(targetPoint.y());

			if (targetPoint.x() < 0) targetPoint.setX(0);

			//int minY = m_pView->m_toolBar->isVisibleTo(m_pView) ? m_pView->m_toolBar->height() : 0;
			int minY = 0;
			if (targetPoint.y() < minY) targetPoint.setY(minY);

			int maxX = m_pView->width() - this->width();
			int maxY = m_pView->height() - this->height();
			if (targetPoint.x() > maxX) targetPoint.setX(maxX);
			if (targetPoint.y() > maxY) targetPoint.setY(maxY);

			this->move(targetPoint);
			m_isMovedByUser = true;

			m_basePoint = targetPoint;
		}
	}

	void InfoLabel::mouseReleaseEvent(QMouseEvent *)
	{
		if (m_isMakeCrosshairHidden)
		{
			m_pView->m_crosshair->setVisible(true);
			m_isMakeCrosshairHidden = false;
		}
	}

	void InfoLabel::moveToInitialPosition()
	{
		if (!m_isMovedByUser)
		{
            // InfoLabel Initially placed in the lower left corner
			QRectF rect = m_pView->getPricePanel()->rect();
			const double leftMargin = Helper::GetMarginSpaceValue(enMarginTextInfoToLeft);
			const double topMargin = Helper::GetMarginSpaceValue(enMarginTextInfoToTop);

            const double width = Helper::GetMarginSpaceValue(enMarginPriceTextInfoW)*1.3;
            const double hight = Helper::GetMarginSpaceValue(enMarginPriceTextInfoH);

            rect.setTop(rect.top() + rect.height() - hight - topMargin);
            #if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
            int top = m_pView->m_pixelSizeRatio * 30;
            rect.setTop(top);
            #endif
			rect.setLeft(rect.left() + leftMargin);
			rect.setWidth(width);
			rect.setHeight(hight);
			QWidget::setGeometry(QRect(rect.x(), rect.y(), rect.width(), rect.height()));
		}
	}

	void InfoLabel::changeText(const QString& infoText, int lines)
	{
		QFontMetrics fontMetrics(this->font());

		int spacing = 0;
		int ver = QSysInfo::windowsVersion();
		if (ver <= 0x0090) spacing = 2;

		int height = (fontMetrics.height() + spacing) * lines;
		this->resize(this->width(), height);
		this->setText(infoText);
		m_lines = lines;

	}
}
