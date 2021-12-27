import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Universal 2.1
import Qt.labs.settings 1.0

import "components"
import "screens"
import "components/dialogs"
import "../images"

AppWindow {

    id: appWindow

    Shortcut {
        sequences: ["Esc", "Back"]
        enabled: stackView.depth > 1
        onActivated: {
            stackView.pop()
            listView.currentIndex = -1
        }
    }


    onBackPressed: {
        if (dialog.visible)
        {
            dialog.close()
        }
        else
        {
            if (stackView.depth > 1)
                stackView.pop()
            else
                Qt.quit()
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: SMainSwipe { }
        enabled: !dialog.visible
    }

    DialogLoader {
        id: dialog
        anchors.fill: parent
    }

    CTooltip {
        id: tooltip
    }

    Component. onCompleted: {
        console. log(" QML Text\' s C++ type - ", stackView);
    }
}
