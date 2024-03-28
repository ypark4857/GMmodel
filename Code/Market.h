#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <iterator>

class Market {
private:
	// Numerical members
	unsigned int total_period;
	double v_h;
	double v_l;
	double initial_price;
	double final_value;
	int pending_quantity; // Quantities of securities that should be traded at current period

	// Bid Ask Spread Time Series
	std::vector<double> bid_history;
	std::vector<double> ask_history;
	std::vector<double> spread_history;

	// Price Time Series
	std::vector<double> price_futures;
	std::vector<double> price_history;

	// Quantity Traded Time Series
	std::vector<int> quantity_history;

	// Functions Not to Be Accessed Outside Class
	void error_checking() const;

public:
	Market(double initial_price, unsigned int level, double v_h, double v_l, std::default_random_engine* const gen_ptr);

	// Getting Public Information (double or int)
	// Price
	double get_last_price() const;
	double get_v_h() const;
	double get_v_l() const;
	// Bid Ask Spread
	double get_current_bid() const;
	double get_current_ask() const;
	double get_current_spread() const;
	// Quantity Traded
	int get_last_quantity() const;
	// Time Related
	unsigned int get_current_period() const;
	unsigned int get_total_period() const;

	// Getting Public Information (vector)
	// Price
	const std::vector<double>* const get_price_history() const;
	// Bid Ask Spread
	const std::vector<double>* const get_all_bid() const;
	const std::vector<double>* const get_all_ask() const;
	const std::vector<double>* const get_all_spread() const;
	// Quantity Traded
	const std::vector<int>* const get_all_quantity() const;

	// Private Information
	double get_final_value() const;
	const std::vector<double>* const get_price_futures() const;

	// Order Submission
	void submit_limit_order(double bid, double ask);
	void submit_market_order(int quantity);

	// Exchange Related
	void time_add();
	void trade_matching();
	bool end_of_trading() const;
};