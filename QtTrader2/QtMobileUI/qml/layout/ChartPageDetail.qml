import QtQuick 2.0
import QtQuick.Window 2.0

Rectangle   {
    id: chartPageDetail
    width: 700;
    height:  1024;
    visible: true;


//    Item {
//        id: chartPageDetail2
//        width: parent.width;
//        height:  parent.height;
//        z:-1;
//    }
    Text {
        id:text;
       focus: true;
       x:50;
       y:50;
       anchors.bottom: t.bottom;
       Keys.enabled: true;//设置键盘可用
       color: "blue";
       text: "hello world";
       font.bold: true;
       font.pointSize: 16;
       styleColor: "#f51515";
       verticalAlignment: Text.AlignVCenter;
       horizontalAlignment: Text.AlignHCenter;
       ColorAnimation on color {
           to: "black";
           duration: 2000;
       }

    }

}
