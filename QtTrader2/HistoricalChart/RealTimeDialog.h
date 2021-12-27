#pragma once
#include "ui_realTimeDialog.h"
#include "Settings.h"

namespace HistoricalChartSpace
{
	class RealTimeDialog : public QDialog
	{
		Q_OBJECT
	public:
		RealTimeDialog(const RealTimePriceSettings& settings, QWidget *parent = 0);
		~RealTimeDialog();
		
		const RealTimePriceSettings& getSettings() const;

	public slots:
			void accept();
			void changeColor();

    signals:
        void UserSetRealTimeChartColorSignSignal();

	private:
		void applyStringResource();
		void init();

	private:
		Ui::realTimeChartSettingsDialog m_ui;
		RealTimePriceSettings m_settings;
	};
}
