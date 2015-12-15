// Copyright 2014-2015 the openage authors. See copying.md for legal info.

#ifndef OPENAGE_GUI_CONTEXT_ADOPTION_H_
#define OPENAGE_GUI_CONTEXT_ADOPTION_H_

struct SDL_Window;

namespace openage {
namespace gui {

/**
 * Passes the native graphic context to Qt.
 */
class ContextAdoption {
public:
	explicit ContextAdoption(SDL_Window *window);

	void render();
};

} // namespace gui
} // namespace openage

#endif
