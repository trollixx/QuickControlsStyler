import QtQuick 2.3
import QtQuick.Controls 1.2

Column {
    spacing: 20

    CheckBox {
        text: qsTr("CheckBox")
    }

    CheckBox {
        checked: true
        text: qsTr("Checked CheckBox")
    }

    CheckBox {
        checkedState: Qt.PartiallyChecked
        text: qsTr("Partially Checked CheckBox")
    }

    CheckBox {
        pressed: true
        text: qsTr("Pressed CheckBox")
    }

    CheckBox {
        checked: true
        pressed: true
        text: qsTr("Pressed Checked CheckBox")
    }

    CheckBox {
        checkedState: Qt.PartiallyChecked
        pressed: true
        text: qsTr("Pressed Partially Checked CheckBox")
    }

    CheckBox {
        enabled: false
        text: qsTr("Disabled CheckBox")
    }

    CheckBox {
        checked: true
        enabled: false
        text: qsTr("Disabled Checked CheckBox")
    }

    CheckBox {
        checkedState: Qt.PartiallyChecked
        enabled: false
        text: qsTr("Disabled Partially Checked CheckBox")
    }
}
