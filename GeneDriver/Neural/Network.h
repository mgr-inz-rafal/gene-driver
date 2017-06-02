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

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/export.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <vector>
#include <list>
#include <functional>
#include <memory>
#include <string>
#include <random>
#include <algorithm>

static char global_neuron_id = 'A';
static std::default_random_engine generator;
static std::normal_distribution<double> distribution;	// Mean=0.0f	Std. deviation=1.0f
static const double MUTATION_PROBABILITY = 0.05f;

class Neuron
{
	friend class boost::serialization::access;
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version);

	double output = 0.0f;
protected:
	char ID;
public:
	std::vector<double> inputs;
	Neuron(); // Needed for deserialization
	Neuron(int _number_of_inputs, bool skip_initialization = false);
	void randomize_weights();
	virtual void set_output(double value);
	virtual double get_output() const;
	virtual std::string get_ID() const;
};

class Bias: public Neuron
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
public:
	Bias(bool skip_initialization = false);
	virtual double get_output() const;
	virtual std::string get_ID() const;
};

class Layer
{
	friend class boost::serialization::access;
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
public:
	std::vector<std::shared_ptr<Neuron>> neurons;
};

class Network
{
	friend class boost::serialization::access;
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
public:
	void build();

public:
	std::function<double(double)> activation_function;
	int inputs;
	int outputs;
	int hidden_layers;
	int hidden_layer_size;
	std::list<Layer> layers;
	double fire();
	void mutate();
	Network(); // Needed for deserialization
	Network(
		const std::function<double(double)>& _activation_function,
		int _inputs,
		int _outputs,
		int _hidden_layers);
	void set_input(int index, double value);
	std::vector<std::shared_ptr<Neuron>> export_neurons();
	void import_neurons(std::vector<std::shared_ptr<Neuron>> _neurons);
};
