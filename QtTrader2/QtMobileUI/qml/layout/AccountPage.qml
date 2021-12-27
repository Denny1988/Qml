import QtQuick 2.7
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2

import "../components"

import "../../js/Helper.js" as JsHelper


Rectangle {

    id:accountDetail
    width: CDimens.pageWidth;
    height:  CDimens.pageHeight;

    //account Model
    QtObject {
        id: accountDetailValue;
        property string customerName:"Test01";
        property string accountName :"TestAcc01";
        property  bool isLogined: false;
    }

    //accountFundDelegate
    Component {

        id:  accountFundDelegate

        Item {
            id:rowotherMore;

            width: parent.width
            height: CDimens.pixelDensity * 12;

            Rectangle {
                id: background
                anchors.fill: parent
                color: parseInt(itemId)%2 ?  "#ffffff" : "#f0f0f0" ;
                z:0
            }

            Row {
                id: row1
                width: parent.width;
                height: parent.height;

                Text {
                    width: (parent.width - CDimens.pixelDensity * 4) * 0.4
                    height: parent.height;
                    text: itemName
                    font.pointSize: CDimens.smallFont
                    color: "#FF5D27"
                    font.bold: true
                    verticalAlignment:Text.AlignVCenter;
                    leftPadding :  2 * CDimens.pixelDensity
                }

                Text {
                    width:  (parent.width - CDimens.pixelDensity * 4) * 0.2
                    height: parent.height;
                    text: itemCode
                    font.pointSize: 12
                    color:"#808080"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter

                }

                Text {
                    width:  (parent.width - CDimens.pixelDensity * 4) * 0.4
                    height: parent.height;
                    text: itemValue
                    font.pointSize: 12
                    color: "black"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter

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
                    if(isItemEnable(itemId) && JsHelper.StringHelper.isNullOrEmpty(itemPage) ){
                        stackView.push(Qt.resolvedUrl(itemPage))
                    } else {
                        console. log("rowotherMore onClicked() itemId " + itemId + "itemPage " + itemPage + " not active");
                    }
                }
            }

        }
    }


    //accountFundModel
    ListModel {
       id: accountFundModel

       ListElement {
           itemId:"000";
           itemName:"结余";
           itemCode:"USD";
           itemValue:"500000.00"
       }

       ListElement {
           itemId:"001";
           itemName:"浮动盈亏";
           itemCode:"USD";
           itemValue:"0"
       }

       ListElement {
           itemId:"002";
           itemName:"有效保证金";
           itemCode:"USD";
           itemValue:"500000.00"
       }


       ListElement {
           itemId:"003";
           itemName:"所需保证金";
           itemCode:"USD";
           itemValue:"0"
       }

       ListElement {
           itemId:"004";
           itemName:"可用保证金";
           itemCode:"USD";
           itemValue:"500000.00"
       }

       ListElement {
           itemId:"005";
           itemName:"备注";
           itemCode:"";
           itemValue:""
       }


       Component.onCompleted: {
           console.log("accountFundModel onCompleted");
       }
    }


    Rectangle {
        color: "#f0f0f0"
        anchors.fill: parent;
        z:0
    }

    Rectangle {
        id: columnLayout
        width: parent.width;
        height: parent.height;

        Rectangle {
            id: rectangleAccountName;
            width: parent.width
            height: 24 * CDimens.pixelDensity

            Row {
                id: rowAccountName
                width: parent.width
                height: parent.height
                spacing: 2
                Rectangle {
                    id:rectangleAccountInfo1
                    width: 20 * CDimens.pixelDensity;
                    height: parent.height;
                    Image {
                        id: image1
                        anchors.centerIn: parent
                        width:  16 * CDimens.pixelDensity
                        height: 16 * CDimens.pixelDensity
                        source: "../../images/canon3.png"

                    }
                }

                Rectangle {
                    width: parent.width / 2 -  rectangleAccountInfo1.width - 4;
                    height: parent.height;
                    CLabel {
                        id: customerName
                        anchors.left: parent.left
                        anchors.top: parent.top
                        width: parent.width;
                        height: parent.height/2;
                        text: accountDetailValue.customerName;
                        color: "#FF5D27"
                    }
                    CLabel {
                        id: accountName
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        width: parent.width;
                        height: parent.height/2;
                        text: accountDetailValue.accountName;
                        font.pointSize: CDimens.smallFont;
                    }
                }
                Rectangle {
                    width: parent.width / 2 -  rectangleAccountInfo4.width - 4;
                    height: parent.height;

                    CLabel {
                        id: cLabelLogin
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        width: parent.width;
                        height: parent.height/2;
                        elide: Text.ElideLeft
                        color: "#404040"
                        text: qsTr("登陆或登出","login or logout");
                        horizontalAlignment: Text.AlignRight
                        transformOrigin: Item.Center
                        font.pointSize: (CDimens.smallFont -2);

                    }
                }

                Rectangle {
                    id:rectangleAccountInfo4
                    width: 8 * CDimens.pixelDensity;
                    height: parent.height;
                    Image {
                        id: imageGo
                        anchors.centerIn: parent
                        width:  4 * CDimens.pixelDensity
                        height: 4 * CDimens.pixelDensity
                        source: "../../images/go.png"

                    }
                }
            }

            Rectangle {
                id: rectangle
                anchors.margins: 2
                anchors.bottom: parent.bottom;
                height: Math.max(1, Math.round(0.3 * CDimens.pixelDensity))
                color: settingsPalette.separator
            }

            MouseArea {
                id: rowAccountNameMouseArea
                anchors.fill: parent;
                onClicked: {
                    console.log("account onClicked");
                    stackView.push(Qt.resolvedUrl("../layout/Login.qml"))
                    console.log("start Login page");
                }
            }
        }

        Rectangle {
            id: rectangleAccountFund;
            width: parent.width
            height: parent.height * 0.7
            anchors.top: rectangleAccountName.bottom;
            anchors.topMargin: 2 * CDimens.pixelDensity;
            ListView {
                id: listView
                width: parent.width
                height: parent.height
                visible: true;
                clip: true;
                spacing: 2;
                delegate: accountFundDelegate;
                model: accountFundModel ;
            }
            Component.onCompleted: {
                console.log("accountFundModel onCompleted listView.width =" + listView.width +", rectangleAccountFund.width = "
                            + rectangleAccountFund.width );
            }
        }

//        Rectangle {
//            id: rectangleOther
//            width: parent.width
//            height: parent.height - rectangleAccountName.height - rectangleAccountFund.height;
//            anchors.top: rectangleAccountFund.bottom;
//        }
    }

    Component.onCompleted: {
        console.log("accountDetail onCompleted accountDetail.width =" + accountDetail.width +", parent.width = " + parent.width );
    }

}
