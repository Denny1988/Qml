#include "DrawingObjectsModel.h"
#include "Settings.h"
#include "BaseDrawing.h"
#include "SR.h"
#include "Helper.h"

namespace HistoricalChartSpace
{
	DrawingObjectsModel::DrawingObjectsModel(const QList<BaseDrawing*>& lineObjects, QObject *parent)
		:QAbstractTableModel(parent), m_lineObjects(lineObjects)
	{
	}

	int DrawingObjectsModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
	{
		return m_lineObjects.count();
	}

	int DrawingObjectsModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
	{
		return 4;
	}

	QVariant DrawingObjectsModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
	{
		if (!index.isValid())
			return QVariant();

		if (role == Qt::TextAlignmentRole)
		{
			return int(Qt::AlignRight | Qt::AlignVCenter);
		}
		else if (role == Qt::DisplayRole)
		{
			const BaseDrawing* lineObject = m_lineObjects[index.row()];
			const DrawingObjectSettings& settings = lineObject->getSettings();
			if (index.column() == 0)
			{
				return Helper::getDrawingObjBaseStrInfo(settings.m_type);
			}
			else if (index.column() == 1)
			{
				return settings.m_name;
			}
			else if (index.column() == 2)
			{
				return settings.m_describe;
			}
			else if (index.column() == 3)
			{
				return QString("%1").arg(settings.m_panelIndex);
			}
		}
		else if (role == Qt::UserRole)
		{
            // Return Index of Model data. Because QSortFilterProxyModel is used, it needs to be converted by this method.
			return index.row();
		}
		return QVariant();
	}

	QVariant DrawingObjectsModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
	{
		if (role == Qt::DisplayRole)
		{
			if (section == 0)
			{
				return SR::get("Type");
			}
			else if (section == 1)
			{
				return SR::get("Name");
			}
			else if (section == 2)
			{
				return SR::get("Description");
			}
			else if (section == 3)
			{
				return SR::get("Window");
			}
		}
		return QVariant();
	}

	BaseDrawing* DrawingObjectsModel::getBaseLine(int index)
	{
		return m_lineObjects[index];
	}

	bool DrawingObjectsModel::removeRows(int row, int count, const QModelIndex &parent /*= QModelIndex()*/)
	{
		beginRemoveRows(QModelIndex(), row, row + count - 1);
		for (int i = 0; i < count; i++)
		{
			m_lineObjects.removeAt(row);
		}
		endRemoveRows();
		return true;
	}

}
