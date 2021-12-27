#ifndef MENUPUSHBUTTON_H
#define MENUPUSHBUTTON_H

#include <QPushButton>

namespace HistoricalChartSpace
{
    enum MenuPushButtonType
    {
        MenuFrequency,
        MenuGraphType,
        MenuIndicator
    };

    class MenuPushButton : public QPushButton
    {
        Q_OBJECT
    public:
        MenuPushButton(int index, MenuPushButtonType menuPushButtonType, QWidget *parent = 0);
        int getIndex() { return _index; }

    private:
        int _index;



    };
}

#endif


