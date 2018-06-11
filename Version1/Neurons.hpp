#pragma once
#include <vector>
#include <iostream>
using std::vector;

struct neuron{
	vector<float> weights;
	float bias;
};
struct layer{
	vector<neuron> neurons;
};

vector<layer> layers;

void setup() {
	int a = 0;
	std::cout << "How many hidden layers do you want?" << std::endl;
	std::cin >> a;
	for (int i = 0; i < a + 1; i++) {
	}

}

