#define _CRT_SECURE_NO_WARNINGS

// img loading
#define STB_IMAGE_IMPLEMENTATION

#define STBI_ONLY_BMP	1
#define STBI_ONLY_PNG	1
#define STBI_ONLY_TGA	1
#define STBI_ONLY_JPEG	1

#include "deps/stb/stb_image.h"


#include "../imgl/imgl.hpp"
using imgl::u8;
using imgl::uint;
using imgl::u64;
using imgl::f32;
typedef imgl::s32v2 iv2;
typedef imgl::fv2 v2;
using imgl::rgba8;
using imgl::rgb8;
using imgl::rgbf;
using imgl::cstr;

#include <vector>
#include <memory>
using std::unique_ptr;
using std::make_unique;

/*
	Trying to implement a neural network evaluator and trainer after having watched 3brown1blue's neural network videos:
		https://www.youtube.com/watch?v=aircAruvnKk
	
	shorthand definitions:
	
	nn = neural network

	first layer:	each neuron only has activation (pixel value [0,1] of input image)
	hidden layers:	each neuron has weights[layer[i-1].neurons.count] + bias + activation (when evaluating)
	final layer:	like hidden layers, but we define a meaning to it's neurons activations (and a expected activation for each input set)

	input layer:	the first layer
	real neuron:	all neurons which have wheights and a bias, ie. not the neurons of the first layer, since they are just conceptual neurons, they do not process signals, they define the source signal (i guess you could call them sensors)
	real layer:		see above, each layer except the input layer
	output layer:	the last layer

*/

#include "../imgl/parse.hpp"

namespace parse {
	using namespace imgl::n_parse;

	auto line_end_c = [] (char c) { return is_newline_c(c) || c == '\0'; };

	bool line_comment (char** pcur) {
		char* cur = *pcur;

		if (*cur != '#')
			return false;
		++cur;

		while (!line_end_c(*cur))
			++cur;

		*pcur = cur;
		return true;
	}
	bool empty_line (char** pcur) {
		char* cur = *pcur;
		whitespace(&cur);

		line_comment(&cur);

		if (!(newline(&cur) || *cur == '\0'))
			return false;

		*pcur = cur;
		return true;
	}

	bool begin_of_line (char** pcur) {
		char* cur = *pcur;

		while (empty_line(&cur));

		*pcur = cur;
		return true;
	}
	bool end_of_line (char** pcur) {
		return empty_line(pcur);
	}

	bool integer (char** pcur, int* out) {
		char* cur = *pcur;
		whitespace(&cur);

		if (!sint(&cur, out))
			return false;

		*pcur = cur;
		return true;
	}
	bool float_ (char** pcur, f32* out) {
		char* cur = *pcur;
		whitespace(&cur);

		if (!float32(&cur, out))
			return false;

		*pcur = cur;
		return true;
	}

	bool char_ (char** pcur, char c) {
		char* cur = *pcur;
		whitespace(&cur);

		if (!character(&cur, c))
			return false;

		*pcur = cur;
		return true;
	}

	bool arraylen (char** pcur, int* out) {
		char* cur = *pcur;
		whitespace(&cur);

		if (!char_(&cur, '[')) return false;

		if (!integer(&cur, out)) return false;

		if (!char_(&cur, ']')) return false;

		*pcur = cur;
		return true;
	}

	bool string (char** pcur, std::string* out) {
		char* cur = *pcur;
		whitespace(&cur);

		char* begin = cur;

		while (!(line_end_c(*cur) || *cur == '#'))
			++cur;

		char* end = cur;

		out->assign(begin, end -begin);

		*pcur = cur;
		return true;
	}

	bool end_of_file (char** pcur) {
		return **pcur == '\0';
	}
}

struct Neural_Network {
	iv2 input_size;

	struct Input {
		std::string			name;
		std::vector<f32>	flat; // 1d version of input data
		std::vector<f32>	expected_activationss;
	};

	struct Weight {
		f32	val; // range [-inf, +inf] ??
	};
	struct Bias {
		f32 val; // range [-inf, +inf] ??
	};

