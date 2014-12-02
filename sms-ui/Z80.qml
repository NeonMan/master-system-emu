import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1

Item {
    anchors.fill: parent
    ColumnLayout{
        anchors.fill: parent
        /*Disasm and registers*/
        RowLayout{
            Layout.fillHeight: true
            Layout.fillWidth: true

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

            ColumnLayout{
                Label{
                    font.bold: true
                    text: "Registers"
                }

                /*AF registers*/
                RowLayout{
                    Label{
                        text: "A:"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }

                    Label{
                        text: "F:"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }
                }

                /*BC registers*/
                RowLayout{
                    Label{
                        text: "B:"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }

                    Label{
                        text: "C:"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }
                }

                /*DE registers*/
                RowLayout{
                    Label{
                        text: "D:"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }

                    Label{
                        text: "E:"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }
                }

                /*HL registers*/
                RowLayout{
                    Label{
                        text: "H:"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }
                    Label{
                        text: "L:"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }
                }

                /*IX/IY registers*/
                RowLayout{
                    Label{
                        text: "IX:"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }
                    Label{
                        text: "IY:"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }
                }

                /*SP/PC registers*/
                RowLayout{
                    Label{
                        text: "SP:"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }
                    Label{
                        text: "PC:"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }
                }

                Label{
                    font.bold: true
                    text: "Shadow registers"
                }

                /*Prime registers*/
                /*A'*/
                RowLayout{
                    Label{
                        text: "A':"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }

                    Label{
                        text: "F':"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }
                }
                /*BC' registers*/
                RowLayout{
                    Label{
                        text: "B':"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }

                    Label{
                        text: "C':"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }
                }

                /*DE' registers*/
                RowLayout{
                    Label{
                        text: "D':"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }

                    Label{
                        text: "E':"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }
                }

                /*HL' registers*/
                RowLayout{
                    Label{
                        text: "H':"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }

                    Label{
                        text: "L':"
                    }
                    TextField{
                        Layout.maximumWidth: 50
                    }
                }

                Label{
                    font.bold: true
                    text: "Stack"
                }

                RowLayout{
                    Label{
                        text: "Depth"
                    }
                    SpinBox{
                        Layout.maximumWidth: 75
                        value: 10
                    }
                }

                RowLayout{
                    Label{
                        text: "Prefix"
                    }
                    SpinBox{
                        Layout.maximumWidth: 75
                        value: 10
                    }
                }

                TextArea{
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumWidth: 200
                    wrapMode: TextEdit.NoWrap
                }
            }
        }
        /*Buttons*/
        RowLayout{
            Label{
                text: "Emulation"
            }
            Button{
                text: "Pause"
            }
            Button{
                text: "Continue"
            }
            Button{
                text: "Step"
            }
        }
    }
}
