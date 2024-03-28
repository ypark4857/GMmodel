#include "GMOriginal.h"

// Constructor for MarketMaker class
MarketMakerOriginal::MarketMakerOriginal(double v_h, double v_l, double pi)
    : MarketMakerBase(v_h, v_l, pi), last_quantity(0) {
}


// Calculates the bid and ask prices based on the current price of the stock and the probabilities of the high and low values of the stock
void MarketMakerOriginal::calculate_bid_ask() {
	// Formulas from GM Model
    double theta = last_quantity > 0 ? theta_plus : theta_minus;
    theta_plus = ((1.0 + pi) / 2.0) / (pi * theta + (1.0 - pi) / 2.0) * theta;
    theta_minus = ((1.0 - pi) / 2.0) / (pi * (1 - theta) + (1.0 - pi) / 2.0) * theta;

    ask_price = theta_plus * v_h + (1 - theta_plus) * v_l;
    bid_price = theta_minus * v_h + (1 - theta_minus) * v_l;
}

// Reads the current price of the stock from the market, calculates the bid and ask prices, and sends them to the market
void MarketMakerOriginal::read_market_info(const Market* const m_ptr) {
    last_quantity = m_ptr->get_last_quantity();
}

// Submits a limit order to the market based on the current bid and ask prices
void MarketMakerOriginal::submit_order(Market* const m_ptr) {
	calculate_bid_ask(); // Updating the Bid Ask Internally
	std::cout << "[MESSAGE] Market Maker: Ask price is " << ask_price << std::endl; 
	std::cout << "[MESSAGE] Market Maker: Bid price is " << bid_price << std::endl;
    m_ptr->submit_limit_order(bid_price, ask_price);
}

// Constructor for Trader class
TraderOriginal::TraderOriginal(double pi, std::default_random_engine* const gen_ptr)
	: TraderBase(pi, gen_ptr) {
}

// Reads the current probability of the high and low values of the stock from the market and updates the trader's probabilities of buying and selling
void TraderOriginal::read_market_info(const Market* const m_ptr) {
	if (informed) {
		m_ptr->get_final_value() == m_ptr->get_v_h() ? value = 1 : value = -1; // Buy if Final Value is v_h, Sell if Final Value is v_l
	}
	else {
		std::uniform_real_distribution<double> distribution(0, 1);
		distribution(*gen_ptr) <= 0.5 ? value = 1 : value = -1; // 50% Chance to Buy and 50% Chance to Sell
	}
}