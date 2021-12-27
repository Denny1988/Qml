#include "CursorPosition.h"
#include "ChartPanel.h"
#include "historicalchart.h"
#include "JulianDate.h"
#include "SR.h"
#include <QDebug>

namespace HistoricalChartSpace
{
	CursorPosition::CursorPosition(HistoricalChart* pView) : m_pView(pView)
	{
	}

	CursorPosition::~CursorPosition()
	{
	}

	void CursorPosition::OnPositionChange(QPointF& scenePoint, ChartPanel* targetPanel)
	{
        //qDebug() <<  " CursorPosition::OnPositionChange:" << scenePoint;
		m_scenePos = scenePoint;
		m_targetPanel = targetPanel;
		QPointF localPoint = targetPanel->mapFromScene(scenePoint);

		int beingIndex = m_pView->getBaseData().getBeginIndex();
		double barDistance = m_pView->getBaseData().getBarDistance();
		m_dataIndex = std::floor(localPoint.x() / barDistance + 0.5);
		m_dataIndex += beingIndex;
		if (m_dataIndex < beingIndex) {
			m_dataIndex = beingIndex;
		}
																// yValue
		m_yValue = targetPanel->Y2Value(localPoint.y());
		m_targetPanel->getPanelType() == enPanelPrice ? m_yValueText.setNum(m_yValue, 'f', m_pView->getDecimalPlaces()) : m_yValueText.setNum(m_yValue);

		// date
		double date;
		int dataSize = m_pView->getBaseData().getDates().size();
		if (m_dataIndex < dataSize)
		{
			date = m_pView->getBaseData().getDate(m_dataIndex);
		}
		else
		{
			date = m_pView->getBaseData().getDate(dataSize - 1);
			date += m_pView->getFrequency().getJDateTimeSpan() * (m_dataIndex - dataSize + 1);
		}
		m_date = JulianDate::JDateToString(date, true, true);
	}

	const QString& CursorPosition::getYValueText()
	{
		return m_yValueText;
	}

	const QString& CursorPosition::getDateText()
	{
		return m_date;
	}

	const QPointF& CursorPosition::getScenePos()
	{
		return m_scenePos;
	}

	QString CursorPosition::getInfoText(int& lines)
	{
		lines = 0;
		QString infoText;
		BaseData& baseData = m_pView->getBaseData();
		if (m_dataIndex < baseData.size())
		{
			const double date = baseData.getDate(m_dataIndex);
			if (m_targetPanel->getPanelType() == enPanelPrice)
			{
				const double open = baseData.getOpens()[m_dataIndex];
				const double close = baseData.getCloses()[m_dataIndex];
				const double high = baseData.getHighs()[m_dataIndex];
				const double low = baseData.getLows()[m_dataIndex];
				const double volume = baseData.getVolumes()[m_dataIndex];

				QString strOpen, strHigh, strLow, strClose;
				int decimalPlaces = m_pView->getDecimalPlaces();
				infoText += "X:" + m_date + "<br>"
					+ "Y:" + m_yValueText + "<br>"
					+ "O:" + strOpen.setNum(open, 'f', decimalPlaces) + "<br>"
					+ "H:" + strHigh.setNum(high, 'f', decimalPlaces) + "<br>"
					+ "L:" + strLow.setNum(low, 'f', decimalPlaces) + "<br>"
					+ "C:" + strClose.setNum(close, 'f', decimalPlaces) + "<br>";
				lines += 6;
			}
            #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
			else if (m_targetPanel->getPanelType() == enPanelIndicator)
			{
				infoText += "X:" + m_date + "<br>"
					+ "Y:" + m_yValueText + "<br>";
				lines += 2;
			}
			const QVector<Indicator*>& indicators = m_targetPanel->getIndicators();
            foreach (Indicator* indicator, indicators)
			{
				map<QString, vector<double>*>& indicatorDatas = indicator->getData();
				const IndicatorSettings& settings = indicator->getIndicatorSettings();
				for (int i = 0; i < settings.m_plotNum; ++i)
				{
					QString lineName = settings.m_vectLineSettings[i].m_lineName;
					auto lineIt = indicatorDatas.find(lineName);
					if (lineIt == indicatorDatas.end())
					{
						continue;
					}
					const vector<double>* datas = indicatorDatas[lineName];
					QString data;
					if (datas->size() > m_dataIndex)
					{
						if (settings.m_isInPriceChart)
						{
							data.setNum(datas->at(m_dataIndex), 'f', m_pView->getDecimalPlaces());
						}
						else
						{
							data = QString("%1").arg(datas->at(m_dataIndex));
						}
					}

					QString color = settings.m_vectLineSettings[i].m_lineColor.name();

					if (indicator->isMa())
					{
						lineName = indicator->getName();
					}
					else if (lineName == SC::OutReal)
					{
						lineName = "Value";
					}
					infoText += QString("<span style=\"color:%1\">%2:%3</span><br>").arg(color).arg(lineName).arg(data);
					lines += 1;
				}
			}
            #endif
        }
		return infoText;
	}

    QString CursorPosition::getIndicatorInfoText(enIndicatorType m_type)
    {
        QString infoText;
        QString intInfoText;

        BaseData& baseData = m_pView->getBaseData();
        if (m_dataIndex < baseData.size())
        {
            const QVector<Indicator*>& indicators = m_targetPanel->getIndicators();
            if(m_type == enIndicatorMovingAverages)
                intInfoText.append("(");
            foreach (Indicator* indicator, indicators)
            {
                map<QString, vector<double>*>& indicatorDatas = indicator->getData();
                const IndicatorSettings& settings = indicator->getIndicatorSettings();
                if(settings.m_type != m_type) continue;
                for (int i = 0; i < settings.m_plotNum; ++i)
                {
                    QString lineName = settings.m_vectLineSettings[i].m_lineName;
                    auto lineIt = indicatorDatas.find(lineName);
                    if (lineIt == indicatorDatas.end())
                    {
                        continue;
                    }
                    const vector<double>* datas = indicatorDatas[lineName];
                    QString data;
                    if (datas->size() > m_dataIndex)
                    {
                        if (settings.m_isInPriceChart)
                        {
                            data.setNum(datas->at(m_dataIndex), 'f', m_pView->getDecimalPlaces());
                        }
                        else
                        {
                            data = QString("%1").arg(datas->at(m_dataIndex));
                        }
                    }

                    QString color = settings.m_vectLineSettings[i].m_lineColor.name();

                    if (indicator->isMa())
                    {
                        lineName = indicator->getName();
                    }
                    else if (lineName == SC::OutReal)
                    {
                        lineName = "Value";
                    }
                    infoText += QString("<span style=\"color:%1\">%2:%3</span> ").arg(color).arg(lineName).arg(data);
                }

                if(settings.m_hashInputIntData.count() > 0)
                {
                    if(m_type != enIndicatorMovingAverages)
                        intInfoText.append("(");
                    QHash<QString,int>::const_iterator it = settings.m_hashInputIntData.begin();
                    for(it; it != settings.m_hashInputIntData.end(); ++it)
                    {
                        if(intInfoText!="(")
                            intInfoText.append(",");
                        intInfoText.append(QString::number(settings.m_hashInputIntData[it.key()]));
                    }
                   if(m_type != enIndicatorMovingAverages)
                       intInfoText.append(")");
                }
            }
            if(m_type == enIndicatorMovingAverages)
                intInfoText.append(")");
        }
        return QString("%1 %2").arg(intInfoText!="()" ? intInfoText : "" , infoText);
    }

}
