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

#include "AllegroProxy.h"

#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <boost/format.hpp>

AllegroProxy::AllegroProxy():
	display(nullptr),
	event_queue(nullptr)
{
}

AllegroProxy::~AllegroProxy()
{
	if(display)
	{
		al_destroy_display(display);
		display = nullptr;
	}
}

void AllegroProxy::init_all()
{
	if(!al_init())
	{
		throw std::runtime_error("al_init() failed");
	}
	if(!al_init_primitives_addon())
	{
		throw std::runtime_error("al_init_primitives_addon() failed");
	}
	if(!al_init_image_addon())
	{
		throw std::runtime_error("al_init_image_addon() failed");
	}
	al_init_font_addon();
	/*
	if(!al_init_font_addon())
	{
		throw std::runtime_error("al_init_font_addon() failed");
	}
	*/
	if(!al_init_ttf_addon())
	{
		throw std::runtime_error("al_init_ttf_addon() failed");
	}
	if(!al_install_keyboard())
	{
		throw std::runtime_error("al_install_keyboard() failed");
	}
}

void AllegroProxy::show_display()
{
//	al_set_new_display_flags(ALLEGRO_FULLSCREEN | ALLEGRO_OPENGL_3_0);
	display = al_create_display(static_cast<int>(screen_size.x()), static_cast<int>(screen_size.y()));
	if(!display)
	{
		throw std::runtime_error("al_create_display() failed");
	}
}

void AllegroProxy::register_event()
{
	// Create event queue if necessary
	if(!event_queue)
	{
		event_queue = al_create_event_queue();
		if(!event_queue)
		{
			throw std::runtime_error("al_create_event_queue() failed");
		}
	}
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
}

ALLEGRO_EVENT_QUEUE* AllegroProxy::get_event_queue()
{
	return event_queue;
}

const Dimension2d<size_t>& AllegroProxy::get_screen_size() const
{
	return screen_size;
}

ALLEGRO_BITMAP* AllegroProxy::load_bitmap(const std::string& filename) const
{
	ALLEGRO_BITMAP* tmp;
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
	tmp = al_load_bitmap(filename.c_str());
	if(!tmp)
	{
		throw std::runtime_error((boost::format("al_load_bitmap(%1%) failed") % filename).str());
	}
	return tmp;
}
