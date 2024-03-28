#include "Market.h"
#include "Common.h"

// Constructor of Market Class
Market::Market(double initial_price, unsigned int level, double v_h, double v_l, std::default_random_engine* const gen_ptr)
: total_period(pow(2, level)), v_h(v_h), v_l(v_l), initial_price(initial_price), pending_quantity(0) {
	//Initial Parameter Checking
	std::cout << "[MESSAGE] Initializing the market" << std::endl;
	error_checking();

	// Randomizing the Final Value of Securities with the Provided v_h and v_l
	std::uniform_int_distribution<int> distribution(0, 1);
	distribution(*gen_ptr) == 0 ? final_value = v_l : final_value = v_h;

	// Pre-calculating the Whole Price Path
	price_futures = brownian_bridge(initial_price, final_value, level, gen_ptr);

	// Initializing the First Period
	time_add();
}

// Checking if Market is Constructed Correctly
void Market::error_checking() const {
	if (v_h <= v_l) {
		// High Value Must be Higher than Low Value
		throw std::runtime_error("[ERROR] v_h is smaller than or equal to v_l");
	}
	if (initial_price >= v_h || initial_price <= v_l) {
		// Initial Price Must be in Range
		throw std::runtime_error("[ERROR] Initial price is not within v_l and v_h");
	}
}

// Function Matching Trades
void Market::trade_matching() {
	quantity_history.push_back(pending_quantity); // Saving the New Traded Quantity
	pending_quantity = 0; // Resetting the Pending Quantity

	return;
}

// Function to Move Time Forward and Terminate the Market
void Market::time_add() {
	price_history.push_back(price_futures.front()); // Pushing Futures to History
	price_futures.erase(price_futures.begin()); // Removing the First Element which is Now History
}

// Function Determining if End of All Trading Periods Happens
bool Market::end_of_trading() const {
	return price_futures.empty();
}

// Function Returning the Last Price
double Market::get_last_price() const {
	return price_history.back();
}

// Function Returning the v_h
double Market::get_v_h() const {
	return v_h;
}

// Function Returning the v_l
double Market::get_v_l() const {
	return v_l;
}

// Function Returning the Current Bid Price to Traders
double Market::get_current_bid() const {
	return bid_history.back();
}

// Function Returning the Current Ask Price to Traders
double Market::get_current_ask() const {
	return ask_history.back();
}

// Function Returning the Current Spread to Traders
double Market::get_current_spread() const {
	return spread_history.back();
}

// Function Returning Last Traded Quantity
int Market::get_last_quantity() const {
	if (quantity_history.empty()) {
		return 0;
	}
	return quantity_history.back();
}

// Function Returning the Current Time Period
unsigned int Market::get_current_period() const {
	return price_history.size();
}

// Function Returning the Total Time Period
unsigned int Market::get_total_period() const {
	return total_period;
}

// Function Revealing Public Information to Market Participants
const std::vector<double>* const Market::get_price_history() const {
	return &price_history;
}

// Function Returning All Bid History
const std::vector<double>* const Market::get_all_bid() const {
	return &bid_history;
}

// Function Returning All Ask History
const std::vector<double>* const Market::get_all_ask() const {
	return &ask_history;
}

// Function Returning All Spread History
const std::vector<double>* const Market::get_all_spread() const {
	return &spread_history;
}

// Function Returning All Quantity Traded
const std::vector<int>* const Market::get_all_quantity() const {
	return &quantity_history;
}

// Function Revealing Final Value of Securities to Informed Trader
double Market::get_final_value() const {
	return final_value;
}

// Function Revealing Private Information to Market Participants
const std::vector<double>* const Market::get_price_futures() const {
	return &price_futures;
}

// Function for Market Maker to Submit Limit Orders
void Market::submit_limit_order(double bid, double ask) {
	int length = price_history.size() - 1;
	if (not (length == bid_history.size() && length == ask_history.size() && length == spread_history.size())) { // Checking if Limit Orders Have Been Submitted for This Period
		throw std::runtime_error("[ERROR] Length of Bid Ask Series Do Not Match Current Period (Limit).");
	}

	// Adding New Bid Ask and Spread
	bid_history.push_back(bid);
	ask_history.push_back(ask);
	spread_history.push_back(ask - bid);

	return;
}

// Function for Traders to Submit Market Orders
void Market::submit_market_order(int quantity) {
	int length = price_history.size();
	if (not (length == bid_history.size() && length == ask_history.size())) { // Checking if Limit Orders Have Been Submitted for This Period (Limit Orders Should Be Submitted)
		throw std::runtime_error("[ERROR] Length of Bid Ask Series Do Not Match Current Period (Market).");
	}
	
	// Updating the Quantity Variable
	pending_quantity = quantity;
}