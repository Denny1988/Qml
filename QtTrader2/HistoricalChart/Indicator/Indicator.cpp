#include "Indicator.h"
#if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
#include "IndicatorSettingDialog.h"
#endif
#include "TASDK.h"
#include <QDebug>

namespace HistoricalChartSpace
{
	Indicator::Indicator(const IndicatorSettings& settings, BaseData& baseData)
		: m_settings(settings), m_baseData(baseData), m_calculator(nullptr), m_isMa(false)
	{
		m_settings.updateTitle();
		for (int i = 0; i < settings.m_plotNum; ++i)
		{
			QPolygonF path;
			m_vecPainterPath.append(path);
		}
	}

	Indicator::~Indicator()
	{
		if (m_calculator)
		{
			m_calculator->removeTasks(this);
		}
	}

	void Indicator::setCalculator(Calculator* calculator)
	{
		m_calculator = calculator;
	}

	Calculator* Indicator::getCalculator()
	{
		return m_calculator;
	}

	bool Indicator::containsPoint(const QPointF& point)
	{
		bool isHit = false;
		for (int i = 0; i < m_vecPainterPath.size(); ++i)
		{
			if (m_vecPainterPath[i].containsPoint(point, Qt::WindingFill))
			{
				isHit = true;
				break;
			}
		}
		return isHit;
	}

	bool Indicator::hasIndicatorData()
	{
		QMutexLocker locker(&m_mutex);
		if (m_indicatorDatas.size() > 0)
		{
			return m_indicatorDatas.begin()->second->size() > 0;
		}
		else
		{
			return false;
		}
	}

	double Indicator::calculateMinValue()
	{
		QMutexLocker locker(&m_mutex);
		double min = std::numeric_limits<double>::quiet_NaN();
		for (map<QString, vector<double>*>::iterator it = m_indicatorDatas.begin(); it != m_indicatorDatas.end(); it++)
		{
			if (it->second)
			{
				double temp = Helper::getMinValue(it->second, m_baseData.getBeginIndex(), m_baseData.getEndIndex());
                if (std::isnan(min))
				{
                    if (!std::isnan(temp))
					{
						min = temp;
					}
				}
                else if (!std::isnan(temp) && min > temp)
				{
					min = temp;
				}
			}
		}
		return min;
	}

	double Indicator::calculateMaxValue()
	{
		QMutexLocker locker(&m_mutex);
		double max = std::numeric_limits<double>::quiet_NaN();
		for (map<QString, vector<double>*>::iterator it = m_indicatorDatas.begin(); it != m_indicatorDatas.end(); it++)
		{
			if (it->second)
			{
				double temp = Helper::getMaxValue(it->second, m_baseData.getBeginIndex(), m_baseData.getEndIndex());
                if (std::isnan(max))
				{
                    if (!std::isnan(temp))
					{
						max = temp;
					}
				}
                else if (!std::isnan(temp) && max < temp)
				{
					max = temp;
				}
			}
		}
		return max;
	}

	const QString& Indicator::getTitle() const
	{
		return m_settings.m_title;
	}

	const QString& Indicator::getName()
	{
		return m_settings.m_vectLineSettings[0].m_lineName;
	}

	const IndicatorSettings& Indicator::getIndicatorSettings() const
	{
		return m_settings;
	}

	void Indicator::setIndicatorSettings(const IndicatorSettings& settings)
	{
		m_settings = settings;
		//update();
	}

