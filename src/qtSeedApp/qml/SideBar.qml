import QtQuick 2.0
import QtQuick.Layouts 1.3

Item {
    anchors.fill: parent

    Item {
        id: sideBarImage
        width: parent.width * 0.9
        height: parent.height * 0.2
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10

        Image {
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: "images/werfenIcon.png"
        }
    }

    Rectangle {
        id: upperSideBarContent
        width: sideBarImage.width * 0.9
        height: width
        anchors.top: sideBarImage.bottom
        anchors.left: sideBarImage.left
        anchors.topMargin: height / 3
        anchors.leftMargin: sideBarImage.height * 0.15
        color: "transparent"

        ColumnLayout {
            width: parent.width
            anchors.top: parent.top
            spacing: 3
            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: upperSideBarContent.height / 4
                Text {
                    text: "Home"
                    color: "white"
                    font.family: "PT mono"
                    font.pixelSize: 16
                    font.bold: (homeMouse.containsMouse) ? true : false
                }
                MouseArea {
                    id: homeMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: dashboardLogic.buttonNavLogout()
                }
            }
//            Item {
//                Layout.fillWidth: true
//                Layout.preferredHeight: upperSideBarContent.height / 4
//                Text {
//                    text: "Patient List"
//                    color: "white"
//                    font.family: "PT mono"
//                    font.pixelSize: 16
//                }
//            }
        }
    }
    Rectangle {
        id: lowerSideBarContent
        width: sideBarImage.width * 0.9
        height: sideBarImage.height * 0.6
        anchors.bottom: parent.bottom
        anchors.bottomMargin: height / 3
        anchors.left: sideBarImage.left
        anchors.leftMargin: sideBarImage.height * 0.15

        color: "transparent"

        ColumnLayout {
            width: parent.width
            anchors.top: parent.top
            spacing: 3
            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: lowerSideBarContent.height / 2
                Text {
                    text: "Configuration"
                    color: "white"
                    font.family: "PT mono"
                    font.pixelSize: 16
                    font.bold: (logoutConf.containsMouse) ? true : false
                    MouseArea {
                        id: logoutConf
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: dashboardLogic.buttonNavConfiguration()
                    }
                }
            }
            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: lowerSideBarContent.height / 2
                Text {
                    text: "Logout"
                    color: "white"
                    font.family: "PT mono"
                    font.pixelSize: 16
                    font.bold: (logoutMouse.containsMouse) ? true : false
                }
                MouseArea {
                    id: logoutMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: dashboardLogic.buttonNavLogout()
                }
            }
        }
    }
}
