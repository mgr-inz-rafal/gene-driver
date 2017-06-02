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

#include "AllegroProxy.h"
#include "World.h"

class AllegroProxy;
class Network;
struct ALLEGRO_FONT;
struct ALLEGRO_BITMAP;

class Painter
{
	ALLEGRO_BITMAP* car;
	ALLEGRO_BITMAP* road;
	ALLEGRO_BITMAP* cow;
	ALLEGRO_BITMAP* dog;
	ALLEGRO_BITMAP* blood;
	ALLEGRO_FONT *font = NULL;
	int road_width = 4*440;	// There are 4 elements, each 440 pixels wide

	const World& world;
	Network* net;
	const AllegroProxy& al;
	const bool& showcase;
	bool show_network = false;

	std::string get_decision_as_string(DECISIONS decision) const;

public:
	Painter(const World& _world, const AllegroProxy& _al, const bool& _showcase);
	void paint_world() const;
	void set_net(Network* const net);
	void set_show_network(const bool show_network);
};
