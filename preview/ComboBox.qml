import QtQuick 2.3
import QtQuick.Controls 1.2

Column {
    spacing: 20

    ComboBox {
        anchors.horizontalCenter: parent.horizontalCenter
        editable: editableCheckBox.checked
        enabled: enabledCheckBox.checked
        model: Qt.fontFamilies()
    }

    CheckBox {
        id: enabledCheckBox
        //anchors.horizontalCenter: parent.horizontalCenter
        checked: true
        text: qsTr("Enabled")
    }

    CheckBox {
        id: editableCheckBox
        //anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Editable")
    }
}
