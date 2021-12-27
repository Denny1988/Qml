#ifndef ROLLINGBOX_H
#define ROLLINGBOX_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QMap>

namespace HistoricalChartSpace
{
    class RollingBox : public QWidget//GIG_C_GOD
    {
        Q_OBJECT
        Q_PROPERTY(int deviation READ readDeviation WRITE setDeviation)
    public:
        explicit RollingBox(int height, int width, QWidget *parent = 0);
        void setDataSource(QMap<int, int> dataSourceIndex, QMap<int, QString> dataSource);
        //Setting range
        void setRange(double min, double max);
        //Get current value
        int readValue();

    protected:
       void mousePressEvent(QMouseEvent *);
       void mouseMoveEvent(QMouseEvent *);
       void mouseReleaseEvent(QMouseEvent *);
       void wheelEvent(QWheelEvent *);
       void paintEvent(QPaintEvent *);
       //Depicting numbers
       void paintNum(QPainter &painter, int num, int deviation);
       //Return the selected number to the middle of the screen
       void homing();
       //Mouse movement offset, default is 0
       int readDeviation();
       //Set offset
       void setDeviation(int n);

    signals:
       void currentValueChanged(int value);
       void deviationChange(int deviation);

    private:
        int m_minRange;      //Minimum
        int m_maxRange;      //Max
        int m_currentIndex;
        QString m_currentValue;
        QMap<int, int> m_dataSourceIndex;
        QMap<int, QString> m_dataSource;
        bool isDragging;        //Whether the mouse is down
        int m_deviation;        //Offset, record the vertical distance moved by the mouse after pressing
        int m_mouseSrcPos;
        int m_numSize;
        QPropertyAnimation *homingAni;
        const int interval;  //Interval size
        const int devide;       //Number of divisions

    };
}
#endif // ROLLINGBOX_H
