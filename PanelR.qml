import QtQuick 2.0

Item {
    height: 200
    width: 200

    //var var_list = server.notSignedInUser.count

    Rectangle {
        anchors.fill : parent
        color: "grey"
//        border.color : "black"
//        border.width : 3
        ListView{
            anchors.fill: parent
            model: NotSignedInUsers

//            Component {
//                id: titleComponent

//                Text {
//                    required property int index;
//                    text : parent.parent.model.NotSignedIn
//                         }
//                }

//            MouseArea {

//                width:200
//                height : 200
//                onClicked : {
//                    //console.log(row)
//                    console.log(index)
//                }

//            }


            delegate: Text {
                text: "°" + model.notSignedIn
                color : "red"
            }
        }

    }
}
