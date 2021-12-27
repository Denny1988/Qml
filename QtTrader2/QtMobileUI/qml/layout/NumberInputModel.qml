import QtQuick 2.7
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import "../components"
import "../../js/Helper.js" as JSHelper

Popup {

    property real m_mianWindowWidth : 480;
    property real m_mianWindowHeight : 800;

    property alias m_textLabel: numberInput.m_textLabel;
    property alias m_max: numberInput.m_maximumValue;
    property alias m_min: numberInput.m_minimumValue;
    property alias m_step: numberInput.m_stepSize;
    property alias m_value: numberInput.m_value;

    id: numberInputBox
    x: 0
    y: m_mianWindowHeight * 0.6
    padding: 0
    //background: Item{}
    //是否使用popup背景
    //dim:false
    width: m_mianWindowWidth
    height: m_mianWindowHeight * 0.4
    modal: true
    focus: true
    closePolicy: Popup.CloseOnPressOutside

    Rectangle {
        id: popRect
        width: numberInputBox.width;
        height: numberInputBox.height;

        NumberInput {
                id: numberInput
                width: parent.width * 0.8;
                height: parent.height * 0.8;
                anchors.centerIn: parent;
                z: 0
                mainRectWidth : width;
                mainRectHeight : height;
        }
    }


    function show( title,  max,  min,  value,  step)
    {
        numberInput.m_textLabel = title;
        numberInput.m_maximumValue = max;
        numberInput.m_minimumValue = min;
        numberInput.m_value = value;
        numberInput.m_stepSize = step;
        numberInputBox.open();
    }

}
