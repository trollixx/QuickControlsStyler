import QtQuick 2.3
import QtQuick.Controls 1.2

Column {
    spacing: 20

    /// TODO: Color chooser for color & secondColor

    ProgressBar {
        anchors.horizontalCenter: parent.horizontalCenter
        enabled: enabledCheckBox.checked
        indeterminate: indeterminateCheckBox.checked
        value: slider.value
    }

    Slider {
        id: slider
        enabled: !indeterminateCheckBox.checked
        value: 0.5
    }

    CheckBox {
        id: enabledCheckBox
        checked: true
        text: qsTr("Enabled")
    }

    CheckBox {
        id: indeterminateCheckBox
        text: qsTr("Indeterminate")
    }
}
