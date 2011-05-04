import QtQuick 1.0

DataTabWidget {
    Text {
        property string label: "Overview"
        property string icon:  "icons/dataOverview.png"
        text: "This is the overview of all the caves"
    }

    Text {
        property string label: "Connections"
        property string icon: "icons/data.png"
        text: "This is the Connection page"
    }

//    Text {
//        property string label: "Calibrations"
//        property string icon: "icons/calibration.png"
//        text: "This is the Team page"
//    }
}
