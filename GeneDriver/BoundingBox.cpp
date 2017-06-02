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

#include "BoundingBox.h"

void BoundingBox::set(float x, float y, float width, float height)
{
	corners[0].x(x);				corners[0].y(y);
	corners[1].x(x + width);		corners[1].y(y);
	corners[2].x(x + width);		corners[2].y(y + height);
	corners[3].x(x);				corners[3].y(y + height);
}

const std::array<Dimension2d<float>, 4>& BoundingBox::get() const
{
	return corners;
}

bool BoundingBox::in_range(float v, float min, float max) const
{
	return (v > min) && (v < max);
}

bool BoundingBox::overlaps(const BoundingBox& b) const
{
    return	(in_range(corners[0].x(), b.corners[0].x(), b.corners[1].x()) ||
			 in_range(b.corners[0].x(), corners[0].x(), corners[1].x()))
			&&
			(in_range(corners[0].y(), b.corners[0].y(), b.corners[3].y()) ||
			 in_range(b.corners[0].y(), corners[0].y(), corners[3].y()));
}
