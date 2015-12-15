import QtQuick 2.0

Item {
	Rectangle {
		id: r

		x: 0
		y: 0
		width: 100
		height: 100
		color: "green"

		MouseArea {
			onClicked: r.color = "blue"
		}
	}
}
