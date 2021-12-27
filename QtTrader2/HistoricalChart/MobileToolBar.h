#pragma  once
#include <QToolBar>
#include <QAction>
#include "Common.h"
#include "Config.h"
#include "Instrument.h"
#include <QComboBox>
#include "MenuPushButton.h"
#include "MobileComboxWidget.h"

namespace HistoricalChartSpace
{
	class HistoricalChart;
    class MobileToolBar : public QWidget
	{
        Q_OBJECT
	public:
        MobileToolBar(bool isBlackTheme, bool orientation, QWidget *parent);
        ~MobileToolBar();
        void createMobileToolBar(const QList<Frequency>& frequencies);

    private:
        bool m_orientation;//True is vertical screen false is horizontal screen
        int m_mobileToolBarWidth;
        bool m_isBlackTheme;
        QList<MenuPushButton *> m_frequencyMenuPushButtons;
        MenuPushButton *m_frequencyButton;
        MenuPushButton * m_graphTypeButton;
        MenuPushButton * m_indicatorButton;
        QList<Frequency> m_Frequencies;

        QMenu* m_frequencyMenu;
        QMenu* m_graphTypeMenu;
        QMenu* m_indicatormenu;

        int m_screenWidth;
        int m_screenHeight;

    public slots:
        void ClickFrequencySlot();
        void FrequencyClickSlot();
        void GraphTypeClickSlot();
        void IndicatorClickSlot();
        void CheckIndicatorMenuSlot(int index);
        void IndicatorMenuCancelSlot();
        void CheckFrequencyMenuSlot(int index);
        void FrequencyMenuCancelSlot();
        void CheckGraphTypeMenuSlot(int index);
        void GraphTypeMenuCancelSlot();



    signals:
        void SetFrequencySignal(int );
        void SetPriceChartSignal(int);
        void SetIndicatorSignal(int);

	};
}
