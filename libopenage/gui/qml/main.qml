// Copyright 2015-2016 the openage authors. See copying.md for legal info.

import QtQuick 2.4
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.3

import yay.sfttech.livereload 1.0
import yay.sfttech.openage 1.0

Item {
	id: root

	GameSpec {
		id: specObj

		/**
		 * States: Null, Loading, Ready
		 */

		// Start loading assets immediately
		active: true

		assetManager: amObj

		LR.tag: "spec"
	}

	AssetManager {
		id: amObj

		dataDir: MainArgs.dataDir

		LR.tag: "am"
	}

	GameMain {
		id: gameObj

		/**
		 * States: Null, Running
		 */

		engine: Engine

		LR.tag: "game"
	}

	FontMetrics {
		id: fontMetrics
	}

	ColumnLayout {
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.topMargin: fontMetrics.averageCharacterWidth * 4
		anchors.leftMargin: fontMetrics.averageCharacterWidth * 2

		spacing: fontMetrics.averageCharacterWidth * 2

		GeneratorParameters {
			id: genParamsObj

			LR.tag: "gen"
		}

		GeneratorParametersConfiguration {
			generatorParameters: genParamsObj
		}

		GeneratorControl {
			generatorParameters: genParamsObj
			gameSpec: specObj
			game: gameObj
		}
	}
}
