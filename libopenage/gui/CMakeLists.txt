add_sources(libopenage
	integration/private/gui_texture_handle.cpp
)

add_subdirectory("guisys")

add_sources(libopenage
	${QT_SDL_SOURCES}
)
