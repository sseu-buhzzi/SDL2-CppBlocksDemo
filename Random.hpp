#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "headers.hpp"

namespace Sseu {
	struct Random{
		Random() {
			srand(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
		}
		double rand_float() {
			return double(rand()) / RAND_MAX;
		}
		bool rand_bool(double possibility) {
			return this->rand_float() < possibility;
		}
		uint32_t rand_range(uint32_t limit) {
			return rand() % limit;
		}
	};
}

#endif
