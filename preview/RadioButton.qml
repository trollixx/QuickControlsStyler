import QtQuick 2.3
import QtQuick.Controls 1.2

Column {
    spacing: 20

    /// TODO: Color chooser for color & secondColor

    RadioButton {
        anchors.horizontalCenter: parent.horizontalCenter
        enabled: enabledCheckBox.checked
        pressed: pressedCheckBox.checked
        text: qsTr("Radio Button")
    }

    CheckBox {
        id: enabledCheckBox
        checked: true
        text: qsTr("Enabled")
    }

    CheckBox {
        id: pressedCheckBox
        text: qsTr("Pressed")
    }
}
