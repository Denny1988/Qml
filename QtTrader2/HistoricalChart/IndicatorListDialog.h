#pragma once
#include <QDialog>
#include <QPushButton>
#include <QTreeWidget>

namespace HistoricalChartSpace
{
	class HistoricalChart;
	class IndicatorListDialog : public QDialog
	{
		Q_OBJECT
	public:
		IndicatorListDialog(HistoricalChart *parent);
		~IndicatorListDialog();

	private slots:
		void ShowPropertyWindowSlot();
		void DeleteIndicatorSlot();
		void CurrentItemChangedSlot(QTreeWidgetItem* current, QTreeWidgetItem* previous);
		void ItemDoubleClickedSlot(QTreeWidgetItem* item, int column);

	private:
		void buildTree();

	private:
		QTreeWidget* m_treeWidget;
		HistoricalChart* m_pView;
		QPushButton* m_PropertiesButton;
		QPushButton* m_DeleteButton;
		QTreeWidgetItem* m_mainChartItem;
		Qt::ItemDataRole m_dataRole;
	};
}