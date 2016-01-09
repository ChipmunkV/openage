// Copyright 2015-2016 the openage authors. See copying.md for legal info.

#include "../public/gui_application.h"

#include "../private/gui_application_impl.h"

namespace openage {
namespace gui {

GuiApplication::GuiApplication()
	:
	application{GuiApplicationImpl::get()} {
}

GuiApplication::GuiApplication(std::shared_ptr<GuiApplicationImpl> application)
	:
	application{application} {
}

GuiApplication::~GuiApplication() {
}

void GuiApplication::processEvents() {
	this->application->processEvents();
}

}} // namespace openage::gui
