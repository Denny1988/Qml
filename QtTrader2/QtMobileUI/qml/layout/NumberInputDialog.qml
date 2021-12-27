import QtQuick 2.7
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

Dialog {

    property real m_mianWindowWidth : 480;
    property real m_mianWindowHeight : 800;

    property double m_max: 100.0;
    property double m_min: 5.0;
    property double m_step: 5;
    property double m_value: 50.0;

    x: Math.round((m_mianWindowWidth- width) * 0.5)
    y: Math.round(m_mianWindowHeight * 0.2)
    width: m_mianWindowWidth * 0.9;
    height: m_mianWindowHeight * 0.6;
    modal: false
    focus: true
    title: qsTr("数值输入对话框");

    contentItem: NumberInput {
        id: numberInput
        width: mainRect.width * 0.8;
        height: mainRect.height * 0.6;
        mainRectWidth : mainRect.width * 0.6;
        mainRectHeight : mainRect.width * 0.4;

        m_textLabel:""
        m_maximumValue: m_max;
        m_minimumValue: m_min;
        m_value: m_value;
        m_stepSize: m_step;
    }
}
