add_sources(libopenage
	assetmanager_link.cpp
	category_contents_list_model.cpp
	engine_link.cpp
	game_control_link.cpp
	game_creator.cpp
	game_main_link.cpp
	game_spec_link.cpp
	generator_link.cpp
	main_args_link.cpp
)

add_sources(libopenage
	integration/public/gui_application_with_logger.cpp
	integration/public/gui_game_spec_image_provider.cpp
)

add_sources(libopenage
	integration/private/gui_filled_texture_handles.cpp
	integration/private/gui_game_spec_image_provider_by_filename_impl.cpp
	integration/private/gui_game_spec_image_provider_by_graphic_id_impl.cpp
	integration/private/gui_game_spec_image_provider_by_id_impl.cpp
	integration/private/gui_game_spec_image_provider_by_terrain_id_impl.cpp
	integration/private/gui_game_spec_image_provider_impl.cpp
	integration/private/gui_image_provider_link.cpp
	integration/private/gui_log.cpp
	integration/private/gui_make_standalone_subtexture.cpp
	integration/private/gui_standalone_subtexture.cpp
	integration/private/gui_texture.cpp
	integration/private/gui_texture_factory.cpp
	integration/private/gui_texture_handle.cpp
)

add_subdirectory("guisys")

add_sources(libopenage
	${QT_SDL_SOURCES}
)
