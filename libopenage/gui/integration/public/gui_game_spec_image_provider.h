// Copyright 2015-2016 the openage authors. See copying.md for legal info.

#pragma once

#include "../../guisys/public/gui_image_provider.h"

namespace openage {
namespace gui {

class GuiEventQueue;

class GuiGameSpecImageProvider : public GuiImageProvider {
public:
	enum class Type {
		ByFilename,
		ByGraphicId,
		ByTerrainId,
	};

	explicit GuiGameSpecImageProvider(GuiEventQueue *render_updater, Type type);
	~GuiGameSpecImageProvider();
};

}} // namespace openage::gui
