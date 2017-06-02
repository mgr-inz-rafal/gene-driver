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

#include <allegro5/allegro.h>

#include <string>

#include "Dimension2d.h"

struct ALLEGRO_DISPLAY;
struct ALLEGRO_EVENT_QUEUE;

class AllegroProxy
{
	const size_t SCREEN_WIDTH	= 1024;
	const size_t SCREEN_HEIGHT	= 768;
	Dimension2d<size_t> screen_size = {SCREEN_WIDTH, SCREEN_HEIGHT};

	ALLEGRO_DISPLAY*		display;
	ALLEGRO_EVENT_QUEUE*	event_queue;

public:
	void init_all();
	void show_display();

	const Dimension2d<size_t>& get_screen_size() const;
	
	void register_event();
	ALLEGRO_EVENT_QUEUE* get_event_queue();

	ALLEGRO_BITMAP* load_bitmap(const std::string& filename) const;

	AllegroProxy();
	~AllegroProxy();
};

