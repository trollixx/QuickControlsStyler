import QtQuick 2.3
import QtQuick.Controls 1.2

Column {
    spacing: 20

    Slider {
        anchors.horizontalCenter: parent.horizontalCenter
        enabled: enabledCheckBox.checked
        orientation: orientationComboBox.currentValue
        tickmarksEnabled: tickmarksEnabledCheckBox.checked
        value: 0.5
    }

    CheckBox {
        id: enabledCheckBox
        anchors.horizontalCenter: parent.horizontalCenter
        checked: true
        text: qsTr("Enabled")
    }

    ComboBox {
        id: orientationComboBox

        property int currentValue: model.get(currentIndex).value

        anchors.horizontalCenter: parent.horizontalCenter

        model: ListModel {
            ListElement {
                text: "Horizontal"
                value: Qt.Horizontal
            }
            ListElement {
                text: "Vertical"
                value: Qt.Vertical
            }
        }
    }

    CheckBox {
        id: tickmarksEnabledCheckBox
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Show tickmarks"
    }
}
