import QtQuick 2.0
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2

import "../components"


Item {

    property bool isShowEnable: true;

    Component {

        id:  openOrderHeader

        Item {
            width: parent.width
            height: 8 * CDimens.pixelDensity

            Rectangle {
                id: background
                anchors.fill: parent
                color: "#e0e0e0";
            }

            Row {
                id: rowHeader
                width: parent.width
                height: parent.height;
                Text {
                    width: parent.width * 0.3
                    height: parent.height;
                    text: qsTr("编号","Code")
                    font.pointSize: 9
                    font.bold: true
                    verticalAlignment:Text.AlignVCenter;
                }

                Text {
                    width: parent.width * 0.3
                    height: parent.height;
                    text: qsTr("价格","Price")
                    font.pointSize: 9
                    font.bold: true
                    verticalAlignment:Text.AlignVCenter;
                }

                Text {
                    width: parent.width * 0.2
                    height: parent.height;
                    text: qsTr("手数","Lot")
                    font.pointSize: 9
                    font.bold: true
                    verticalAlignment:Text.AlignVCenter;
                }

                Text {
                    width: parent.width * 0.2
                    height: parent.height;
                    text: qsTr("平仓","close-Lot")
                    font.pointSize: 9
                    font.bold: true
                    verticalAlignment:Text.AlignVCenter;
                }

                spacing: 10
            }
        }
    }

    Component {

        id:  openOrderDelegate

        Item {
            id:rowopenOrder;

            width: parent.width
            height: 7 * CDimens.pixelDensity

            Rectangle {
                id: background
                anchors.fill: parent
                color:"#f0f0f0";
                z:0
            }

            Row {
                id: row1
                width: parent.width
                height: parent.height;

                CheckBox {
                    width: parent.width * 0.3
                    height: parent.height;
                    id:chcekbox1
                    text: orderCode
                    font.pointSize: 8
                    onCheckedChanged:   {
                        console.log("order Checked Changed : ",orderCode);
                    }
                }

                Text {
                    width: parent.width * 0.3
                    height: parent.height;
                    text: price
                    font.pointSize: 8
                    verticalAlignment:Text.AlignVCenter;
                }

                Text {
                    width: parent.width * 0.2
                    height: parent.height;
                    text: (isBuy ? "B " : "S ")  + lot;
                    font.pointSize: 8
                    color:(isBuy ? "Red" : "Green")
                    verticalAlignment:Text.AlignVCenter;
                }

                Text {
                    width: parent.width * 0.2
                    height: parent.height;
                    text: "" + closeLot
                    font.pointSize: 8
                    verticalAlignment:Text.AlignVCenter;
                }

                spacing: 10
            }
        }
    }


    Rectangle {
        id: rectangleTitleBar
        width: parent.width;
        height: 8 * CDimens.pixelDensity;

        Row {
            width: parent.width;
            height: parent.height;

            Text {
                id: labelSelectOpenOrder
                height: parent.height;
                verticalAlignment:Text.AlignVCenter;
                text:qsTr("选取开仓单平仓：", "select-openOrder");
            }

            CheckBox {
                id: checkBoxBuy
                font.pointSize: 8
                text: qsTr("买","Check Box Buy")
            }

            CheckBox {
                id: checkBoxSell
                font.pointSize: 8
                text: qsTr("卖","Check Box Sell")
            }

            Rectangle {
              width: parent.width*0.2;
              height: parent.height;
            }

            Switch {
                id: switch1
                text: qsTr("隐藏/展开","Switch")
                checked: true;
                onCheckedChanged: {
                    isShowEnable = checked;
                    console.log("switch1.checked = " + checked);
                }
            }
        }

    }

    Rectangle {
        id: rectangleList
        width: parent.width;
        height: parent.height - rectangleTitleBar.height ;
        clip: true
        anchors.top: rectangleTitleBar.bottom


        ListView {
            id: listView
            width: parent.width
            height: parent.height
            visible: true;
            spacing: 2;
            header:openOrderHeader;
            delegate:openOrderDelegate;

            model: ListModel {

                id: openOrderModel

                ListElement {
                    orderId:"0001"
                    orderCode:"0001";
                    price:"1234.080";
                    isBuy:false;
                    lot:2;
                    closeLot: "0";
                }

                ListElement {
                    orderId:"0002"
                    orderCode:"0002";
                    price:"4567.080";
                    isBuy:true;
                    lot:3;
                    closeLot: "0";
                }

            }

        }

    }
}
