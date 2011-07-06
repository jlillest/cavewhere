import Qt 4.7
import Cavewhere 1.0

Rectangle {
    property alias notesModel: galleryView.model;

    anchors.fill: parent
    anchors.margins: 3

    Component {
        id: listDelegate

        Item {
            id: container

            property int border: 3
            property alias imageSource: image.source

            width: 200
            height: {
                if(image.status == Image.Ready) {
                    return image.height + imageContainter.border.width + border * 2
                }
                return 200;
            }

            Rectangle {
                id: imageContainter

                border.color: "black"
                border.width: 1
                anchors.fill: parent;
                anchors.margins: container.border

                Image {
                    id: image
                    asynchronous: true
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.margins: 1
                    source: imagePath
                    sourceSize.width: imageContainter.width - imageContainter.border.width
                    //sourceSize.height: imageContainter.height
                    fillMode: Image.PreserveAspectFit
                }

                /**
                  Probably could be allocated and deleted on the fly
                  */
                Image {
                    id: statusImage
                    anchors.centerIn: parent
                    visible: image.status == Image.Loading

                    source: "qrc:icons/loadingSwirl.png"

                    NumberAnimation {
                        running: image.status == Image.Loading
                        target: statusImage;
                        property: "rotation";
                        from: 0
                        to: 360
                        duration: 1500
                        loops: Animation.Infinite
                    }
                }
            }
        }
    }

    Rectangle {
        id: galleryContainer
        anchors.bottom: parent.bottom
        anchors.top:  parent.top
        anchors.right: parent.right
        width: 210
        radius: 7

        color: "#4A4F57"

        ListView {
            id: galleryView

            anchors.fill: parent
            anchors.margins: 4

            delegate: listDelegate
            //model: surveyNoteModel

            clip: true

            orientation: ListView.Vertical

            highlight: Rectangle {
                color: "#418CFF"
                radius:  3
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    console.log("Clicked!");
                    var index = galleryView.indexAt(mouseX, mouseY);
                    galleryView.currentIndex = index;
                }

            }

            onCurrentIndexChanged: {
                console.log(currentItem.imageSource);
                noteArea.imageSource = currentItem.imageSource;
            }
        }


    }

    Button {
        id: addNoteButton

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 2

        iconSource: "qrc:/icons/plus.png"
        text: "Add Notes"

        z:1

    }

    NoteItem {
        id: noteArea
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: galleryContainer.left
        anchors.bottom: parent.bottom

        anchors.margins: 3

        clip: true

        //  onImageSourceChanged: fitToView();
        glWidget: mainGLWidget

    }

}
