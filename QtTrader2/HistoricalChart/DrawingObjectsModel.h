#pragma once
#include <QAbstractTableModel>

namespace HistoricalChartSpace
{
	class BaseDrawing;

	class DrawingObjectsModel : public QAbstractTableModel
	{
	public:
		DrawingObjectsModel(const QList<BaseDrawing*>& lineObjects, QObject *parent = 0);

		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
		virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
		virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

		BaseDrawing* getBaseLine(int index);

	private:
		QList<BaseDrawing*> m_lineObjects;
	};
}