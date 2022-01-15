import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Rectangle {
        anchors.fill: parent

        color: "grey"

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.margins: 50
            spacing : 100

            Column {
                spacing: 25


                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 50

                //we receive the ip adresses as a Qlist<QString>, ListView can display

                Button {
                    id: startButton
                    signal launchServer(string ipport);
                    text: "launch server"
                    font.pointSize: 25
                    font.family: "Times New Roman"
                    padding: 5

                    onClicked: {
                        server.toggleStart()
                    }
                }

                Text{
                    id:loggedUsers
                    text: "Usernames :"
                    color: "lightgreen"
                }

                PanelL {

                }
            }

            Column {
                spacing: 45

                Rectangle {
                    id: connexionLight

                    width: 25
                    height: 25
                    radius: 100
                    opacity: 0.3
                    anchors {

                        margins: 50
                    }

                    color: "lightgreen"
                }

                Text {
                    id: unacceptedConnexions
                    text : "Unaccepted Incomming connexions :"
                    color: "red"
                }

                PanelR {

                }
            }
        }
    }

    Connections {
        target: server
        function onServerStarted() {
            connexionLight.opacity = 1;
        }
//        function onServerStopped() {
//            connexionLight.opacity = 0.3;
//        }

    }



}
