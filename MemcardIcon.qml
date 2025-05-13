import QtQuick 2.0

Item {
    id:base
    property int fileId;
    property int frameCount;
    property int frameId: 0;

    property int imageSize: 16;

    Rectangle {
        color: "black"
        width: base.width
        height: base.height


        Image {
            id:imageView
            width: imageSize
            height: imageSize
            anchors.centerIn:parent
            source: "image://memcard/0"
            cache: false
        }
    }



    Timer{
        running: true
        interval: 300
        repeat: true

        onTriggered: {
            frameId = frameId + 1;
            if(frameId >= frameCount){
                frameId = 0;
            }

            imageView.source = "image://memcard/" + fileId + "/" + frameId

            //console.log("tick: " + fileId  + "/" + frameId)
        }
    }
}
