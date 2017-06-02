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

#include <cmath>

template<typename T>
class Dimension2d
{
protected:
	T	_x;
	T	_y;

	static const double DOUBLE_COMPARE_TOLERANCE;

public:
	Dimension2d() {}
	Dimension2d(T ix, T iy): _x(ix), _y(iy) {}

	T x() const
	{
		return _x;
	}

	T y() const
	{
		return _y;
	}

	void x(T ix)
	{
		_x = ix;
	}

	void y(T iy)
	{
		_y = iy;
	}

	inline bool operator==(const Dimension2d<T>& rhs) const
	{
		return (this->_x == rhs._x) && (this->_y == rhs._y);
	}
	inline bool operator!=(const Dimension2d<T>& rhs) const
	{
		return !(*this == rhs);
	}
};

template<typename T>
const double Dimension2d<T>::DOUBLE_COMPARE_TOLERANCE = 5.0f;

// Do not simply compare two doubles. Allow small differences.
template<>
bool Dimension2d<double>::operator==(const Dimension2d<double>& rhs) const
{
	return 
		(std::abs(this->_x - rhs._x) < DOUBLE_COMPARE_TOLERANCE) && 
		(std::abs(this->_y - rhs._y) < DOUBLE_COMPARE_TOLERANCE);
}
template<>
bool Dimension2d<double>::operator!=(const Dimension2d<double>& rhs) const
{
	return !Dimension2d<double>::operator==(rhs);
}
