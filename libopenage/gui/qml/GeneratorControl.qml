// Copyright 2015-2016 the openage authors. See copying.md for legal info.

import QtQuick 2.4
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import com.openage 1.0

Item {
	id: root

	property GeneratorParameters generatorParameters
	property GameSpec gameSpec
	property GameMain game

	implicitWidth: elements.width
	implicitHeight: elements.height

	Column {
		id: elements

		spacing: genActions.spacing

		ColumnLayout {
			id: genActions

			GameCreator {
				id: gameCreator

				game: root.game
				gameSpec: root.gameSpec
				generatorParameters: root.generatorParameters
			}

			ButtonFlat {
				Layout.fillWidth: true

				text: "generate_game"
				onClicked: gameCreator.activate()
			}

			ButtonFlat {
				Layout.fillWidth: true

				text: "end_game"
				onClicked: {
					game.clear()
					gameCreator.clearErrors()
				}
			}

			ButtonFlat {
				Layout.fillWidth: true

				text: "reload_assets"
				onClicked: gameSpec.invalidate()
			}
		}

		Text {
			color: gameCreator.errorString ? "red" : "white"

			text: if (gameCreator.errorString)
					"Error: " + gameCreator.errorString
				else
					switch (root.game.state) {
						case GameMain.Null:
							"Not running"
							break

						case GameMain.Running:
							"Running"
							break

						default:
							"Unknown"
							break
					}
		}
	}
}