	void Indicator::paintIndicator(QPainter* painter, const PaintArgs& paintArgs)
	{
		QMutexLocker locker(&m_mutex);
		if (m_indicatorDatas.begin()->second->size() > m_baseData.getEndIndex())
		{
			if (m_settings.isDataValid() && m_vecPainterPath.size() == m_settings.m_plotNum && m_settings.m_plotNum <= m_indicatorDatas.size())
			{
				for (int i = 0; i < m_settings.m_plotNum; ++i)
				{
					QPen pen(m_settings.m_vectLineSettings[i].m_lineColor, m_settings.m_vectLineSettings[i].m_lineWidth, (Qt::PenStyle)m_settings.m_vectLineSettings[i].m_lineType);
					painter->setPen(pen);

					switch (m_settings.m_vectLineSettings[i].m_drawType)
					{
					case enIndicatorDrawLine:
						drawLineType(painter, paintArgs, m_settings.m_vectLineSettings[i].m_lineName, m_vecPainterPath[i]);
						break;
					case enIndicatorDrawColumn:
						drawColumnType(painter, paintArgs, m_settings.m_vectLineSettings[i].m_lineName, m_vecPainterPath[i]);
						break;
					case enIndicatorDrawCurve:
						painter->save();
						painter->setRenderHints(QPainter::Antialiasing);
						pen.setJoinStyle(Qt::MiterJoin);
						pen.setCapStyle(Qt::RoundCap);
						painter->setPen(pen);

						drawLineType(painter, paintArgs, m_settings.m_vectLineSettings[i].m_lineName, m_vecPainterPath[i]);
						painter->restore();
						break;
					case enIndicatorDrawDot:
						drawDotType(painter, paintArgs, m_settings.m_vectLineSettings[i].m_lineName, m_vecPainterPath[i]);
						break;
					case enIndicatorDrawUnknown:
						break;
					default:
						break;
					}
				}
			}
		}
	}

	void Indicator::getParameterRange(const QString& key, int& min, int& max)
	{
		min = 1;
		max = 3000;
	}

	void Indicator::getParameterRange(const QString& key, double& min, double& max, double& decimals)
	{
		min = 0.0001;
		max = 3000;
		decimals = 6;
	}

	int Indicator::extendDataBuffer()
	{
		int newSize = m_baseData.size();
		if (m_indicatorDatas.begin()->second->size() < newSize)
		{
			QMutexLocker locker(&m_mutex);
			map<QString, vector<double>*>::iterator it;
			for (it = m_indicatorDatas.begin(); it != m_indicatorDatas.end(); it++)
			{
				it->second->resize(newSize, std::numeric_limits<double>::quiet_NaN());
			}
		}
		else if (m_indicatorDatas.begin()->second->size() > newSize)
		{
			// Caused by truncateBefore operation of BaseData 
			QMutexLocker locker(&m_mutex);
			map<QString, vector<double>*>::iterator it;
			for (it = m_indicatorDatas.begin(); it != m_indicatorDatas.end(); it++)
			{
				it->second->clear();
				it->second->resize(newSize, std::numeric_limits<double>::quiet_NaN());
			}
		}
		return newSize;
	}

	void Indicator::GetCPDA(enCPD type, const double opens[], const double closes[], const double highs[], const double lows[], double result[], int size)
	{
		switch (type)
		{
		case HistoricalChartSpace::enCPDOpen:
			memcpy(result, opens, sizeof(double) * size);
			break;
		case HistoricalChartSpace::enCPDClose:
			memcpy(result, closes, sizeof(double) * size);
			break;
		case HistoricalChartSpace::enCPDHigh:
			memcpy(result, highs, sizeof(double) * size);
			break;
		case HistoricalChartSpace::enCPDLow:
			memcpy(result, lows, sizeof(double) * size);
			break;
		case HistoricalChartSpace::enCPDMedian: // HL / 2
		{
			for (int i = 0; i < size; ++i)
			{
				result[i] = (highs[i] + lows[i]) / 2;
			}
		}
		break;
		case HistoricalChartSpace::enCPDTypical:// HLC / 3
		{
			for (int i = 0; i < size; ++i)
			{
				result[i] = (highs[i] + lows[i] + closes[i]) / 3;
			}
		}
		break;
		case HistoricalChartSpace::enCPDWeightedClose:// HLCC / 4
		{
			for (int i = 0; i < size; ++i)
			{
				result[i] = (highs[i] + lows[i] + closes[i] * 2) / 4;
			}
		}
		break;
		case HistoricalChartSpace::enCPDUnknown:
			break;
		default:
			break;
		}
	}

