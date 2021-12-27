#include "IndicatorListDialog.h"
#include "historicalchart.h"
#include "PricePanel.h"
#include "IndicatorPanel.h"
#include "ChartPanel.h"
#include "Indicator/Indicator.h"
#include "IndicatorDialog.h"
#include "SR.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidgetItem>

namespace HistoricalChartSpace
{
	IndicatorListDialog::IndicatorListDialog(HistoricalChart *parent) :QDialog(parent->getDialogParent()), m_pView(parent), m_dataRole(Qt::StatusTipRole)
	{
		this->setMinimumSize(QSize(400, 300));
		this->setWindowTitle(SR::get("IndicatorList"));
		
		this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		m_PropertiesButton = new QPushButton(SR::get("Properties"), this);
		connect(m_PropertiesButton, SIGNAL(clicked()), SLOT(ShowPropertyWindowSlot()));
		m_DeleteButton = new QPushButton(SR::get("Delete"), this);
		connect(m_DeleteButton, SIGNAL(clicked()), SLOT(DeleteIndicatorSlot()));
		QPushButton* closeButton = new QPushButton(SR::get("Close"), this);
		connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));

		QHBoxLayout* hboxLayout = new QHBoxLayout(this);
		QVBoxLayout* vboxLayout = new QVBoxLayout(this);
		m_treeWidget = new QTreeWidget(this);

		vboxLayout->addWidget(m_PropertiesButton);
		vboxLayout->addWidget(m_DeleteButton);
		vboxLayout->addStretch();
		vboxLayout->addWidget(closeButton);

		hboxLayout->addWidget(m_treeWidget);
		hboxLayout->addLayout(vboxLayout);

		this->setLayout(hboxLayout);

		m_treeWidget->headerItem()->setHidden(true);
		this->buildTree();

		m_PropertiesButton->setEnabled(false);
		m_DeleteButton->setEnabled(false);
		connect(m_treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), SLOT(CurrentItemChangedSlot(QTreeWidgetItem*, QTreeWidgetItem*)));
		connect(m_treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(ItemDoubleClickedSlot(QTreeWidgetItem*, int)));
	}

	IndicatorListDialog::~IndicatorListDialog()
	{
	}

	void IndicatorListDialog::ShowPropertyWindowSlot()
	{
		QList<QTreeWidgetItem*> selectedItems = m_treeWidget->selectedItems();
		if (selectedItems.count() > 0)
		{
			Indicator* indicator = (Indicator*)selectedItems[0]->data(0, m_dataRole).value<void*>();
			IndicatorDialog::ShowDialog(indicator, this);
		}
	}

	void IndicatorListDialog::DeleteIndicatorSlot()
	{
		QList<QTreeWidgetItem*> selectedItems = m_treeWidget->selectedItems();
		if (selectedItems.count() > 0)
		{
			QTreeWidgetItem* selectedItem = selectedItems[0];
			QTreeWidgetItem* parentItem = selectedItem->parent();
			if (parentItem)
			{
				Indicator* indicator = (Indicator*)selectedItems[0]->data(0, m_dataRole).value<void*>();
				if (parentItem == m_mainChartItem)
				{
					m_pView->getPricePanel()->removeIndicator(indicator);
				}
				else
				{
					IndicatorPanel* panel = (IndicatorPanel*)selectedItem->parent()->data(0, m_dataRole).value<void*>();
					if (panel)
					{
						panel->removeIndicator(indicator);
					}
				}
			}
			else
			{
				IndicatorPanel* panel = (IndicatorPanel*)selectedItems[0]->data(0, m_dataRole).value<void*>();
				m_pView->removeIndicatorPanel(panel);
			}

			m_treeWidget->clear();
			this->buildTree();
		}
	}

	void IndicatorListDialog::CurrentItemChangedSlot(QTreeWidgetItem* current, QTreeWidgetItem* previous)
	{
		if (current)
		{
			bool isOnMainChart = current == m_mainChartItem;
			bool isOnIndicator = !isOnMainChart && current->parent();
			if (isOnIndicator)
			{
				m_PropertiesButton->setEnabled(true);
				m_DeleteButton->setEnabled(true);
			}
			else if (isOnMainChart)
			{
				m_PropertiesButton->setEnabled(false);
				m_DeleteButton->setEnabled(false);
			}
			else // on chart window
			{
				m_PropertiesButton->setEnabled(false);
				m_DeleteButton->setEnabled(true);
			}
		}
		else
		{
			m_PropertiesButton->setEnabled(false);
			m_DeleteButton->setEnabled(false);
		}
	}

	void IndicatorListDialog::ItemDoubleClickedSlot(QTreeWidgetItem* item, int column)
	{
		bool isOnMainChart = item == m_mainChartItem;
		bool isOnIndicator = !isOnMainChart && item->parent();
		if (isOnIndicator)
		{
			Indicator* indicator = (Indicator*)item->data(0, m_dataRole).value<void*>();
			IndicatorDialog::ShowDialog(indicator, this);
		}
	}

	void IndicatorListDialog::buildTree()
	{
		m_mainChartItem = new QTreeWidgetItem(m_treeWidget, QStringList(SR::get("MainChart")));
		const QVector<Indicator*>& indicators = m_pView->getPricePanel()->getIndicators();
        foreach (Indicator* indicator, indicators)
		{
			QTreeWidgetItem* indicatorItem = new QTreeWidgetItem(m_mainChartItem, QStringList(indicator->getTitle()));
			indicatorItem->setData(0, m_dataRole, QVariant::fromValue((void*)indicator));
		}

		const QVector<IndicatorPanel*>& indicatorPanels = m_pView->getIndicatorPanels();
		for (int i = 0; i < indicatorPanels.size(); i++)
		{
			QTreeWidgetItem* indicatorWindowItem = new QTreeWidgetItem(m_treeWidget, QStringList(SR::get("IndicatorWindow") + QString::number(i + 1)));
			indicatorWindowItem->setData(0, m_dataRole, QVariant::fromValue((void*)indicatorPanels[i]));
			const QVector<Indicator*>& indicators = indicatorPanels[i]->getIndicators();
            foreach (Indicator* indicator, indicators)
			{
				QTreeWidgetItem* indicatorItem = new QTreeWidgetItem(indicatorWindowItem, QStringList(indicator->getTitle()));
				indicatorItem->setData(0, m_dataRole, QVariant::fromValue((void*)indicator));
			}
		}
		m_treeWidget->expandAll();
	}

}

#include "moc_IndicatorListDialog.cpp"
