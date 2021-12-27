#pragma once
#include <QDialog>
#include <QPushButton>
#include <QTableView>
#include <QHeaderView>
#include <QSortFilterProxyModel>

namespace HistoricalChartSpace
{
	class HistoricalChart;
	class BaseDrawing;
	class DrawingObjectsModel;

	class DrawingObjectListDialog : public QDialog
	{
		Q_OBJECT
	public:
		DrawingObjectListDialog(const QList<BaseDrawing*>& lineObjects, HistoricalChart *parent);

		private slots:
		void ShowPropertyWindowSlot();
		void DeleteIndicatorSlot();
		void ViewDoubleClickedSlot(const QModelIndex&);

	private:
		BaseDrawing* getSelectedLine();

	private:
		HistoricalChart* m_pView;
		QPushButton* m_PropertiesButton;
		QPushButton* m_DeleteButton;
		QTableView* m_tableView;
		DrawingObjectsModel* m_model;
	};
}
