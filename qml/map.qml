import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15

Rectangle {
    width: 800
    height: 600
    
    property double latitude: 0.0
    property double longitude: 0.0
    
    Plugin {
        id: mapPlugin
        name: "osm" // Using OpenStreetMap
    }
    
    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(latitude, longitude)
        zoomLevel: 14
        
        // Enable pan, flick, and pinch-to-zoom
        gesture.enabled: true
        gesture.acceptedGestures: MapGestureArea.PanGesture | MapGestureArea.FlickGesture | MapGestureArea.PinchGesture

        MapQuickItem {
            id: marker
            anchorPoint.x: image.width/2
            anchorPoint.y: image.height
            coordinate: QtPositioning.coordinate(latitude, longitude)
            
            sourceItem: Image {
                id: image
                source: "qrc:/images/marker.png"
                width: 32
                height: 32
            }
        }
    }
    
    // Function to update marker position
    function updatePosition(lat, lon) {
        latitude = lat;
        longitude = lon;
        map.center = QtPositioning.coordinate(lat, lon);
        marker.coordinate = QtPositioning.coordinate(lat, lon);
    }
} 