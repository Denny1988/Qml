import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import Qt.labs.platform 1.0

import "layout"
import "screens"
import "components"

BlankScreen {

    id: mainSwipeScreen;

    SwipeView {
        id: swipeView
        anchors.fill: parent;
        anchors.top: parent.top
        anchors.bottom: tabBar.top
        currentIndex: tabBar.currentIndex

        QuotationsPage {

        }

        TradingPage {

        }

        OpenPositionsPage {

        }

        AccountPage {

        }

        OtherMorePage {

        }

        onCurrentIndexChanged : {
            console. log("swipeView.onCurrentIndexChanged Index = " + swipeView.currentIndex);
        }
    }

    CTabBar {
        id: tabBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        currentIndex: swipeView.currentIndex
        CTabButton {
            text: qsTr("报价", "Quotation")
        }
        CTabButton {
            text: qsTr("交易", "Trading")
        }
        CTabButton {
            text: qsTr("持仓", "Orders")
        }
        CTabButton {
            text: qsTr("账户", "Account")
        }
        CTabButton {
            text: qsTr("更多", "OtherMore")
        }

        Component.onCompleted: {
            console. log(" QML Text\' s C++ type - ", stackView);
        }
        onCurrentIndexChanged : {
            console. log("QQ2.TabBar.onCurrentIndexChanged ", swipeView.currentIndex);
        }
    }


}