	double Indicator::GetCPDValue(int index)
	{
		double value = numeric_limits<double>::quiet_NaN();
		switch (m_settings.m_dependency)
		{
		case HistoricalChartSpace::enCPDOpen:
			value = m_baseData.getOpens()[index];
			break;
		case HistoricalChartSpace::enCPDClose:
			value = m_baseData.getCloses()[index];
			break;
		case HistoricalChartSpace::enCPDHigh:
			value = m_baseData.getHighs()[index];
			break;
		case HistoricalChartSpace::enCPDLow:
			value = m_baseData.getLows()[index];
			break;
		case HistoricalChartSpace::enCPDMedian: // HL / 2
			value = (m_baseData.getHighs()[index] + m_baseData.getLows()[index]) / 2;
			break;
		case HistoricalChartSpace::enCPDTypical: // HLC / 3
			value = (m_baseData.getHighs()[index] + m_baseData.getLows()[index] + m_baseData.getCloses()[index]) / 3;
			break;
		case HistoricalChartSpace::enCPDWeightedClose: // HLCC / 4
			value = (m_baseData.getHighs()[index] + m_baseData.getLows()[index] + m_baseData.getCloses()[index] * 2) / 4;
			break;
		case HistoricalChartSpace::enCPDUnknown:
			break;
		default:
			break;
		}
		return value;
	}

	/* inputTypes length: OHLC */
	CDatabase* Indicator::prepare(int beginIndex, int endIndex, int period, const char* inputTypes/* = nullptr*/)
	{
		m_dataStartIndex = beginIndex - (period - 1);
		if (m_dataStartIndex < 0) m_dataStartIndex = 0;
		m_inputDataBufferSize = endIndex - m_dataStartIndex + 1;

		// CRecordset pointer will be deleted in CDatabase
		CRecordset* recordset = new CRecordset();

		// fields pointer will be deleted in CRecordset
		if (inputTypes)
		{
			for (int i = 0; i < 5; i++)
			{
				char dataType = inputTypes[i];

				if (dataType == 'C')
				{
					CField* field = new CField(m_inputDataBufferSize, SC::Close);
					for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
					{
						field->setValue(bufferIndex + 1, m_baseData.getCloses()[m_dataStartIndex + bufferIndex]);
					}
					recordset->addField(field);
				}
				else if (dataType == 'H')
				{
					CField* field = new CField(m_inputDataBufferSize, SC::High);
					for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
					{
						field->setValue(bufferIndex + 1, m_baseData.getHighs()[m_dataStartIndex + bufferIndex]);
					}
					recordset->addField(field);
				}
				else if (dataType == 'L')
				{
					CField* field = new CField(m_inputDataBufferSize, SC::Low);
					for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
					{
						field->setValue(bufferIndex + 1, m_baseData.getLows()[m_dataStartIndex + bufferIndex]);
					}
					recordset->addField(field);
				}
				else if (dataType == 'O')
				{
					CField* field = new CField(m_inputDataBufferSize, SC::Open);
					for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
					{
						field->setValue(bufferIndex + 1, m_baseData.getOpens()[m_dataStartIndex + bufferIndex]);
					}
					recordset->addField(field);
				}
				else if (dataType == 'V')
				{
					CField* field = new CField(m_inputDataBufferSize, SC::Volume);
					for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
					{
						field->setValue(bufferIndex + 1, m_baseData.getVolumes()[m_dataStartIndex + bufferIndex]);
					}
					recordset->addField(field);
				}
				else if (dataType = '\0')
				{
					break;
				}
			}
		}
		else
		{
			if (m_settings.m_dependency == enCPDUnknown)
			{
				m_settings.m_dependency = enCPDClose;
			}
			m_inputSeries = new CField(m_inputDataBufferSize, "input");
			for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
			{
				m_inputSeries->setValue(bufferIndex + 1, GetCPDValue(m_dataStartIndex + bufferIndex));
			}
			recordset->addField(m_inputSeries);
		}

		CDatabase* database = new CDatabase();
		database->setRecordset(recordset, m_inputDataBufferSize);
		return database;
	}

