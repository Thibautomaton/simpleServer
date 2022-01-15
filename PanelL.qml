import QtQuick 2.0

Item {
    width : 200
    height: 200

    Rectangle {
        anchors.fill: parent
        //border.color: "black"
        //border.width : 3
        color: "grey"
        ListView {
            anchors.fill: parent
            spacing:10
            model : ConnectedUsers
            delegate : Text {
                text : "° " + model.signedIn
                color : "lightgreen"
            }
        }
    }


}
