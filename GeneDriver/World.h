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

#include <random>
#include <memory>
#include <array>
#include <climits>

#include "BoundingBox.h"

enum class DECISIONS
{
	KEY_UP,
	KEY_DOWN,
	KEY_NONE
};

class World
{
	std::random_device rd;  
	std::mt19937 rng;
	std::uniform_int_distribution<int> uni_lanes;
	std::uniform_int_distribution<int> uni_cow_start;
	std::uniform_int_distribution<int> uni_dog_start;
	std::uniform_int_distribution<int> uni_cow_spawn;
	const int COW_PROBABILITY = 17;

public:
	bool human_interference;
	DECISIONS network_decision;
	int generation;
	int individual;
	int population_size;

	const int COW_BOTTOM = 230;
	const int COW_TOP = 110;

	const int DOG_BOTTOM = 240;
	const int DOG_TOP = 110;

	float px;
	float py;
	float road_offset;
	float road_y;
	float cow_x;
	float cow_y;
	float dog_x;
	float dog_y;

	// Collision detection
	std::array<BoundingBox, 3> bbx;		// 0 - car, 1 - cow, 2 - dog
	bool collision;
	bool dog_collision;
	bool dog_alive;

	// Sensors
	float speed;
	float distance_top;
	float distance_bottom;
	float distance_top_dog;
	float distance_bottom_dog;
	int lane;		// 0 - top, 1 - bottom
	int cow_lane;
	bool cow_present;
	int dog_lane;

	// Fitness
	int fitness;
	int max_fitness = INT_MIN;
	int max_generation = 0;

	enum class CAR_STATE
	{
		CS_IDLE,
		CS_GOING_DOWN,
		CS_GOING_UP
	};

	CAR_STATE car_state;

	void random_cow_position();
	void random_dog_position();
	void calculate_distance();
	void check_collision();
	void check_dog_collision();
	void score(int value = 1);

	void init();

public:
	World();
	void tick();
	void car_down();
	void car_up();	
};
