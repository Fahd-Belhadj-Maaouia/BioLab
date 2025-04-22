import QtQuick 2.15
import QtQuick.Window 2.15
import QtLocation 5.15
import QtPositioning 5.15

Item {
    id: mapView
    width: 400
    height: 300

    property alias center: map.center
    property alias zoomLevel: map.zoomLevel

    Plugin {
        id: mapPlugin
        name: "osm"
        PluginParameter {
            name: "osm.mapping.providersrepository.disabled"
            value: "true"
        }
        PluginParameter {
            name: "osm.mapping.providersrepository.address"
            value: "http://maps-redirect.qt.io/osm/5.8/"
        }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(36.8997, 10.1897) // Default to Tunis
        zoomLevel: 14

        MapQuickItem {
            id: marker
            anchorPoint.x: image.width/2
            anchorPoint.y: image.height
            coordinate: map.center

            sourceItem: Image {
                id: image
                source: "qrc:/icons/marker.png"
                width: 30
                height: 30
            }
        }
    }

    function setLocation(latitude, longitude) {
        map.center = QtPositioning.coordinate(latitude, longitude)
    }

    function searchLocation(address) {
        var geocodeModel = Qt.createQmlObject('import QtLocation 5.15; GeocodeModel { plugin: mapPlugin }', mapView)
        geocodeModel.query = address
        geocodeModel.update()
        
        geocodeModel.onStatusChanged = function() {
            if (geocodeModel.status === GeocodeModel.Ready) {
                if (geocodeModel.count > 0) {
                    var location = geocodeModel.get(0).coordinate
                    map.center = location
                    map.zoomLevel = 14
                }
            }
        }
    }
} 