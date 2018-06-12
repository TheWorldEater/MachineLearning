#pragma once
#include <vector>
#include <iostream>
using std::vector;

struct nodes
{
	vector<float> weight;
	float bias;
};

struct layers {
	vector<nodes> node;
};

vector<layers> layer;

void create() {
	int a;
	std::cout << "Wie viele hidden layers wollen sie?" << std::endl;
	std::cin >> a;

	int b;
	std::cout << "Wie viele nodes sollen die hidden layers haben?" << std::endl;
	std::cin >> b;

	layer.push_back({});
	for (int i = 0; i < b; i++) {
		layer.at(0).node.push_back({});
	}
	if (a > 0) {
		for (int j = 0; j < layer.at(0).node.size; j++) {
			for (int i = 0; i < shading.size(); i++) {
				layer.at(0).node.at(j).weight.push_back({ 0 });
			}
		}
		layer.at(0).node.at(0).bias = 0;
	}

	for (int i = 1; i < a; i++) {
		layer.push_back({});
		for (int j = 0; j < b; j++) {
			layer.at(i).node.push_back({});
			for (int l = 0; l < layer.at(i - 1).node.size(); l++) {
				layer.at(0).node.at(i).weight.push_back({ 0 });
			}
		}
	}

	int c;
	std::cout << "Wei viel Output wollen sie haben?" << std::endl;
	std::cin >> c;




}


