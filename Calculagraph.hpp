#ifndef CALCULAGRAPH_HPP
#define CALCULAGRAPH_HPP

#include "headers.hpp"

struct Calculagraph : std::chrono::system_clock::time_point{
	double_t diff;
	Calculagraph *get(double_t const unit) {
		this->diff = (std::chrono::system_clock::now() - *this).count() * unit;
		return this;
	}
	Calculagraph *set() {
		*(std::chrono::system_clock::time_point *)(this) = std::chrono::system_clock::now();
		return this;
	}
};

#endif
