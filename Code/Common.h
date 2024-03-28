#pragma once
#include <vector>
#include <iterator>
#include <random>
#include <cmath>
#include <iostream>

// Function Constructing a Brownian Bridge
std::vector<double> brownian_bridge(double initial, double final_value, unsigned int level, std::default_random_engine* const gen_ptr);

// Function for Getting Convergence of Spread
unsigned int get_cut_off(const std::vector<double>* v, double cutoff);

// Function for Getting the Average of Vector
double get_vector_average(std::vector<double>* v);

// Function for Printing a Vector
void print_vector(std::vector<double>* const v);