// Copyright 2015-2016 the openage authors. See copying.md for legal info.

#include <algorithm>
#include <array>
#include <stdexcept>

#include "../log/log.h"
#include "../engine.h"
#include "input_manager.h"

namespace openage {
namespace input {

InputManager::InputManager()
	: relative_mode{false} {
}

void InputManager::initialize() {
	// TODO not hardcoded anymore
	this->set_bind("`", "TOGGLE_CONSOLE");
	this->set_bind("Return", "START_GAME");
	this->set_bind("Escape", "STOP_GAME");
	this->set_bind("F1", "TOGGLE_HUD");
	this->set_bind("F2", "SCREENSHOT");
	this->set_bind("F3", "TOGGLE_DEBUG_OVERLAY");
	this->set_bind("F4", "TOGGLE_DEBUG_GRID");
	this->set_bind("F5", "QUICK_SAVE");
	this->set_bind("F9", "QUICK_LOAD");
	this->set_bind("Space", "TOGGLE_BLENDING");
	this->set_bind("F12", "TOGGLE_PROFILER");
	this->set_bind("m", "TOGGLE_CONSTRUCT_MODE");
	this->set_bind("p", "TOGGLE_UNIT_DEBUG");
	this->set_bind("t", "TRAIN_OBJECT");
	this->set_bind("y", "ENABLE_BUILDING_PLACEMENT");
	this->set_bind("Ctrl z", "DISABLE_SET_ABILITY");
	this->set_bind("Ctrl x", "SET_ABILITY_MOVE");
	this->set_bind("Ctrl c", "SET_ABILITY_GATHER");
	this->set_bind("g", "SET_ABILITY_GARRISON");
	this->set_bind("v", "SPAWN_VILLAGER");
	this->set_bind("Delete", "KILL_UNIT");
	this->set_bind("q", "BUILDING_1");
	this->set_bind("w", "BUILDING_2");
	this->set_bind("e", "BUILDING_3");
	this->set_bind("r", "BUILDING_4");
	this->set_bind("Ctrl q", "BUILDING_5");
	this->set_bind("Ctrl w", "BUILDING_6");
	this->set_bind("Ctrl e", "BUILDING_7");
	this->set_bind("Ctrl r", "BUILDING_8");
	this->set_bind("z", "BUILDING_TOWN_CENTER");
	this->set_bind("1", "SWITCH_TO_PLAYER_1");
	this->set_bind("2", "SWITCH_TO_PLAYER_2");
	this->set_bind("3", "SWITCH_TO_PLAYER_3");
	this->set_bind("4", "SWITCH_TO_PLAYER_4");
	this->set_bind("5", "SWITCH_TO_PLAYER_5");
	this->set_bind("6", "SWITCH_TO_PLAYER_6");
	this->set_bind("7", "SWITCH_TO_PLAYER_7");
	this->set_bind("8", "SWITCH_TO_PLAYER_8");
	this->set_bind("Up", "UP_ARROW");
	this->set_bind("Down", "DOWN_ARROW");
	this->set_bind("Left", "LEFT_ARROW");
	this->set_bind("Right", "RIGHT_ARROW");
	this->set_bind("MOUSE 1", "PAINT_TERRAIN");
	this->set_bind("WHEEL 1", "FORWARD");
	this->set_bind("WHEEL -1", "BACK");
	this->set_bind("MOUSE 1", "SELECT");
	this->set_bind("Shift MOUSE 1", "INCREASE_SELECTION");
	this->set_bind("MOUSE 1", "BUILD");
	this->set_bind("Shift MOUSE 1", "KEEP_BUILDING");
	this->set_bind("MOUSE 3", "ORDER_SELECT");
}

std::string InputManager::get_bind(const std::string &action_str) {
	ActionManager &action_manager = Engine::get().get_action_manager();

	for (auto &it : this->keys) {
		if (action_manager.is(action_str, it.second)) {
			switch (it.first.cc.eclass) {
			case event_class::MOUSE_BUTTON :
				return this->mouse_bind_to_string(it.first);
			case event_class::MOUSE_WHEEL :
				return this->wheel_bind_to_string(it.first);
			default:
				return this->key_bind_to_string(it.first);
			}
		}
	}
	return "";
}

bool InputManager::set_bind(const char *bind_char, const std::string action_str) {
	try {
		ActionManager &action_manager = Engine::get().get_action_manager();

		action_t action = action_manager.get(action_str);
		if (action_manager.is("UNDEFINED",action)) {
			return false;
		}

		Event ev = this->text_to_event(bind_char);
		for(auto &it : this->keys) {
			if (it.second == action) {
				this->keys.erase(it.first);
			}
		}
		this->keys.emplace(ev, action);

		return true;
	}
	catch (int error) {
		return false;
	}
}

Event InputManager::text_to_event(const char *event_char) {
	std::string event_str = std::string(event_char);
	int mod = 0;
	std::size_t start = 0;
	std::size_t end;
	while (true) {
		end = event_str.find(" ", start+1);
		if (end == std::string::npos) {
			break;
		}
		std::string substring = event_str.substr(start,end-start);
		int sub_mod = this->parse_mod(substring);
		if (sub_mod > 0) {
			mod |= sub_mod;
		}
		else if (substring != " ") {
			break;
		}
		start = end+1;
	}

	std::string event_main = event_str.substr(start,std::string::npos);
	std::string event_type = event_main.substr(0,5);
	if (event_type == "MOUSE") {
		return this->text_to_mouse_event(event_main, mod);
	}
	else if (event_type == "WHEEL") {
		return this->text_to_wheel_event(event_main, mod);
	}
	else {
		return this->text_to_key_event(event_main, mod);
	}
}

Event InputManager::text_to_key_event(const std::string &key_str, const int mod) {
	SDL_Keycode key_code = SDL_GetKeyFromName(key_str.c_str());
	if (key_code == SDLK_UNKNOWN) {
		throw 0;
	}
	return sdl_key(key_code, static_cast<SDL_Keymod>(mod));
}

Event InputManager::text_to_mouse_event(const std::string &event_str, const int mod) {
	try {
		std::size_t last_space = event_str.rfind(" ");
		std::string button = event_str.substr(last_space+1, std::string::npos);
		return sdl_mouse(std::stoi(button), static_cast<SDL_Keymod>(mod));
	}
	catch (const std::out_of_range &oor) {
		throw 0;
	}
	catch (const std::invalid_argument &ia) {
		throw 0;
	}
}

Event InputManager::text_to_wheel_event(const std::string &event_str, const int mod) {
	std::size_t last_space = event_str.rfind(" ");
	std::string direction = event_str.substr(last_space+1, std::string::npos);

	if (direction == "1" || direction == "UP") {
		return sdl_wheel(1,static_cast<SDL_Keymod>(mod));
	}
	else if (direction == "-1" || direction == "DOWN") {
		return sdl_wheel(-1,static_cast<SDL_Keymod>(mod));
	}
	throw 0;
}

std::string InputManager::key_bind_to_string(const Event &ev) {
	std::string key_str = std::string(SDL_GetKeyName(ev.cc.code));

	auto end = ev.mod.end();
	if (ev.mod.find(modifier::ALT) != end) {
		key_str = "Alt "+key_str;
	}
	if (ev.mod.find(modifier::SHIFT) != end) {
		key_str = "Shift "+key_str;
	}
	if (ev.mod.find(modifier::CTRL) != end) {
		key_str = "Ctrl "+key_str;
	}
	return key_str;
}

std::string InputManager::mouse_bind_to_string(const Event &ev) {
	return "MOUSE " + std::to_string(ev.cc.code);
}

std::string InputManager::wheel_bind_to_string(const Event &ev) {
	std::string base = "WHEEL ";
	switch (ev.cc.code) {
	case 1:
		return base + "UP";
	case -1:
		return base + "DOWN";
	default:
		return "";
	}
}

int InputManager::parse_mod(const std::string &mod) const {
	auto it = this->string_to_mod.find(mod);
	if (it == this->string_to_mod.end()) {
		return KMOD_NONE;
	}
	return it->second;
}

InputContext &InputManager::get_global_context() {
	return this->global_hotkeys;
}

InputContext &InputManager::get_top_context() {
	if (this->contexts.empty()) {
		return this->global_hotkeys;
	}
	return *this->contexts.back();
}

void InputManager::register_context(InputContext *context) {
	// Create a context list if none exist
	this->contexts.push_back(context);
}


void InputManager::remove_context(InputContext *context) {
	if (this->contexts.empty()) {
		return;
	}

	for (auto it = this->contexts.begin(); it != this->contexts.end(); ++it) {
		if ((*it) == context) {
			this->contexts.erase(it);
			return;
		}
	}
}


bool InputManager::ignored(const Event &e) {
	// filter duplicate utf8 events
	// these are ignored unless the top mode enables
	// utf8 mode, in which case regular char codes are ignored
	return ((e.cc.has_class(event_class::CHAR) || e.cc.has_class(event_class::UTF8)) &&
	        this->get_top_context().utf8_mode != e.cc.has_class(event_class::UTF8));
}


bool InputManager::trigger(const Event &e) {
	if (this->ignored(e)) {
		return false;
	}

	// arg passed to receivers
	action_arg_t arg{e, this->mouse_position, this->mouse_motion, {}};

	// Check whether key combination is bound to an action
	auto range = keys.equal_range(e);
	if (range.first != keys.end()) {
		std::for_each(range.first, range.second, [&arg](binding_map_t::value_type &v) {
			arg.hints.emplace_back(v.second);
		});
	}

	// Check context list on top of the stack (most recent bound first)
	for (auto it = this->contexts.rbegin(); it != this->contexts.rend(); ++it) {
		if ((*it)->execute_if_bound(arg)) {
			return true;
		}
	}

	// If no local keybinds were bound, check the global keybinds
	return this->global_hotkeys.execute_if_bound(arg);
}


void InputManager::set_state(const Event &ev, bool is_down) {
	if (this->ignored(ev)) {
		return;
	}

	// update key states
	this->keymod = ev.mod;
	bool was_down = this->states[ev.cc];
	this->states[ev.cc] = is_down;

	// a key going from pressed to unpressed
	// will automatically trigger event handling
	if (was_down && !is_down) {
		this->trigger(ev);
	}
}


void InputManager::set_mouse(int x, int y) {
	coord::window last_position = this->mouse_position;
	this->mouse_position = coord::window {(coord::pixel_t) x, (coord::pixel_t) y};
	this->mouse_motion = this->mouse_position - last_position;
}


void InputManager::set_motion(int x, int y) {
	this->mouse_motion.x = x;
	this->mouse_motion.y = y;
}


void InputManager::set_relative(bool mode) {
	if (this->relative_mode == mode) {
		return;
	}

	// change mode
	this->relative_mode = mode;
	if (this->relative_mode) {
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else {
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}


bool InputManager::is_down(const ClassCode &cc) const {
	auto it = this->states.find(cc);
	if (it != this->states.end()) {
		return it->second;
	}
	return false;
}


bool InputManager::is_down(event_class ec, code_t code) const {
	return is_down(ClassCode(ec, code));
}


bool InputManager::is_down(SDL_Keycode k) const {
	return is_down(sdl_key(k).cc);
}


bool InputManager::is_mod_down(modifier mod) const {
	return (this->keymod.count(mod) > 0);
}


modset_t InputManager::get_mod() const {
	SDL_Keymod mod = SDL_GetModState();
	return sdl_mod(mod);
}


bool InputManager::on_input(SDL_Event *e) {

	// top level input handler
	switch (e->type) {

	case SDL_KEYUP: {
		SDL_Keycode code = reinterpret_cast<SDL_KeyboardEvent *>(e)->keysym.sym;
		Event ev = sdl_key(code, SDL_GetModState());
		this->set_state(ev, false);
		break;
	} // case SDL_KEYUP

	case SDL_KEYDOWN: {
		SDL_Keycode code = reinterpret_cast<SDL_KeyboardEvent *>(e)->keysym.sym;
		this->set_state(sdl_key(code, SDL_GetModState()), true);
		break;
	} // case SDL_KEYDOWN

	case SDL_TEXTINPUT: {
		this->trigger(utf8(e->text.text));
		break;
	} // case SDL_TEXTINPUT

	case SDL_MOUSEBUTTONUP: {
		this->set_relative(false);
		Event ev = sdl_mouse(e->button.button, SDL_GetModState());
		this->set_state(ev, false);
		break;
	} // case SDL_MOUSEBUTTONUP

	case SDL_MOUSEBUTTONDOWN: {

		// TODO: set which buttons
		if (e->button.button == 2) {
			this->set_relative(true);
		}
		Event ev = sdl_mouse(e->button.button, SDL_GetModState());
		this->set_state(ev, true);
		break;
	} // case SDL_MOUSEBUTTONDOWN

	case SDL_MOUSEMOTION: {
		if (this->relative_mode) {
			this->set_motion(e->motion.xrel, e->motion.yrel);
		}
		else {
			this->set_mouse(e->button.x, e->button.y);
		}

		// must occur after setting mouse position
		Event ev(event_class::MOUSE_MOTION, 0, this->get_mod());
		this->trigger(ev);
		break;
	} // case SDL_MOUSEMOTION

	case SDL_MOUSEWHEEL: {
		Event ev = sdl_wheel(e->wheel.y, SDL_GetModState());
		this->trigger(ev);
		break;
	} // case SDL_MOUSEWHEEL

	} // switch (e->type)

	return true;
}


}} // openage::input
