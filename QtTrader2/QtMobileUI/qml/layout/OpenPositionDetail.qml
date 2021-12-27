import QtQuick 2.7
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2

import "../components"

Item {
    property string instrumentCode: "insCode"
    property variant containerQML: null;
    property variant openPosition: null;

    function getItemValue(itemCode, itemValue)
    {
        if( itemValue !== null && itemValue !== ""){
            return itemValue ;
        }
        return openPosition[itemCode];
    }


    Component.onCompleted: {
        try{
            //if(openPositionModel !== null && openPosition !== null)
            for (var Key in openPosition){
                //"insCode":"USD/CAD", orderId:"USDCAD01", isVisible:true, "isChecked":false,  time:"18-06-04", lot:"4", isBuy:true, price:"123.00", "tradePLFloat":"50.00"}
                var itemName = Key;
                var itemCode = Key;
                var itemValue = openPosition[itemCode];
                if(Key == "insCode"){
                    itemName = qsTr("交易项目","openOrder-insCode");;
                } else if(Key == "orderId"){
                    itemName =qsTr("编号","openOrder-orderId");
                } else if(Key == "time"){
                    itemName =qsTr("成交时间","openOrder-time");
                } else if(Key == "isBuy"){
                    itemName =qsTr("买卖","openOrder-buySell");
                    itemValue = openPosition[itemCode] ? "买入": "卖出";
                } else if(Key == "lot"){
                    itemName =qsTr("手数","openOrder-lot");
                }else if(Key == "price"){
                    itemName =qsTr("价格","openOrder-price");
                    itemValue = openPosition[itemCode];
                } else if(Key == "tradePLFloat"){
                    itemName =qsTr("浮动盈亏","openOrder-tradePLFloat");
                    itemValue = "";
                } else {
                    itemCode = "";
                }
                if(itemCode !== null && itemCode !== ""){
                    var listEl = {
                        "itemName":itemName,
                        "itemCode":itemCode,
                        "itemValue":itemValue, //"DEM2018060606"
                    };
                    openPositionModel.append(listEl);
                }
            }
            openPositionModel.append({itemName:"apendFoot",itemCode:"",itemValue:"limitClose,dqClose"});
            console.log("OpenPositionDetail Component.onCompleted");
        }
        catch(ex){
            console.log("OpenPositionDetail Component.onCompleted:", ex);
        }
    }

    //openPositionDelegate
    Component {

        id:  openPositionDelegate

        Item {
            id:rowotherMore;

            width: parent.width
            height: (itemName == "apendHead" || itemName == "apendFoot") ? CDimens.pixelDensity * 13 : CDimens.pixelDensity * 10;

            Rectangle {
                id: background
                anchors.fill: parent
                color:  index % 2 == 0 ?  "#ffffff" : "#f0f0f0" ;
                z:0
            }
            Row {
                id: row0
                width: parent.width;
                height: parent.height;
                visible:(itemName == "apendHead")
                Rectangle{
                    width: parent.width
                    height: parent.height;
                    CLabel {
                        id:subOrderHead;
                        anchors.centerIn: parent;
                        color: "#FF5D27"
                        font.pointSize: 14
                        font.bold: true
                        text:qsTr("开仓单","openOrder");
                    }
                }
            }

            Row {
                id: row1
                width: parent.width;
                height: parent.height;
                visible: (itemName != "apend" && itemName != "apendHead" && itemName != "apendFoot")
                Text {
                    width: (parent.width - CDimens.pixelDensity * 2) * 0.4
                    height: parent.height;
                    text: itemName
                    font.pointSize: 12
                    color: "#FF5D27"
                    font.bold: true
                    verticalAlignment:Text.AlignVCenter;
                    leftPadding :  2 * CDimens.pixelDensity
                }

                Text {
                    width:  (parent.width - CDimens.pixelDensity * 2) * 0.6
                    height: parent.height;
                    text: getItemValue(itemCode, itemValue);
                    font.pointSize: 12
                    color: "black"
                    horizontalAlignment: Text.AlignLeft;
                    verticalAlignment: Text.AlignVCenter

                }
            }
            Row {
                id: row2
                width: parent.width;
                height: parent.height;
                visible:(itemName == "apendFoot")
                Rectangle{
                    width: parent.width * 0.5
                    height: parent.height;
                    CButton {
                        id:limitClose;
                        width: parent.width * 0.8
                        height: parent.height - 4;
                        anchors.centerIn: parent;
                        text:qsTr("限价指令","limitClose");
                    }
                }

                Rectangle{
                    width: parent.width * 0.5
                    height: parent.height;
                    CButton {
                        id:dqClose
                        width: parent.width * 0.8
                        height: parent.height - 4;
                        anchors.centerIn: parent;
                        text:qsTr("平仓指令","dqClose");
                    }
                }
            }
        }
    }

    //openPositionModel
    ListModel {
       id: openPositionModel

       ListElement {
           itemName:"apendHead";
           itemCode:"";
           itemValue:"openOrder";
       }

       Component.onCompleted: {
           console.log("openPositionModel onCompleted");
       }
    }
    Rectangle {
        id: rectangleOpenPosition;
        width: parent.width
        height: parent.height
        anchors.top: rectangleAccountName.bottom;
        anchors.topMargin: 1 * CDimens.pixelDensity;
        ListView {
            id: listView
            width: parent.width
            height: parent.height
            visible: true;
            clip: true;
            spacing: 2;
            delegate: openPositionDelegate;
            model: openPositionModel ;
        }
        Component.onCompleted: {
            console.log("openPositionModel onCompleted listView.width =" + listView.width +", rectangleopenPosition.width = "
                        + rectangleopenPosition.width );
        }
    }
}
