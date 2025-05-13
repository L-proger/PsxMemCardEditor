import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
//import QtQuick.Dialogs 1.2
import "qrc:/MaterialDesignIcons/MaterialDesignIcons.js" as MaterialDesignIcons




ApplicationWindow {
    visible: true
    width: 640
    height: 480
    id: window

    property var nnn: "NNN"




    FontLoader { id: iconFont; source: "MaterialDesignIcons/MaterialDesignIconsFont.ttf" }
    property var icons: MaterialDesignIcons

    property var memcard;

    function closeMemcard() {
        console.log("Closing memcard...")
        memcard.destroy()
    }

    function openMemcard() {
        console.log("Opening memcard...")
        memcard = app.openMemcard();
        console.log("Opening memcard done")
    }

    Component.onCompleted:  {
        //openMemcard()
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: Home {}
    }

    Dialog {
        id: aboutDialog
        anchors.centerIn: parent
        standardButtons: DialogButtonBox.Ok

           property alias text : textContainer.text

           Text {
               id: textContainer
               text: "ABOUT!"

               anchors.fill: parent

               horizontalAlignment: Qt.AlignLeft
               verticalAlignment: Qt.AlignTop
           }

       // standardButtons: StandardButton.Ok
    }

    Drawer {
        id: drawer
        width: window.width * 0.66
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Settings")
                width: parent.width
                onClicked: {
                    stackView.push("Settings.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("About")
                width: parent.width
                onClicked: function(){ drawer.close(); aboutDialog.open(); }
            }
        }
    }


    header: ToolBar {
        ToolButton {
            text:  stackView.depth > 1 ? icons.menu_left : icons.menu
            font.pixelSize: 20
            onClicked: {
            if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }
        Label{
            text: stackView.currentItem.title
            anchors.centerIn: parent
            font.pixelSize: 20
            elide: Label.ElideRight
        }
    }

//    ListView{
//        model: 3
//        width: 400
//        height: 400

//        delegate:MemcardIcon{
//            width: 92
//            height: 92
//            imageSize: 64
//            fileId: index
//            frameCount: 3
//        }
//    }
}

//Window {

//
//    title: qsTr("Hello World")

//    //AnimatedImage { source: "image://memcard/0" }

////    Grid {
////        rows: 5
////        columns: 3
////        Repeater {
////            model:15
////            delegate:MemcardIcon{
////                width: 92
////                height: 92
////                imageSize: 64
////                fileId: index
////                frameCount: 3
////            }
////        }
////    }



//}
