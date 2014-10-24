import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Column {
    spacing: 20

    SpinBox {
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: horizontalAlignmentComboBox.currentValue
        decimals: decimalsSpinBox.value
        enabled: enabledCheckBox.checked
        prefix: prefixTextField.text
        suffix: suffixTextField.text
    }

    CheckBox {
        id: enabledCheckBox
        anchors.horizontalCenter: parent.horizontalCenter
        checked: true
        text: qsTr("Enabled")
    }

    GridLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        columns: 2
        width: 200

        Label {
            text: qsTr("Decimals")
        }

        SpinBox {
            id: decimalsSpinBox
            maximumValue: 10
            minimumValue: 0
            value: 0
        }

        Label {
            text: qsTr("Horizontal Alignment")
        }

        ComboBox {
            id: horizontalAlignmentComboBox

            /// TODO: Read default value from style
            property int currentValue: model.get(currentIndex).value

            model: ListModel {
                ListElement {
                    text: "AlignLeft"
                    value: Qt.AlignLeft
                }
                ListElement {
                    text: "AlignHCenter"
                    value: Qt.AlignHCenter
                }
                ListElement {
                    text: "AlignRight"
                    value: Qt.AlignRight
                }
            }
        }

        Label {
            text: qsTr("Prefix")
        }

        TextField {
            id: prefixTextField
        }

        Label {
            text: qsTr("Suffix")
        }

        TextField {
            id: suffixTextField
        }
    }
}
