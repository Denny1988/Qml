import QtQuick 2.0

Item {
    id: cSwitcher;

    signal clicked(int index);

    property string str1 : "str1";
    property string str2 : "str2";

    property int selectIndex : 0;

    Rectangle
    {
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent

        Rectangle {
            id: back1
            width: parent.width * 0.5;
            height: parent.height;
            anchors.left: parent.left;
            color: selectIndex == 0 ? "#d0d0d0" : "#b0b0b0"
            z:1
        }

        Rectangle {
            id: back2
            width: parent.width * 0.5;
            height: parent.height;
            anchors.right: parent.right;
            color: selectIndex== 1 ? "#d0d0d0" : "#b0b0b0"
            z:1
        }

        Text {
            id: text1
            width: parent.width * 0.5;
            height: parent.height;
            anchors.left: parent.left;
            font.pointSize: 13;
            text:str1;
            font.bold: selectIndex == 0
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment:Text.AlignVCenter;
            color: selectIndex == 0 ? "#46BAFD" : "#000000"
            z:2
        }

        Text {
            id: text2
            width: parent.width * 0.5;
            height: parent.height;
            anchors.right: parent.right;
            font.pointSize: 13;
            font.bold: selectIndex == 1
            text:str2;
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment:Text.AlignVCenter;
            color: selectIndex == 1 ?  "#46BAFD" : "#000000"
            z:2
        }

        MouseArea {
            id: mouseArea
            width: parent.width;
            height: parent.height;
            anchors.right: parent.right;
            onClicked: {
                if(mouse.x < text1.width){
                    selectIndex = 0;
                    cSwitcher.clicked(0);
                }
                else {
                    selectIndex = 1;
                    cSwitcher.clicked(1);
                }
            }
        }


    }
}
