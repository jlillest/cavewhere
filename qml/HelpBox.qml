/**************************************************************************
**
**    Copyright (C) 2013 by Philip Schuchardt
**    www.cavewhere.com
**
**************************************************************************/

import QtQuick 2.0

ShadowRectangle {
    id: helpBox

    property alias text: textId.text
    property alias font: textId.font

    width: textId.width + 10
    height: textId.height + 10

    anchors.bottom: parent.bottom
    anchors.bottomMargin: 20
    anchors.horizontalCenter: parent.horizontalCenter

    color: "#85c1f4"
    radius: 5


    Text {
        id: textId
        anchors.centerIn: parent

        font.pointSize: 14
        horizontalAlignment: Text.AlignHCenter
    }

    states: [
        State {
            name: "VISIBLE"
            when: visible
        }
    ]

    transitions: [
        Transition {
            to: "VISIBLE"
            NumberAnimation {
                target: helpBox;
                property: "opacity";
                from: 0.0
                to: 1.0;
                duration: 200
            }
        }

        //FIXME: This transition doesn't work (check in beta if it works)
//        Transition {
//            from: "VISIBLE"
//            SequentialAnimation {
//                PropertyAction {
//                    target: helpBox
//                    property: "visible"
//                    value: true
//                }

//                NumberAnimation {
//                    target: helpBox;
//                    property: "opacity";
//                    from: 1.0
//                    to: 0.0;
//                    duration: 200
//                }

//                PropertyAction {
//                    target: helpBox
//                    property: "visible"
//                    value: false
//                }
//            }
//        }
    ]

}
