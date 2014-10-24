/****************************************************************************
**
**  Copyright (C) 2014 Oleg Shparber <trollixx+github@gmail.com>
**
**  This file is part of QuickControlsStyler (QCStyler).
**  Web site: https://github.com/trollixx/QuickControlsStyler
**
**  QCStyler is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  QCStyler is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with QCStyler. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

// Use only plain QtQuick here to avoid any dependency on styles

import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0

Rectangle {
    border {
        color: systemPalette.dark
        width: 1
    }
    color: systemPalette.window

    /// TODO: Remove once https://codereview.qt-project.org/97060 gets into Qt
    readonly property bool patchedQt: {
        var patched = false;
        try {
            // If Settings.styleName is readonly, then Qt Quick Controls have my patch
            var styleName = Settings.styleName;
            Settings.styleName = styleName;
        } catch (e) {
            patched = true;
        }
        return patched;
    }

    function updateStyle() {
        if (patchedQt) {
            Settings.style = ""
            __qcStyler.trimCache();
            Settings.style = "file://" + __qcStyler.stylePath + "/" + __qcStyler.styleName
        } else {
            Settings.stylePath = ""
            __qcStyler.trimCache();
            Settings.stylePath = __qcStyler.stylePath
            Settings.styleName = __qcStyler.styleName
        }
    }

    Component.onCompleted: {
        __qcStyler.styleInfoChanged.connect(updateStyle);
        updateStyle();
    }

    SystemPalette {
        id: systemPalette
    }

    Flickable {
        id: flickable
        anchors.fill: parent
        boundsBehavior: Flickable.StopAtBounds
        contentWidth: container.width * container.scale; contentHeight: container.height * container.scale

        Item {
            id: container
            transformOrigin: Item.TopLeft
            width: flickable.width; height: flickable.height

            MouseArea {
                anchors.fill: parent

                readonly property int maxScale: 5
                readonly property int minScale: 1
                readonly property double scaleStep: 0.5

                onWheel: {
                    if (!(wheel.modifiers & Qt.ControlModifier))
                        return;

                    var scale;
                    if (wheel.angleDelta.y > 0)
                        scale = Math.min(maxScale, container.scale + scaleStep);
                    else
                        scale = Math.max(minScale, container.scale - scaleStep);

                    if (container.scale === scale)
                        return;

                    container.scale = scale;
                }
            }

            Loader {
                id: loader
                anchors.centerIn: parent
                source: __qcStyler.currentControl + ".qml"
                width: item ? item.implicitWidth : parent.width
                height: item ? item.implicitHeight : parent.height
            }

            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Preview is not available for the selected control.")
                verticalAlignment: Text.AlignVCenter
                visible: loader.status !== Loader.Ready
                wrapMode: Text.WordWrap
            }
        }
    }
}
