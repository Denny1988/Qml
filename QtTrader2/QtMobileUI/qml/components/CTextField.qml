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
import "."
Item {
    id: cTextField

    implicitHeight: 14 * CDimens.pixelDensity

    anchors.left: parent.left
    anchors.right: parent.right

    property alias text: textInput.text
    property alias placeholder: placeholderTextInput.text
    property alias validator: textInput.validator

    signal valueChanged(var value);

    Rectangle {
        anchors.fill: parent
        color: settingsPalette.textFieldBackground
        border.width: Math.max(1, Math.round(0.3 * CDimens.pixelDensity))
        border.color: settingsPalette.textFieldBorder
    }

    TextInput {
        id: placeholderTextInput
        anchors.fill: parent
        anchors.leftMargin: 4 * CDimens.pixelDensity
        anchors.rightMargin: 4 * CDimens.pixelDensity
        font.family: "Roboto"
        font.pixelSize: 6 * CDimens.pixelDensity
        verticalAlignment: Text.AlignVCenter
        color: settingsPalette.textFieldPlaceholder
        readOnly: true
        visible: textInput.length === 0
    }

    TextInput {
        id: textInput
        anchors.fill: parent
        anchors.leftMargin: 4 * CDimens.pixelDensity
        anchors.rightMargin: 4 * CDimens.pixelDensity
        inputMethodHints: Qt.ImhNoPredictiveText
        activeFocusOnPress: false
        font.family: "Roboto"
        font.pixelSize: 6 * CDimens.pixelDensity
        verticalAlignment: Text.AlignVCenter
        color: settingsPalette.label
        clip: true



        MouseArea {
            anchors.fill: parent
            onPressed: {
                textInput.cursorPosition = textInput.positionAt(mouse.x, mouse.y)

                if (!Qt.inputMethod.visible) {
                    textInput.forceActiveFocus(Qt.MouseFocusReason)
                    Qt.inputMethod.show()
                }
            }
        }
    }
}
