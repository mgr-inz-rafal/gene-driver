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

#include "Painter.h"
#include "AllegroProxy.h"
#include "World.h"
#include "Neural/Network.h"

#include <string>

#include <boost/format.hpp>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

void Painter::set_net(Network* const net)
{
	this->net = net;
}

void Painter::set_show_network(const bool show_network)
{
	this->show_network = show_network;
}

Painter::Painter(const World& _world, const AllegroProxy& _al, const bool& _showcase): world(_world), al(_al), showcase(_showcase)
{
	dog = al.load_bitmap("../Resources/dog.png");
	al_convert_mask_to_alpha(dog, al_map_rgb(255, 255, 255));
	blood = al.load_bitmap("../Resources/blood.png");
	al_convert_mask_to_alpha(blood, al_map_rgb(0, 0, 0));
	car = al.load_bitmap("../Resources/car.png");
	al_convert_mask_to_alpha(car, al_map_rgb(0, 0, 0));
	cow = al.load_bitmap("../Resources/cow.png");
	al_convert_mask_to_alpha(cow, al_map_rgb(0, 0, 0));
	road = al.load_bitmap("../Resources/road.png");

	font = al_load_ttf_font("../Resources/unispace.ttf", 16, 0);
	if(!font)
	{
		throw std::exception("al_load_ttf_font() failed");
	}
}

