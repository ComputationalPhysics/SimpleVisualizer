import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1
import CompPhys 1.0

Item {
    id: controllerRoot
    
    property real aspectRatio: width/height
    property bool applicationActive: {
        if(Qt.platform.os === "android" || Qt.platform.os === "ios") {
            if(Qt.application.state === Qt.ApplicationActive) {
                return true
            } else {
                return false
            }
        } else {
            return true
        }
    }
    
    width: 1080
    height: 1080

    focus: true
    
    Component.onCompleted: {
        console.log("Controller started.")
        console.log("Platform: " + Qt.platform.os)
    }
    
    MyController {
        id: controller
        anchors.fill: parent
        running: true

        springConstant: slider.value
        
        Timer {
            id: timer
            property real lastTime: Date.now()
            property real lastSampleTime: Date.now()
            running: controller.running && controllerRoot.applicationActive
            repeat: true
            interval: 1
            onTriggered: {
                if(!controller.previousStepCompleted) {
                    return
                }
                
                var currentTime = Date.now()
                var dt = currentTime - lastTime
                dt /= 1000
                controller.step(dt)
                
                var sampleTimeDifference = (currentTime - lastSampleTime)/1000
                lastTime = currentTime
            }
        }
    }

    Slider {
        id: slider
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        minimumValue: 0.1
        maximumValue: 1
        value: 0.5
        stepSize: 0.01
    }
    
    Keys.onPressed: {
        if(event.key === Qt.Key_Escape) {
            Qt.quit()
        }
    }
}