	void Indicator::drawLineType(QPainter* painter, const PaintArgs& paintArgs, QString lineName, QPolygonF& polygon)
	{
		auto lineIt = m_indicatorDatas.find(lineName);
		if (lineIt == m_indicatorDatas.end()) return;
		const vector<double>* datas = m_indicatorDatas[lineName];

		const int endIndex = m_baseData.getEndIndex();
		if (datas->size() - 1 < endIndex) return;

		const int beginIndex = m_baseData.getBeginIndex();
		const double barDistance = m_baseData.getBarDistance();

		const double margin = Helper::GetMarginSpaceValue(enMarginHalfSpaceLineSelect);
		const double margin2 = 2 * margin;

		QPointF lastPoint;
		QVector<QPointF> vecPointUp, vecPointDown;

		int validDataStartIndex = this->getStartIndex(datas);
		int startDrawIndex = (beginIndex < validDataStartIndex) ? validDataStartIndex : beginIndex;
		for (int i = startDrawIndex; i <= endIndex; ++i)
		{
			double  X = paintArgs.startX + (i - beginIndex)  * barDistance;
			double  Y = paintArgs.minY - ((*datas)[i] - paintArgs.minValue) * paintArgs.scaleY;

			//if (Y < rect().top() || Y > rect().top() + rect().height()) continue;
            if (std::isnan(X) || std::isnan(Y)) continue;
			QPointF point(X, Y);

			if (i > startDrawIndex)
			{
				painter->drawLine(lastPoint, point);
			}

			lastPoint = point;

			QPointF upPoint(X, Y - margin2);
			QPointF downPoint(X, Y + margin2);

			vecPointUp << upPoint;
            /*vecPointDown.push_front(downPoint);*/ // Do not use this function very slowly, adjust it to the last
			vecPointDown << downPoint;
		}

        // Invert the data below
		int vecSize = vecPointDown.size();
		int middle = vecSize / 2;
		for (int i = 0; i < middle; ++i)
		{
			QPointF tempPoint = vecPointDown[i];
			vecPointDown[i] = vecPointDown[vecSize - i - 1];
			vecPointDown[vecSize - i - 1] = tempPoint;
		}
		polygon.clear();
		polygon << vecPointUp << vecPointDown;

		//// test 
		//QPen boundPen(Qt::red,2.0,Qt::DashDotDotLine);
		//painter->setPen(boundPen);
		//painter->drawPolyline(polygon);
	}

	void Indicator::drawColumnType(QPainter* painter, const PaintArgs& paintArgs, QString lineName, QPolygonF& polygon)
	{
		auto lineIt = m_indicatorDatas.find(lineName);
		if (lineIt == m_indicatorDatas.end()) return;
		const vector<double>* datas = m_indicatorDatas[lineName];

		const int endIndex = m_baseData.getEndIndex();
		if (datas->size() - 1 < endIndex) return;

		const double yZero = paintArgs.minY + paintArgs.minValue * paintArgs.scaleY;
		QPointF basePoint(0.0, yZero);
		QVector<QPointF> vecPointUp, vecPointDown;

		//static int si = 0;
		//qDebug() << "drawLineType,Begin:" << paintArgs.beginIndex << "  End:" << paintArgs.endIndex << ". -" << si++;

		const int beginIndex = m_baseData.getBeginIndex();
		const double barDistance = m_baseData.getBarDistance();
		const double margin = Helper::GetMarginSpaceValue(enMarginHalfSpaceLineSelect);
		int validDataStartIndex = this->getStartIndex(datas);
		int startDrawIndex = (beginIndex < validDataStartIndex) ? validDataStartIndex : beginIndex;
		for (int i = startDrawIndex; i <= endIndex; ++i)
		{
			double  X = paintArgs.startX + (i - beginIndex)  * barDistance;
			double YValue = (*datas)[i];
			double  Y = paintArgs.minY - (YValue - paintArgs.minValue) * paintArgs.scaleY;

			//if (Y < rect().top() || Y > rect().top() + rect().height()) continue;
			QPointF point(X, Y);
			basePoint.setX(X);

            // Draw a line
			double adjust = painter->pen().width() / 2.0;
			if (point.y() < basePoint.y()) adjust = -adjust;
			painter->drawLine(basePoint.x(), basePoint.y() + adjust, point.x(), point.y());

            // Draw the surrounding area
			if (Y > yZero)
			{
				QPointF leftHigh(X - margin, Y + margin);
				QPointF leftLow(X - margin, yZero + margin);
				QPointF rightHigh(X + margin, Y + margin);
				QPointF rightLow(X + margin, yZero + margin);

				vecPointDown << leftLow << leftHigh << rightHigh << rightLow;
			}
			else
			{
				QPointF leftHigh(X - margin, Y - margin);
				QPointF leftLow(X - margin, yZero - margin);
				QPointF rightHigh(X + margin, Y - margin);
				QPointF rightLow(X + margin, yZero - margin);

				vecPointUp << leftLow << leftHigh << rightHigh << rightLow;
			}

            // When starting or ending, add corresponding points on the opposite side to facilitate the drawing of figures including the residence
			if (i == startDrawIndex)
			{
				Y > yZero ? vecPointUp << QPointF(X - margin, yZero - margin) : vecPointDown << QPointF(X - margin, yZero + margin);
			}
			else if (i == endIndex)
			{
				Y > yZero ? vecPointUp << QPointF(X + margin, yZero - margin) : vecPointDown << QPointF(X + margin, yZero + margin);
			}
		}

        // Draw line 0 axis
		QPen pen(paintArgs.foreground, 1.0, Qt::DotLine);
		painter->setPen(pen);
		//double right = this->rect().left() + this->rect().width() - Common::GetMarginSpaceValue(enMarginYCoordinate);
		double right = paintArgs.startX + paintArgs.canvasWidth;// -Common::GetMarginSpaceValue(enMarginYCoordinate);
		painter->drawLine(paintArgs.startX, basePoint.y(), right, basePoint.y());

        // Invert the data below
		int vecSize = vecPointDown.size();
		int middle = vecSize / 2;
		for (int i = 0; i < middle; ++i)
		{
			QPointF tempPoint = vecPointDown[i];
			vecPointDown[i] = vecPointDown[vecSize - i - 1];
			vecPointDown[vecSize - i - 1] = tempPoint;
		}
		polygon.clear();
		polygon << vecPointUp << vecPointDown;

		// test 
		//QPen boundPen(Qt::red,2.0,Qt::DashDotDotLine);
		//painter->setPen(boundPen);
		//painter->drawPolyline(polygon);
	}

