import QtQuick 2.13
import QtQuick.Controls 2.13

Page {
    title: qsTr("Home")


    Column {
        spacing: 10
        Button{
            text: "Open memcard"
            onClicked: {
                window.openMemcard()
            }
        }
        Button{
            text: "Close memcard"
            onClicked: {
                window.closeMemcard()
            }
        }
        Label {
            //window.closeMemcard()
            //anchors.centerIn: parent
           // text: qsTr("Home Screen")
            text: window.memcard.name

        }
    }




}
