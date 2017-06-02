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

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <functional>
#include <cmath>

#include "World.h"
#include "Painter.h"
#include "AllegroProxy.h"
#include "Neural/Network.h"

const int population_size = 16;
auto KEY_UP_THRESHOLD = 0.4f;
auto KEY_DOWN_THRESHOLD = 0.6f;

// ------ Flags to control code execution ------

// If true then app loads the generation from "showcase_file", selects
// the top individual and let it play the game.
// After this individual crashes, game just stays still.
// Use when you developed an interesting individual and
// want to replay his capabilities.
const bool showcase = false;
const std::string showcase_file = "FILENAME";

// If set to true then decisions made by neural network will be
// applied to the car. If false car will only react
// to arrow keys
const bool interpret_network_output = true;

// If true then app loads the generation from "continue_learning_file"
// and resumes learning.
// Use when you want to resume learning process that you interrupted earlier.
const bool continue_learning = false;
const std::string continue_learning_file = "FILENAME";

// If set to true, game will not run
bool do_nothing = false;

double sigmoid_function(double x)
{
	return 1.0f / (1.0f + std::exp(-x));
}

DECISIONS interpret_output(double value)
{
	if(value < KEY_UP_THRESHOLD)
	{
		return DECISIONS::KEY_UP;
	}
	if(value > KEY_DOWN_THRESHOLD)
	{
		return DECISIONS::KEY_DOWN;
	}
	return DECISIONS::KEY_NONE;
}

class Individual
{
	friend class boost::serialization::access;
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(network);
        ar & BOOST_SERIALIZATION_NVP(fitness);
    }
public:
	Network network;
	int fitness;
	Individual() {};
	Individual(const Network& _network): network(_network)
	{
	}
	bool operator<(const Individual& rhs)
	{
		return fitness < rhs.fitness;
	}
	bool operator>(const Individual& rhs)
	{
		return fitness > rhs.fitness;
	}
};

