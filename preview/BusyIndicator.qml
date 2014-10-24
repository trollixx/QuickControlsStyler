import QtQuick 2.3
import QtQuick.Controls 1.2

Column {
    spacing: 20

    BusyIndicator {
        anchors.horizontalCenter: parent.horizontalCenter
        running: runningCheckBox.checked
    }

    CheckBox {
        id: runningCheckBox
        anchors.horizontalCenter: parent.horizontalCenter
        checked: true
        text: "Running"
    }
}
