import QtQuick 2.7
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

import "../layout"
import "../components"
import "../components/dialogs"

Rectangle {
    id: mainRect
    width: 480;
    height: 800;
    //color: rowQuotationMouseArea.pressed ? "#f0f0f0" : "#e0e0e0";
    z:0

    RowLayout {

        anchors.centerIn: parent;
        Button {
            id: button1
            width: 0.5*parent.width;
            height: 0.3* parent.width;
            text:"Test 1"
            onClicked: {
                numberInputModel.open();
            }
        }

        Button {
            id: button2
            width: 0.5*parent.width;
            height: 0.3* parent.width;
            text:"Test 2"
            onClicked: {
                show();
            }
        }

    }


    NumberInputModel{
        id: numberInputModel;
        m_mianWindowWidth : mainRect.width;
        m_mianWindowHeight: mainRect.height;
        m_textLabel: "";
        m_max: 100.0;
        m_min: 20.0;
        m_step: 5;
        m_value: 10.0;
    }

    function show()
    {
       numberInputModel.show("输入测试", 100, 10, 20, 5);
    }
}