	struct Real_Neuron {
		std::vector<Weight>		weights; // weight[i] is associated with the connection between this neuron and the previous layer's neuron[i]
		Bias					bias;

	};
	struct Real_Layer {
		std::vector<Real_Neuron>	neurons;

	};

	std::vector<Real_Layer>	real_layers;

	std::vector<std::string> output_neuron_names;

};

Neural_Network						nn;
std::vector<Neural_Network::Input>	input;

bool parse_text_file (cstr filepath) {

	std::string text;

	if (!imgl::load_text_file(filepath, &text)) return false;

	char* cur = (char*)text.c_str();

	using namespace parse;

	{
		begin_of_line(&cur);

		if (!integer(&cur, &nn.input_size.x)) return false;

		if (!char_(&cur, 'x')) return false;

		if (!integer(&cur, &nn.input_size.y)) return false;

		if (!end_of_line(&cur)) return false;
	}

	{
		int hidden_layers_count;
		{
			begin_of_line(&cur);

			if (!arraylen(&cur, &hidden_layers_count)) return false;

			if (!end_of_line(&cur)) return false;
		}

		nn.real_layers.resize(hidden_layers_count);

		for (int i=0; i<hidden_layers_count; ++i) {
			int neuron_count;
			{
				begin_of_line(&cur);

				if (!integer(&cur, &neuron_count)) return false;

				if (!end_of_line(&cur)) return false;
			}
			nn.real_layers[i].neurons.resize(neuron_count);
		}
	}

	{
		nn.real_layers.emplace_back();
		auto& output_layer = nn.real_layers.back();

		int output_neurons_count;
		{
			begin_of_line(&cur);

			if (!arraylen(&cur, &output_neurons_count)) return false;

			if (!end_of_line(&cur)) return false;
		}
		output_layer.neurons.resize(output_neurons_count);

		for (int i=0; i<output_neurons_count; ++i) {
			begin_of_line(&cur);

			nn.output_neuron_names.emplace_back();
			if (!string(&cur, &nn.output_neuron_names.back())) return false;

			if (!end_of_line(&cur)) return false;
		}
	}

	for (int i=0; i<(int)nn.real_layers.size(); ++i) {
		auto& l = nn.real_layers[i];
		
		for (auto& n : l.neurons)
			n.weights.resize( i == 0 ?
				nn.input_size.x * nn.input_size.y :
				(int)nn.real_layers[i -1].neurons.size() );
	}

	for (int i=0;; ++i) {
		if (end_of_file(&cur))
			break;

		input.emplace_back();
		auto& inp = input.back();

		{
			begin_of_line(&cur);

			if (!string(&cur, &inp.name)) break;

			if (!end_of_line(&cur)) return false;
		}

		for (int y=0; y<nn.input_size.y; ++y) {
			begin_of_line(&cur);

			for (int x=0; x<nn.input_size.x; ++x) {

				input[i].flat.emplace_back();
				if (!float_(&cur, &input[i].flat.back())) return false;

			}

			if (!end_of_line(&cur)) return false;
		}

		auto& output_layer = nn.real_layers.back();
		{
			begin_of_line(&cur);

			for (int j=0; j<(int)output_layer.neurons.size(); ++j) {
				inp.expected_activationss.emplace_back();
				if (!float_(&cur, &inp.expected_activationss.back())) break;
			}

			if (!end_of_line(&cur)) return false;
		}
	}

	return true;
}

#include <random>

f32 mean = 1 ? 0 : 0.5f;
f32 deviation = 0.5f;

std::default_random_engine		generator;
std::normal_distribution<f32>	normal_distribution (mean, deviation);

f32 random_value () {
	return normal_distribution(generator);
}

Neural_Network::Weight random_weight () {
	return { random_value() };
}
Neural_Network::Bias random_bias () {
	return { random_value() };
}

void init_nn_with_random_weights () {
	for (auto& l : nn.real_layers) {
		for (auto& n : l.neurons) {
			for (auto& w : n.weights) {
				w = random_weight();
			}
			n.bias = random_bias();
		}
	}
}

