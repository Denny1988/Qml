#include "DrawingObjectListDialog.h"
#include "historicalchart.h"
#include "BaseDrawing.h"
#include "ChartPanel.h"
#include "SR.h"
#include "DrawingObjectsModel.h"
#include "DrawingObjectDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace HistoricalChartSpace
{
	DrawingObjectListDialog::DrawingObjectListDialog(const QList<BaseDrawing*>& lineObjects, HistoricalChart *parent) :QDialog(parent->getDialogParent()), m_pView(parent)
	{
		this->setMinimumSize(QSize(510, 300));
		this->setWindowTitle(SR::get("DrawingObjectList"));

		m_tableView = new QTableView(this);

		
		QObject::connect(m_tableView->horizontalHeader(), &QHeaderView::sectionClicked, [=](int){
			m_tableView->setSortingEnabled(true);
		});

		m_model = new DrawingObjectsModel(lineObjects, this);
		QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
		proxyModel->setSourceModel(m_model);

        //Set up the view model
		m_tableView->setModel(proxyModel);

        //Set alternate colors
		m_tableView->setAlternatingRowColors(true);
		m_tableView->verticalHeader()->hide();

        // Can only select rows
		m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        // Only single line can be selected
		m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
		connect(m_tableView, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(ViewDoubleClickedSlot(const QModelIndex&)));

		// Layout
		QHBoxLayout* hboxLayout = new QHBoxLayout(this);
		QVBoxLayout* vboxLayout = new QVBoxLayout(this);
		
		this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		m_PropertiesButton = new QPushButton(SR::get("Properties"), this);
		connect(m_PropertiesButton, SIGNAL(clicked()), SLOT(ShowPropertyWindowSlot()));
		m_DeleteButton = new QPushButton(SR::get("Delete"), this);
		connect(m_DeleteButton, SIGNAL(clicked()), SLOT(DeleteIndicatorSlot()));
		QPushButton* closeButton = new QPushButton(SR::get("Close"), this);
		connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));

		vboxLayout->addWidget(m_PropertiesButton);
		vboxLayout->addWidget(m_DeleteButton);
		vboxLayout->addStretch();
		vboxLayout->addWidget(closeButton);

		hboxLayout->addWidget(m_tableView);
		hboxLayout->addLayout(vboxLayout);

		this->setLayout(hboxLayout);

	}

	void DrawingObjectListDialog::ShowPropertyWindowSlot()
	{
		BaseDrawing* baseLine = this->getSelectedLine();
		if (baseLine)
		{
			DrawingObjectDialog dialog(baseLine->getSettings(), m_pView);
			dialog.exec();

			if (dialog.result() == QDialog::Accepted)
			{
				baseLine->setSettings(dialog.getSettings());
			}
		}
	}

	void DrawingObjectListDialog::DeleteIndicatorSlot()
	{
		BaseDrawing* baseLine = this->getSelectedLine();
		if (baseLine)
		{
			if (baseLine->getPanel()->removeLineObject(baseLine))
			{
				int row = m_tableView->selectionModel()->selectedRows().at(0).row();
				m_model->removeRows(row, 1);
			}
		}
	}

	void DrawingObjectListDialog::ViewDoubleClickedSlot(const QModelIndex& index)
	{
		this->ShowPropertyWindowSlot();
	}

	BaseDrawing* DrawingObjectListDialog::getSelectedLine()
	{
		BaseDrawing* baseLine = nullptr;
		QModelIndexList selection = m_tableView->selectionModel()->selectedRows();
		if (selection.count() > 0)
		{
			int index = selection.at(0).data(Qt::UserRole).toInt();
			baseLine = m_model->getBaseLine(index);
		}
		return baseLine;
	}
}

#include "moc_DrawingObjectListDialog.cpp"
