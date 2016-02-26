// Copyright 2015-2016 the openage authors. See copying.md for legal info.

#pragma once

#include <string>

#include "gui/integration/public/gui_application_with_logger.h"
#include "gui/guisys/public/gui_engine.h"
#include "gui/guisys/public/gui_event_queue.h"
#include "gui/guisys/public/gui_input.h"
#include "gui/guisys/public/gui_renderer.h"
#include "gui/guisys/public/gui_subtree.h"
#include "handlers.h"

#include "gui/integration/public/gui_game_spec_image_provider.h"

namespace openage {
namespace gui {

class GuiSingletonItemsInfo;

/**
 * Smallest gui configuration.
 */
class GuiBasic : public InputHandler, public ResizeHandler, public HudHandler {
public:
	explicit GuiBasic(SDL_Window *window, const std::string &source, GuiSingletonItemsInfo *singleton_items_info=nullptr);
	virtual ~GuiBasic();

	void process_events();

private:
	virtual bool on_resize(coord::window new_size) override;
	virtual bool on_input(SDL_Event *event) override;
	virtual bool on_drawhud() override;

	GuiApplicationWithLogger application;
	GuiEventQueue render_updater;
	GuiRenderer renderer;
	GuiEventQueue game_logic_updater;
	GuiGameSpecImageProvider image_provider_by_filename;
	GuiGameSpecImageProvider image_provider_by_graphic_id;
	GuiGameSpecImageProvider image_provider_by_terrain_id;
	GuiEngine engine;
	GuiSubtree subtree;
	GuiInput input;
};

}} // namespace openage::gui
