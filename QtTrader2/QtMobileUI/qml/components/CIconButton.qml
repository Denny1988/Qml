/**/

import QtQuick 2.0
import QtQuick.Layouts 1.2
import "."

Item {
    id: cBackButton

    property alias text: cIcon.text

    property color clickColor: "#303030"
    property color textColor: "#000000"

    signal clicked();

    Rectangle {
        id: background
        anchors.fill: parent
        color: settingsPalette.button
        visible: mouseArea.pressed
        z:0
    }

    Rectangle {
        anchors.fill: parent
        z:0

        CIcon {
            id:cIcon
            anchors.centerIn: parent
            text: "*"
            font.pointSize: CDimens.normalFont;
            color: mouseArea.pressed ? clickColor : textColor
        }

    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            clicked();
        }
    }
}
