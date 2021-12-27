#include "rollingbox.h"
#include <QDebug>

namespace HistoricalChartSpace
{
    RollingBox::RollingBox(int height, int width, QWidget *parent) :
        QWidget(parent),
        m_minRange(0),      //The minimum value is 0 by default
        m_maxRange(100),    //The maximum default is 100
        isDragging(false),
        m_deviation(0),     //The default offset is 0
        m_numSize(12),
        interval(1),      //The default interval is 0.5
        devide(7)           //Divided into 7 grids by default
    {
        this->setFixedSize(width, height);
        homingAni = new QPropertyAnimation(this, "deviation");
        homingAni->setDuration(300);
        homingAni->setEasingCurve(QEasingCurve::OutQuad);
    }

    void RollingBox::setRange(double min,double max)
    {
        //   m_minRange = min;
        //   m_maxRange = max;
        //   if (m_currentValue < min)
        //   {
        //       m_currentValue = min;
        //   }
        //   if (m_currentValue > max)
        //   {
        //       m_currentValue = max;
        //   }
        //   repaint();
    }

    void RollingBox::setDataSource(QMap<int, int> dataSourceIndex, QMap<int, QString> dataSource)
    {
        this->m_dataSourceIndex = dataSourceIndex;
        this->m_dataSource = dataSource;
        m_maxRange = dataSourceIndex.count()-1;
        m_minRange = 0;
        m_currentIndex=0;
        if (m_currentIndex < m_minRange)
        {
            m_currentIndex = m_minRange;
        }
        if (m_currentIndex > m_maxRange)
        {
            m_currentIndex = m_maxRange;
        }
        repaint();
    }


    int  RollingBox::readValue()
    {
        return this->m_dataSourceIndex[m_currentIndex];
    }

    void RollingBox::mousePressEvent(QMouseEvent *e)
    {
        qDebug()<<"mouse pressed on vertical scroll";

        homingAni->stop();
        isDragging = true;
        m_mouseSrcPos = e->pos().y();
        QWidget::mousePressEvent(e);
    }

    void RollingBox::mouseMoveEvent(QMouseEvent *e)
    {
        qDebug() << " mouseMoveEvent: start isDragging:" <<isDragging;
        if (isDragging)
        {
            if ((m_currentIndex == m_minRange && e->pos().y() >= m_mouseSrcPos) ||
                    ( m_currentIndex == m_maxRange && e->pos().y() <= m_mouseSrcPos ))
            {
                return;
            }

            m_deviation = e->pos().y() - m_mouseSrcPos;
            //If the moving speed is too fast, limit it
            if (m_deviation > (height() - 1) / devide)
            {
                m_deviation = (height() - 1) / devide;
            }
            else if (m_deviation < -(height() - 1) / devide)
            {
                m_deviation = -( height() - 1) / devide;
            }

            emit deviationChange((double)m_deviation / ((height() - 1) / devide));
            repaint();
        }
    }

    void RollingBox::mouseReleaseEvent(QMouseEvent *)
    {
        qDebug() << " mouseReleaseEvent: start isDragging:" <<isDragging;
        if (isDragging)
        {
            isDragging = false;
            homing();
        }
    }

    void RollingBox::wheelEvent(QWheelEvent *e)
    {
        if (e->delta() > 0)
        {
            m_deviation = (this->height() - 1) / devide;
        }
        else
        {
            m_deviation = -(this->height() - 1) / devide;
        }
        homing();
        repaint();
    }

    void RollingBox::paintEvent(QPaintEvent *)
    {
         qDebug() << " RollingBox::paintEvent: start";
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        int Height = height() - 1;

        if ( m_deviation >= Height / devide && m_currentIndex > m_minRange )
        {
            m_mouseSrcPos += Height / devide;
            m_deviation -= Height / devide;
            m_currentIndex -= interval;
        }

        if ( m_deviation <= -Height / devide && m_currentIndex < m_maxRange )
        {
            m_mouseSrcPos -= Height / devide;
            m_deviation += Height / devide;
            m_currentIndex += interval;
        }

        //middle number
        paintNum(painter, m_currentIndex, m_deviation);

        //Number 1 on both sides
        if (m_currentIndex != m_minRange)
        {
            paintNum(painter, m_currentIndex - interval, m_deviation - Height / devide);
        }

        if (m_currentIndex != m_maxRange)
        {
            paintNum(painter, m_currentIndex + interval, m_deviation + Height / devide);
        }

        for (int i=2; i <= devide/2; ++i)
        {
            if (m_currentIndex - interval * i >= m_minRange)
            {
                paintNum(painter, m_currentIndex - interval * i, m_deviation - Height / devide * i);
            }

            if (m_currentIndex + interval * i <= m_maxRange)
            {
                paintNum(painter, m_currentIndex + interval * i, m_deviation + Height / devide * i);
            }
        }
        //frame
        painter.setPen(QPen(QColor(0,0,0,120),2));
        painter.drawLine(0, Height/7*3, width(), Height/7*3);
        painter.drawLine(0, Height/7*4, width(), Height/7*4);
    }
    /*
     * Draw numbers based on offset
     * double num The number that needs to be displayed
     * int deviation The offset of the number relative to the middle
    */
    void RollingBox::paintNum(QPainter &painter, int num, int deviation)
    {
        int Width = width() - 1;
        int Height = height() - 1;
        int size = (Height - qAbs(deviation)) / m_numSize; //The larger the offset, the smaller the number
        int transparency = 255 - 255 * qAbs(deviation) / Height;
        int height = Height / devide;
        int y = Height / 2 + deviation - height / 2;

        QFont font;
        font.setPixelSize(size);
        painter.setFont(font);
        painter.setPen(QColor(0, 0, 0,transparency));

        if ( y >= 0 && y + height < Height)
        {
            painter.drawText(QRectF(0, y, Width-20, height),
                             Qt::AlignCenter,
                             this->m_dataSource[this->m_dataSourceIndex[num]]);
        }
    }

    /*
     * 使选中的数字回到屏幕中间
    */
    void RollingBox::homing()
    {
        if ( m_deviation > height() / 10)
        {
            homingAni->setStartValue( ( height() - 1 ) / 8 - m_deviation);
            homingAni->setEndValue(0);
            m_currentIndex -= interval;
        }
        else if ( m_deviation > -height() / 10 )
        {
            homingAni->setStartValue(m_deviation);
            homingAni->setEndValue(0);
        }
        else if ( m_deviation < -height() / 10 )
        {
            homingAni->setStartValue(-(height() - 1) / 8 - m_deviation);
            homingAni->setEndValue(0);
            m_currentIndex += interval;
        }

        emit currentValueChanged(m_currentIndex);
        homingAni->start();
    }

    int RollingBox::readDeviation()
    {
        return m_deviation;
    }

    void RollingBox::setDeviation(int n)
    {
        m_deviation = n;
        repaint();
    }
}
