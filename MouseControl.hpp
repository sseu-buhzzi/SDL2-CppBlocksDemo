#ifndef MOUSE_CONTROL_HPP
#define MOUSE_CONTROL_HPP

#include "headers.hpp"

struct MouseControl{
	int32_t x, y;
	int32_t x_diff, y_diff;
	uint32_t mouse_state;
	uint32_t down_diff, up_diff;
	MouseControl *get() {
		uint32_t state_diff{SDL_GetMouseState(&this->x_diff, &this->y_diff) ^ this->mouse_state};
		this->x_diff -= this->x;
		this->y_diff -= this->y;
		this->down_diff = this->mouse_state & state_diff;
		this->up_diff = ~this->mouse_state & state_diff;
		return this;
	}
	MouseControl *set() {
		this->mouse_state = SDL_GetMouseState(&this->x, &this->y);
		return this;
	}
};

#endif