	void Indicator::drawDotType(QPainter* painter, const PaintArgs& paintArgs, QString lineName, QPolygonF& polygon)
	{
		auto lineIt = m_indicatorDatas.find(lineName);
		if (lineIt == m_indicatorDatas.end()) return;
		const vector<double>* datas = m_indicatorDatas[lineName];

		const int endIndex = m_baseData.getEndIndex();
		if (datas->size() - 1 < endIndex) return;

		QVector<QPointF> vecPointUp, vecPointDown;

		double radius = painter->pen().widthF();
		QPen pen(painter->pen().color(), 1);
		QBrush brush(pen.color());

		painter->setPen(pen);
		painter->setBrush(brush);

		const int beginIndex = m_baseData.getBeginIndex();
		const double barDistance = m_baseData.getBarDistance();

		const double margin = Helper::GetMarginSpaceValue(enMarginHalfSpaceLineSelect);
		const double margin2 = 2 * margin;
		int validDataStartIndex = this->getStartIndex(datas);
		int startDrawIndex = (beginIndex < validDataStartIndex) ? validDataStartIndex : beginIndex;
		for (int i = startDrawIndex; i <= endIndex; ++i)
		{
			double  X = paintArgs.startX + (i - beginIndex)  * barDistance;
			double  Y = paintArgs.minY - ((*datas)[i] - paintArgs.minValue) * paintArgs.scaleY;

			QPointF point(X, Y);
			painter->drawEllipse(point, radius, radius);

			QPointF upPoint(X, Y - margin2);
			QPointF downPoint(X, Y + margin2);

			vecPointUp << upPoint;
            /*vecPointDown.push_front(downPoint);*/ // Do not use this function very slowly, adjust it to the last
			vecPointDown << downPoint;
		}

        // Invert the data below
		int vecSize = vecPointDown.size();
		int middle = vecSize / 2;
		for (int i = 0; i < middle; ++i)
		{
			QPointF tempPoint = vecPointDown[i];
			vecPointDown[i] = vecPointDown[vecSize - i - 1];
			vecPointDown[vecSize - i - 1] = tempPoint;
		}
		polygon.clear();
		polygon << vecPointUp << vecPointDown;

		//// test 
		//QPen boundPen(Qt::red,2.0,Qt::DashDotDotLine);
		//painter->setPen(boundPen);
		//painter->drawPolyline(polygon);
	}

	int Indicator::getStartIndex(const vector<double>* datas)
	{
		int startIndex;
		int endIndex = datas->size() - 1;
		for (startIndex = 0; startIndex <= endIndex; startIndex++)
		{
            if (!std::isnan((*datas)[startIndex])) break;
		}
		return startIndex;
	}

	void Indicator::calculateAll()
	{
		int dataSize = m_baseData.size();
		if (dataSize > 0)
		{
			this->calculate(0, dataSize - 1);
		}
	}
}
