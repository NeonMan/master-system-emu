import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    visible: true
    width: 800
    height: 480
    title: qsTr("Hello World")

    // --- Actions and objects used by the GUI ---

    FileDialog {
        id: fileDialog
        nameFilters: [ "ROM files (*.sms *.bin)", "All files (*.*)" ]
        onAccepted: console.log("Selected file: " + fileUrl);
    }

    Action{
        id: actionOpen
        text: "&Open"
        onTriggered: fileDialog.open()
        tooltip: "Load a ROM"
    }

    Action{
        id: actionExit
        text: "&Exit"
        onTriggered: Qt.quit();
        tooltip: "Exit application"
    }

    // --- The GUI itself, no biz code below this line ---

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem { action: actionOpen }
            MenuItem { action: actionExit }
        }
    }

    TabView{
        anchors.fill: parent
        Tab{
            title: "Memory"
            Memory{}
        }
        Tab{
            title: "z80"
            Z80{}
        }
    }
}
