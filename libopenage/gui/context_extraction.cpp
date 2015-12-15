// Copyright 2014-2015 the openage authors. See copying.md for legal info.
#include "context_extraction.h"

#include <cassert>

#include <QtPlatformHeaders/QGLXNativeContext>

#include "SDL_syswm.h"
#include "GL/glx.h"

// DO NOT INCLUDE ANYTHING HERE, X11 HEADERS BREAK STUFF

namespace openage {
namespace gui {

std::tuple<QVariant, WId> extract_native_context(SDL_Window *window) {
	assert(window);

	GLXContext currentContext;
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	if (SDL_GetWindowWMInfo(window, &wmInfo)) {
		Display *display = wmInfo.info.x11.display;
		assert(display);
		currentContext = glXGetCurrentContext();
		assert(currentContext);
	}

	return std::make_tuple(
			QVariant::fromValue<QGLXNativeContext>(
					QGLXNativeContext(QGLXNativeContext(currentContext, wmInfo.info.x11.display, wmInfo.info.x11.window))),
			wmInfo.info.x11.window);
}

} // namespace gui
} // namespace openage
