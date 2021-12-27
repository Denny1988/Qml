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

    Button {
        id: button1
        width: 0.5*parent.width;
        height: 0.3* parent.width;
        anchors.centerIn: parent;
        text:qsTr("测试输入控件","test number input")
        onClicked: {
            numberInputDialog.open();
        }
    }

    NumberInputDialog {
        id:numberInputDialog
        m_mianWindowWidth : mainRect.width;
        m_mianWindowHeight: mainRect.height;
        m_max: 100.0;
        m_min: 20.0;
        m_step: 5;
        m_value: 10.0;



        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            numberInputDialog.close()
        }
        onRejected: {
            numberInputDialog.close()
        }
    }

}
