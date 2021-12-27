#include "MenuPushButton.h"

namespace HistoricalChartSpace
{
    MenuPushButton::MenuPushButton(int index, MenuPushButtonType menuPushButtonType, QWidget *parent): QPushButton(parent)
    {
        this->_index = index;        
        this->setStyleSheet("border: 1px solid black;border-width:1px;border-right-width:0px;background-color:#ffffff;color:balck;font-size:14pt; text-align:center;");
    }
}

#include "moc_MenuPushButton.cpp"
