// Copyright 2015-2016 the openage authors. See copying.md for legal info.

#include "gui_application_impl.h"

#include <cassert>

namespace qtsdl {

std::weak_ptr<GuiApplicationImpl> GuiApplicationImpl::instance;

std::shared_ptr<GuiApplicationImpl> GuiApplicationImpl::get() {
	std::shared_ptr<GuiApplicationImpl> candidate = GuiApplicationImpl::instance.lock();

	assert(!candidate || std::this_thread::get_id() == candidate->owner);

	return candidate ? candidate : std::shared_ptr<GuiApplicationImpl>{new GuiApplicationImpl};
}

GuiApplicationImpl::~GuiApplicationImpl() {
	assert(std::this_thread::get_id() == this->owner);
}

void GuiApplicationImpl::processEvents() {
	assert(std::this_thread::get_id() == this->owner);
	this->app.processEvents();
}

namespace {
	int argc = 1;
	char arg[] = "qtsdl";
	char *argv = &arg[0];
}

GuiApplicationImpl::GuiApplicationImpl()
	:
#ifndef NDEBUG
	owner{std::this_thread::get_id()},
#endif
	app{argc, &argv}
{
#ifdef Q_OS_UNIX
		// TODO: the game should set a correct locale from the start.
		auto lc_all = qgetenv("LC_ALL");
		if (lc_all != "C" && lc_all != "POSIX")
			qWarning("The GUI will apply your non-C locale. But that can break the game logic. Use LC_ALL=C as workaround.");
#endif
}

} // namespace qtsdl
