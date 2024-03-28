#include "Common.h"

std::vector<double> brownian_bridge(double initial, double final_value, unsigned int level, std::default_random_engine* const gen_ptr) {
	// Generating Standard Normal Distribution for Brownian Bridge Construction
	std::normal_distribution<double> normal_dist(0.0, 1.0);

	// Constructing Brownian Bridge for Price Generation
	int total_period = pow(2, level);
	std::vector<double> bridge;
	bridge.resize(total_period + 1);
	bridge.front() = initial;
	bridge.back() = final_value;

	int delta_n = total_period;
	int n = 0;

	// Looping over Levels to Generate Price for Each Time Step
	for (int m = 0; m < level; m++) {
		for (int k = 0; k < pow(2, m); k++) {
			n = k * delta_n;
			bridge.at(n + floor(delta_n / 2)) = std::max(0.0, 0.5 * (bridge.at(n) + bridge.at(n + delta_n)) + sqrt(delta_n / 4) * normal_dist(*gen_ptr));
		}
		delta_n = floor(delta_n / 2);
	}

	return bridge;
}

unsigned int get_cut_off(const std::vector<double>* const v, double cutoff)
{
	for (int i = v->size() - 1; i >= 0; i--) { // Starting from the Back
		if (v->at(i) > cutoff) {
			return i + 1;
		}
	}

	return v->size();
}

double get_vector_average(std::vector<double>* const v) {
	double result = 0;
	unsigned int n = v->size();

	for (unsigned int i = 0; i < n; i++) {
		result += v->at(i);
	}

	return result / n;
}

void print_vector(std::vector<double>* const v) {
	std::for_each(v->begin(), v->end(), [](const double n) { std::cout << n << ' '; });
	std::cout << std::endl;
	return;
}
