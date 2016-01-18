add_sources(libopenage
	integration/private/gui_make_standalone_subtexture.cpp
	integration/private/gui_standalone_subtexture.cpp
	integration/private/gui_texture.cpp
	integration/private/gui_texture_handle.cpp
)

add_subdirectory("guisys")

add_sources(libopenage
	${QT_SDL_SOURCES}
)
