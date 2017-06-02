/*
 *
 * Gene-Driver by mgr_inz_rafal.
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <rchabowski@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 *														// mgr inz. Rafal
 * ----------------------------------------------------------------------------
 * 
 * This code is provided as-is. It has been written with the "get-shit-done"
 * attitude, therefore it shouldn't be used as C++ coding reference.
 * Encapsulation is sparse, inheritance is futile...
 * But it works and learns :)
 * I will rather rewrite it from scratch than refactor to industry standards.
 * 
 * ----------------------------------------------------------------------------
 */

#pragma once

#include "Dimension2d.h"

#include <array>

class BoundingBox
{
	static const int NUM_CORNERS = 4;

	std::array<Dimension2d<float>, NUM_CORNERS> corners;
	bool in_range(float v, float min, float max) const;

public:
	void set(float x, float y, float widht, float height);
	const std::array<Dimension2d<float>, NUM_CORNERS>& get() const;
	bool overlaps(const BoundingBox& b) const;
};
