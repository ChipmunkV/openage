// Copyright 2014-2015 the openage authors. See copying.md for legal info.

#ifndef OPENAGE_GUI_CONTEXT_EXTRACTION_H_
#define OPENAGE_GUI_CONTEXT_EXTRACTION_H_

#include <tuple>

#include <QWindow>
#include <QVariant>

struct SDL_Window;

namespace openage {
namespace gui {

std::tuple<QVariant, WId> extract_native_context(SDL_Window *window);

} // namespace gui
} // namespace openage

#endif /* OPENAGE_GUI_CONTEXT_EXTRACTION_H_ */
