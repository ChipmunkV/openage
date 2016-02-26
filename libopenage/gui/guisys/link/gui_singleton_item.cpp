// Copyright 2015-2016 the openage authors. See copying.md for legal info.

#include "gui_singleton_item.h"

namespace openage {
namespace gui {

GuiSingletonItem::GuiSingletonItem(QObject *parent)
	:
	QObject{parent},
	GuiItemLink{} {
}

GuiSingletonItem::~GuiSingletonItem() {
}

}} // namespace openage::gui