f32 sigmoid (f32 x) {
	//		1   / (e^(-x) +1)
	// OR:	e^x / (e^x    +1)

	f32 tmp = exp(x);

	return tmp / (tmp +1);
}

f32 activation_func (f32 val) {
	return sigmoid(val);
}

f32 evaluate_neuron (std::vector<f32> const& prev_layer_activations, std::vector<Neural_Network::Weight> const& weights, Neural_Network::Bias const& bias) {
	assert(prev_layer_activations.size() == weights.size());

	f32 total = 0;
	for (auto i=0; i<(int)weights.size(); ++i)
		total += weights[i].val * prev_layer_activations[i];

	total += bias.val;

	f32 activation = activation_func(total);

	return activation;
}

std::vector<f32> eval_layer_activations (Neural_Network::Real_Layer const& layer, std::vector<f32> const& prev_layer_activations) {
	std::vector<f32> activations ( layer.neurons.size() );
	
	for (int neuron_i=0; neuron_i<layer.neurons.size(); ++neuron_i) {
		auto& n = layer.neurons[neuron_i];

		activations[neuron_i] = evaluate_neuron(prev_layer_activations, n.weights, n.bias);
	}

	return activations;
}

std::vector< std::vector<f32> > eval_nn (Neural_Network const& nn, Neural_Network::Input const& input) {
	
	std::vector< std::vector<f32> > layer_activations ( nn.real_layers.size() );

	for (int i=0; i<(int)nn.real_layers.size(); ++i) {
		auto& l = nn.real_layers[i];
		
		layer_activations[i] = std::move( eval_layer_activations(l, i == 0 ? input.flat : layer_activations[i -1]) );
	}

	return layer_activations;
}

void print_output (Neural_Network::Input const& input, std::vector<f32> const& output_activations, std::vector<f32> const& cost, f32 avg_cost) {
	printf("%s:\n\n", input.name.c_str());

	for (int i=0; i<(int)output_activations.size(); ++i) {
		printf("  %-20s: %.1f %7.3f -> %7.3f\n", nn.output_neuron_names[i].c_str(), input.expected_activationss[i], output_activations[i], cost[i]);
	}

	printf("\n     -> %7.3f\n", avg_cost);
}

std::vector<f32> eval_cost_func (std::vector<f32> const& activations, std::vector<f32> const& expected_activations, f32* avg_cost) {
	int count = (int)activations.size();

	std::vector<f32> cost ( count );
	f32 total = 0;

	for (int i=0; i<count; ++i) {
		f32 diff = expected_activations[i] -activations[i];

		cost[i] = diff * diff;
		total += cost[i];
	}

	*avg_cost = total / (f32)count;
	return cost;
}

#include "time.h"

void build_and_evaluate_random_nn () {
	
	cstr filepath = "test_nn.txt";

	bool sucess = parse_text_file(filepath);
	if (!sucess) {
		fprintf(stderr, "error in parsing %s", filepath);
	}

	generator.seed( 1 ? 0 : (uint)time(NULL) );

	init_nn_with_random_weights();

	for (auto& i : input) {
		
		auto activations = eval_nn(nn, i);

		f32 avg_cost;
		auto cost = eval_cost_func(activations.back(), i.expected_activationss, &avg_cost);

		print_output(i, activations.back(), cost, avg_cost);
	}

}

void draw () {
	
	#if 0
	iv2 sz = iv2(infoheader.biWidth,infoheader.biHeight);
	auto copy = make_unique<rgba8[]>(sz.x * sz.y);
		
	for (int y=0; y<sz.y; ++y) {
		for (int x=0; x<sz.x; ++x) {
			copy[y * sz.x +x] = rgba8(rgb8(shading[y * sz.x +x]), 255);
		}
	}
		
	imgl::image_rect(copy.get(), sz, imgl::get_current_region().fit_aspect((v2)sz), false);
	#endif
}
void draw_loop () {
	imgl::window_loop("Neural Network", draw, 0, imgl::POLL_FOR_INPUT);
}

int main() {

	//draw_loop();
	build_and_evaluate_random_nn();

}
