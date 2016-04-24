// Copyright 2015-2016 the openage authors. See copying.md for legal info.

import QtQuick 2.4
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import yay.sfttech.openage 1.0

Item {
	id: root

	property ActionMode actionMode
	property string playerName
	property int civIndex

	readonly property int topStripSubid: 0
	readonly property int leftRectSubid: 1
	readonly property int rightRectSubid: 2
	readonly property int resBaseSubid: 3

	readonly property string srcPrefix: "image://by-filename/converted/interface/hud"
	readonly property string pad: "0000"
	readonly property string srcSuffix: ".slp.png"
	property string hudImageSource: srcPrefix + (pad + civIndex).slice(-pad.length) + srcSuffix

	width: 1289
	height: 960

	FontMetrics {
		id: fontMetrics
	}

	QtObject {
		id: metrics
		property int unit: fontMetrics.averageCharacterWidth
	}

	Item {
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top

		height: metrics.unit * 1.5 * 2.5

		Image {
			anchors.fill: parent
			source: hudImageSource + "." + root.topStripSubid

			sourceSize.height: parent.height
			fillMode: Image.Tile
		}

		RowLayout {
			anchors.fill: parent
			anchors.leftMargin: metrics.unit
			anchors.rightMargin: metrics.unit
			anchors.verticalCenter: parent.verticalCenter
			spacing: metrics.unit * 0.7

			Row {
				spacing: metrics.unit * 0.7

				Component {
					id: resourceIndicator

					Rectangle {
						property string amount
						property int iconIndex

						width: metrics.unit * 1.5 * 6.5
						height: metrics.unit * 1.5 * 1.7

						color: "#7FFFFFFF"

						Rectangle {
							anchors.fill: parent
							anchors.rightMargin: metrics.unit * 0.3
							anchors.bottomMargin: metrics.unit * 0.3

							color: "black"

							Image {
								sourceSize.height: parent.height

								source: hudImageSource + "." + (root.resBaseSubid + iconIndex)
								fillMode: Image.PreserveAspectFit
							}

							Text {
								anchors.right: parent.right
								anchors.rightMargin: metrics.unit / 2
								anchors.verticalCenter: parent.verticalCenter
								text: amount

								color: "white"
							}
						}
					}
				}

				Repeater {
					model: Resources {
						actionMode: root.actionMode
					}

					delegate: Loader {
						sourceComponent: resourceIndicator

						onLoaded: {
							item.amount = Qt.binding(function() { return display })
							item.iconIndex = Qt.binding(function() { return index })
						}
					}
				}

				Loader {
					sourceComponent: resourceIndicator

					onLoaded: {
						item.amount = "13/42"
						item.iconIndex = 4
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

		Image {
			anchors.left: parent.left
			anchors.top: parent.top
			anchors.bottom: parent.bottom

			width: height * 1.63

			source: hudImageSource + "." + root.leftRectSubid
			fillMode: Image.Stretch
		}

		Image {
			anchors.right: parent.right
			anchors.top: parent.top
			anchors.bottom: parent.bottom

			width: height * 2.01

			source: hudImageSource + "." + root.rightRectSubid
			fillMode: Image.Stretch
		}
	}
}
