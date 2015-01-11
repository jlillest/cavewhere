/**************************************************************************
**
**    Copyright (C) 2013 by Philip Schuchardt
**    www.cavewhere.com
**
**************************************************************************/

// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import QtQuick.Controls 1.0


MenuBar {
    property var terrainRenderer; //For taking screenshots
//    property var dataPage; //Should be a DataMainPage
    property Loader mainContentLoader;

    signal openAboutWindow;

    Menu {
        title: "File"

        MenuItem {
            text: "New"
            shortcut: "Ctrl+N"
            onTriggered:{
//                dataPage.resetSideBar(); //Fixes a crash when a new project is loaded
                project.newProject();
            }
        }

        MenuItem {
            text: "Open"
            shortcut: "Ctrl+O"
            onTriggered: {
//                dataPage.resetSideBar() //Fixes a crash when a new project is loaded
                project.load();
            }
        }

        MenuSeparator {}

        MenuItem {
            text: "Save"
            shortcut: "Ctrl+S"
            onTriggered: project.save();
        }

        MenuItem {
            text: "Save As"
            onTriggered: project.saveAs();
        }

        MenuSeparator {}

        MenuItem {
            text: "About Cavewhere"
            onTriggered: openAboutWindow();
        }

        MenuItem {
            text: "Quit"
            shortcut: "Ctrl+Q"
            onTriggered: Qt.quit()
        }
    }

    Menu {
        title: "Debug"

        MenuItem {
            text: "Screenshot"
            onTriggered: {

                var exporter = Qt.createQmlObject('import QtQuick 2.0; import Cavewhere 1.0; ExportRegioonViewerToImageTask {}', fileMenuButton, "");
                exporter.window = quickView;
                exporter.regionViewer = terrainRenderer;

                //Hide all children
                var i;
                for(i in terrainRenderer.children) {
                    terrainRenderer.children[i].visible = false
                }

                exporter.takeScreenshot();
                exporter.destroy();

                //Show all Children
                for(i in terrainRenderer.children) {
                    terrainRenderer.children[i].visible = true
                }
            }
        }

        MenuItem {
            text: "Compute Scraps"
            onTriggered: scrapManager.updateAllScraps()
        }

        MenuItem {
            text: "Reload"
            shortcut: "Ctrl+R"
            onTriggered: {
                //Keep the current address for the current page
                var currentAddress = rootData.pageSelectionModel.currentPageAddress;
                rootData.pageSelectionModel.clear();

                var currentSource = mainContentLoader.source;
                mainContentLoader.source = ""
                qmlReloader.reload();
                mainContentLoader.source = currentSource;

                console.log("Loader status:" + mainContentLoader.status + "Loader:" + Loader.Ready + " " + currentAddress)

                rootData.pageSelectionModel.currentPageAddress = currentAddress;
            }
        }

        MenuItem {
            text: "Scraps Visible"
            checked: regionSceneManager.scraps.visible
            checkable: true
            onTriggered: {
                regionSceneManager.scraps.visible = !regionSceneManager.scraps.visible
                terrainRenderer.update()
            }
        }

        Menu {
            title: "Event Recording"

            MenuItem {
                text: eventRecorderModel.recording ? "Stop" : "Start"
                shortcut: "Ctrl+P"
                onTriggered: {
                    if(eventRecorderModel.recording) {
                        eventRecorderModel.stopRecording()
                    } else {
                        eventRecorderModel.startRecording()
                    }
                }
            }

            MenuItem {
                text: "Play Previous Recording"
                shortcut: "Ctrl+."
                enabled: !eventRecorderModel.recording
                onTriggered: {
                    eventRecorderModel.replayLastRecording();
                }
            }
        }
    }
}
