import QtQuick 2.3
import QtQuick.Controls 1.2

Column {
    spacing: 20

    Calendar {
        id: calendar
        enabled: enabledCheckBox.checked
        dayOfWeekFormat: dayOfWeekFormatComboBox.currentValue
        frameVisible: frameVisibleCheckBox.checked
        weekNumbersVisible: weekNumbersVisibleCheckBox.checked
    }

    CheckBox {
        id: enabledCheckBox
        anchors.horizontalCenter: parent.horizontalCenter
        checked: true
        text: qsTr("Enabled")
    }

    ComboBox {
        id: dayOfWeekFormatComboBox

        property int currentValue: model.get(currentIndex).value

        model: ListModel {
            ListElement {
                text: "Locale.ShortFormat"
                value: Locale.ShortFormat
            }
            ListElement {
                text: "Locale.LongFormat"
                value: Locale.LongFormat
            }
            ListElement {
                text: "Locale.NarrowFormat"
                value: Locale.NarrowFormat
            }
        }
        width: calendar.width
    }

    CheckBox {
        id: frameVisibleCheckBox
        anchors.horizontalCenter: parent.horizontalCenter
        checked: true
        text: "Visible frame"
    }

    CheckBox {
        id: weekNumbersVisibleCheckBox
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Show week numbers"
    }
}