int main()
{
	bool slowdown = false;

	using namespace std::chrono_literals;

	std::ostringstream ss;
	std::time_t t = std::time(nullptr);
	ss << "Data\\";
	ss << std::put_time(std::localtime(&t), "%Y-%m-%d_%H-%M-%S");
	std::string directory = ss.str();
 
	boost::filesystem::path dir(directory);
	boost::filesystem::create_directory(dir);

	try
	{
		ALLEGRO_EVENT ev;
		AllegroProxy al;
		al.init_all();
		al.show_display();
		al.register_event();
		World world;
		Painter painter(world, al, showcase);

		auto decision = DECISIONS::KEY_NONE;

		world.generation = 0;
		world.population_size = population_size;

		std::vector<Individual> generation;
		if(showcase)
		{
			// Deserialize from disk
			{
				std::ifstream ifs(showcase_file);
				boost::archive::xml_iarchive ia(ifs);
				ia >> BOOST_SERIALIZATION_NVP(generation);
			}
		}
		else if(continue_learning)
		{
			// Deserialize from disk
			{
				std::ifstream ifs(continue_learning_file);
				boost::archive::xml_iarchive ia(ifs);
				ia >> BOOST_SERIALIZATION_NVP(generation);
			}
		}
		else
		{
			for(auto i = 0; i < population_size; ++i)
			{
				// Six sensors, single output, two hidden layers
				generation.push_back(Individual(Network (sigmoid_function, 6, 1, 2)));
			}
		}

		for(;;)
		{
			++world.generation;

			// Main loop
			for(auto i = 0; i < population_size; ++i)
			{
				world.individual = i+1;
				auto& net = generation[i].network;
				if((showcase) || (continue_learning))
				{
					net.activation_function = sigmoid_function;
				}
				painter.set_net(&net);

				for(;;)
				{
					if(!al_event_queue_is_empty(al.get_event_queue()))
					{
						al_get_next_event(al.get_event_queue(), &ev);
						if(ALLEGRO_EVENT_DISPLAY_CLOSE == ev.type)
						{
							return 0;
						}
						else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
						{
							switch(ev.keyboard.keycode)
							{
								case ALLEGRO_KEY_DOWN:
									world.human_interference = true;
									world.car_down();
									break;
								case ALLEGRO_KEY_UP:
									world.human_interference = true;
									world.car_up();
									break;
								case ALLEGRO_KEY_LSHIFT:
								case ALLEGRO_KEY_RSHIFT:
									painter.set_show_network(true);
									break;
								case ALLEGRO_KEY_LCTRL:
								case ALLEGRO_KEY_RCTRL:
									slowdown = true;
									break;
							}			
						}
						else if(ev.type == ALLEGRO_EVENT_KEY_UP)
						{
							switch(ev.keyboard.keycode)
							{
								case ALLEGRO_KEY_LSHIFT:
								case ALLEGRO_KEY_RSHIFT:
									painter.set_show_network(false);
									break;
								case ALLEGRO_KEY_LCTRL:
								case ALLEGRO_KEY_RCTRL:
									slowdown = false;
									break;
							}			
						}
					}

					if(!do_nothing)
					{
						world.tick();
						if(slowdown)
						{
							std::this_thread::sleep_for(std::chrono::milliseconds(5));
						}

						net.set_input(0, world.speed);
						net.set_input(1, world.lane);
						net.set_input(2, world.distance_top);
						net.set_input(3, world.distance_bottom);
						net.set_input(4, world.distance_top_dog);
						net.set_input(5, world.distance_bottom_dog);

						world.network_decision = interpret_output(net.fire());
						if(interpret_network_output)
						{
							switch(world.network_decision)
							{
							case DECISIONS::KEY_DOWN:
								world.car_down();
								break;
							case DECISIONS::KEY_UP:
								world.car_up();
								break;
							}
						}
					}

					painter.paint_world();

					if(do_nothing)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
						continue;
					}

					if(world.collision)
					{
						if(!showcase)
						{
							generation[i].fitness = world.fitness;
						}
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
						if(!showcase)
						{
							world.init();
						}
						else
						{
							do_nothing = true;
						}
						break;
					}
				}

			}

			if(!showcase)
			{
				// *** Generation finished ***
				// Pick best parents
				std::sort(generation.begin(), generation.end(), std::greater<>());

				// Serialize to disk
				{
					std::string filename = (boost::format("%s\\gen_%04i.xml") % directory % world.generation).str();
					std::ofstream ofs(filename);
					boost::archive::xml_oarchive oa(ofs);
					oa << BOOST_SERIALIZATION_NVP(generation);
				}

				// Remove weakest from population
				// TODO: Sometimes allow weaker offspring to proliferate
				generation.erase(++++generation.begin(), generation.end()); // C+++++++.... :-)

				// Crossover parents
				auto n1 = generation[0].network.export_neurons();
				auto n2 = generation[1].network.export_neurons();

				std::random_device rd;
				std::mt19937 rng(rd());
				std::uniform_int_distribution<size_t> uni(1, n1.size()-1); 
				for(auto i = 0; i < (population_size-2)/2; ++i)
				{
					auto crossover_point = uni(rng);
					std::vector<std::shared_ptr<Neuron>> child1_neurons;
					std::vector<std::shared_ptr<Neuron>> child2_neurons;
					for(auto j = 0; j < n1.size(); ++j)
					{
						auto p1_neuron = n1[j];
						auto p2_neuron = n2[j];
						if(j < crossover_point)
						{
							child1_neurons.push_back(p1_neuron);
							child2_neurons.push_back(p2_neuron);
						}
						else
						{
							child1_neurons.push_back(p2_neuron);
							child2_neurons.push_back(p1_neuron);
						}
					}

					Network net1 = generation[0].network;
					net1.import_neurons(child1_neurons);
					Network net2 = generation[0].network;
					net2.import_neurons(child2_neurons);

					auto new_individual_1 = Individual(net1);
					new_individual_1.network.mutate();
					auto new_individual_2 = Individual(net2);
					new_individual_2.network.mutate();
					generation.push_back(new_individual_1);
					generation.push_back(new_individual_2);
				}
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}
	return 0;
}
