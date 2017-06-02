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

#include "Network.h"

static std::random_device rd_mut;  
static std::mt19937 rng_mut(rd_mut());
static std::uniform_real_distribution<float> uni_mut(0.0f, 1.0f);

template<class Archive>
void Neuron::serialize(Archive & ar, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(output);
    ar & BOOST_SERIALIZATION_NVP(ID);
    ar & BOOST_SERIALIZATION_NVP(inputs);
}

Neuron::Neuron()
{
}

Neuron::Neuron(int _number_of_inputs, bool skip_initialization)
{
	ID = global_neuron_id++; // TODO: This overflows as the mutations progress...
	inputs.resize(_number_of_inputs);
	if(!skip_initialization)
	{
		randomize_weights();
	}
}

void Neuron::randomize_weights()
{
	std::for_each(inputs.begin(), inputs.end(), [&](auto& x) {x = distribution(generator);} );
}

void Neuron::set_output(double value)
{
	output = value;
}

double Neuron::get_output() const
{
	return output;
}

std::string Neuron::get_ID() const
{
	return std::string(1, ID);
}

template<class Archive>
void Bias::serialize(Archive & ar, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Neuron);
}

Bias::Bias(bool skip_initialization): Neuron(0, skip_initialization)
{
}

double Bias::get_output() const
{
	return 1.0f;
}

std::string Bias::get_ID() const
{
	return "B" + std::string(1, ID);
}

template<class Archive>
void Layer::serialize(Archive & ar, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(neurons);
}

template<class Archive>
void Network::serialize(Archive & ar, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(inputs);
    ar & BOOST_SERIALIZATION_NVP(outputs);
    ar & BOOST_SERIALIZATION_NVP(hidden_layers);
    ar & BOOST_SERIALIZATION_NVP(hidden_layer_size);
    ar & BOOST_SERIALIZATION_NVP(layers);
}

void Network::build()
{
	global_neuron_id = 'A';

	// Output layer
	{
		Layer l;
		for(auto i = 0; i < outputs; ++i)
		{
			l.neurons.push_back(std::make_shared<Neuron>(hidden_layer_size + 1));	// +1 for Bias
		}
		layers.push_front(l);
	}

	// Hidden layers
	{
		for(auto i = 0; i < hidden_layers; ++i)
		{
			Layer l;
			auto input_count = (i == hidden_layers - 1) ? inputs + 1 : hidden_layer_size + 1;
			for(auto j = 0; j < hidden_layer_size; ++j)
			{
				l.neurons.push_back(std::make_shared<Neuron>(input_count));
			}
			l.neurons.push_back(std::make_shared<Bias>());
			layers.push_front(l);
		}
	}

	// Input layer
	{
		Layer l;
		for(auto i = 0; i < inputs; ++i)
		{
			l.neurons.push_back(std::make_shared<Neuron>(0, true));
		}
		l.neurons.push_back(std::make_shared<Bias>(true));
		layers.push_front(l);
	}
}

double Network::fire()
{
	double last;
	for(auto l = ++layers.begin(); l != layers.end(); ++l)
	{
		auto pl = l;	// Previous layer
		pl--;
		for(auto j = 0; j < l->neurons.size(); ++j)
		{
			double calc = 0.0f;
			for(auto k = 0; k < l->neurons[j]->inputs.size(); ++k)
			{
				calc += pl->neurons[k]->get_output() * l->neurons[j]->inputs[k];
			}
			last = activation_function(calc);
			l->neurons[j]->set_output(last);
		}
	}
	return last;
}

Network::Network()
{
}

Network::Network(
	const std::function<double(double)>& _activation_function,
	int _inputs,
	int _outputs,
	int _hidden_layers):
		activation_function(_activation_function),
		inputs(_inputs),
		outputs(_outputs),
		hidden_layers(_hidden_layers)
{
	// Size of hidden layer is an average of input and output layer size.
	// +4, because why not :)
	// Well, I mean I needed more neurons after introducing "collectibles"
	hidden_layer_size = 4 + static_cast<int>(std::ceil((inputs + outputs) / 2.0f));

	build();
};

void Network::set_input(int index, double value)
{
	layers.front().neurons[index]->set_output(value);
}

std::vector<std::shared_ptr<Neuron>> Network::export_neurons()
{
	std::vector<std::shared_ptr<Neuron>> ret;

	// Export hidden layers (exclulding bias neurons)
	auto hidden_layers_begin = ++layers.begin();
	auto hidden_layers_end = --layers.end();
	for(auto it = hidden_layers_begin; it != hidden_layers_end; ++it)
	{
		for(auto i = 0; i < it->neurons.size() - 1; ++i)
		{
			ret.push_back(it->neurons[i]);
		}
	}

	// Export output layer
	for(auto i = 0; i < layers.back().neurons.size(); ++i)
	{
		ret.push_back(layers.back().neurons[i]);
	}

	return ret;
}

void Network::import_neurons(std::vector<std::shared_ptr<Neuron>> _neurons)
{
	auto import_iterator = _neurons.begin();

	// Import hidden layers (exclulding bias neurons)
	auto hidden_layers_begin = ++layers.begin();
	auto hidden_layers_end = --layers.end();
	for(auto it = hidden_layers_begin; it != hidden_layers_end; ++it)
	{
		auto x = it->neurons.size();
		it->neurons.clear();
		for(auto i = 0; i < x - 1; ++i)
		{
			it->neurons.push_back(*import_iterator++);
		}
		it->neurons.push_back(std::make_shared<Bias>());
	}

	// Export output layer
	auto x = layers.back().neurons.size();
	layers.back().neurons.clear();
	for(auto i = 0; i < x; ++i)
	{
		layers.back().neurons.push_back(*import_iterator++);
	}
}

void Network::mutate()
{
	for(auto& l: layers)
	{
		for(std::vector<std::shared_ptr<Neuron>>::iterator it = l.neurons.begin(); it != l.neurons.end(); ++it)
		{
			std::shared_ptr<Neuron> o = std::make_shared<Neuron>(**it);
			for(auto i = 0; i < o->inputs.size(); ++i)
			{
				auto r = uni_mut(rng_mut);
				if(r < MUTATION_PROBABILITY)
				{
					o->inputs[i] = distribution(generator);
				}
			}
			*it = std::make_shared<Neuron>(*o);
		}
	}
}

BOOST_CLASS_EXPORT(Layer)
BOOST_CLASS_EXPORT(Network)
BOOST_CLASS_EXPORT(Neuron)
BOOST_CLASS_EXPORT(Bias)
