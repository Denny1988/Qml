import QtQuick 2.0
import "."
Item {
    id: cHintButton;

    signal clicked();

    property alias backgroundColor: background.color

    property alias hintColor: cHintLabel.color
    property alias hint: cHintLabel.text

    property alias btnTextColor: cTextLabel.color
    property alias btnText: cTextLabel.text

    Rectangle {
        id: background
        anchors.fill: parent
    }

    Text {
        id: cHintLabel
        font.pointSize:9
        verticalAlignment: Text.AlignVCenter

    }

    Text {
        id: cTextLabel
        font.pointSize: 14
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        anchors.centerIn: parent

    }

    MouseArea {
        id: mouseArea
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
        onClicked: {
            cHintButton.clicked();
        }
    }

}
