import QtQuick 2.7
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2

import "../components"
import "../../js/WorkingOrders.js" as WorkingOrders

Item {

    width: 480
    height: 480

    //instrumentGroupsModel for instrumentGroups listView
    ListModel {
        id: instrumentGroupsModel
        Component.onCompleted: {
            try{
                instrumentGroupsModel.append({"insName":"USD/CAD","orderGroupDate":"2018-05-22","subOrders":[]});
                instrumentGroupsModel.get(0).subOrders.append(
                            {orderId:"USDCAD01", stateText:"Placed",timeText:"12:00:50",lotText:"1.0",typeText:"Limit",priceText:"12345"});

                WorkingOrders.initOrders();
                for(var i =0; i<WorkingOrders.orderGroups.length; i++)
                {
                    instrumentGroupsModel.append(WorkingOrders.orderGroups[i]);
                }
            }catch(ex){
                console.log("WorkingOrders.initOrders:",ex);
            }

        }
    }
    //subOrdersDelegate of instrumentGroup for subOrder Repeater
    Component {
        id: subOrdersDelegate
        Rectangle {
            id:subOrderRectangle
            width: parent.width;
            height: CDimens.listLineHeight;

            Rectangle {
                id: cHorizontalSeparator
                anchors.bottom: parent.bottom
                width: parent.width;
                height: Math.max(1, Math.round(0.3 * CDimens.pixelDensity))
                color: settingsPalette.separator
            }

            Row {
                id:subOrders
                width: parent.width;
                height: parent.height - cHorizontalSeparator.height;
                z:1

                Text {
                    id: subOrderState
                    width: parent.width * 0.2
                    height: parent.height
                    text: stateText
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                    id: subOrderTime
                    width: parent.width * 0.2
                    height: parent.height
                    text: timeText
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                    id: subOrderType
                    width: parent.width * 0.2
                    height: parent.height
                    text: typeText
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                    id: subOrderLot
                    width: parent.width * 0.2
                    height: parent.height
                    text: lotText
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                    id: subOrderPrice
                    width: parent.width * 0.2
                    height: parent.height
                    text: priceText
                    verticalAlignment: Text.AlignVCenter
                }
            }


        }
    }

    //instrumentGroupDelegate for instrumentGroup listView
    Component {
        id: instrumentGroupDelegate
        Column {

            width: parent.width
            Rectangle {
                id : groupHeader;
                color:settingsPalette.background
                width: parent.width
                height: CDimens.listLineHeight
                Row {
                    width: parent.width
                    height: parent.height
                    Text {
                        id: instrumentName
                        width: parent.width * 0.5
                        height: parent.height
                        text: insName
                        verticalAlignment: Text.AlignVCenter
                    }

                    Text {
                        id: instrumentPrice
                        width: parent.width * 0.5
                        height: parent.height
                        text: orderGroupDate
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
            Repeater {
                model: subOrders
                delegate: subOrdersDelegate
            }
        }
    }



    Rectangle {
        id:mainRectangle
        width: parent.width;
        height:parent.height;
        color: settingsPalette.headerTitleBarBackground

        Rectangle {
            id:orderHeaderBar
            width: parent.width;
            height: CDimens.headerTitleBarHeight;
            visible: true;
            z: 2 //top to the listview
            Rectangle {
                color: settingsPalette.headerTitleBarBackground
                anchors.fill: parent;
                z:0
            }
            Row {
                z:2
                width: parent.width;
                height: parent.height;
                Text {
                    width: parent.width * 0.2
                    height: parent.height
                    text: qsTr("状态", "Order-state")
                    verticalAlignment: Text.AlignVCenter
                    color: settingsPalette.headerTitleBarText
                }

                Text {
                    width: parent.width *0.2
                    height: parent.height
                    text: qsTr("时间", "Order-time")
                    verticalAlignment: Text.AlignVCenter
                    color: settingsPalette.headerTitleBarText
                }
                Text {
                    width: parent.width *0.2
                    height: parent.height
                    text: qsTr("类型", "Order-type")
                    verticalAlignment: Text.AlignVCenter
                    color: settingsPalette.headerTitleBarText
                }
                Text {
                    width: parent.width *0.2
                    height: parent.height
                    text: qsTr("数量", "Order-lot")
                    verticalAlignment: Text.AlignVCenter
                    color: settingsPalette.headerTitleBarText
                }
                Text {
                    width: parent.width *0.2
                    height: parent.height
                    text: qsTr("价格", "Order-price")
                    verticalAlignment: Text.AlignVCenter
                    color: settingsPalette.headerTitleBarText
                }
            }
        }

        Rectangle {

           anchors.top: orderHeaderBar.bottom
           width: parent.width;
           height:parent.height - orderHeaderBar.height - CDimens.footerHeight;
           clip: true;
           z: 1;
           //View
           ListView{
               id:instrumentGroupListView
               anchors.fill: parent;
               model:instrumentGroupsModel
               delegate: instrumentGroupDelegate
           }
        }

    }
}
