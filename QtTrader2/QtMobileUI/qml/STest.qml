import QtQuick 2.7
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

//import QtQuick.Controls 2.2

import "./layout"
import "./components"
import "./components/dialogs"

Rectangle {
    id: mainRect
    width: 400;
    height: 300;
    //color: rowQuotationMouseArea.pressed ? "#f0f0f0" : "#e0e0e0";
    z:0

    Button {
        id: button1
        width: 0.5*parent.width;
        height: 0.3* parent.width;
        anchors.centerIn: parent;
        onClicked: {
            dialog.open();
        }
    }
}
