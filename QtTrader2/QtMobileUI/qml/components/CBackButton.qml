/**/

import QtQuick 2.0
import QtQuick.Layouts 1.2
import "."

Item {
    id: cBackButton

    property alias text: buttonLabel.text

    Rectangle {
        id: background
        anchors.fill: parent
        color: settingsPalette.button
        visible: mouseArea.pressed
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 5 * CDimens.pixelDensity
        anchors.rightMargin: 5 * CDimens.pixelDensity
        spacing: 3 * CDimens.pixelDensity

        CIcon {
            text: "<"
        }

        CLabel {
            id: buttonLabel
            Layout.fillWidth: true
            Layout.fillHeight: true
            font.pointSize: 8
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            if (!stackView.busy)
                stackView.pop()
        }
    }
}
