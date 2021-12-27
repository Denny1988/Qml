import QtQuick 2.0
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2

import "../components"


Item {
    id: pageHeader;

    property alias title: title.text

    Rectangle {
        width: parent.width;
        height: parent.height;

        ToolBar {
            RowLayout {
                anchors.fill: parent
                ToolButton {
                    text: qsTr("‹")
                    onClicked: stack.pop()
                }
                Label {
                    id:title
                    text: qsTr("Title")
                    elide: Label.ElideRight
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    Layout.fillWidth: true
                }
                ToolButton {
                    text: qsTr("⋮")
                    onClicked: menu.open()
                }
            }
        }
    }


}
