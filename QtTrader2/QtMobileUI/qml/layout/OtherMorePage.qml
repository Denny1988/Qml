import QtQuick 2.0
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

import "../components"

import "../../js/Helper.js" as JsHelper

Item {

    function isItemEnable(itemId){
        return true;
    }

    Component {

        id:  otherMoreDelegate

        Item {
            id:rowotherMore;

            width: parent.width
            height: CDimens.pixelDensity * 12;

            Rectangle {
                id: background
                anchors.fill: parent
                color: rowotherMoreMouseArea.pressed ? "#e0e0e0" : "#ffffff";
                z:0
            }

            Row {
                id: row1
                width: parent.width
                height: parent.height;

                Rectangle {
                    id:rectangleItemImage
                    width: CDimens.pixelDensity * 8;
                    height: parent.height;
                    Image {
                        anchors.centerIn: parent
                        width: CDimens.pixelDensity * 6;
                        height: CDimens.pixelDensity * 6;
                        source: "../../images/" + itemImage
                        clip: true;
                    }

                }

                Text {
                    width: (parent.width - CDimens.pixelDensity * 16) * 0.4
                    height: parent.height;
                    text: itemName
                    font.pointSize: 16
                    color:"black"
                    font.bold: true
                    verticalAlignment:Text.AlignVCenter;
                }

                Text {
                    width:  (parent.width - CDimens.pixelDensity * 16) * 0.6
                    height: parent.height;
                    text: itemDescription
                    font.pointSize: 12
                    color:"#C0C0C0"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter

                }

                Rectangle {
                    id:rectangleGo
                    width: CDimens.pixelDensity * 8;
                    height: parent.height;
                    Image {
                        anchors.centerIn: parent
                        width: CDimens.pixelDensity * 5;
                        height: CDimens.pixelDensity * 5;
                        source: "../../images/go.png"
                        clip: true;
                    }
                }
            }


            MouseArea {
                id: rowotherMoreMouseArea
                anchors.left: rowotherMore.left;
                anchors.top:  rowotherMore.top;
                width: rowotherMore.width;
                height: rowotherMore.height;

                onClicked: {
                    console. log("rowotherMore onClicked " + itemId + "itemPage " + itemPage);
                    if(isItemEnable(itemId) && !JsHelper.StringHelper.isNullOrEmpty(itemPage) ){
                        stackView.push(Qt.resolvedUrl(itemPage))
                    } else {
                        console. log("rowotherMore onClicked() itemId " + itemId + "itemPage " + itemPage + " not active");
                    }
                }
            }

        }
    }

    ListView {
        id: listView
        width: parent.width
        height: parent.height
        visible: true;
        spacing: 2;
        delegate:   otherMoreDelegate;


        model: ListModel {

            id: otherMoreModel

            ListElement {
                itemId:"000";
                itemName:"????????????";
                itemImage:"lemanager_fix_manager.png";
                itemDescription:"";
                itemPage:""
            }

            ListElement {
                itemId:"001";
                itemName:"????????????";
                itemImage:"lemanager_deal_record.png";
                itemDescription:"";
                itemPage:""
            }

            ListElement {
                itemId:"002";
                itemName:"????????????";
                itemImage:"lemanager_account_security.png";
                itemDescription:"????????????";
                itemPage:"./ChartPage.qml"
            }


            ListElement {
                itemId:"003";
                itemName:"????????????";
                itemImage:"lemanager_highzone.png";
                itemDescription:"";
                itemPage:""
            }

            ListElement {
                itemId:"004";
                itemName:"????????????";
                itemImage:"lemanager_account_security.png";
                itemDescription:"??????????????????????????????";
                itemPage:"./AccountDetailPage.qml"
            }

            ListElement {
                itemId:"005";
                itemName:"????????????";
                itemImage:"lemanager_my_bankcard.png";
                itemDescription:"??????????????????????????????????????????";
                itemPage:""
            }

        }

    }



}
