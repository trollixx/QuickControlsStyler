import QtQuick 2.3
import QtQuick.Controls 1.2

Column {
    spacing: 20

    Switch {
        anchors.horizontalCenter: parent.horizontalCenter
        enabled: enabledCheckBox.checked
    }

    CheckBox {
        id: enabledCheckBox
        checked: true
        text: qsTr("Enabled")
    }
}