void Painter::paint_world() const
{
	// Paint road
	al_draw_bitmap(road, world.road_offset, world.road_y, 0);

	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

	// Draw obstacle
	al_draw_bitmap(cow, world.cow_x, world.cow_y, 0);

	// Draw collectible
	if(world.dog_alive)
	{
		al_draw_bitmap(dog, world.dog_x, world.dog_y, 0);
	}
	else
	{
		al_draw_bitmap(blood, world.dog_x, world.dog_y, 0);
	}

	// Paint player
	if(world.human_interference)
	{
		al_draw_tinted_bitmap(car, al_map_rgba_f(0, 0, 1, 1.0), world.px, world.py, 0);
	}
	else
	{
		al_draw_bitmap(car, world.px, world.py, 0);
	}

	/*
	// Draw top distance
	if((world.distance_top > 0) && (world.car_state == World::CAR_STATE::CS_IDLE))
	{
		al_draw_line(world.px, 164, world.px - world.distance_top, 164, al_map_rgb(0, 255, 0), 2.0f);
		al_draw_line(world.px, 164 - 8, world.px, 164 + 8, al_map_rgb(0, 255, 0), 2.0f);
		al_draw_line(world.px - world.distance_top, 164 - 8, world.px - world.distance_top, 164 + 8, al_map_rgb(0, 255, 0), 2.0f);
	}

	// Draw bottom distance
	if((world.distance_bottom > 0) && (world.car_state == World::CAR_STATE::CS_IDLE))
	{
		al_draw_line(world.px, 164+110, world.px - world.distance_bottom, 164+110, al_map_rgb(0, 255, 0), 2.0f);
		al_draw_line(world.px, 164+110 - 8, world.px, 164+110 + 8, al_map_rgb(0, 255, 0), 2.0f);
		al_draw_line(world.px - world.distance_bottom, 164+110 - 8, world.px - world.distance_bottom, 164+110 + 8, al_map_rgb(0, 255, 0), 2.0f);
	}
	*/

	// Draw stats
	al_draw_text(font, al_map_rgb(0, 255, 0), 10, 432+32*-1, ALLEGRO_ALIGN_LEFT, "SENSORS:");
	al_draw_text(font, al_map_rgb(255, 255, 255), 10, 432+32*0, ALLEGRO_ALIGN_LEFT, 
		(boost::format("Speed:           %1$6.2fkph  [ %2% ]") % (30 * world.speed) % world.speed).str().c_str());
	al_draw_text(font, al_map_rgb(255, 255, 255), 10, 432+32*1, ALLEGRO_ALIGN_LEFT, 
		(boost::format("Lane:             %1$-s     [ %2% ]") % (world.lane ? "LEFT " : "RIGHT") % world.lane).str().c_str());
	al_draw_text(font, al_map_rgb(255, 255, 255), 10, 432+32*2, ALLEGRO_ALIGN_LEFT, 
		(boost::format("Top distance:    %1$6.2fm    [ %2% ]") % (world.distance_top / 20) % world.distance_top).str().c_str());
	al_draw_text(font, al_map_rgb(255, 255, 255), 10, 432+32*3, ALLEGRO_ALIGN_LEFT, 
		(boost::format("Bottom distance: %1$6.2fm    [ %2% ]") % (world.distance_bottom / 20) % world.distance_bottom).str().c_str());
	al_draw_text(font, al_map_rgb(255, 255, 255), 10, 432+32*4, ALLEGRO_ALIGN_LEFT, 
		(boost::format("Top dog dist.:   %1$6.2fm    [ %2% ]") % (world.distance_top_dog / 20) % world.distance_top_dog).str().c_str());
	al_draw_text(font, al_map_rgb(255, 255, 255), 10, 432+32*5, ALLEGRO_ALIGN_LEFT, 
		(boost::format("Bottom dog dist: %1$6.2fm    [ %2% ]") % (world.distance_bottom_dog / 20) % world.distance_bottom_dog).str().c_str());

	al_draw_text(font, al_map_rgb(0, 255, 0), 400, 432+32*-1, ALLEGRO_ALIGN_LEFT, "STATUS:");
	if(!showcase)
	{
		al_draw_text(font, al_map_rgb(255, 255, 255), 400, 432+32*0, ALLEGRO_ALIGN_LEFT, 
			(boost::format("Fitness:   %1% (max: %2% in gen. %3%)") % world.fitness % world.max_fitness % world.max_generation).str().c_str());
	}
	else
	{
		al_draw_text(font, al_map_rgb(255, 255, 255), 400, 432+32*0, ALLEGRO_ALIGN_LEFT, 
			(boost::format("Fitness:   %1%") % world.fitness).str().c_str());
	}
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 432+32*1, ALLEGRO_ALIGN_LEFT, 
		(boost::format("Collision: %1%") % world.collision).str().c_str());
	if(!showcase)
	{
		al_draw_text(font, al_map_rgb(255, 255, 255), 400, 432+32*2, ALLEGRO_ALIGN_LEFT, 
			(boost::format("Generation: %1%") % world.generation).str().c_str());
		al_draw_text(font, al_map_rgb(255, 255, 255), 400, 432+32*3, ALLEGRO_ALIGN_LEFT, 
			(boost::format("Individual: %1%/%2%") % world.individual % world.population_size).str().c_str());
	}
	else
	{
		al_draw_text(font, al_map_rgb(255, 255, 255), 400, 432+32*2, ALLEGRO_ALIGN_LEFT, "Showcasing from file");
		al_draw_text(font, al_map_rgb(255, 255, 255), 400, 432+32*3, ALLEGRO_ALIGN_LEFT, "Press \"Ctrl\" to slowdown");
	}

	al_draw_text(font, al_map_rgb(0, 255, 0), 750, 432+32*-1, ALLEGRO_ALIGN_LEFT, "OUTPUT:");
	al_draw_text(font, al_map_rgb(255, 255, 255), 750, 432+32*0, ALLEGRO_ALIGN_LEFT, 
		(boost::format("Decision:   %1%") % get_decision_as_string(world.network_decision)).str().c_str());

	// Draw collision boxes
	/*
	for(int j = 0; j < 3; ++j)
	{
		for(int i = 0; i < 4; ++i)
		{
			al_draw_line(
				world.bbx[j].get()[i].x(),
				world.bbx[j].get()[i].y(),
				world.bbx[j].get()[(i+1==4) ? 0 : i+1].x(),
				world.bbx[j].get()[(i+1==4) ? 0 : i+1].y(),
				al_map_rgb(255, 255, 128 + j * 64),
				2.0f);
		}
	}
	*/

	if(show_network)
	{
		al_draw_filled_rectangle(0, 0, 1024, 768, al_map_rgba(0, 0, 0, 256 * 0.85));

		int i = 0;
		for(auto& l: net->layers)
		{
			for(auto j = 0; j < l.neurons.size(); ++j)
			{
				auto x = 128 + i * 256;
				auto y = 8 + j * 112;
				auto& n = l.neurons[j];
				al_draw_text(font, al_map_rgb(255, 255, 255), x, y, ALLEGRO_ALIGN_LEFT, n->get_ID().c_str());
				for(auto k = 0; k < n->inputs.size(); ++k)
				{
					al_draw_text(font, al_map_rgb(0, 255, 255), x - 96, y + 15 * k, ALLEGRO_ALIGN_LEFT, 
						(boost::format("%1%") % n->inputs[k]).str().c_str());
				}
				al_draw_text(font, al_map_rgb(0, 255, 255), x + 32, y, ALLEGRO_ALIGN_LEFT, 
					(boost::format("%1%") % n->get_output()).str().c_str());
			}
			++i;
		}
	}

	al_flip_display();
	al_clear_to_color(al_map_rgb(0, 0, 0));
}

std::string Painter::get_decision_as_string(DECISIONS decision) const
{
	switch(decision)
	{
	case DECISIONS::KEY_DOWN:
		return "Press DOWN";
	case DECISIONS::KEY_UP:
		return "Press UP";
	case DECISIONS::KEY_NONE:
		return "Release keys";
	default:
		return "Czapka";
	}
}
