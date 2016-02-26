// Copyright 2015-2016 the openage authors. See copying.md for legal info.

#pragma once

#include "../gamestate/generator.h"

#include "guisys/link/gui_list_model.h"
#include "guisys/link/gui_item_list_model.h"

namespace openage {

class Generator;

namespace gui {

class GeneratorLink;

template<>
struct Wrap<Generator> {
	typedef GeneratorLink Type;
};

template<>
struct Unwrap<GeneratorLink> {
	typedef Generator Type;
};

class GeneratorLink : public GuiListModel, public GuiItemListModel<GeneratorLink> {
	Q_OBJECT

public:
	GeneratorLink(QObject *parent=nullptr);
	virtual ~GeneratorLink();
};

}} // namespace openage::gui
