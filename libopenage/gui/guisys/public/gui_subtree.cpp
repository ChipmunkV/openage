// Copyright 2015-2016 the openage authors. See copying.md for legal info.

#include "../public/gui_subtree.h"

#include <QString>

#include "../private/gui_subtree_impl.h"

namespace openage {
namespace gui {

GuiSubtree::GuiSubtree(GuiRenderer *renderer, GuiEventQueue *game_logic_updater, GuiEngine *engine, const std::string &source)
	:
	impl{std::make_unique<GuiSubtreeImpl>(renderer, game_logic_updater, engine, QString::fromStdString(source))} {
}

GuiSubtree::~GuiSubtree() {
}

}} // namespace openage::gui
