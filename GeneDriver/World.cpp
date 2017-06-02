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

#include "World.h"

#include <thread>

World::World(): uni_lanes(0, 1), uni_cow_start(-300, -150), uni_dog_start(-800, -300), uni_cow_spawn(0, 10000)
{
	rng.seed(rd());
	init();
}

void World::score(int value)
{
	fitness += value;
	if(max_fitness < fitness)
	{
		max_fitness = fitness;
		max_generation = generation;
	}
}

void World::tick()
{
	road_offset += speed;
	if(road_offset >= 0)
	{
		road_offset = -440;
	}

	if(cow_present)
	{
		cow_x += speed;
		if(cow_x >= 1000)
		{
			cow_x = static_cast<float>(uni_cow_start(rng));
			score();
			cow_present = false;
		}
	}

	dog_x += speed;
	if(dog_x >= 1000)
	{
		dog_x = static_cast<float>(uni_cow_start(rng));
		if(dog_alive)
		{
			score(-1);	// Missed a collectible
		}
		dog_alive = true;
		random_dog_position();
	}

	if(!cow_present)
	{
		random_cow_position();
	}

	speed += 0.0001f;

	switch(car_state)
	{
	case CAR_STATE::CS_IDLE:
		break;
	case CAR_STATE::CS_GOING_DOWN:
		py++;
		if(py == 210)
		{
			car_state = CAR_STATE::CS_IDLE;
			lane = 1; // Bottom
			human_interference = false;
		}
		break;
	case CAR_STATE::CS_GOING_UP:
		py--;
		if(py == 100)
		{
			car_state = CAR_STATE::CS_IDLE;
			lane = 0; // Top
			human_interference = false;
		}
		break;
	}

	calculate_distance();
	check_dog_collision();
	check_collision();
}

void World::car_down()
{
	if((CAR_STATE::CS_IDLE == car_state) && (py != 210))
	{
		car_state = CAR_STATE::CS_GOING_DOWN;
	}
}

void World::car_up()
{
	if((CAR_STATE::CS_IDLE == car_state) && (py != 100))
	{
		car_state = CAR_STATE::CS_GOING_UP;
	}
}

void World::random_cow_position()
{
	if(!cow_present)
	{
		auto r = uni_cow_spawn(rng);
		if(r < COW_PROBABILITY)
		{
			switch(uni_lanes(rng))
			{
			case 0:
				cow_y = static_cast<float>(COW_TOP);
				cow_lane = 0;
				break;
			case 1:
				cow_lane = 1;
				cow_y = static_cast<float>(COW_BOTTOM);
				break;
			default:
				throw std::runtime_error("random fuckup");
			}
			cow_present = true;
		}
	}
}

void World::random_dog_position()
{
	switch(uni_lanes(rng))
	{
	case 0:
		dog_y = static_cast<float>(DOG_TOP);
		dog_lane = 0;
		break;
	case 1:
		dog_lane = 1;
		dog_y = static_cast<float>(DOG_BOTTOM);
		break;
	default:
		throw std::runtime_error("random fuckup");
	}
}

void World::calculate_distance()
{
	if(cow_present)
	{
		if(cow_lane == 0)
		{
			distance_top = px - cow_x - 125; // 125 = cow width
			distance_bottom = 3000;
		}
		else
		{
			distance_bottom = px - cow_x - 125; // 125 = cow width
			distance_top = 3000;
		}
	}
	else
	{
		distance_bottom = distance_top = 3000;
	}

	if(dog_lane == 0)
	{
		distance_top_dog = px - dog_x - 90; // 90 = dog width
		distance_bottom_dog = 3000;
	}
	else
	{
		distance_bottom_dog = px - dog_x - 90; // 90 = dog width
		distance_top_dog = 3000;
	}
}

void World::check_collision()
{
	bbx[0].set(px, py+32, 128, 64);
	bbx[1].set(cow_x, cow_y + 8, 120, 100-32);
	bbx[2].set(dog_x, dog_y, 84, 73);

	collision = bbx[0].overlaps(bbx[1]);
}

void World::check_dog_collision()
{
	bbx[0].set(px, py+32, 128, 64);
	bbx[1].set(cow_x, cow_y + 8, 120, 100-32);
	bbx[2].set(dog_x, dog_y, 84, 73);

	dog_collision = dog_alive && bbx[0].overlaps(bbx[2]);
	if(dog_collision)
	{
		score();
		dog_alive = false;
	}
}

void World::init()
{
	px = 800;
	py = 100;
	road_offset = -440;
	road_y = 90;
	cow_present = false;
	cow_x = static_cast<float>(uni_cow_start(rng));
	dog_x = static_cast<float>(uni_dog_start(rng));
	dog_alive = true;
	collision = false;
	speed = 1.0f;
	lane = 0;
	cow_lane = 0;
	fitness = 0;
	car_state = CAR_STATE::CS_IDLE;
	human_interference = false;
	random_cow_position();
	random_dog_position();
	calculate_distance();
}
