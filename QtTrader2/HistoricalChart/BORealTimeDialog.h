#pragma once
#include "ui_BORealTimeDialog.h"
#include "Settings.h"

namespace HistoricalChartSpace
{
	class BORealTimeDialog : public QDialog
	{
		Q_OBJECT
	public:
		BORealTimeDialog(const BORealTimePriceSettings& settings, QWidget *parent = 0);
		~BORealTimeDialog();
		
		const BORealTimePriceSettings& getSettings() const;

	public slots:
		void accept();
		void changeColor();

	private:
		void applyStringResource();
		void init();

	private:
		Ui::BORealTimeChartSettingsDialog m_ui;
		BORealTimePriceSettings m_settings;
	};
}
