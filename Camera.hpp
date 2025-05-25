#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "PackageScreenInterface.hpp"

struct Camera{
	double_t x, y, z;
	double_t phi, theta;
	double_t viewport_vector[3];
	BlockInstance *selected_block;
	inline void look() const;
	inline void set_viewport_vector(double_t const, double_t const);
	inline bool get_viewport_vector(double_t const, double_t const, double_t *) const;
	inline bool get_pixel_coord3dv(double_t const, double_t *) const;
	inline bool get_pixel_coord3llv(double_t const, int64_t *) const;
	inline bool v_get_pixel_coord3llv(double_t *, double_t const, int64_t *) const;
	inline void select_block();
};

#endif
