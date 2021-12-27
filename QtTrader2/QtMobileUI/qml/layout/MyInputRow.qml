import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

RowLayout {

    anchors.margins: 2
    Layout.preferredWidth: parent.width
    Layout.preferredHeight: 30;
    Layout.maximumHeight: 40
    Layout.minimumHeight: 20

    Label {
        id: labelLoginName
        text: qsTr("账户","LoginName")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        Layout.preferredWidth: 60
        Layout.preferredHeight:parent.height
    }

    TextField {
        id: textFieldLoginName
        Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
        horizontalAlignment: Text.AlignRight
        Layout.preferredWidth: parent.width - labelLoginName.width -10
        Layout.preferredHeight:parent.height
    }

}
