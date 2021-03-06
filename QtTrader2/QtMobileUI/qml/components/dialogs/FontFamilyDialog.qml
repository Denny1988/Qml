/****************************************************************************
**
** Copyright (C) 2013-2015 Oleg Yadrov
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
** http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
****************************************************************************/

import QtQuick 2.0
import QtGraphicalEffects 1.0
import ".."

BaseDialog {
    id: fontFamilyDialog

    property alias title: titleLabel.text
    property alias model: fontsList.model
    property alias currentIndex: fontsList.currentIndex

    function initialize(parameters) {
        for (var attr in parameters) {
            if (attr === "currentIndex")
                continue;

            fontFamilyDialog[attr] = parameters[attr];
        }

        fontsList.currentIndex = parameters.currentIndex;
        fontsList.positionViewAtIndex(currentIndex, ListView.Contain)
    }

    DropShadow {
        anchors.fill: contentBackground
        radius: 5 * CDimens.pixelDensity
        color: palette.dialogShadow
        transparentBorder: true
        fast: true
        source: contentBackground
    }

    Rectangle {
        id: contentBackground
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        color: palette.dialogBackground
    }

    Item {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent

        Rectangle {
            id: header

            height: 22 * CDimens.pixelDensity
            anchors.left: parent.left
            anchors.right: parent.right
            color: palette.toolBarBackground

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                height: Math.max(1, Math.round(0.8 * CDimens.pixelDensity))
                color: palette.toolBarStripe
            }

            CLabel {
                id: titleLabel
                anchors.fill: parent
                anchors.leftMargin: 5 * CDimens.pixelDensity
                font.pixelSize: 10 * CDimens.pixelDensity
            }
        }

        ListView {
            id: fontsList

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: header.bottom
            anchors.bottom: parent.bottom
            boundsBehavior: Flickable.StopAtBounds
            clip: true
            delegate: CCheckBox {
                text: model.name
                checked: index === fontsList.currentIndex
                onClicked: fontFamilyDialog.process(index)
                label.font.family: model.name
            }
        }

        CScrollBar {
            flickableItem: fontsList
        }
    }
}
