import QtQuick 2.3
import QtQuick.Controls 1.2

Column {
    spacing: 20

    Button {
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Button")
    }

    Button {
        anchors.horizontalCenter: parent.horizontalCenter
        enabled: false
        text: qsTr("Disabled Button")
    }

    Button {
        anchors.horizontalCenter: parent.horizontalCenter
        checkable: true
        checked: true
        text: qsTr("Checkable Button")
    }

    Button {
        anchors.horizontalCenter: parent.horizontalCenter
        menu: Menu {
            MenuItem { text: qsTr("Item 1") }
            MenuItem { text: qsTr("Item 2") }
            MenuItem { text: qsTr("Item 3") }
        }
        text: qsTr("Menu Button")
    }

}
