// Copyright 2015-2016 the openage authors. See copying.md for legal info.

import QtQuick 2.4
import QtGraphicalEffects 1.0

Item {
	id: root

	FontMetrics {
		id: fontMetrics
	}

	/*
	 * Average character width that the paper will adapt to (by scaling its texture).
	 */
	property int metricsUnit: fontMetrics.averageCharacterWidth

	/*
	 * A grayscale paper texture.
	 */
	property url paperTextureSource: "image://by-filename/textures/paper.png.0"

	/*
	 * This texture is tiled along the item edges, bottom faces the inside.
	 * So, alpha should be zero for the pixels where to keep the paper, solid color - where it's torn off.
	 * TODO: do the inverse, because the solid background color is limiting.
	 */
	property url edgeHmapSource: "image://by-filename/textures/torn_paper_edge.png.0"

	property color paperColor: "#e2c19f"

	/*
	 * Gets rid of artifacts caused by alpha blending of the torn edges.
	 * See 'Layer Opacity vs Item Opacity'.
	 */
	layer.enabled: true

	/*
	 * Changing metrics.unit (currently bound to the font) scales all textures.
	 * So the details on the textures stay exactly the same relatively to metricsUnit.
	 */
	property real scaling: 8. / metricsUnit

	transform: Scale { xScale: 1. / root.scaling; yScale: 1. / root.scaling}

	Item {
		id: paper

		anchors.left: parent.left
		anchors.top: parent.top

		/*
		 * Do +.5 to avoid being less then parent due to truncation.
		 */
		width: (parent.width + .5) * root.scaling
		height: (parent.height + .5) * root.scaling

		Rectangle {
			id: paperColoredRect

			anchors.fill: parent

			color: root.paperColor
		}

		Image {
			id: paperTexture

			anchors.fill: parent

			source: root.paperTextureSource
			fillMode: Image.Tile
			visible: false
		}

		Blend {
			id: texturedColoredPaper

			anchors.fill: parent

			source: paperTexture
			foregroundSource: paperColoredRect
			mode: "hardLight"

			layer.enabled: true
			layer.wrapMode: ShaderEffectSource.Repeat
			layer.samplerName: "paperSource"
			layer.effect: ShaderEffect {
				property var edgeHeighSource: edgeHeightmap;
				property real hightmapToImageScale: texturedColoredPaper.width / edgeHeightmap.width

				property var burnTextureSource: paperTexture;

				fragmentShader: "
					uniform lowp sampler2D paperSource;
					varying highp vec2 qt_TexCoord0;

					const float exposure = .5;
					const float times = 3.;

					vec3 burntMidtones(vec3 src, float exposure, float times) {
						float factor = 1. + exposure * (.33);
						return pow(src.rgb, vec3(factor * times));
					}

					void main() {
						vec4 bkg = texture2D(paperSource, qt_TexCoord0);
						vec3 burnt = burntMidtones(bkg.rgb, exposure, times);

						float burnGradient = pow(pow((qt_TexCoord0.x - .5), 2.) + pow((qt_TexCoord0.y - 1.), 2.), 1.) * .9;

						gl_FragColor = vec4(mix(bkg.rgb, burnt, vec3(burnGradient)), 1.);
					}
				"
			}
		}

		readonly property url hmapSource: root.edgeHmapSource

		Image {
			id: edgeHeightmap
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.top: parent.top

			source: parent.hmapSource
			fillMode: Image.Tile
		}

		Image {
			id: leftRdgeHeightmap
			anchors.left: paper.left
			anchors.leftMargin: paper.height
			anchors.top: paper.top

			width: paper.height

			source: parent.hmapSource
			fillMode: Image.Tile

			transform: Rotation {
				origin.y: paper.height
				angle: -90
			}
		}

		Image {
			id: rightRdgeHeightmap
			anchors.right: paper.right
			anchors.rightMargin: paper.height
			anchors.top: paper.top

			width: paper.height

			source: parent.hmapSource
			fillMode: Image.Tile

			transform: Rotation {
				origin.y: paper.height
				origin.x: paper.height
				angle: 90
			}

			layer.enabled: true
			layer.wrapMode: ShaderEffectSource.Repeat
			layer.samplerName: "edgeSource"
			layer.effect: ShaderEffect {
				fragmentShader: "
					uniform lowp sampler2D edgeSource;
					varying highp vec2 qt_TexCoord0;

					void main() {
						gl_FragColor = texture2D(edgeSource, qt_TexCoord0 + vec2(0.7, 0.));
					}
				"
			}
		}
	}
}
