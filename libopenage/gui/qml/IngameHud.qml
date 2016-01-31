import QtQuick 2.4
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import com.openage 1.0

Item {
	id: root

	property ActionMode actionMode
	property string playerName

	width: 1289
	height: 960

	FontMetrics {
		id: fontMetrics
	}

	QtObject {
		id: metrics
		property int unit: fontMetrics.averageCharacterWidth
	}

	Rectangle {
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top

		height: metrics.unit * 1.5 * 2.5

		color: "transparent"
		border.width: 1
		border.color: "white"

		RowLayout {
			anchors.fill: parent
			anchors.leftMargin: metrics.unit
			anchors.rightMargin: metrics.unit
			anchors.topMargin: metrics.unit * 0.5
			anchors.bottomMargin: metrics.unit * 0.5
			spacing: metrics.unit * 0.7

			Row {
				spacing: metrics.unit * 0.7

				Repeater {
					model: [ actionMode.food, actionMode.wood, actionMode.gold, actionMode.stone, "13/42" ]

					Rectangle {
						width: metrics.unit * 1.5 * 6.5
						height: metrics.unit * 1.5 * 1.7

						color: "transparent"
						border.width: 1
						border.color: "white"

						Rectangle {
							width: parent.height
							height: width

							color: "transparent"
							border.width: 1
							border.color: "white"
						}

						Text {
							anchors.right: parent.right
							anchors.rightMargin: metrics.unit / 2
							anchors.verticalCenter: parent.verticalCenter
							text: modelData

							color: "white"
						}
					}
				}
			}

			Item {
				Layout.fillWidth: true
				Layout.minimumWidth: epoch.implicitWidth

				Text {
					id: epoch
					anchors.centerIn: parent
					color: "white"
					text: root.playerName + (actionMode.ability.length ? (" (" + actionMode.ability + ")") : "")
				}
			}

			Repeater {
				model: 5

				Rectangle {
					width: metrics.unit * 1.5 * 5
					height: metrics.unit * 1.5 * 1.5

					color: "transparent"
					border.width: 1
					border.color: "white"
				}
			}
		}
	}

	Rectangle {
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		height: metrics.unit * 1.5 * 16

		color: "transparent"

		border.width: 1
		border.color: "white"

		Rectangle {
			anchors.left: parent.left
			anchors.top: parent.top
			anchors.bottom: parent.bottom

			width: height * 1.63

			color: "transparent"
			border.width: 1
			border.color: "white"
		}

		Rectangle {
			anchors.right: parent.right
			anchors.top: parent.top
			anchors.bottom: parent.bottom

			width: height * 2.01

			color: "transparent"
			border.width: 1
			border.color: "white"
		}
	}
}
