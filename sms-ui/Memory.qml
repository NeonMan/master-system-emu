import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import emu.rom.Test 1.0 as RomTest

Item {
    anchors.fill: parent

    ColumnLayout{
        //Layout.fillHeight: true
        //Layout.fillWidth: true
        anchors.fill: parent

        TextArea{
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop

            readOnly: true
            wrapMode: TextEdit.NoWrap

            font.family: "Courier"
            text: "z80  ROM Addr 00 01 02 03 04 05 06 07  08 09 0A 0B 0D 0E 0F                    \n" +
                  "0000 00000000 12 34 56 78 9A BC DE F0  12 34 56 78 9A BC DE    A74D8JHNGFPDCTGR\n" +
                  "0010 00000010 12 34 56 78 9A BC DE F0  12 34 56 78 9A BC DE    A74D8JHNGFPDCTGR\n" +
                  "0020 00000020 12 34 56 78 9A BC DE F0  12 34 56 78 9A BC DE    A74D8JHNGFPDCTGR\n"
        }

        RowLayout{
            Label{
                text: "Address mode:"
            }

            ExclusiveGroup{ id: addressModeGroup }
            RadioButton{
                text: "z80"
                exclusiveGroup: addressModeGroup
            }
            RadioButton{
                text: "ROM"
                checked: true
                exclusiveGroup: addressModeGroup
            }

            Label{
                text: "Slot 0: "
            }
            TextField{
                Layout.maximumWidth: 50
                text: "0x00"
            }

            Label{
                text: "Slot 1: "
            }
            TextField{
                Layout.maximumWidth: 50
                text: "0x01"
            }

            Label{
                text: "Slot 2: "
            }
            TextField{
                Layout.maximumWidth: 50
                text: "0x02"
            }
        }
    }
}
