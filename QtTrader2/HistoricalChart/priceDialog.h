#pragma  once
#include "ui_priceDialog.h"
#include "Settings.h"


namespace HistoricalChartSpace
{
	class PriceDialog : public QDialog
	{
		Q_OBJECT
	public:
		PriceDialog(CommonSettings& commonSettings, PriceSettings& priceSettings, QWidget *parent);
		~PriceDialog();

		//const CommonSettings& getCommonSettings() const;
		//const PriceSettings& getPriceSettings() const;

    public slots:
		void accept();

    signals:
        void UserSetChartColorSignSignal();

	private:
		void applyStringResource();
		void initSettings();
		void setComBoxDefaultColor(QComboBox* cbb, const QColor& color);
		enPriceType getPriceDrawType() const;
		ZoomLevel getElementZoomRatio() const;
	private:
		Ui::Dialog		m_ui;
		CommonSettings&	m_commonSettings;
		PriceSettings&	m_priceSettings;
	};
}


